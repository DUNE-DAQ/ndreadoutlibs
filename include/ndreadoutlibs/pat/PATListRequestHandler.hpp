#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PAT_PATLISTREQUESTHANDLER_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PAT_PATLISTREQUESTHANDLER_HPP_

#include "readoutlibs/FrameErrorRegistry.hpp"
#include "readoutlibs/ReadoutIssues.hpp"
#include "readoutlibs/models/DefaultRequestHandlerModel.hpp"
#include "readoutlibs/models/SkipListLatencyBufferModel.hpp"

#include "nddetdataformats/PATFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutPATTypeAdapter.hpp"
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

namespace dunedaq
{
  namespace ndreadoutlibs
  {
    class PATListRequestHandler: public readoutlibs::DefaultRequestHandlerModel<types::NDReadoutPATTypeAdapter, readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPATTypeAdapter>>
    {
    public:
      using inherited    = readoutlibs::DefaultRequestHandlerModel<types::NDReadoutPATTypeAdapter, readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPATTypeAdapter>>;
      using SkipListAcc  = typename folly::ConcurrentSkipList<types::NDReadoutPATTypeAdapter>::Accessor;
      using SkipListSkip = typename folly::ConcurrentSkipList<types::NDReadoutPATTypeAdapter>::Skipper;

      PATListRequestHandler(std::unique_ptr<readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPATTypeAdapter>>& latency_buffer,
			    std::unique_ptr<readoutlibs::FrameErrorRegistry> &error_registry): DefaultRequestHandlerModel<types::NDReadoutPATTypeAdapter, readoutlibs::SkipListLatencyBufferModel<types::NDReadoutPATTypeAdapter>>(latency_buffer, error_registry)
	{
	  TLOG_DEBUG(TLVL_WORK_STEPS) << "PATListRequestHandler created...";
	}
    protected:
    private:
    };
  }
}

#endif
