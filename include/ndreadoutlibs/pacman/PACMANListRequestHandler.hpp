/**
 * @file PACMANListRequestHandler.hpp Trigger matching mechanism for PACMAN frames.
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANLISTREQUESTHANDLER_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PACMAN_PACMANLISTREQUESTHANDLER_HPP_


#include "datahandlinglibs/FrameErrorRegistry.hpp"
#include "datahandlinglibs/DataHandlingIssues.hpp"
#include "datahandlinglibs/ReadoutLogging.hpp"
#include "datahandlinglibs/models/DefaultRequestHandlerModel.hpp"
#include "datahandlinglibs/models/SkipListLatencyBufferModel.hpp"

#include "nddetdataformats/PACMANFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutPACMANTypeAdapter.hpp"

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

using dunedaq::datahandlinglibs::logging::TLVL_WORK_STEPS;

namespace dunedaq {
namespace ndreadoutlibs {

class PACMANListRequestHandler
  : public datahandlinglibs::DefaultRequestHandlerModel<
      types::NDReadoutPACMANTypeAdapter,
      datahandlinglibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>
{
public:
  using inherited =
    datahandlinglibs::DefaultRequestHandlerModel<types::NDReadoutPACMANTypeAdapter,
                                            datahandlinglibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>;
  using SkipListAcc = typename folly::ConcurrentSkipList<types::NDReadoutPACMANTypeAdapter>::Accessor;
  using SkipListSkip = typename folly::ConcurrentSkipList<types::NDReadoutPACMANTypeAdapter>::Skipper;

  PACMANListRequestHandler(
    std::shared_ptr<datahandlinglibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>& latency_buffer,
    std::unique_ptr<datahandlinglibs::FrameErrorRegistry>& error_registry)
    : DefaultRequestHandlerModel<types::NDReadoutPACMANTypeAdapter,
                                 datahandlinglibs::SkipListLatencyBufferModel<types::NDReadoutPACMANTypeAdapter>>(latency_buffer,
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
