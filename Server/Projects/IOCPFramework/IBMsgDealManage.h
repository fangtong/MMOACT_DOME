#include "stdafx.h"
#include "DataStruct.h"
#include "BaseBehavior.h"
#include "BaseBehaviorMsgDeal.h"
#pragma once

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{

			//typedef bool (BaseBehavior::BaseBehaviorMsgDeal::*CallBackFun)(
			//	IOCP_IO* i_iocpIO,
			//	char* i_lpChar,
			//	int i_charArrLength
			//	);

			namespace BaseBehavior{
				class IBMsgDealManage{
				public:
					virtual ~IBMsgDealManage(){}
					//virtual bool MsgDealManage(
					//	CallBackFun pCallBack,
					//	IOCP_IO* i_iocpIO,
					//	char* i_lpChar,
					//	int i_charArrLength
					//	)=0;

					virtual bool MsgDealManage(
						BaseBehaviorMainThread* pIBMainThread,
						IOCP_IO* i_iocpIO,
						char* i_lpChar,
						int i_charArrLength
						)=0;
				};
			}
		}
	}
}