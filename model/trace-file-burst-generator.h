/*
 * Copyright (c) 2021 SIGNET Lab, Department of Information Engineering,
 * University of Padova
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 *
 */

#ifndef TRACE_FILE_BURST_GENERATOR_H
#define TRACE_FILE_BURST_GENERATOR_H

#include "burst-generator.h"

#include <queue>

namespace ns3
{

class Time;

/**
 * @ingroup applications
 *
 * @brief Reads a traffic trace file to generate bursts
 *
 * The generator reads a trace file and generates bursts accordingly.
 * A trace file should be formatted following the guidelines given
 * by the documentation of ns3::CsvReader.
 *
 * If the same trace file is used by multiple users in the same network,
 * the generated bursts can be decoupled by assigning different
 * StartTimes to different users, if the trace is long enough with respect
 * to the simulation duration.
 *
 */
class TraceFileBurstGenerator : public BurstGenerator
{
  public:
    TraceFileBurstGenerator();
    virtual ~TraceFileBurstGenerator();

    // inherited from Object
    static TypeId GetTypeId();

    // inherited from BurstGenerator
    virtual std::pair<uint32_t, Time> GenerateBurst(void) override;
    /**
     * @brief Returns true while more bursts are present in the trace
     * @return false when the end of the trace is reached
     */
    virtual bool HasNextBurst(void) override;

    /**
     * @brief Returns the duration of the imported trace starting from the StartTime attribute
     * @return the duration of the trace
     */
    double GetTraceDuration(void);

  protected:
    virtual void DoDispose(void) override;

  private:
    // Private attribute accessors
    void SetStartTime(double startTime);
    double GetStartTime(void) const;

    void SetTraceFile(std::string traceFile);
    std::string GetTraceFile(void) const;

    /**
     * Clears the burst queue
     */
    void ClearBurstQueue(void);

    /**
     * Import the trace file into the burst queue
     */
    void ImportTrace(void);

    std::string m_traceFile{""}; //!< The name of the trace file
    double m_startTime{
        0.0}; //!< The trace will only generate traced traffic after a start time offset
    double m_traceDuration{-1.0}; //!< The duration of the trace file considering the start time
    bool m_isFinalized{
        false}; //!< The generator is finalized only once ImportTrace ends with no errors
    std::queue<std::pair<uint32_t, Time>>
        m_burstQueue; //!< The burst information read from the trace
};

} // namespace ns3

#endif // TRACE_FILE_BURST_GENERATOR_H
