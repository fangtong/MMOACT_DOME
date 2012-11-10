#include "MainThread2.h"

namespace IOCPFramework{
	namespace Behavior{
		MainThread2::MainThread2(void)
		{
			this->_hMainThread=CreateThread(NULL, 0, &ServerRoutine, (LPVOID)this, 0, NULL );
		}

		MainThread2::~MainThread2(void)
		{
		}

		DWORD MainThread2::ServerRoutine(LPVOID lp){
			//这里运行Init();
			int i=0;
			while(true){
				i++;
			}
			return 0;
		}

		void MainThread2::Start(int i_intStartSeconds){

		}

	}
}