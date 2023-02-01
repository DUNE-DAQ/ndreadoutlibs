// Declarations for MPDFrameProcessor

namespace dunedaq {
namespace ndreadoutlibs {

void 
MPDFrameProcessor::conf(const nlohmann::json& args)
{
  auto config = args["rawdataprocessorconf"].get<readoutlibs::readoutconfig::RawDataProcessorConf>();
  m_clock_frequency = config.clock_speed_hz;

  readoutlibs::TaskRawDataProcessorModel<types::MPD_MESSAGE_STRUCT>::add_preprocess_task(
    std::bind(&MPDFrameProcessor::timestamp_check, this, std::placeholders::_1));
  // m_tasklist.push_back( std::bind(&MPDFrameProcessor::frame_error_check, this, std::placeholders::_1) );
  TaskRawDataProcessorModel<types::MPD_MESSAGE_STRUCT>::conf(args);
}

/**
 * Pipeline Stage 1.: Check proper timestamp increments in DAPHNE frame
 * */
void 
MPDFrameProcessor::timestamp_check(frameptr fp)
{
  // If EMU data, emulate perfectly incrementing timestamp
  if (inherited::m_emulator_mode) { // emulate perfectly incrementing timestamp
    // FIX ME - add fake timestamp to MPD message struct
  }

  // Acquire timestamp
  m_current_ts = fp->get_timestamp();
  TLOG_DEBUG(TLVL_FRAME_RECEIVED) << "Received MPD frame timestamp value of " << m_current_ts << " ticks (..." << std::fixed << std::setprecision(8) << (static_cast<double>(m_current_ts % (m_clock_frequency*1000)) / static_cast<double>(m_clock_frequency)) << " sec)"; // NOLINT

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
void MPDFrameProcessor::frame_error_check(frameptr /*fp*/)
{
  // check error fields
  // FIX ME - to be implemented

  // fp->inspect_message();
}

} // namespace ndreadoutlibs
} // namespace dunedaq
