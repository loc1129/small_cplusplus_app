#pragma once
#include "DataStore.h"
#include <string>
#include <stdlib.h>
#include <stdio.h>


class Parser
{
	private:
		DataStore* mDataStore;
		std::string mFileName;
		char * mData;
		int mDataSize;
		int mHeaderSize;
		int mOrderAckSize;

		void logWarn(const std::string & msg);
		uint16_t htons_ld(uint16_t little_endian_value);
		uint32_t htonl_ld(uint32_t little_endian_value);
		uint64_t htonll_ld(uint64_t little_endian_value);
		void mymemcpy(char* dest, char* src, int len);

		bool isValidHeader(int pos, int & endpos);
		bool isValidOrderNonAckMsg(int pos, int &endpos, bool &isOrderEntry, int & bodylen);
		bool isValidOrderAckMsg(int pos, int &endpos);
		
		void parseOrderEntryMsg(int pos);
		void parseOrderAckMsg(int pos);
		void parseOrderFillMsg(int pos);
		void parserHeader(int pos);

		void initData();

	public:
		Parser(const std::string & filename, DataStore* datastore);

		~Parser()
		{
		}

		void ParseFile();
};
