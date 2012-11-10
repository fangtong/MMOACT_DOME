#pragma once
#include "stdafx.h"

namespace IOCPFramework{
	namespace BaseBehavior{
		class BaseBehaviorQuit{
		private:
		protected:
		public:
			virtual void Quit(SOCKET i_soket)=0;
		};
	}
}