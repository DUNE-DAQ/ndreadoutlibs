/**
 * @file NDReadoutTypes.hpp Payload type structures for the DUNE Near Detector
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTTYPES_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTTYPES_HPP_

#include "iomanager/IOManager.hpp"

#include "daqdataformats/FragmentHeader.hpp"
#include "daqdataformats/SourceID.hpp"
#include "detdataformats/pacman/PACMANFrame.hpp"
#include "detdataformats/mpd/MPDFrame.hpp"

#include <cstdint> // uint_t types
#include <memory>  // unique_ptr

namespace dunedaq {
  namespace ndreadoutlibs {
    namespace types {

      /**
       * @brief PACMAN frame
       * Size = 816[Bytes] (12*64+1*32+2*8)
       * */
      const constexpr std::size_t PACMAN_FRAME_SIZE = 1024 * 1024;
      struct PACMAN_MESSAGE_STRUCT
      {
	using FrameType = PACMAN_MESSAGE_STRUCT;
	// data
	char data[PACMAN_FRAME_SIZE];
	// comparable based on first timestamp
	bool operator<(const PACMAN_MESSAGE_STRUCT& other) const
	{
	  auto thisptr = reinterpret_cast<const dunedaq::detdataformats::pacman::PACMANFrame*>(&data);        // NOLINT
	  auto otherptr = reinterpret_cast<const dunedaq::detdataformats::pacman::PACMANFrame*>(&other.data); // NOLINT
	  return (thisptr->get_msg_header((void*)&data)->unix_ts) <
	    (otherptr->get_msg_header((void*)&other.data)->unix_ts) // NOLINT
	    ? true
	    : false;
	}

	// message UNIX timestamp - NOT individual packet timestamps
	uint64_t get_timestamp() const // NOLINT(build/unsigned)
	{
	  return ((uint64_t)( // NOLINT
			     reinterpret_cast<const dunedaq::detdataformats::pacman::PACMANFrame*>(&data)
			     ->get_msg_header((void*)&data)
			     ->unix_ts) * // NOLINT
		  // 1000000000);
		  // FIX ME!!! HARDCODED CONVERSION TO TICKS FROM SECONDS. MULTIPLYING BY 5E7 SINCE
		  // 50 MHz CLOCK.
		  50000000);
	}

	uint64_t get_first_timestamp() const { return get_timestamp(); }

	// FIX ME - implement this in the frame later
	void set_first_timestamp(uint64_t /*ts*/) // NOLINT(build/unsigned)
	{
	  // reinterpret_cast<dunedaq::detdataformats::pacman::PACMANFrame*>(&data)->set_timestamp(ts); // NOLINT
	}

	uint64_t get_message_type() const // NOLINT(build/unsigned)
	{
	  return reinterpret_cast<const dunedaq::detdataformats::pacman::PACMANFrame*>(&data) // NOLINT
	    ->get_msg_header((void*)&data)                                                    // NOLINT
	    ->type;
	}
	void inspect_message() const
	{
	  TLOG_DEBUG(1) << "Message timestamp: " << get_timestamp();

	  TLOG_DEBUG(1) << "Message Type: " << (char)get_message_type(); // NOLINT

	  uint16_t numWords = // NOLINT
	    reinterpret_cast<const dunedaq::detdataformats::pacman::PACMANFrame*>(&data)
	    ->get_msg_header((void*)&data)
	    ->words; // NOLINT

	  TLOG_DEBUG(1) << "Num words in message: " << numWords;

	  for (unsigned int i = 0; i < numWords; i++) {
	    TLOG_DEBUG(1) << "Inspecting word " << i;

	    dunedaq::detdataformats::pacman::PACMANFrame::PACMANMessageWord* theWord =
	      reinterpret_cast<const dunedaq::detdataformats::pacman::PACMANFrame*>(&data)->get_msg_word((void*)&data,
													 i); // NOLINT

	    TLOG_DEBUG(1) << "Word type: " << (char)theWord->data_word.type;                // NOLINT
	    TLOG_DEBUG(1) << "PACMAN I/O Channel: " << (char)theWord->data_word.channel_id; // NOLINT
	    TLOG_DEBUG(1) << "Word receipt timestamp: " << theWord->data_word.receipt_timestamp;

	    dunedaq::detdataformats::pacman::PACMANFrame::LArPixPacket* thePacket = &(theWord->data_word.larpix_word);

	    TLOG_DEBUG(1) << "Inspecting packet";

	    TLOG_DEBUG(1) << "Packet Type: " << thePacket->data_packet.type;
	    TLOG_DEBUG(1) << "Packet Chip ID: " << thePacket->data_packet.chipid;
	    TLOG_DEBUG(1) << "Packet Channel ID: " << thePacket->data_packet.channelid;

	    TLOG_DEBUG(1) << "packet timestamp: " << thePacket->data_packet.timestamp;
	  }
	}

	FrameType* begin()
	{
	  return reinterpret_cast<FrameType*>(&data[0]); // NOLINT
	}

	FrameType* end()
	{
	  return reinterpret_cast<FrameType*>(data + PACMAN_FRAME_SIZE); // NOLINT
	}

	size_t get_payload_size() { return PACMAN_FRAME_SIZE; }

	size_t get_num_frames() { return 1; }

	size_t get_frame_size() { return PACMAN_FRAME_SIZE; }

	static const constexpr daqdataformats::GeoID::SystemType system_type = daqdataformats::GeoID::SystemType::kNDLArTPC;
	static const constexpr daqdataformats::FragmentType fragment_type = daqdataformats::FragmentType::kNDLArTPC;

	// Set the right value for this field
	static const constexpr uint64_t expected_tick_difference = 0; // NOLINT(build/unsigned)
      };

      /**
       * Key finder for LBs.
       * */
      struct PACMANTimestampGetter
      {
	uint64_t operator()(const PACMAN_MESSAGE_STRUCT& data) // NOLINT(build/unsigned)
	{
	  return data.get_timestamp();
	}
      };

      typedef dunedaq::iomanager::SenderConcept<PACMAN_MESSAGE_STRUCT> PACMANFrameSink;
      typedef std::shared_ptr<PACMANFrameSink> SharedPACMANFrameSink;
      using PACMANFramePtrSink = dunedaq::iomanager::SenderConcept<std::unique_ptr<types::PACMAN_MESSAGE_STRUCT>>;
      using SharedPACMANFramePtrSink = std::shared_ptr<PACMANFramePtrSink>;
  
      /**
       * @brief MPD frame
       **/
      const constexpr std::size_t MPD_FRAME_SIZE = 1024; // CLARIFY MEANING OF THIS. Number of readout units in frame?
      struct MPD_MESSAGE_STRUCT {
	using FrameType = MPD_MESSAGE_STRUCT;
	char data[MPD_FRAME_SIZE];

	bool operator<(const MPD_MESSAGE_STRUCT& other) const
	{
	  auto thisptr = reinterpret_cast<const dunedaq::detdataformats::mpd::MPDFrame*>(&data);        // NOLINT
	  auto otherptr = reinterpret_cast<const dunedaq::detdataformats::mpd::MPDFrame*>(&other.data); // NOLINT
	  return (thisptr->get_timestamp() < otherptr->get_timestamp()) // NOLINT
		      ? true
		      : false;
	}
	
	uint64_t get_timestamp() const // NOLINT(build/unsigned)
	{
	  auto thisptr = reinterpret_cast<const dunedaq::detdataformats::mpd::MPDFrame*>(&data);        // NOLINT
	  return thisptr->get_timestamp();
	}

	uint64_t get_first_timestamp() const { return get_timestamp(); }

	void set_first_timestamp(uint64_t ts) // NOLINT(build/unsigned)
	{
	  //	  auto thisptr = reinterpret_cast<const dunedaq::detdataformats::mpd::MPDFrame*>(&data);        // NOLINT
	  //thisptr -> settimestamp(ts);
	  // Need to implement in MPDFrame first
	}
	// ! Need these to compile - TODO: understand specifics of light sytem
	size_t get_payload_size() { return MPD_FRAME_SIZE; }
	size_t get_num_frames() { return 1; }
	size_t get_frame_size() { return MPD_FRAME_SIZE; }
	// Set the right value for this field
	static const constexpr uint64_t expected_tick_difference = 0; // NOLINT(build/unsigned)

	FrameType* begin() { return reinterpret_cast<FrameType*>(&data[0]); }
	FrameType* end()   { return reinterpret_cast<FrameType*>(data + MPD_FRAME_SIZE); }

	static const constexpr daqdataformats::GeoID::SystemType system_type = daqdataformats::GeoID::SystemType::kNDPD;
	static const constexpr daqdataformats::FragmentType fragment_type = daqdataformats::FragmentType::kNDPD;

      };
    } // namespace types
  } // namespace ndreadoutlibs
} // namespace dunedaq

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTTYPES_HPP_
