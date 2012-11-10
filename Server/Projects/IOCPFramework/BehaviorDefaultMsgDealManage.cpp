#include "BehaviorDefaultMsgDealManage.h"
#include "DataStruct.h"
#pragma once

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace Behavior{
				BehaviorDefaultMsgDealManage::BehaviorDefaultMsgDealManage(void)
				{
				}

				BehaviorDefaultMsgDealManage::~BehaviorDefaultMsgDealManage(void)
				{
				}

				//bool BehaviorDefaultMsgDealManage::MsgDealManage( 
				//	CallBackFun pCallBack, 
				//	IOCP_IO* i_iocpIO, 
				//	char* i_lpChar, 
				//	int i_charArrLength 
				//	)
				//{
				//	return (this->*pCallBack)(
				//		i_iocpIO, 
				//		i_lpChar, 
				//		i_charArrLength 
				//		);
				//}

				bool BehaviorDefaultMsgDealManage::MsgDealManage( 
					BaseBehavior::BaseBehaviorMainThread* lpIBMainThread, 
					IOCP_IO* i_iocpIO, 
					char* i_lpChar, 
					int i_charArrLength 
					)
				{
					//Net::MsgPackageManage mpm(i_lpChar,i_charArrLength);
					//MsgPackageArr mpa=mpm.GetMsgPackages();

					//for (unsigned int i=0;i<mpa.size();i++)
					//{
					//	return lpIBMainThread->_lpBaseBehaviorMsgDeal->MsgDeal(
					//		i_iocpIO,
					//		mpa[i]->lpcMsgPackage,
					//		mpa[i]->uiPackageLength
					//		);
					//}

					//return true;

					return lpIBMainThread->_lpBaseBehaviorMsgDeal->MsgDeal(
						i_iocpIO,
						i_lpChar,
						i_charArrLength
						);
				}

				
			}
		}
	}
}