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

#include "nddetdataformats/PACMANFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutPACMANTypeAdapter.hpp"
#include "readoutlibs/ReadoutLogging.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <string>

using dunedaq::readoutlibs::logging::TLVL_BOOKKEEPING;
using dunedaq::readoutlibs::logging::TLVL_FRAME_RECEIVED;

namespace dunedaq {
namespace ndreadoutlibs {

class PACMANFrameProcessor : public readoutlibs::TaskRawDataProcessorModel<types::NDReadoutPACMANTypeAdapter>
{
public:
  using inherited = readoutlibs::TaskRawDataProcessorModel<types::NDReadoutPACMANTypeAdapter>;
  using frameptr = types::NDReadoutPACMANTypeAdapter*;
  using pacmanframeptr = dunedaq::nddetdataformats::PACMANFrame*;
  using timestamp_t = std::uint64_t; // NOLINT(build/unsigned)

  explicit PACMANFrameProcessor(std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry)
    : readoutlibs::TaskRawDataProcessorModel<types::NDReadoutPACMANTypeAdapter>(error_registry)
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
#include "detail/PACMANFrameProcessor.hxx"

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANFRAMEPROCESSOR_HPP_
