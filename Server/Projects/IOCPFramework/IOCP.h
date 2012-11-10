#pragma once

#include "stdafx.h"
#include <iostream>
#include "BaseBehaviorMainThread.h"
#include "BaseBehaviorMsgDeal.h"
#include "IBMsgDealManage.h"
//#include "BaseBehaviorMsgSend.h"
//#include "BaseBehaviorQuit.h"


using namespace std;

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{

			class IOCP
			{
			public:
				__declspec(dllexport) IOCP(void);
				__declspec(dllexport) virtual ~IOCP(void);
				__declspec(dllexport) IOCP(
					char* i_strIPAddr,
					int i_intPort,
					int i_intExecuteThreadNumOfIOCPTaskList
					);

				__declspec(dllexport) void SetIpAddress(const char* i_strAddr);
				__declspec(dllexport) void SetThreadCPU(int);
				__declspec(dllexport) void MyIOCPSetPort(int i_intPort);
				__declspec(dllexport) void SetPort(int i_intPort);
				__declspec(dllexport) void SetExecuteThreadNumOfIOCPTaskList(
					int i_intExecuteThreadNumOfIOCPTaskList);

				__declspec(dllexport) string GetIpAddress(void);
				__declspec(dllexport) int GetPort(void);
				__declspec(dllexport) int GetExecuteThreadNumOfIOCPTaskList(void);


				__declspec(dllexport) void SetBehaviorMsgDeal(
					BaseBehavior::BaseBehaviorMsgDeal* i_lpBaseBehaviorMsgDeal
					);
				__declspec(dllexport) void SetBehaviorMsgDealManage(
					BaseBehavior::IBMsgDealManage* i_lpIBMsgDealManage
					);
				//__declspec(dllexport) void SetBehaviorMsgSend(BaseBehavior::BaseBehaviorMsgSend* i_lpBaseBehaviorMsgSend);
				//__declspec(dllexport) void SetBehaviorQuit(BaseBehavior::BaseBehaviorQuit* i_lpBaseBehaviorBehaviorQuit);
				__declspec(dllexport) int Start();
				__declspec(dllexport) int Stop();

				__declspec(dllexport) const char* GetErrorStr();

				//__declspec(dllexport) bool MsgDeal(SOCKET i_soket,char* i_lpChar,int i_charArrLength);
				//__declspec(dllexport) bool MsgSend(SOCKET i_soket,char* i_lpChar,int i_charArrLength);
				//__declspec(dllexport) bool Quit(SOCKET i_soket);

			private:
				const char* _strIPAddr;	//服务器端ip地址
				int _intPort;	//服务器端端口号
				int _intExecuteThreadNumOfIOCPTaskList;	//执行iocp任务队列的线程数

				int _intErrRes;

				//行为端口
				BaseBehavior::BaseBehaviorMainThread* _lpBaseBehaviorMainThread;
				BaseBehavior::BaseBehaviorMsgDeal* _lpBaseBehaviorMsgDeal;
				BaseBehavior::IBMsgDealManage* _lpIBMsgDealManage;
				//BaseBehavior::BaseBehaviorMsgSend* _lpBaseBehaviorMsgSend;
				//BaseBehavior::BaseBehaviorQuit* _lpBaseBehaviorQuit;

				void SetBehaviorMainThread(BaseBehavior::BaseBehaviorMainThread* i_lpBaseBehaviorMainThread);

			};
		}
	}
}