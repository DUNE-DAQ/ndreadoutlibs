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

#include "detdataformats/pacman/PACMANFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutTypes.hpp"
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
      types::PACMAN_MESSAGE_STRUCT,
      readoutlibs::SkipListLatencyBufferModel<types::PACMAN_MESSAGE_STRUCT>>
{
public:
  using inherited =
    readoutlibs::DefaultRequestHandlerModel<types::PACMAN_MESSAGE_STRUCT,
                                            readoutlibs::SkipListLatencyBufferModel<types::PACMAN_MESSAGE_STRUCT>>;
  using SkipListAcc = typename folly::ConcurrentSkipList<types::PACMAN_MESSAGE_STRUCT>::Accessor;
  using SkipListSkip = typename folly::ConcurrentSkipList<types::PACMAN_MESSAGE_STRUCT>::Skipper;

  PACMANListRequestHandler(
    std::unique_ptr<readoutlibs::SkipListLatencyBufferModel<types::PACMAN_MESSAGE_STRUCT>>& latency_buffer,
    std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry)
    : DefaultRequestHandlerModel<types::PACMAN_MESSAGE_STRUCT,
                                 readoutlibs::SkipListLatencyBufferModel<types::PACMAN_MESSAGE_STRUCT>>(latency_buffer,
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
