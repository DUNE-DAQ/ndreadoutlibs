/**
 * @file NDReadoutMPDTypeAdapter.hpp Payload type structures for the DUNE Near Detector
 *
 * This is part of the DUNE DAQ , copyright 2021.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */
#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTMPDTYPEADAPTER_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTMPDTYPEADAPTER_HPP_

#include "iomanager/IOManager.hpp"
#include "daqdataformats/FragmentHeader.hpp"
#include "daqdataformats/SourceID.hpp"
#include "nddetdataformats/MPDFrame.hpp"
#include "logging/Logging.hpp"
#include <cstdint> // uint_t types
#include <memory>  // unique_ptr

namespace dunedaq {
  namespace ndreadoutlibs {
    namespace types {
      /**
       * @brief MPD frame
       * */
      struct NDReadoutMPDTypeAdapter {
	using FrameType = NDReadoutMPDTypeAdapter;
	std::vector<char> data ;

	void load_message( const void * load_data, const unsigned int size ) {
	  data.resize(size);
	  memcpy(&data[0], load_data, size);
	}

	bool operator<(const NDReadoutMPDTypeAdapter & other) const
	{
          uint64_t this_timestamp  = 0;  //NOLINT
          uint64_t other_timestamp = 0;  //NOLINT

          // 27-Apr-2023, KAB: the size of the data member needs to be checked to see whether it
          // actually contains any data, before trying to interpret the contents.
          if (data.size() >= sizeof(dunedaq::nddetdataformats::MPDFrame)) {
            auto thisptr = reinterpret_cast<const dunedaq::nddetdataformats::MPDFrame*>(&data[0]);  // NOLINT
            this_timestamp = thisptr->get_timestamp();
          }
          if (other.data.size() >= sizeof(dunedaq::nddetdataformats::MPDFrame)) {
            auto otherptr = reinterpret_cast<const dunedaq::nddetdataformats::MPDFrame*>(&other.data[0]);  // NOLINT
            other_timestamp = otherptr->get_timestamp();
          }

	  return (this_timestamp < other_timestamp) ? true : false;
	}
	
	uint64_t get_timestamp() const // NOLINT(build/unsigned)
	{
	  auto thisptr = reinterpret_cast<const dunedaq::nddetdataformats::MPDFrame*>(&data[0]);        // NOLINT
	  return thisptr->get_timestamp();
	}

	uint64_t get_first_timestamp() const { return get_timestamp(); }

	void set_first_timestamp(uint64_t /*ts*/) // NOLINT(build/unsigned)
	{
	  //
	}

	size_t get_payload_size() { return data.size(); }
	size_t get_num_frames() { return 1; }
	size_t get_frame_size() { return data.size(); }
	// Set the right value for this field
	static const constexpr uint64_t expected_tick_difference = 0; // NOLINT(build/unsigned)

	FrameType* begin() { return reinterpret_cast<FrameType*>(&data[0]); }
	FrameType* end()   { return reinterpret_cast<FrameType*>(data[0] + data.size()); }

	static const constexpr daqdataformats::SourceID::Subsystem subsystem = daqdataformats::SourceID::Subsystem::kDetectorReadout;
	static const constexpr daqdataformats::FragmentType fragment_type = daqdataformats::FragmentType::kMPD;

      };
    } // namespace types
  } // namespace ndreadoutlibs
} // namespace dunedaq

#endif // NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTMPDTYPEADAPTER_HPP_
