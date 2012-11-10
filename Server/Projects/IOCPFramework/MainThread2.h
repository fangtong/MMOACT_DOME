#include "basebehaviormainthread.h"
#pragma once



namespace IOCPFramework{
	namespace Behavior{
		class MainThread2:
			public BaseBehavior::BaseBehaviorMainThread
		{
		public:
			__declspec(dllexport) MainThread2(void);
			__declspec(dllexport) ~MainThread2(void);
		public:
			static	DWORD WINAPI ServerRoutine(LPVOID lp);
			__declspec(dllexport) void Start(int i_intStartSeconds);
		};

		
	}
}