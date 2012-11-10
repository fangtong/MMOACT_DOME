#include "BehaviorDefaultMsgDeal.h"

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace Behavior{
				BehaviorDefaultMsgDeal::BehaviorDefaultMsgDeal(void)
				{

				}

				BehaviorDefaultMsgDeal::~BehaviorDefaultMsgDeal(void)
				{

				}

				bool BehaviorDefaultMsgDeal::MsgQuit(IOCP_IO* i_iocpIO){
					return TRUE;
				}

				bool BehaviorDefaultMsgDeal::MsgDeal(IOCP_IO* i_iocpIO,char* i_lpChar,int i_charArrLength){
					return this->Send(i_iocpIO,i_lpChar,i_charArrLength);
				}
			}
		}
	}
}