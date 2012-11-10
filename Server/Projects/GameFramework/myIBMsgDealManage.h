#pragma once
#include "IBMsgDealManage.h"
#include "DataStruct.h"
#include "BaseBehaviorMainThread.h"

using namespace SevenSmile::Net::IOCPFramework::BaseBehavior;
using namespace SevenSmile::Net::IOCPFramework;

class myIBMsgDealManage :
	public SevenSmile::Net::IOCPFramework::BaseBehavior::IBMsgDealManage
{
private:
	myIBMsgDealManage(){};
public:
	myIBMsgDealManage(unsigned int i_point);
	~myIBMsgDealManage(void);
	virtual bool MsgDealManage(
		BaseBehaviorMainThread* pIBMainThread,
		IOCP_IO* i_iocpIO,
		char* i_lpChar,
		int i_charArrLength
		);
	unsigned int point;
};
