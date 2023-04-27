/**
 * @file PACMANListRequestHandler.hpp Trigger matching mechanism for PACMAN frames.
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANLISTREQUESTHANDLER_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANLISTREQUESTHANDLER_HPP_

#include "readoutlibs/FrameErrorRegistry.hpp"
#include "readoutlibs/ReadoutIssues.hpp"
#include "readoutlibs/models/DefaultRequestHandlerModel.hpp"
#include "readoutlibs/models/SkipListLatencyBufferModel.hpp"

#include "nddetdataformats/PACMANFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutPACMANTypeAdapter.hpp"
#include "readoutlibs/ReadoutLogging.hpp"

#include <atomic>
#include <deque>
#include <functional>
#include <future>
#include <iomanip>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <vector>

using dunedaq::readoutlibs::logging::TLVL_WORK_STEPS;

namespace dunedaq {
namespace ndreadoutlibs {

class PACMANListRequestHandler
  : public readoutlibs::DefaultRequestHandlerModel<
      types::NDReadoutPACMANTypeAdapter,
      readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>
{
public:
  using inherited =
    readoutlibs::DefaultRequestHandlerModel<types::NDReadoutPACMANTypeAdapter,
                                            readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>;
  using SkipListAcc = typename folly::ConcurrentSkipList<types::NDReadoutPACMANTypeAdapter>::Accessor;
  using SkipListSkip = typename folly::ConcurrentSkipList<types::NDReadoutPACMANTypeAdapter>::Skipper;

  PACMANListRequestHandler(
    std::unique_ptr<readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>& latency_buffer,
    std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry)
    : DefaultRequestHandlerModel<types::NDReadoutPACMANTypeAdapter,
                                 readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>(latency_buffer,
													     error_registry)
  {
    TLOG_DEBUG(TLVL_WORK_STEPS) << "PACMANistRequestHandler created...";
  }

protected:
private:
};

} // namespace ndreadoutlibs
} // namespace dunedaq

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANLISTREQUESTHANDLER_HPP_
