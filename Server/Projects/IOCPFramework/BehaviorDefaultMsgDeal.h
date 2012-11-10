#include "BaseBehaviorMsgDeal.h"
#pragma once

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace Behavior{
				class BehaviorDefaultMsgDeal:
					public BaseBehavior::BaseBehaviorMsgDeal{
				private:

				protected:
				public:
					__declspec(dllexport) BehaviorDefaultMsgDeal(void);
					~BehaviorDefaultMsgDeal(void);

				public:
					__declspec(dllexport) virtual bool MsgDeal(IOCP_IO* i_iocpIO,char* i_lpChar,int i_charArrLength);
					__declspec(dllexport) virtual bool MsgQuit(IOCP_IO* i_iocpIO);
				};
			}
		}
	}
}