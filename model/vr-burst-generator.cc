/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//
// Copyright (c) 2021 SIGNET Lab, Department of Information Engineering,
// University of Padova
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation;
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "ns3/log.h"
#include "ns3/random-variable-stream.h"
#include "ns3/double.h"
#include "ns3/data-rate.h"
#include "ns3/nstime.h"
#include "ns3/object-factory.h"
#include "vr-burst-generator.h"
#include <algorithm>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("VrBurstGenerator");

NS_OBJECT_ENSURE_REGISTERED (VrBurstGenerator);

TypeId
VrBurstGenerator::GetTypeId (void)
{
  static TypeId tid =
      TypeId ("ns3::VrBurstGenerator")
          .SetParent<BurstGenerator> ()
          .SetGroupName ("Applications")
          .AddConstructor<VrBurstGenerator> ()
          .AddAttribute ("FrameRate",
                         "The frame rate of the VR application [FPS].",
                         DoubleValue (60),
                         MakeDoubleAccessor (&VrBurstGenerator::SetFrameRate,
                                             &VrBurstGenerator::GetFrameRate),
                         MakeDoubleChecker<double> (0))
          .AddAttribute ("TargetDataRate",
                         "The target data rate that the VR application will try to achieve.",
                         DataRateValue (DataRate ("20Mbps")),
                         MakeDataRateAccessor (&VrBurstGenerator::SetTargetDataRate,
                                               &VrBurstGenerator::GetTargetDataRate),
                         MakeDataRateChecker ());
  return tid;
}

VrBurstGenerator::VrBurstGenerator ()
{
  NS_LOG_FUNCTION (this);
}

VrBurstGenerator::~VrBurstGenerator ()
{
  NS_LOG_FUNCTION (this);
}

int64_t
VrBurstGenerator::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_periodRv->SetStream (stream);
  m_frameSizeRv->SetStream (stream + 1);
  return 2;
}

void
VrBurstGenerator::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  m_periodRv = 0;
  m_frameSizeRv = 0;

  // chain up
  BurstGenerator::DoDispose ();
}

void
VrBurstGenerator::SetFrameRate (double frameRate)
{
  NS_LOG_FUNCTION (this << frameRate);

  NS_ABORT_MSG_IF (frameRate <= 0, "Frame rate must be positive, instead: " << frameRate);
  m_frameRate = frameRate;

  SetupModel ();
}

double
VrBurstGenerator::GetFrameRate (void) const
{
  return m_frameRate;
}

void
VrBurstGenerator::SetTargetDataRate (DataRate targetDataRate)
{
  NS_LOG_FUNCTION (this << targetDataRate);

  NS_ABORT_MSG_IF (targetDataRate.GetBitRate () <= 0,
                   "Target data rate must be positive, instead: " << targetDataRate);
  m_targetDataRate = targetDataRate;

  SetupModel ();
}

DataRate
VrBurstGenerator::GetTargetDataRate (void) const
{
  return m_targetDataRate;
}

bool
VrBurstGenerator::HasNextBurst (void)
{
  NS_LOG_FUNCTION (this);
  // this burst generator has no limits on the number of bursts
  return true;
}

std::pair<uint32_t, Time>
VrBurstGenerator::GenerateBurst ()
{
  NS_LOG_FUNCTION (this);

  // sample current frame size
  uint32_t frameSize = m_frameSizeRv->GetInteger ();

  // sample period before next frame
  Time period = Seconds (m_periodRv->GetValue ());
  NS_ABORT_MSG_IF (!period.IsPositive (),
                   "Period must be non-negative, instead found period=" << period.As (Time::S));

  NS_LOG_DEBUG ("Frame size: " << frameSize << " B, period: " << period.As (Time::S));
  return std::make_pair (frameSize, period);
}

void
VrBurstGenerator::SetupModel ()
{
  NS_LOG_FUNCTION (this);

  double S = m_targetDataRate.GetBitRate () / 8.0 / m_frameRate; // expected frame size [B]
  double ifi = 1.0 / m_frameRate; // expected inter frame interarrival [s]

  // Model frame size stats
  double sP = 0.9008;
  double sI = 1.1764;

  double aP = 9.0399;
  double aI = 26.2065;
  double bP = 0.6251;
  double bI = 0.5730;

  double wP = 0.6400; // wI = 1 - wP

  double meanP = sP * S;
  double stdP = aP * std::pow (S, bP);

  double meanI = sI * S;
  double stdI = aI * std::pow (S, bI);

  NS_LOG_DEBUG ("Frame size: 2 component GMM with "
                << "N1(mean=" << meanP << ",std=" << stdP << ") with wP=" << wP << ", "
                << "N2(mean=" << meanI << ",std=" << stdI << ") with wI=" << 1 - wP);

  Ptr<NormalRandomVariable> rvP = CreateObjectWithAttributes<NormalRandomVariable> (
      "Mean", DoubleValue (meanP),
      "Variance", DoubleValue (stdP * stdP),
      "Bound", DoubleValue (meanP)); // avoid negative frame sizes
  Ptr<NormalRandomVariable> rvI = CreateObjectWithAttributes<NormalRandomVariable> (
      "Mean", DoubleValue (meanI),
      "Variance", DoubleValue (stdI * stdI),
      "Bound", DoubleValue (meanI)); // avoid negative frame sizes

  m_frameSizeRv = CreateObject<MixtureRandomVariable> ();
  std::vector<double> wCdf{wP, 1.0};
  std::vector<Ptr<RandomVariableStream>> rvs{rvP, rvI};
  m_frameSizeRv->SetRvs (wCdf, rvs);

  // Model inter frame interarrival
  double location = ifi;
  double ifiStd = 0.0827205 / m_frameRate; // [s]
  double scale = ifiStd * std::sqrt (3) / M_PI; // [s]

  m_periodRv = CreateObjectWithAttributes<LogisticRandomVariable> (
      "Location", DoubleValue (location),
      "Scale", DoubleValue (scale),
      "Bound", DoubleValue (location));
}

} // Namespace ns3
