/**
 * @file MPDListRequestHandler.hpp Trigger matching mechanism for MPD frames.
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_MPD_MPDLISTREQUESTHANDLER_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_MPD_MPDLISTREQUESTHANDLER_HPP_

#include "readoutlibs/FrameErrorRegistry.hpp"
#include "readoutlibs/ReadoutIssues.hpp"
#include "readoutlibs/models/DefaultRequestHandlerModel.hpp"
#include "readoutlibs/models/SkipListLatencyBufferModel.hpp"

#include "nddetdataformats/MPDFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutMPDTypeAdapter.hpp"
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
  
class MPDListRequestHandler
  : public readoutlibs::DefaultRequestHandlerModel<types::NDReadoutMPDTypeAdapter, readoutlibs::SkipListLatencyBufferModel<types::NDReadoutMPDTypeAdapter>>
{
public:
  using inherited =
    readoutlibs::DefaultRequestHandlerModel<types::NDReadoutMPDTypeAdapter, readoutlibs::SkipListLatencyBufferModel<types::NDReadoutMPDTypeAdapter>>;
  using SkipListAcc = typename folly::ConcurrentSkipList<types::NDReadoutMPDTypeAdapter>::Accessor;
  using SkipListSkip = typename folly::ConcurrentSkipList<types::NDReadoutMPDTypeAdapter>::Skipper;

  MPDListRequestHandler(
    std::unique_ptr<readoutlibs::SkipListLatencyBufferModel<types::NDReadoutMPDTypeAdapter>>& latency_buffer,
    std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry)
    : DefaultRequestHandlerModel<types::NDReadoutMPDTypeAdapter, readoutlibs::SkipListLatencyBufferModel<types::NDReadoutMPDTypeAdapter>>(latency_buffer, error_registry)
  {
    TLOG_DEBUG(TLVL_WORK_STEPS) << "MPDistRequestHandler created...";
  }

protected:
private:
};

} // namespace ndreadoutlibs
} // namespace dunedaq

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_MPD_MPDLISTREQUESTHANDLER_HPP_
