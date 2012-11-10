#pragma once
#include "BaseBehaviorMsgSend.h"

namespace IOCPFramework{
	namespace Behavior{
		class BehaviorDefaultMsgSend:
			public BaseBehavior::BaseBehaviorMsgSend{
		private:
		protected:
		public:
			__declspec(dllexport) BehaviorDefaultMsgSend(void);
			~BehaviorDefaultMsgSend(void);
		public:
			__declspec(dllexport) void MsgSend(SOCKET i_soket,char* i_lpChar,int i_charArrLength);
		};
	}
}