// Declarations for PACMANFrameProcessor

namespace dunedaq {
namespace ndreadoutlibs {

void 
PACMANFrameProcessor::conf(const appmodel::DataHandlerModule* conf)
{
  
  // m_tasklist.push_back( std::bind(&PACMANFrameProcessor::frame_error_check, this, std::placeholders::_1) );
  for (auto output : conf->get_outputs()) {
    TLOG() << "On outputs... (" << output->UID() << "," << output->get_data_type() << ")";
    try {
      if (output->get_data_type() == "TriggerPrimitiveVector") {
        TLOG() << "Found TP sink.";
        m_tp_sink = get_iom_sender<std::vector<trigger::TriggerPrimitiveTypeAdapter>>(output->UID());
        TLOG() << " SINK INITIALIZED for TriggerPrimitives with UID : " << output->UID();
        }
    }
    catch (const ers::Issue& excpt) {
      ers::error(datahandlinglibs::ResourceQueueError(ERS_HERE, "tp", "DefaultRequestHandlerModel", excpt));
    }
  }
  datahandlinglibs::TaskRawDataProcessorModel<types::NDReadoutPACMANTypeAdapter>::add_preprocess_task(
    std::bind(&PACMANFrameProcessor::timestamp_check, this, std::placeholders::_1)
  );
    
  TaskRawDataProcessorModel<types::NDReadoutPACMANTypeAdapter>::conf(conf);

  // auto dp = conf->get_module_configuration()->get_data_processor();
  // if (dp == nullptr) {
  //   TLOG()<< " PACMAN Data processor does not exist.";
  // } else {
  //   auto proc_conf = dp->cast<appmodel::RawDataProcessor>();
  //   if (proc_conf == nullptr) {
  //     TLOG()<< "PACMAN DataProcessor does not exist.";
  //   } else { 
  //     auto geo_id = conf->get_geo_id();
  //     if (geo_id != nullptr) {
  //       m_det_id = geo_id->get_detector_id();
  //       m_crate_id = geo_id->get_crate_id();
  //       m_slot_id = geo_id->get_slot_id();
  //       m_stream_id = geo_id->get_stream_id();
  //     }
    
  //   }
  // }
      
  // if (m_post_processing_enabled) { 
  //   // Extract TPs back as a pre-processing task, due to LatencyBuffer post-proc issues using SkipList.
  //   inherited::add_preprocess_task(std::bind(&PACMANFrameProcessor::extract_tps, this, std::placeholders::_1));
  // }

}

/**
 * Pipeline Stage 1.: Check proper timestamp increments in DAPHNE frame
 * */
void 
PACMANFrameProcessor::timestamp_check(frameptr fp)
{
  // If EMU data, emulate perfectly incrementing timestamp
  //if (inherited::m_emulator_mode) { // emulate perfectly incrementing timestamp
    // FIX ME - add fake timestamp to PACMAN message struct
  //}

  // Acquire timestamp
  m_current_ts = fp->get_timestamp();
  TLOG_DEBUG(TLVL_FRAME_RECEIVED) << "Received PACMAN frame timestamp value of " << m_current_ts << " ticks (..." << std::fixed
                                  << std::setprecision(8) << (static_cast<double>(m_current_ts % (50000000000)) / 50000000.0) << " sec)";

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
void 
PACMANFrameProcessor::frame_error_check(frameptr /*fp*/)
{
  // check error fields
  // FIX ME - to be implemented

  // fp->inspect_message();
}

void PACMANFrameProcessor::extract_tps(constframeptr fp){
  if (!fp || fp==nullptr){
    return;
  }

  frameptr non_constfp = const_cast<frameptr>(fp);

  // Only one TP generated
  std::vector<trigger::TriggerPrimitiveTypeAdapter> tpp;
  for(unsigned int i=0; i<non_constfp->get_num_frames(); i++){
    if(non_constfp->get_message_type()!=dunedaq::nddetdataformats::PACMANFrame::DATA_WORD){
      TLOG()<<"Found message with non-dataword type";
      continue;
    }
    trigger::TriggerPrimitiveTypeAdapter tpa;
    dunedaq::trgdataformats::TriggerPrimitive tp;

    // Turn dataword into TP
    tp.adc_integral = non_constfp->get_dataword(i);
    tp.adc_peak = non_constfp->get_dataword(i);
    tp.time_start = non_constfp->get_first_timestamp();
    tp.channel = non_constfp->get_channel_id(i);

    // HARDCODED
    tp.detid = 0;
    tp.samples_to_peak = 1;
    tp.samples_over_threshold = 1;
    tpa.tp = tp;
    tp.detid = dunedaq::trgdataformats::INVALID_DETID;
    tpp.push_back(tpa);
  }
  if(!tpp.empty()){
    if (!m_tp_sink->try_send(std::move(tpp), iomanager::Sender::s_no_block)) {
      // TODO Make a proper issue
      TLOG()<<"Failed to send";
    }
  }
  else{
    TLOG()<<"TPP empty!"; 
  }

  return; 

}


} // namespace ndreadoutlibs
} // namespace dunedaq
