/*
 * Copyright (c) 2021 SIGNET Lab, Department of Information Engineering,
 * University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 *
 */
#include "ns3/command-line.h"
#include "ns3/double.h"
#include "ns3/my-random-variable-stream.h"
#include "ns3/nstime.h"
#include "ns3/object-factory.h"
#include "ns3/random-variable-stream.h"
#include "ns3/simulator.h"

#include <iostream>
#include <vector>

/**
 * @file
 * @ingroup core-examples
 * @ingroup randomvariable
 * Example program illustrating use of ns3::MixtureRandomVariable
 */

using namespace ns3;

int
main(int argc, char* argv[])
{
    uint32_t nSamples = 1000000;

    CommandLine cmd(__FILE__);
    cmd.AddValue("nSamples", "Number of samples", nSamples);
    cmd.Parse(argc, argv);

    Ptr<MixtureRandomVariable> x = CreateObject<MixtureRandomVariable>();

    // setup weights cdf
    std::vector<double> w{0.7, 1.0}; // p1 = 0.7, p2 = 0.3
    // setup random variables
    std::vector<Ptr<RandomVariableStream>> rvs;
    rvs.push_back(CreateObjectWithAttributes<NormalRandomVariable>("Mean",
                                                                   DoubleValue(5),
                                                                   "Variance",
                                                                   DoubleValue(1)));
    rvs.push_back(CreateObjectWithAttributes<NormalRandomVariable>("Mean",
                                                                   DoubleValue(10),
                                                                   "Variance",
                                                                   DoubleValue(4)));

    x->SetRvs(w, rvs);

    for (uint32_t i = 0; i < nSamples; i++)
    {
        std::cout << x->GetValue() << std::endl;
    }

    return 0;
}
