#include "NetWorkDelegate.h"
#include "MsgDealImp.h"
#include "myIBMsgDealManage.h"
#include "IOCP.h"
#include "INetWorkSession.h"
#include "FinalClass.h"
#include <string>
#include <memory>
#include "Log.h"
#include <assert.h>
#include <set>
#include "StringHelper.h"
#include <winsock.h>
#include "ObjectManage.h"

using namespace SevenSmile::GameFramework;
using namespace std::tr1;
using namespace SevenSmile::LogNameSpace;
using namespace SevenSmile::Util::StringHelper;
using namespace SevenSmile::Manage;

SevenSmile::GameFramework::NetWorkDelegate::NetWorkDelegate( HANDLE hEventRecive )
:_lpMsgDeal(0),
_lpMsgDealManage(0),
_hEventRecive(hEventRecive)
{
	_sPtrIocp= shared_ptr<IOCP>(new IOCP());
}

SevenSmile::GameFramework::NetWorkDelegate::~NetWorkDelegate( void )
{
	Lock lock(*this);
	_hashUintToReciveDelegateHandle.clear();
	_hashIocpToReciveMsg.clear();
	_hashIocpToSendMsg.clear();
	_hashIocpToSession.clear();
	_setQuitSession.clear();
	_sPtrIocp->SetBehaviorMsgDeal(0);
	_sPtrIocp->SetBehaviorMsgDealManage(0);
	ObjectManage<INetWorkSession>::Destroy();
	if(_lpMsgDeal)
	{
		delete _lpMsgDeal;
		_lpMsgDeal = 0;
	}
	if(_lpMsgDealManage)
	{
		delete _lpMsgDealManage;
		_lpMsgDealManage = 0;
	}
}

bool SevenSmile::GameFramework::NetWorkDelegate::Connect( const char* i_cIOCPAddress, unsigned int i_uiIOCPPort )
{
	_lpMsgDeal = new MsgDealImp(this);
	_sPtrIocp->SetBehaviorMsgDeal(_lpMsgDeal);
	_lpMsgDealManage = new myIBMsgDealManage(i_uiIOCPPort);
	_sPtrIocp->SetBehaviorMsgDealManage(_lpMsgDealManage);
	_sPtrIocp->SetIpAddress(i_cIOCPAddress);
	_sPtrIocp->MyIOCPSetPort(i_uiIOCPPort);
	_sPtrIocp->SetExecuteThreadNumOfIOCPTaskList(4);
	{
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		::WSAStartup(sockVersion, &wsaData);

		char lv_name[50];
		gethostname(lv_name, 50);
		hostent * lv_pHostent;
		if( NULL == (lv_pHostent = gethostbyname(lv_name)))
		{
			printf("get Hosrname Fail \n");
			assert(0);
		}
		SOCKADDR_IN lv_sa;
		memcpy(&lv_sa.sin_addr.S_un.S_addr, lv_pHostent->h_addr_list[0], lv_pHostent->h_length);
		m_ip = inet_ntoa(lv_sa.sin_addr);
	}

	return (_sPtrIocp->Start() == 0);
}


bool SevenSmile::GameFramework::NetWorkDelegate::RemoveSession( IOCP_IO* i_iocp )
{
	HashIocpToSession::Lock lock(_hashIocpToSession);
	HashIocpToSession::iterator itr = _hashIocpToSession.find(i_iocp);
	if(_hashIocpToSession.end() == itr)
	{
		return false;
	}
	_hashIocpToSession.erase(itr);
	return true;
}


shared_ptr<INetWorkSession> SevenSmile::GameFramework::NetWorkDelegate::InsertSession( IOCP_IO* i_iocp )
{
	shared_ptr<INetWorkSession>	res = NullShardPtr(INetWorkSession);
	{
		HashIocpToSession::Lock lock(_hashIocpToSession);
		HashIocpToSession::iterator itr = _hashIocpToSession.find(i_iocp);
		if(_hashIocpToSession.end() != itr)
		{
			res = itr->second;
		}
	}
	if(!res){
		res = CreateSession(i_iocp);
		res->SetNetWorkDelegate(shared_from_this());
		res->SetIOCP(i_iocp);
		printf("CreateSession %s! \n",NetWorkDelegate::GetSocketStr(i_iocp).c_str());
		HashIocpToSession::Lock lock(_hashIocpToSession);
		_hashIocpToSession[i_iocp] = res;
	}
	assert(_hashIocpToSession[i_iocp]);
	return res;
}

shared_ptr<INetWorkSession> SevenSmile::GameFramework::NetWorkDelegate::FindSession( IOCP_IO* i_iocp )
{
	HashIocpToSession::Lock lock(_hashIocpToSession);
	HashIocpToSession::iterator itr = _hashIocpToSession.find(i_iocp);
	shared_ptr<INetWorkSession>	res = NullShardPtr(INetWorkSession);
	if(itr != _hashIocpToSession.end())
		res = itr->second;
	return res;
}

shared_ptr<INetWorkSession> SevenSmile::GameFramework::NetWorkDelegate::DelSession( IOCP_IO* i_iocp )
{
	HashIocpToSession::Lock lock(_hashIocpToSession);
	HashIocpToSession::iterator itr = _hashIocpToSession.find(i_iocp);
	shared_ptr<INetWorkSession>	res = NullShardPtr(INetWorkSession);
	if(itr != _hashIocpToSession.end())
	{
		res = itr->second;
		_hashIocpToSession.erase(itr);
	}
	return res;
}

void SevenSmile::GameFramework::NetWorkDelegate::RegisterReciveDelegateHandle( unsigned int i_action,ReciveDelegateHandle i_handle )
{
	Lock lock(*this);
	HashUintToReciveDelegateHandle::iterator it = 
		_hashUintToReciveDelegateHandle.find(i_action);
	if(it != _hashUintToReciveDelegateHandle.end())
		return;
	_hashUintToReciveDelegateHandle[i_action] = i_handle;
}


void SevenSmile::GameFramework::NetWorkDelegate::RemoveReciveDelegateHandle( unsigned int i_action,ReciveDelegateHandle i_handle )
{
	Lock lock(*this);
	HashUintToReciveDelegateHandle::iterator it = 
		_hashUintToReciveDelegateHandle.find(i_action);
	if(it != _hashUintToReciveDelegateHandle.end())
		return;
	_hashUintToReciveDelegateHandle.erase(it);
}


void SevenSmile::GameFramework::NetWorkDelegate::ExecuteReciveDelegateHandle( shared_ptr<MsgPackage> const i_sPtrData,shared_ptr<INetWorkSession> const i_sPtrSession )
{
	//Lock lock(*this);
#ifdef _DEBUG
	unsigned short act = i_sPtrData->GetPackageAction();
	if(i_sPtrData->GetPackageAction() % 2 != 0){
		printf("Client");
	}else{
		printf("Server");
	}
	printf(" >> ExecuteReciveDelegateHandle Action :%d \n", act);
#endif // _DEBUG
	HashUintToReciveDelegateHandle::iterator it = 
		_hashUintToReciveDelegateHandle.find(i_sPtrData->GetPackageAction());
	if(it == _hashUintToReciveDelegateHandle.end()){
		//		printf(" >> ExecuteReciveDelegateHandle Error!\n");
		i_sPtrSession->Quit();
		return ;
	}
	it->second(i_sPtrData,i_sPtrSession);
}

shared_ptr<INetWorkSession> SevenSmile::GameFramework::NetWorkDelegate::CreateSession( IOCP_IO* i_iocp )
{
	return NullShardPtr(INetWorkSession);
}

unsigned int SevenSmile::GameFramework::NetWorkDelegate::ReciveAll()
{
	HashIocpToPkg hashCopyMsg;
	{
		HashIocpToPkg::Lock lock(_hashIocpToReciveMsg);
		//hashCopyMsg.insert(_hashIocpToReciveMsg.begin(),_hashIocpToReciveMsg.end());
		//_hashIocpToReciveMsg.clear();
		hashCopyMsg.swap(_hashIocpToReciveMsg);
	}

	IOCP_IO* io = 0;
	
	unsigned int count = 0;
	HashIocpToPkg::iterator it = hashCopyMsg.begin();
	for (it; it != hashCopyMsg.end(); it++)
	{
		io = it->first;
		VctMsgPkg& msg = it->second;
		if(//!io || 
			!msg){ continue;}
		shared_ptr<INetWorkSession> session = FindSession(io);
		if(!session) continue;
		//session->Deal((char*)msg->c_str(),msg->length());
		session->Deal(msg);
		count ++;
	}
#ifdef _DEBUG
	if (count > 0)
	{
		printf("ReciveAll Count:%d\n",count);
	}
#endif
	return count;
}

void SevenSmile::GameFramework::NetWorkDelegate::SendAll()
{

	if(!_lpMsgDeal) return;
	unsigned int count = 0;
	HashIocpToMsg copyHashIocpToSendMsg;
	{
		HashIocpToMsg::Lock lock(_hashIocpToSendMsg);
		copyHashIocpToSendMsg.swap(_hashIocpToSendMsg);
	}
	
	HashIocpToMsg::iterator it = copyHashIocpToSendMsg.begin();
	IOCP_IO* io = 0;
	shared_ptr<string> sendMsg = NullShardPtr(string);
	for(it; it != copyHashIocpToSendMsg.end();it++)
	{
		io = it->first;
		sendMsg = it->second;
		if (!sendMsg || !io ) {continue;}
		if(!_lpMsgDeal->Send(io,(char*)sendMsg->c_str(),sendMsg->size(),true))
		{
			_lpMsgDeal->MsgQuit(io);
			printf("Error:Session %s Send Msg! \n",NetWorkDelegate::GetSocketStr(io).c_str());
		}
		count++;

	}
	copyHashIocpToSendMsg.clear();

#ifdef _DEBUG
	if (count > 0)
	{
		printf("SendAll Count:%d\n",count);
	}
#endif
}

void SevenSmile::GameFramework::NetWorkDelegate::ClearExitSession()
{
	shared_ptr<INetWorkSession> sPtrSession = NullShardPtr(INetWorkSession);
	LockedSetLpSession copyList;
	{
		LockedSetLpSession::Lock lock(_setQuitSession);
		copyList.insert(_setQuitSession.begin(),_setQuitSession.end());
		_setQuitSession.clear();
	}
	unsigned int exitCount = 0;
	{
		LockedSetLpSession::iterator it = copyList.begin();

		for (it; it != copyList.end();it++)
		{
			sPtrSession = *it;
			if(sPtrSession)
			{ 
				//sPtrSession->Quit();
				if(!sPtrSession->isQuit()) continue;
				if(sPtrSession->GetIOCP()){
					printf("close socket %s! \n",NetWorkDelegate::GetSocketStr(sPtrSession->GetIOCP()).c_str());
					int res = closesocket(sPtrSession->GetIOCP()->socket);
					//int res = shutdown(sPtrSession->GetIOCP()->socket,SD_SEND);
					if(res != 0) printf("CloseSocket Field %d!\n",WSAGetLastError());
				}
				RemoveSession(sPtrSession->GetIOCP());
				sPtrSession->SetNetWorkDelegate(NullShardPtr(NetWorkDelegate));
				sPtrSession->SetIOCP(NULL); //TODO FIX 这里有问题，将导致其他线程出错
				exitCount++;
			}
		}
		copyList.clear();
	}


	//HashIocpToSession::Lock lock(_hashIocpToSession);
	//HashIocpToSession::iterator it = _hashIocpToSession.begin();
	//unsigned int exitCount = 0;
	//for (it;it != _hashIocpToSession.end();)
	//{
	//	sPtrSession = it->second;
	//	if(sPtrSession
	//		&& sPtrSession->isQuit())
	//	{
	//		//if(sPtrSession->GetIOCP()){
	//		//	//int res = closesocket(sPtrSession->GetIOCP()->socket);
	//		//	printf("close socket %s! \n",NetWorkDelegate::GetSocketStr(sPtrSession->GetIOCP()).c_str());
	//		//	int res = closesocket(sPtrSession->GetIOCP()->socket);
	//		//	if(res != 0) printf("CloseSocket Field %d!\n",WSAGetLastError());
	//		//}
	//		sPtrSession->SetNetWorkDelegate(NullShardPtr(NetWorkDelegate));
	//		it = _hashIocpToSession.erase(it);
	//		exitCount++;
	//		continue;
	//	}
	//	it++;
	//}
#ifdef _DEBUG
	if(exitCount>0){
		printf("ExitSession Count%d\n",exitCount);
	}
#endif
}

void SevenSmile::GameFramework::NetWorkDelegate::SendToLocalGlobal( const char* i_msg, unsigned int i_len )
{
	if(!_lpMsgDeal){
		printf("Error:SendToLocalGlobal Deal is Null!\n");
		return;
	}
	_lpMsgDeal->MsgDeal(0,(char*)i_msg,i_len);
}

void SevenSmile::GameFramework::NetWorkDelegate::CloseAllSession()
{
	Lock lock(*this);
	_sPtrIocp->Stop();
	_hashUintToReciveDelegateHandle.clear();
	
	{
		shared_ptr<INetWorkSession> sPtrSession = NullShardPtr(INetWorkSession);
		HashIocpToSession::Lock lock(_hashIocpToSession);
		HashIocpToSession::iterator it = _hashIocpToSession.begin();
		for (it;it != _hashIocpToSession.end();)
		{
			sPtrSession = it->second;
			if(sPtrSession)
			{
				if(sPtrSession->GetIOCP()){
					int res = closesocket(sPtrSession->GetIOCP()->socket);
					if(res == 0) printf("CloseSocket Field %d!\n",WSAGetLastError());
				}
				sPtrSession->SetNetWorkDelegate(NullShardPtr(NetWorkDelegate));
				it = _hashIocpToSession.erase(it);
				continue;
			}
			it++;
		}
	}
}

std::string SevenSmile::GameFramework::NetWorkDelegate::GetSocketStr( IOCP_IO* i_io )
{
	string res;
	do 
	{
		if(0 == i_io) break;

		sockaddr_in addrMy;
		memset(&addrMy,0,sizeof(addrMy));
		int len = sizeof(addrMy);
		//int ret = getsockname(i_io->socket,(sockaddr*)&addrMy,&len);
		int ret = getpeername(i_io->socket,(sockaddr*)&addrMy,&len);
		if(ret != 0) break;

		res.append(inet_ntoa(addrMy.sin_addr));
		res.append(":");
		res.append(ToString(addrMy.sin_port));
	} while (0);
	return res;
}

unsigned int SevenSmile::GameFramework::NetWorkDelegate::GetDisConnSessionCount()
{
	LockedSetLpSession::Lock lock(_setQuitSession);
	return _setQuitSession.size();
}

unsigned int SevenSmile::GameFramework::NetWorkDelegate::GetSessionCount()
{
	HashIocpToSession::Lock lock(_hashIocpToSession);
	return _hashIocpToSession.size();
}

void SevenSmile::GameFramework::NetWorkDelegate::NotifyRecive()
{
	::SetEvent(_hEventRecive);
}
