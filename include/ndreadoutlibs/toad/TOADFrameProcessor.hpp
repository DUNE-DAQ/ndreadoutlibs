/**
 * @file TOADFrameProcessor.hpp TOAD specific Task based raw processor
 *
 * This is part of the DUNE DAQ , copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_TOAD_TOADFRAMEPROCESSOR_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_TOAD_TOADFRAMEPROCESSOR_HPP_

#include "readoutlibs/ReadoutIssues.hpp"
#include "readoutlibs/models/TaskRawDataProcessorModel.hpp"

#include "detdataformats/toad/TOADFrameOverlay.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutTypes.hpp"
#include "readoutlibs/ReadoutLogging.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <string>

using dunedaq::readoutlibs::logging::TLVL_BOOKKEEPING;

namespace dunedaq {
namespace ndreadoutlibs {

class TOADFrameProcessor : public readoutlibs::TaskRawDataProcessorModel<types::TOAD_MESSAGE_STRUCT>
{
public:
  using inherited = readoutlibs::TaskRawDataProcessorModel<types::TOAD_MESSAGE_STRUCT>;
  using frameptr = types::TOAD_MESSAGE_STRUCT*;
  using pacmanframeptr = dunedaq::detdataformats::toad::TOADFrameOverlay*;
  using timestamp_t = std::uint64_t; // NOLINT(build/unsigned)

  explicit TOADFrameProcessor(std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry)
    : readoutlibs::TaskRawDataProcessorModel<types::TOAD_MESSAGE_STRUCT>(error_registry)
  {}

  // Custom pipeline registration
  void conf(const nlohmann::json& args) override;

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
  void timestamp_check(frameptr fp);

  /**
   * Pipeline Stage 2.: Check headers for error flags
   * */
  void frame_error_check(frameptr /*fp*/);

private:
};

} // namespace ndreadoutlibs
} // namespace dunedaq

// Declarations
#include "detail/TOADFrameProcessor.hxx"

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_TOAD_TOADFRAMEPROCESSOR_HPP_
