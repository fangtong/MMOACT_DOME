#pragma once
#include "BaseBehaviorQuit.h"

namespace IOCPFramework{
	namespace Behavior{
		class BehaviorDefaultQuit:
			public BaseBehavior::BaseBehaviorQuit{
		private:
		protected:
		public:
			__declspec(dllexport) BehaviorDefaultQuit(void);
			~BehaviorDefaultQuit(void);
		public:
			__declspec(dllexport) void Quit(SOCKET i_soket);
		};
	}
}