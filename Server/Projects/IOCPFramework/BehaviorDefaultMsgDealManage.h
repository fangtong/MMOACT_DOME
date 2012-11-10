#include "stdafx.h"
//#include "IBMsgDealManage.h"
//#include "DataStruct.h"
#include "BaseBehaviorMainThread.h"
#pragma once

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace Behavior{
				class BehaviorDefaultMsgDealManage
					:public BaseBehavior::IBMsgDealManage
				{
				public:
					BehaviorDefaultMsgDealManage(void);
					~BehaviorDefaultMsgDealManage(void);

					//virtual bool MsgDealManage(
					//	CallBackFun pCallBack,
					//	IOCP_IO* i_iocpIO,
					//	char* i_lpChar,
					//	int i_charArrLength
					//	);

					virtual bool MsgDealManage(
						BaseBehavior::BaseBehaviorMainThread* pIBMainThread,
						IOCP_IO* i_iocpIO,
						char* i_lpChar,
						int i_charArrLength
						);
				};
			}
		}
	}
}