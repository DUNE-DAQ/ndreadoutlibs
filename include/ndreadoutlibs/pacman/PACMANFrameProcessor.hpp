/**
 * @file PACMANFrameProcessor.hpp PACMAN specific Task based raw processor
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANFRAMEPROCESSOR_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANFRAMEPROCESSOR_HPP_

#include "readoutlibs/ReadoutIssues.hpp"
#include "readoutlibs/models/TaskRawDataProcessorModel.hpp"

#include "detdataformats/pacman/PACMANFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutTypes.hpp"
#include "readoutlibs/ReadoutLogging.hpp"

#include "detdataformats/toad/TOADFrameOverlay.hpp"
#include <atomic>
#include <functional>
#include <memory>
#include <string>

using dunedaq::readoutlibs::logging::TLVL_BOOKKEEPING;

namespace dunedaq {
namespace ndreadoutlibs {

class PACMANFrameProcessor : public readoutlibs::TaskRawDataProcessorModel<types::PACMAN_MESSAGE_STRUCT>
{
public:
  using inherited = readoutlibs::TaskRawDataProcessorModel<types::PACMAN_MESSAGE_STRUCT>;
  using frameptr = types::PACMAN_MESSAGE_STRUCT*;
  //using pacmanframeptr = dunedaq::detdataformats::pacman::PACMANFrame*;
  using pacmanframeptr = dunedaq::detdataformats::toad::TOADFrameOverlay*;
  using timestamp_t = std::uint64_t; // NOLINT(build/unsigned)

  explicit PACMANFrameProcessor(std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry)
    : readoutlibs::TaskRawDataProcessorModel<types::PACMAN_MESSAGE_STRUCT>(error_registry)
  {}

  void conf(const nlohmann::json& args) override
  {
    readoutlibs::TaskRawDataProcessorModel<types::PACMAN_MESSAGE_STRUCT>::add_preprocess_task(
      std::bind(&PACMANFrameProcessor::timestamp_check, this, std::placeholders::_1));
    // m_tasklist.push_back( std::bind(&PACMANFrameProcessor::frame_error_check, this, std::placeholders::_1) );
    TaskRawDataProcessorModel<types::PACMAN_MESSAGE_STRUCT>::conf(args);
  }

protected:
  // Internals
  timestamp_t m_previous_ts = 0;
  timestamp_t m_current_ts = 0;
  bool m_first_ts_fake = true;
  bool m_first_ts_missmatch = true;
  bool m_problem_reported = false;
  std::atomic<int> m_ts_error_ctr{ 0 };

  /**
   * Pipeline Stage 1.: Check proper timestamp increments in DAPHNE frame
   * */
  void timestamp_check(frameptr fp)
  {
    // If EMU data, emulate perfectly incrementing timestamp
    if (inherited::m_emulator_mode) { // emulate perfectly incrementing timestamp
      // FIX ME - add fake timestamp to PACMAN message struct
    }

    // Acquire timestamp
    m_current_ts = fp->get_timestamp();
    // 12-Jan-2023, KAB: added debug statement when frame is received. In later versions of this code,
    // we will provide a constant variable for the debug level.
    TLOG_DEBUG(19) << "Received PACMAN frame timestamp value of " << m_current_ts << " ticks";

    // Check timestamp
    // RS warning : not fixed rate!
    if (m_current_ts - m_previous_ts <= 0) {
      ++m_ts_error_ctr;
      TLOG_DEBUG(TLVL_BOOKKEEPING) << "Timestamp continuity MISSMATCH! -> | previous: " << std::to_string(m_previous_ts)
                                   << " current: " + std::to_string(m_current_ts);
    }

    if (m_ts_error_ctr > 1000) {
      if (!m_problem_reported) {
        TLOG() << "*** Data Integrity ERROR *** Timestamp continuity is completely broken! "
               << "Something is wrong with the FE source or with the configuration!";
        m_problem_reported = true;
      }
    }

    m_previous_ts = m_current_ts;
    m_last_processed_daq_ts = m_current_ts;
  }

  /**
   * Pipeline Stage 2.: Check headers for error flags
   * */
  void frame_error_check(frameptr /*fp*/)
  {
    // check error fields
    // FIX ME - to be implemented

    // fp->inspect_message();
  }

private:
};

} // namespace ndreadoutlibs
} // namespace dunedaq

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANFRAMEPROCESSOR_HPP_
