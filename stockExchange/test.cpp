#include "DataStore.h"
#include "Parser.h"
#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;
int main(int argc, char*argv[])
{
	if(argc != 2)
	{
		std::cout << "please enter the file name" << std::endl;
		return 1;
	}

	std::cout << "the file name is: " << argv[1] << std::endl;

	DataStore * data = new DataStore();
	if(!data)
	{
		std::cout << "memory out" << std::endl;
		return 1;
	}
	Parser * parser = new Parser(std::string(argv[1]), data);
	if(!parser)
	{
		std::cout << "memory out" << std::endl;
		delete data;
		return 1;
	}

	//std::shared_ptr<DataStore> data = std::make_shared<DataStore>();
	//std::shared_ptr<Parser> parser = std::make_shared<Parser>(std::string(argv[1]), data);

	std::cout << data->GetPacketNum() << ", " << 
	             data->GetMsgNum() << ", " <<
				 data->GetMostActiveTraderTag() << ", " <<
				 data->GetMostLiquidityTraderTag() << ", " <<
				 data->GetVolumePerInstrument() << std::endl;

	//std::cout << data->dump() << std::endl;

	delete parser;
	delete data;
	return 0;
}

