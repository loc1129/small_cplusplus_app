#include "DataStore.h"
#include <iostream>
#include <sstream>

void DataStore::logWarn(const std::string & msg)
{
	//std::cout << "WARN: " << msg << std::endl;
}

void DataStore::UpdateClientId2Info(uint64_t clientId, const std::string & instrument, const std::string & tag)
{
	std::map<uint64_t, tClintInfo>::iterator it = mClientId2Info.find(clientId);
	if(it != mClientId2Info.end())
	{
		logWarn("Already exist in mClientId2Info");
	}

	mClientId2Info[clientId] = std::make_pair(instrument, tag);
}

void DataStore::UpdateOrderId2ClientId(uint64_t key, uint64_t value)
{
	std::map<uint64_t, uint64_t>::iterator it = mOrderId2ClientId.find(key);
	if(it != mOrderId2ClientId.end()) {
		logWarn("Already exist in mOrderId2ClientId");
	}

	mOrderId2ClientId[key] = value;
}

void DataStore::UpdateOrderId2FilledVolume(uint64_t key, uint64_t value)
{
	std::map<uint64_t, uint64_t>::iterator it = mOrderId2FilledVolume.find(key);
	if(it != mOrderId2FilledVolume.end())
	{
		logWarn("Already exist in mOrderId2FilledVolume");
	}

	mOrderId2FilledVolume[key] = value;
}

void DataStore::UpdateClientId2GfdVolume(uint64_t key, uint64_t value)
{
	std::map<uint64_t, uint64_t>::iterator it = mClientId2GfdVolume.find(key);
	if(it != mClientId2GfdVolume.end())
	{
		logWarn("Already exist in mClientId2GfdVolume");
	}

	mClientId2GfdVolume[key] += value;
}

void DataStore::IncreaseMsgNum()
{
	++mMsgNum;
}

void DataStore::IncreasePacketNum()
{
	++mPacketNum;
}

uint64_t DataStore::GetMsgNum() const
{
	return mMsgNum;
}

uint64_t DataStore::GetPacketNum() const
{
	return mPacketNum;
}

std::string DataStore::GetMostActiveTraderTag()
{
	std::map<uint64_t, uint64_t>::iterator it = mOrderId2FilledVolume.begin();
	uint64_t orderid4largestVolume = 0;
	uint64_t largestVolume = 0;
	for(; it != mOrderId2FilledVolume.end(); ++it)
	{
		if(it->second > largestVolume)
		{
			largestVolume = it->second;
			orderid4largestVolume = it->first;
		}
	}

	it = mOrderId2ClientId.find(orderid4largestVolume);
	if(it == mOrderId2ClientId.end())
	{
		return "";
	}

	std::map<uint64_t, tClintInfo>::iterator itr = mClientId2Info.find(it->second);
	if(itr == mClientId2Info.end())
	{
		return "";
	}

	return (itr->second).second;
}

std::string DataStore::GetMostLiquidityTraderTag()
{
	uint64_t clientid4largestLiquidity = 0;
	uint64_t largestLiquidity = 0;
	std::map<uint64_t, uint64_t>::iterator it = mClientId2GfdVolume.begin();
	for(; it != mClientId2GfdVolume.end(); ++it)
	{
		if(it->second > largestLiquidity)
		{
			largestLiquidity = it->second;
			clientid4largestLiquidity = it->first;
		}
	}

	std::map<uint64_t, tClintInfo>::iterator itr = mClientId2Info.find(clientid4largestLiquidity);
	if(itr == mClientId2Info.end())
	{
		return "";
	}

	return (itr->second).second;
}

std::string DataStore::GetVolumePerInstrument()
{
	std::ostringstream ss;

	std::map<uint64_t, uint64_t>::iterator it = mOrderId2FilledVolume.begin();
	for(; it != mOrderId2FilledVolume.end(); ++it)
	{
		uint64_t volume = it->second;	
		std::map<uint64_t, uint64_t>::iterator it1 = mOrderId2ClientId.find(it->first);
		if(it1 == mOrderId2ClientId.end())
		{
			logWarn("can not find the corresonding client id");
			continue;
		}

		std::map<uint64_t, tClintInfo>::iterator it2 = mClientId2Info.find(it1->second);
		if(it2 == mClientId2Info.end())
		{
			logWarn("can not find the corresponding client info");
			continue;
		}

		ss << it2->second.first << " : " << volume << ",";
	}

	return ss.str();
}

std::string DataStore::dump()
{
	std::ostringstream ss;
	ss << "mClientId2Info: " << std::endl;
	std::map<uint64_t, tClintInfo>::iterator it = mClientId2Info.begin();
	for(; it != mClientId2Info.end(); ++it)
	{
		ss << it->first << " : " << it->second.first << " " << it->second.second << std::endl;
	}

	ss << "mOrderId2ClientId: " << std::endl;
	std::map<uint64_t, uint64_t>::iterator it1 = mOrderId2ClientId.begin();
	for(; it1 != mOrderId2ClientId.end(); ++it1)
	{
		ss << it1->first << " : " << it1->second << std::endl;
	}

	ss << "mOrderId2FilledVolume: " << std::endl;
	it1 = mOrderId2FilledVolume.begin();
	for(; it1 != mOrderId2FilledVolume.end(); ++it1)
	{
		ss << it1->first << " : " << it1->second << std::endl;
	}

	ss << "mClientId2GfdVolume: " << std::endl;
	it1 = mClientId2GfdVolume.begin();
	for(; it1 != mClientId2GfdVolume.end(); ++it1)
	{
		ss << it1->first << " : " << it1->second << std::endl;
	}

	ss << std::endl;
	return ss.str();	
}

