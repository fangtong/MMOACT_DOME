#include "BaseBehaviorMainThread.h"
#include "Condition.h"
#pragma once
#include "ThreadingModel.h"

using namespace SevenSmile::Util::ThreadHelper;

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace Behavior{

#define	MAXTHREAD_COUNT	8
				static GUID g_GUIDAcceptEx = WSAID_ACCEPTEX;

				//创建主线程行为。继承主线程行为接口
				class BehaviorCreateMainThread :
					public BaseBehavior::BaseBehaviorMainThread,
					public ObjectLevelLockable<BehaviorCreateMainThread>
				{
				public:
					LPFN_ACCEPTEX lpAcceptEx;

					__declspec(dllexport) BehaviorCreateMainThread(void);
					__declspec(dllexport) BehaviorCreateMainThread(
						char* i_lpCharIpAddr,
						int i_intPort,
						int i_intExecuteThreadNum);
					~BehaviorCreateMainThread(void);

					//void SetListenSoketIPAddr(char* i_lpChar);
					//void SetListenSoketPort(int i_intPort);

					__declspec(dllexport) void SetListenSoketIPAddr(const char* i_lpChar);
					__declspec(dllexport) void SetListenSoketPort(int i_intPort);
					__declspec(dllexport) void SetExecuteThreadNum(int i_intThreadNum);
					//void SetPostNum(int i_intPostNum);

					static DWORD WINAPI ServerRoutine(LPVOID lp);
					void CloseThreadHandle(int count);
					static DWORD WINAPI	CompletionRoutine(LPVOID lp_param);

					__declspec(dllexport) int Start(int i_intStartSeconds);
					__declspec(dllexport) int Close();
					//__declspec(dllexport) void Start(int i_intStartSeconds);
					__declspec(dllexport) int InitIOCP(void);
					__declspec(dllexport) bool InitSocket(void);
					__declspec(dllexport) bool BindAndListenSocket(void);
					__declspec(dllexport) bool StartThread(void);
					__declspec(dllexport) bool GetFunPointer(void);
					__declspec(dllexport) bool PostAcceptEx(void);
					__declspec(dllexport) bool RegAcceptEvent(void);

					__declspec(dllexport) void SetThreadCPU(int);



					__declspec(dllexport) void InitIoContext(IOCP_IO* lp_io);
					__declspec(dllexport) void Remove(IOCP_IO* lp_io,IOCP_KEY* lp_key);
					//__declspec(dllexport) bool HandleData(IOCP_IO* lp_io, int nFlags);
					//__declspec(dllexport) bool GetAddrAndPort(IOCP_IO* lp_io);
					__declspec(dllexport) bool ContinueRecv(IOCP_IO* lp_io,IOCP_KEY* lp_key);
					//__declspec(dllexport) bool MsgSend(SOCKET s,char* buf,int len);
					//__declspec(dllexport) bool MsgSend();
					__declspec(dllexport) bool MainLoop();

					//设置外部函数

					__declspec(dllexport) void SetBehaviorMsgDeal(
						BaseBehavior::BaseBehaviorMsgDeal* i_lpBaseBehaviorMsgDeal
						);

					__declspec(dllexport) virtual void SetIBMsgDealManage(
						BaseBehavior::IBMsgDealManage* i_lpIBMsgDealManage
						);

					__declspec(dllexport) void CondCompletionThreadClose(){
						Lock lock(*this);
						--_intMaxExecuteThreadNum;
						_condCompletionThreadClose.Signal();
					}

				private:
					int			_cpuMask;
					HANDLE		_hMainThread;	//主线程句柄
					//DWORD		_dwMainThreadId;	//主线程id
					HANDLE		_hIOCP;			//iocp句柄
					SOCKET		_soketMainListenSoket;
					//KEY_GROUP	m_key_group;
					KEY_GROUP	_keyGroup;
					IO_GROUP	_ioGroup;
					//IO_GROUP	_ioSendGroup;
					IOCP_KEY*	_lpMainListenIOCPKey;
					char		ADDR[20];
					int			PORT;
					HANDLE*		_hArrThread;	//处理线程句柄数组

					HANDLE		_hAcceptEvent;	//事件

					Condition	_condCompletionThreadClose;//处理线程关闭派发
					bool		_bDestroy;

					int			_intMaxExecuteThreadNum;
					//int		_intPostNum;
					//HANDLE		_hEvent;
					int			_initRes;

					//BaseBehavior::BaseBehaviorMsgSend* _lpBaseBehaviorMsgSend;
					//BaseBehavior::BaseBehaviorQuit* _lpBaseBehaviorQuit;

					BaseBehavior::BaseBehaviorMsgSend* _lpBehaviorMsgSend;

					const static char chOpt=1;

				private:
					void Init(void);
				};
			}
		}
	}
}