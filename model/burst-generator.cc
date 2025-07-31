/*
 * Copyright (c) 2021 SIGNET Lab, Department of Information Engineering,
 * University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 *
 *
 */

#include "burst-generator.h"

#include "ns3/log.h"

namespace ns3
{

NS_LOG_COMPONENT_DEFINE("BurstGenerator");

NS_OBJECT_ENSURE_REGISTERED(BurstGenerator);

BurstGenerator::BurstGenerator()
{
}

BurstGenerator::~BurstGenerator()
{
}

TypeId
BurstGenerator::GetTypeId()
{
    static TypeId tid =
        TypeId("ns3::BurstGenerator").SetParent<Object>().SetGroupName("Applications");
    return tid;
}

void
BurstGenerator::DoDispose()
{
    // chain up
    Object::DoDispose();
}

} // namespace ns3
