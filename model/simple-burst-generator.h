/*
 * Copyright (c) 2021 SIGNET Lab, Department of Information Engineering,
 * University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 *
 */

#ifndef SIMPLE_BURST_GENERATOR_H
#define SIMPLE_BURST_GENERATOR_H

#include "burst-generator.h"

namespace ns3
{

class RandomVariableStream;

/**
 * @ingroup applications
 *
 * @brief Simple burst generator
 *
 * This burst generator implements the BurstGenerator interface.
 * While being simple, it allows the user to customize the distributions
 * of the burst size and period with independent RandomVariableStreams.
 *
 */
class SimpleBurstGenerator : public BurstGenerator
{
  public:
    SimpleBurstGenerator();
    virtual ~SimpleBurstGenerator();

    // inherited from Object
    static TypeId GetTypeId();

    // inherited from BurstGenerator
    virtual std::pair<uint32_t, Time> GenerateBurst(void) override;
    /**
     * @brief This generator has not limits
     * @return always true
     */
    virtual bool HasNextBurst(void) override;

    /**
     * @brief Assign a fixed random variable stream number to the random variables
     * used by this model.
     *
     * @param stream first stream index to use
     * @return the number of stream indices assigned by this model
     */
    int64_t AssignStreams(int64_t stream);

  protected:
    virtual void DoDispose(void) override;

  private:
    Ptr<RandomVariableStream> m_periodRv{0};    //!< rng for period duration [s]
    Ptr<RandomVariableStream> m_burstSizeRv{0}; //!< rng for burst size [B]
};

} // namespace ns3

#endif // SIMPLE_BURST_GENERATOR_H
