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
                         DoubleValue (30),
                         MakeDoubleAccessor (&VrBurstGenerator::SetFrameRate, &VrBurstGenerator::GetFrameRate),
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

  NS_ABORT_MSG_IF (targetDataRate.GetBitRate () == 0,
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
  NS_ASSERT_MSG (m_frameRate == 30,
                 "Only 30 FPS supported for the moment, instead, frame rate was set to: " << m_frameRate);

  double S = m_targetDataRate.GetBitRate () / 8 / m_frameRate; // expected frame size [B]
  double ifi = 1 / m_frameRate; // expected inter frame interarrival [s]

  // Model frame size stats
  double s1 = 0.8826;
  double s2 = 1.2430;

  double d01 = -7286.7632 / 8; // [B]
  double d02 = 104474.1219 / 8; // [B]
  double d1 = 0.1474;
  double d2 = 0.0193;

  double p1 = 0.6744; // p2 = 1 - p1

  double mean1 = s1 * S;
  double std1 = std::max (0.0, d01 + d1 * S);

  double mean2 = s2 * S;
  double std2 = std::max (0.0, d02 + d2 * S);

  NS_LOG_DEBUG ("Frame size: 2 component GMM with " <<
                "N1(mean=" << mean1 << ",std=" << std1 << ") with p1=" << p1 << ", "
                "N2(mean=" << mean2 << ",std=" << std2 << ") with p2=" << 1 - p1);

  Ptr<NormalRandomVariable> rv1 = CreateObjectWithAttributes<NormalRandomVariable> (
      "Mean", DoubleValue (mean1),
      "Variance", DoubleValue (std1 * std1),
      "Bound", DoubleValue (mean1)); // avoid negative frame sizes
  Ptr<NormalRandomVariable> rv2 = CreateObjectWithAttributes<NormalRandomVariable> (
      "Mean", DoubleValue (mean2),
      "Variance", DoubleValue (std2 * std2),
      "Bound", DoubleValue (mean2)); // avoid negative frame sizes

  m_frameSizeRv = CreateObject<MixtureRandomVariable> ();
  std::vector<double> wCdf{p1, 1.0};
  std::vector<Ptr<RandomVariableStream>> rvs{rv1, rv2};
  m_frameSizeRv->SetRvs (wCdf, rvs);

  // Model inter frame interarrival
  double location = ifi;
  double scale = 5.3934e-3 / std::sqrt (2); // [s]

  m_periodRv = CreateObjectWithAttributes<LaplaceRandomVariable> (
      "Location", DoubleValue (location),
      "Scale", DoubleValue (scale),
      "Bound", DoubleValue (location)); // TODO improve
}

} // Namespace ns3
