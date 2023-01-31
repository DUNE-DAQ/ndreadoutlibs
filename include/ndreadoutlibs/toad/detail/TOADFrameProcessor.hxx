// Declarations for TOADFrameProcessor

namespace dunedaq {
namespace ndreadoutlibs {

void 
TOADFrameProcessor::conf(const nlohmann::json& args)
{
  readoutlibs::TaskRawDataProcessorModel<types::TOAD_MESSAGE_STRUCT>::add_preprocess_task(
    std::bind(&TOADFrameProcessor::timestamp_check, this, std::placeholders::_1));
  // m_tasklist.push_back( std::bind(&PACMANFrameProcessor::frame_error_check, this, std::placeholders::_1) );
  TaskRawDataProcessorModel<types::TOAD_MESSAGE_STRUCT>::conf(args);
}

/**
 * Pipeline Stage 1.: Check proper timestamp increments in DAPHNE frame
 * */
//NO TIMESTAMP CHECK FOR TOAD - Timestamps may not arrive in order
void 
TOADFrameProcessor::timestamp_check(frameptr fp)
{
  m_current_ts = (timestamp_t)(fp->get_timestamp());
  TLOG() << "Timestamp at line " << __LINE__ << " is " << m_current_ts;
  m_previous_ts = m_current_ts;
  m_last_processed_daq_ts = m_current_ts;
}

/* 
void 
TOADFrameProcessor::timestamp_check(frameptr fp)
{
  // If EMU data, emulate perfectly incrementing timestamp
  if (inherited::m_emulator_mode) { // emulate perfectly incrementing timestamp
    // FIX ME - add fake timestamp to PACMAN message struct
  }

  // Acquire timestamp
  m_current_ts = fp->get_timestamp();

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
*/

/**
 * Pipeline Stage 2.: Check headers for error flags
 * */
void 
TOADFrameProcessor::frame_error_check(frameptr /*fp*/)
{
  // check error fields
  // FIX ME - to be implemented

  // fp->inspect_message();
}

} // namespace ndreadoutlibs
} // namespace dunedaq
