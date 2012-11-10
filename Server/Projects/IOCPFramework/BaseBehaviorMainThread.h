#include "stdafx.h"
#include "BaseBehaviorMsgDeal.h"
#include "IBMsgDealManage.h"
#pragma once

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace BaseBehavior{
				//主线程接口
				class BaseBehaviorMainThread
				{
				public:
					virtual ~BaseBehaviorMainThread(){}

					BaseBehavior::BaseBehaviorMsgDeal* _lpBaseBehaviorMsgDeal;
					BaseBehavior::IBMsgDealManage* _lpIBMsgDealManage;

					virtual void SetListenSoketIPAddr(const char* i_lpChar)=0;
					virtual void SetListenSoketPort(int i_intPort)=0;
					virtual void SetExecuteThreadNum(int i_intThreadNum)=0;

					//线程启动
					virtual int Start(int i_intStartSeconds)=0;
					virtual int Close() = 0;
					virtual int InitIOCP(void)=0;
					virtual bool InitSocket()=0;
					virtual bool BindAndListenSocket(void)=0;
					virtual bool StartThread(void)=0;
					virtual bool GetFunPointer(void)=0;
					virtual bool PostAcceptEx(void)=0;
					virtual bool RegAcceptEvent(void)=0;
					virtual void SetThreadCPU(int)=0;

					//////////////////////////////////////////////////////////////////////////
					virtual void InitIoContext(IOCP_IO* lp_io)=0;
					virtual void Remove(IOCP_IO* lp_io,IOCP_KEY* lp_key)=0;
					//virtual bool HandleData(IOCP_IO* lp_io, int nFlags)=0;
					//virtual bool GetAddrAndPort(IOCP_IO* lp_io)=0;
					virtual bool ContinueRecv(IOCP_IO* lp_io,IOCP_KEY* lp_key)=0;
					//virtual bool MsgSend(SOCKET s,char* buf,int len)=0;
					//virtual bool MsgSend()=0;
					virtual bool MainLoop(void)=0;

					//设置外部函数
					//virtual void SetBehaviorMsgDeal(SOCKET s,char* buf,int len)=0;
					//virtual void SetBehaviorMsgSend(SOCKET s,char* buf,int len)=0;
					//virtual void SetBehaviorQuit(void)=0;

					virtual void SetBehaviorMsgDeal(BaseBehavior::BaseBehaviorMsgDeal* i_lpBaseBehaviorMsgDeal)=0;
					//virtual void SetBehaviorMsgSend(BaseBehavior::BaseBehaviorMsgSend* i_lpBaseBehaviorMsgSend)=0;
					//virtual void SetBehaviorQuit(BaseBehavior::BaseBehaviorQuit* i_lpBaseBehaviorBehaviorQuit)=0;
					virtual void SetIBMsgDealManage(BaseBehavior::IBMsgDealManage* i_lpIBMsgDealManage)=0;
					
					int operator[]( int idx );
				};
			}
		}
	}
}