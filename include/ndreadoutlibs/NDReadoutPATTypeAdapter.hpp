#ifndef NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTPATTYPEADAPTER_HPP_
#define NDREADOUTLIBS_INCLUDE_NDREADOUTLIBS_NDREADOUTPATTYPEADAPTER_HPP_

#include "iomanager/IOManager.hpp"
#include "daqdataformats/FragmentHeader.hpp"
#include "daqdataformats/SourceID.hpp"
#include "nddetdataformats/PATFrame.hpp"
#include "logging/Logging.hpp"
#include <cstdint>
#include <memory>

namespace dunedaq
{
  namespace ndreadoutlibs
  {
    namespace types
    {
      struct NDReadoutPATTypeAdapter
      {
	using FrameType = NDReadoutPATTypeAdapter;
	std::vector<uint8_t> data;
	
	void load_message(const void *load_data, const uint size)
	{
	  data.resize(size);
	  memcpy(&data[0], load_data, size);
	}
	
	bool operator<(const NDReadoutPATTypeAdapter &other) const
	{
	  uint64_t this_timestamp  = 0;
	  uint64_t other_timestamp = 0;

	  if (data.size() >= sizeof(dunedaq::nddetdataformats::PATFrame))
	    {
	      auto thisptr   = reinterpret_cast<const dunedaq::nddetdataformats::PATFrame*>(&data[0]);
	      this_timestamp = thisptr->get_timestamp();
	    }
	  if (other.data.size() >= sizeof(dunedaq::nddetdataformats::PATFrame))
	    {
	      auto otherptr = reinterpret_cast<const dunedaq::nddetdataformats::PATFrame*>(&other.data[0]);
	      other_timestamp = otherptr->get_timestamp();
	    }

	  return (this_timestamp < other_timestamp) ? true : false;
	}
	
	uint64_t get_timestamp() const
	{
	  auto thisptr = reinterpret_cast<const dunedaq::nddetdataformats::PATFrame*>(&data[0]);
	  return thisptr->get_timestamp();
	}

	uint64_t get_first_timestamp() const {return get_timestamp();}

	void set_first_timestamp(uint64_t)
	{ }

	size_t get_payload_size() { return data.size(); }
	size_t get_num_frames()   { return 1; }
	size_t get_frame_size()   { return data.size(); }
	
	static const constexpr uint64_t expected_tick_difference = 0;
	
	FrameType *begin() { return reinterpret_cast<FrameType*>(&data[0]); }
	FrameType *end()   { return reinterpret_cast<FrameType*>(data[0] + data.size()); }
	
	static const constexpr daqdataformats::SourceID::Subsystem subsystem = daqdataformats::SourceID::Subsystem::kDetectorReadout;
	static const constexpr daqdataformats::FragmentType fragment_type    = daqdataformats::FragmentType::kPAT;
      };
    }
  }
}

#endif
