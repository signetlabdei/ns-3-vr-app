/*
 * Copyright (c) 2021 SIGNET Lab, Department of Information Engineering,
 * University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 *
 */

#ifndef BURST_GENERATOR_H
#define BURST_GENERATOR_H

#include "ns3/object.h"

namespace ns3
{

class Time;

/**
 * @ingroup applications
 *
 * @brief Virtual interface for burst generators.
 *
 * A virtual interface defining the API for burst generators.
 * Burst generators are thought to be used by BurstyApplication,
 * although their interface might be re-used by other applications.
 *
 * The interface defines two purely virtual methods:
 * - GenerateBurst: generates a new burst, returning its total burst
 * size and the time before the next burst
 * - HasNextBurst: to check whether GenerateBurst can be safely called.
 *
 * The interface is meant to allow for great flexibility in its child
 * classes: such classes can include complex behavior, such as arbitrary
 * probability distributions for burst size and period, correlations
 * among successive burst sizes and periods, cross-correlation between
 * burst size and period, etc.
 *
 */
class BurstGenerator : public Object
{
  public:
    BurstGenerator();
    virtual ~BurstGenerator();

    /**
     * @brief Get the type ID.
     * @return the object TypeId
     */
    static TypeId GetTypeId();

    /**
     * Generate the next burst, composed of a burst size, expressed in Bytes,
     * and a time before the next burst.
     *
     * @return pair with burst size [B] and the time before the next burst
     */
    virtual std::pair<uint32_t, Time> GenerateBurst(void) = 0;

    /**
     * Check whether a next burst can be generated.
     * If not, an error may occur.
     *
     * @return true if a new burst can be generated
     */
    virtual bool HasNextBurst(void) = 0;

  protected:
    virtual void DoDispose(void) override;
};

} // namespace ns3

#endif // BURST_GENERATOR_H
