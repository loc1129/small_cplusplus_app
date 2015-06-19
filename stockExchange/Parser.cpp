#include "Parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <assert.h>

Parser::Parser(const std::string & filename, DataStore* datastore):
	mDataStore(datastore),
	mFileName(filename),
	mData(NULL),
	mDataSize(0)
{
	mHeaderSize = sizeof(uint16_t) +
		sizeof(uint8_t) +
		sizeof(uint64_t) +
		sizeof(uint64_t) +
		sizeof(uint8_t) +
		sizeof(uint16_t);
	mOrderAckSize = sizeof(uint32_t) +
		sizeof(uint64_t) +
		sizeof(uint8_t) +
		sizeof(uint8_t) + 
		sizeof(char) * 8;
	initData();
	ParseFile();
}

Parser::~Parser()
{
	if(mData)
	{
		delete mData;
	}
}

void Parser::logWarn(const std::string & msg)
{
	//std::cout << "WARN: " << msg << std::endl;
}

void Parser::initData()
{
	if(mFileName.empty())
	{
		logWarn("empty file name");
		return;
	}

	std::fstream file(mFileName, std::ios::in | std::ios::binary);
	if(!file.is_open())
	{
		logWarn("can not open file");
		return;
	}

	file.seekg(0, std::ios::end);
	mDataSize = file.tellg();
	if(!mDataSize)
	{
		file.close();
		return;
	}

	mData = new char[mDataSize];
	if(!mData)
	{
		logWarn("memory out");
		file.close();
		return;
	}

	file.seekg(0, std::ios::beg);
	file.read(mData, mDataSize);
	file.close();
}

void Parser::ParseFile()
{
	if(!mData)
	{
		return;
	}

	int pos = 0;
	while(pos < mDataSize)
	{
		int endpos = pos;
		bool isOrderEntry = false;
		int bodylen = 0;
		if(isValidOrderNonAckMsg(pos, endpos, isOrderEntry, bodylen))
		{
			if(isOrderEntry)
			{
				parseOrderEntryMsg(pos);					
			}
			else
			{
				parseOrderFillMsg(pos);	
			}
		}
		else if(isValidOrderAckMsg(pos, endpos))
		{
			parseOrderAckMsg(pos);	
		}
		else if(isValidHeader(pos, endpos))
		{
			parserHeader(pos);	
		}

		pos = endpos + 1;
	}
}

uint16_t Parser::htons_ld(uint16_t little_endian_value)
{
	return ((little_endian_value & 0xff00) >> 8) | ((little_endian_value & 0x00ff) << 8);
}

uint32_t Parser::htonl_ld(uint32_t little_endian_value)
{
	return 	(((little_endian_value & 0xff000000) >> 24) | 
			((little_endian_value & 0x00ff0000) >> 8) | 
			((little_endian_value & 0x0000ff00) << 8) |
			((little_endian_value & 0x000000ff) << 24));
}

uint64_t Parser::htonll_ld(uint64_t little_endian_value)
{
	return 	(((little_endian_value & 0xff00000000000000) >> 56) | 
			((little_endian_value & 0x00ff000000000000) >> 40) | 
			((little_endian_value & 0x0000ff0000000000) >> 24) |
			((little_endian_value & 0x000000ff00000000) >> 8)|
			((little_endian_value & 0x00000000ff000000) << 8) | 
			((little_endian_value & 0x0000000000ff0000) << 24) | 
			((little_endian_value & 0x000000000000ff00) << 40) |
			((little_endian_value & 0x00000000000000ff) << 56));
}

bool Parser::isValidHeader(int pos, int & endpos)
{
	if((mDataSize - pos) < mHeaderSize )
	{
		return false;
	}

	if((mData[pos] == 'S') && (mData[pos+1] == 'T'))
	{
		uint16_t msglen_ld = 0;
		int size4msglen = sizeof(uint16_t);
		int pos4msglen = pos + (mHeaderSize - size4msglen);
		mymemcpy((char*)&msglen_ld, &(mData[pos4msglen]), size4msglen);
		uint16_t msglen = htons_ld(msglen_ld);

		endpos = pos + mHeaderSize + msglen - 1;
		return true;
	}

	return false;
}

void Parser::mymemcpy(char* dest, char* src, int len)
{
	for(int i = 0; i < len; ++i)
	{
		dest[i] = src[i];
	}
}

bool Parser::isValidOrderNonAckMsg(int pos, int &endpos, bool &isOrderEntry, int & bodylen)
{
	if(!isValidHeader(pos, endpos))
	{
		return false;
	}

	int size4maker = sizeof(uint16_t);
	uint8_t msgtype = mData[pos + size4maker];
	int msgtypeV = (int)msgtype;
	if(msgtypeV != 1 && msgtypeV != 3)
	{
		return false;
	}

	uint16_t msglen_ld = 0;
	int size4msglen = sizeof(uint16_t);
	int pos4msglen = pos + (mHeaderSize - size4msglen);
	mymemcpy((char*)&msglen_ld, &(mData[pos4msglen]), size4msglen);

	uint16_t msglen = htons_ld(msglen_ld);
	if((mDataSize - pos) < (mHeaderSize + msglen))
	{
		return false;
	}

	bodylen = msglen;
	isOrderEntry = (msgtypeV == 1);
	endpos = pos + mHeaderSize + msglen - 1;
	return true;
}

bool Parser::isValidOrderAckMsg(int pos, int &endpos)
{
	if(!isValidHeader(pos, endpos))
	{
		return false;
	}

	int size4maker = sizeof(uint16_t);
	uint8_t msgtype = mData[pos + size4maker];
	int msgtypeV = (int)msgtype; 
	if(msgtypeV != 2)
	{
		return false;
	}

	if((mDataSize - pos) < (mHeaderSize + mOrderAckSize))
	{
		return false;
	}

	endpos = pos + mHeaderSize + mOrderAckSize - 1;
	return true;
}

void Parser::parseOrderEntryMsg(int pos)
{
	char instrument[11];		
	char tag[4];

	int size4marker = sizeof(uint16_t);
	int size4msgtype = sizeof(uint8_t);
	int size4id = sizeof(uint64_t);
	int size4price = sizeof(uint64_t);
	int size4qty = sizeof(uint32_t);
	int size4instrument = sizeof(char) * 10;
	int size4side = sizeof(char);
	int size4clientid = sizeof(uint64_t);
	int size4time_in_force = sizeof(uint8_t);
	int size4tag = sizeof(char) * 3;
	int pos4instrment = pos + mHeaderSize + size4price + size4qty;
	mymemcpy(instrument, &(mData[pos4instrment]), size4instrument);
	instrument[10] = '\0';

	uint64_t id_ld = 0;
	int pos4id = pos + size4marker + size4msgtype;
	mymemcpy((char*)&id_ld, &(mData[pos4id]), size4id);
	uint64_t id = htonll_ld(id_ld);

	int pos4tag = pos + mHeaderSize + size4price +
		size4qty + size4instrument + size4side + size4clientid + 
		size4time_in_force;
	mymemcpy(tag, &(mData[pos4tag]), size4tag);
	tag[3] = '\0';

	uint64_t clientid = 0;	
	int pos4clientid = pos + mHeaderSize + size4price + size4qty +
		size4instrument + size4side;
	mymemcpy((char*)&clientid, &(mData[pos4clientid]), size4clientid);

	uint8_t time_in_force = 0;
	int pos4time_in_force = pos + mHeaderSize + size4price + size4qty +
		size4instrument + size4side + size4clientid;
	mymemcpy((char*)&time_in_force, &(mData[pos4time_in_force]), size4time_in_force);

	uint64_t price_ld = 0;
	int pos4price = pos + mHeaderSize;
	mymemcpy((char*)&price_ld, &(mData[pos4price]), size4price);
	uint64_t price = htonll_ld(price_ld);

	uint32_t qty_ld = 0;
	int pos4qty = pos + mHeaderSize + size4price;
	mymemcpy((char*)&qty_ld, &(mData[pos4qty]), size4qty);
	uint32_t qty = htonl_ld(qty_ld);

	mDataStore->IncreasePacketNum();
	mDataStore->IncreaseMsgNum();
	mDataStore->UpdateClientId2Info(clientid, std::string(instrument), std::string(tag));
	int time_in_forceV = (int)time_in_force;
	if(time_in_forceV == 2)
	{
		mDataStore->UpdateClientId2GfdVolume(clientid, price * qty);	
	}
}

void Parser::parseOrderAckMsg(int pos)
{
	int size4marker = sizeof(uint16_t);
	int size4msgtype = sizeof(uint8_t);
	int size4id = sizeof(uint64_t);
	int size4orderid = sizeof(uint32_t);
	int size4clientid = sizeof(uint64_t);
	int size4orderstatus = sizeof(uint8_t);

	uint64_t id_ld = 0;
	int pos4id = pos + size4marker + size4msgtype;
	mymemcpy((char*)&id_ld, &(mData[pos4id]), size4id);
	uint64_t id = htonll_ld(id_ld);

	uint32_t orderid = 0;
	int pos4orderid = pos + mHeaderSize;
	mymemcpy((char*)&orderid, &(mData[pos4orderid]), size4orderid);

	uint64_t clientid = 0;
	int pos4clientid = pos + mHeaderSize + size4orderid;
	mymemcpy((char*)&clientid, &(mData[pos4clientid]), size4clientid);

	uint8_t orderstatus = 0;
	int pos4orderstatus = pos + mHeaderSize + size4orderid + size4clientid;
	mymemcpy((char*)&orderstatus, &(mData[pos4orderstatus]), size4orderstatus);

	mDataStore->IncreasePacketNum();
	mDataStore->IncreaseMsgNum();
	mDataStore->UpdateOrderId2ClientId(orderid, clientid);
}

void Parser::parseOrderFillMsg(int pos)
{
	int size4marker = sizeof(uint16_t);
	int size4msgtype = sizeof(uint8_t);
	int size4id = sizeof(uint64_t);
	int size4orderid = sizeof(uint32_t);
	int size4fillprice = sizeof(uint64_t);
	int size4fillqty = sizeof(uint32_t);

	uint64_t id_ld = 0;
	int pos4id = pos + size4marker + size4msgtype;
	mymemcpy((char*)&id_ld, &(mData[pos4id]), size4id);
	uint64_t id = htonll_ld(id_ld);

	uint32_t orderid = 0;
	int pos4orderid = pos + mHeaderSize;
	mymemcpy((char*)&orderid, &(mData[pos4orderid]), size4orderid);

	uint64_t fillprice_ld = 0;
	int pos4fillprice = pos + mHeaderSize + size4orderid;
	mymemcpy((char*)&fillprice_ld, &(mData[pos4fillprice]), size4fillprice);
	uint64_t fillprice = htonll_ld(fillprice_ld);

	uint32_t fillqty_ld = 0;
	int pos4fillqty = pos + mHeaderSize + size4orderid + size4fillprice;
	mymemcpy((char*)&fillqty_ld, &(mData[pos4fillqty]), size4fillqty);
	uint32_t fillqty = htonl_ld(fillqty_ld);

	mDataStore->IncreasePacketNum();
	mDataStore->IncreaseMsgNum();
	mDataStore->UpdateOrderId2FilledVolume(orderid, fillprice * fillqty);
}

void Parser::parserHeader(int pos)
{
	mDataStore->IncreasePacketNum();
}
