#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <utility>
#include <string>
#include <set>

class DataStore
{
	private:
		uint64_t mMsgNum;
		uint64_t mPacketNum;

		typedef std::pair<std::string, std::string> tClintInfo;
		std::map<uint64_t, tClintInfo> mClientId2Info;
		std::map<uint64_t, uint64_t> mOrderId2ClientId;
		std::map<uint64_t, uint64_t> mOrderId2FilledVolume;
		std::map<uint64_t, uint64_t> mClientId2GfdVolume;

		void logWarn(const std::string & msg);

	public:
		DataStore():
			mMsgNum(0),
			mPacketNum(0)
		{}

		void UpdateClientId2Info(uint64_t clientId, const std::string & instrument, const std::string & tag);
		
		void UpdateOrderId2ClientId(uint64_t key, uint64_t value);

		void UpdateOrderId2FilledVolume(uint64_t key, uint64_t value);

		void UpdateClientId2GfdVolume(uint64_t key, uint64_t value);

		void IncreaseMsgNum();
		void IncreasePacketNum();

		uint64_t GetMsgNum() const;
		uint64_t GetPacketNum() const;

		std::string GetMostActiveTraderTag();

		std::string GetMostLiquidityTraderTag();

		std::string GetVolumePerInstrument();

		std::string dump();
};
