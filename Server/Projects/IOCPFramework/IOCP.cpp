#include "IOCP.h"
#include "BehaviorCreateMainThread.h"
#include "BehaviorDefaultMsgDeal.h"
#include "BehaviorDefaultMsgDealManage.h"

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			IOCP::IOCP(void)
			{
				this->_strIPAddr="127.0.0.1";
				this->_intPort=5555;
				this->_intExecuteThreadNumOfIOCPTaskList=10;

				this->_lpBaseBehaviorMainThread=new Behavior::BehaviorCreateMainThread();
				this->_lpBaseBehaviorMsgDeal=NULL;
				this->_lpIBMsgDealManage=NULL;
				//this->_lpBaseBehaviorMainThread=NULL;
				//this->_lpBaseBehaviorMsgSend=NULL;
				//this->_lpBaseBehaviorQuit=NULL;
			}

			IOCP::IOCP(char* i_strIPAddr, int i_intPort, int i_intExecuteThreadNumOfIOCPTaskList){
				IOCP();

				this->_strIPAddr=i_strIPAddr;
				this->_intPort=i_intPort;
				this->_intExecuteThreadNumOfIOCPTaskList=i_intExecuteThreadNumOfIOCPTaskList;
			}

			IOCP::~IOCP(void)
			{
				if (NULL!=_lpBaseBehaviorMainThread){
					delete this->_lpBaseBehaviorMainThread;
					this->_lpBaseBehaviorMainThread=NULL;
				}

				if (NULL!=_lpBaseBehaviorMsgDeal){
					delete this->_lpBaseBehaviorMsgDeal;
					this->_lpBaseBehaviorMsgDeal=NULL;
				}

				if (NULL!=_lpIBMsgDealManage){
					delete this->_lpIBMsgDealManage;
					this->_lpIBMsgDealManage=NULL;
				}
			}

			void IOCP::SetIpAddress(const char* i_strAddr){
				this->_strIPAddr=i_strAddr;
			}

			void IOCP::MyIOCPSetPort(int i_intPort){
				this->_intPort=i_intPort;
			}

			void IOCP::SetPort(int i_intPort){
				this->_intPort=i_intPort;
			}

			void IOCP::SetExecuteThreadNumOfIOCPTaskList(int i_intExecuteThreadNumOfIOCPTaskList){
				this->_intExecuteThreadNumOfIOCPTaskList=i_intExecuteThreadNumOfIOCPTaskList;
			}

			void IOCP::SetBehaviorMainThread(BaseBehavior::BaseBehaviorMainThread* i_lpBaseBehaviorMainThread){
				if (NULL!=this->_lpBaseBehaviorMainThread){
					delete this->_lpBaseBehaviorMainThread;
					this->_lpBaseBehaviorMainThread=NULL;
				}
				this->_lpBaseBehaviorMainThread=i_lpBaseBehaviorMainThread;
			}

			void IOCP::SetBehaviorMsgDeal(BaseBehavior::BaseBehaviorMsgDeal* i_lpBaseBehaviorMsgDeal){
				if (NULL!=this->_lpBaseBehaviorMsgDeal){
					delete this->_lpBaseBehaviorMsgDeal;
					this->_lpBaseBehaviorMsgDeal=NULL;
				}
				this->_lpBaseBehaviorMainThread->SetBehaviorMsgDeal(i_lpBaseBehaviorMsgDeal);
			}

			void IOCP::SetBehaviorMsgDealManage( BaseBehavior::IBMsgDealManage* i_lpIBMsgDealManage )
			{
				if (NULL!=this->_lpIBMsgDealManage){
					delete this->_lpIBMsgDealManage;
					this->_lpIBMsgDealManage=NULL;
				}
				this->_lpBaseBehaviorMainThread->SetIBMsgDealManage(i_lpIBMsgDealManage);
			}

			//void IOCP::SetBehaviorMsgSend(BaseBehavior::BaseBehaviorMsgSend* i_lpBaseBehaviorMsgSend){
			//	this->_lpBaseBehaviorMainThread->SetBehaviorMsgSend(i_lpBaseBehaviorMsgSend);
			//}

			//void IOCP::SetBehaviorQuit(BaseBehavior::BaseBehaviorQuit* i_lpBaseBehaviorBehaviorQuit){
			//	this->_lpBaseBehaviorMainThread->SetBehaviorQuit(i_lpBaseBehaviorBehaviorQuit);
			//}

			int IOCP::Start(){
				//if (NULL==this->_lpBaseBehaviorMainThread){
				//	this->SetBehaviorMainThread(new Behavior::BehaviorCreateMainThread());
				//}

				if (NULL==this->_lpBaseBehaviorMainThread->_lpBaseBehaviorMsgDeal){
					this->SetBehaviorMsgDeal(new Behavior::BehaviorDefaultMsgDeal);
				}

				if (NULL==this->_lpBaseBehaviorMainThread->_lpIBMsgDealManage){
					this->SetBehaviorMsgDealManage(new Behavior::BehaviorDefaultMsgDealManage);
				}

				this->_lpBaseBehaviorMainThread->SetListenSoketIPAddr(this->_strIPAddr);
				this->_lpBaseBehaviorMainThread->SetListenSoketPort(this->_intPort);
				this->_lpBaseBehaviorMainThread->SetExecuteThreadNum(this->_intExecuteThreadNumOfIOCPTaskList);
				_intErrRes=this->_lpBaseBehaviorMainThread->Start(0);
				return _intErrRes;
			}

			int IOCP::Stop()
			{
				return this->_lpBaseBehaviorMainThread->Close();
			}
			const char* IOCP::GetErrorStr()
			{
				if(_intErrRes==0)
				{
					return "It's OK";
				}
				if(_intErrRes==1)
				{
					return "WSAStartup fail!";
				}
				if(_intErrRes==2)
				{
					return "CreateIoCompletionPort fail!";
				}
				if(_intErrRes==3)
				{
					return "Init sociket fail!";
				}
				if(_intErrRes==4)
				{
					return "start thread fail!";
				}
				if(_intErrRes==5)
				{
					return "BindAndListenSocket fail!";
				}
				if(_intErrRes==6)
				{
					return "GetFunPointer fail!";
				}
				if(_intErrRes==7)
				{
					return "PostAcceptEx fail!";
				}
				if(_intErrRes==8)
				{
					return "RegAcceptEvent fail!";
				}

				return "UnKnow Result!";
			}

			//bool IOCP::MsgDeal(SOCKET i_soket,char* i_lpChar,int i_charArrLength){
			//	this->_lpBaseBehaviorMsgDeal->MsgDeal(i_soket,i_lpChar,i_charArrLength);
			//	return true;
			//}

			//bool IOCP::MsgSend(SOCKET i_soket,char* i_lpChar,int i_charArrLength){
			//	//this->_lpBaseBehaviorMsgSend->Send(i_soket,i_lpChar,i_charArrLength);
			//	this->_lpBaseBehaviorMsgDeal->Send(i_soket,i_lpChar,i_charArrLength);
			//	return true;
			//}

			//bool IOCP::Quit(SOCKET i_soket){
			//	this->_lpBaseBehaviorQuit->Quit(i_soket);
			//	return true;
			//}

			string IOCP::GetIpAddress(void){
				return this->_strIPAddr;
			}

			int IOCP::GetPort(void){
				return this->_intPort;
			}

			int IOCP::GetExecuteThreadNumOfIOCPTaskList(){
				return this->_intExecuteThreadNumOfIOCPTaskList;
			}

			void IOCP::SetThreadCPU( int _mask){
				this->_lpBaseBehaviorMainThread->SetThreadCPU(_mask);
			}

		}
	}
}