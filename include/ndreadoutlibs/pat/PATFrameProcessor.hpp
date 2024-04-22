#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PAT_PATFRAMEPROCESSOR_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_PAT_PATFRAMEPROCESSOR_HPP_

#include "readoutlibs/ReadoutIssues.hpp"
#include "readoutlibs/models/TaskRawDataProcessorModel.hpp"

#include "nddetdataformats/PATFrame.hpp"
#include "logging/Logging.hpp"
#include "ndreadoutlibs/NDReadoutPATTypeAdapter.hpp"
#include "readoutlibs/ReadoutLogging.hpp"

#include <atomic>
#include <functional>
#include <memory>
#include <string>

using dunedaq::readoutlibs::logging::TLVL_BOOKKEEPING;

namespace dunedaq
{
  namespace ndreadoutlibs
  {
    class PATFrameProcessor: public readoutlibs::TaskRawDataProcessorModel<types::NDReadoutPATTypeAdapter>
    {
    public:
      using inherited   = readoutlibs::TaskRawDataProcessorModel<types::NDReadoutPATTypeAdapter>;
      using frameptr    = types::NDReadoutPATTypeAdapter*;
      using patframeptr = dunedaq::nddetdataformats::PATFrame*;
      using timestamp_t = std::uint64_t;

      explicit PATFrameProcessor(std::unique_ptr<readoutlibs::FrameErrorRegistry>& error_registry): readoutlibs::TaskRawDataProcessorModel<types::NDReadoutPATTypeAdapter>(error_registry) 
      { }
      
      void conf(const nlohmann::json &args) override;

    protected:

      timestamp_t m_previous_ts = 0;
      timestamp_t m_current_ts  = 0;
      bool m_first_ts_fake      = true;
      bool m_frist_ts_missmatch = true;
      bool m_problem_reported   = false;
      std::atomic<int> m_ts_error_ctr{ 0 };
      
      void timestamp_check(frameptr fp);

      void frame_error_check(frameptr);

    private:
      uint64_t m_clock_frequency;

    };
  }
}

#include "detail/PATFrameProcessor.hxx"

#endif
