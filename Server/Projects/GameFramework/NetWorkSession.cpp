#include "NetWorkDelegate.h"
#include "NetWorkSession.h"
#include "INetWorkExpend.h"

#include <memory>
#include "MsgPackageManage.h"
#include "MsgPackage.h"
#include "ObjectManage.h"
#include <assert.h>
#include "GroupManager.h"
#include <xstring>
#include <wsipv6ok.h>
#include <memory.h>
#include <winsock.h>
#include "StringHelper.h"

using namespace std;
using namespace SevenSmile::Net;
using namespace SevenSmile::Manage;
using namespace SevenSmile::Util::StringHelper;

void NetWorkSession::SetExpend( shared_ptr<INetWorkExpend> i_sPtr )
{
	if(_expand) _expand->SetNetWorkSession(NullShardPtr(INetWorkSession));
	_expand = i_sPtr;
	if(_expand) _expand->SetNetWorkSession(shared_from_this());
}


void NetWorkSession::Quit()
{
	_isQuit = true;
	//SetIOCP(NULL);
	DelFromObjectManage();
	EraseGroupS(HALL_GROUP,shared_from_this());
	if(_sPtrNetWorkDelegate)
	{
		NetWorkDelegate::LockedSetLpSession& quitSessions = 
			_sPtrNetWorkDelegate->GetSetQuitSession();
		NetWorkDelegate::LockedSetLpSession::Lock lock(quitSessions);
		quitSessions.insert(shared_from_this());
	}
}

void NetWorkSession::Send( const char* msg,const int len )
{
	if(_isQuit 
	|| !_lpIocp 
	|| !_sPtrNetWorkDelegate
	|| !msg 
	|| len == 0) return;

	NetWorkDelegate::HashIocpToMsg& hashSendMsg = 
		_sPtrNetWorkDelegate->GetHashIocpToSendMsg();
	NetWorkDelegate::HashIocpToMsg::Lock lock(hashSendMsg);
	NetWorkDelegate::HashIocpToMsg::iterator it =  hashSendMsg.find(_lpIocp);
	shared_ptr<string> sendMsg;
	if(hashSendMsg.end() != it)
	{
		sendMsg = it->second;
	}
	if(!sendMsg)
	{
		sendMsg = shared_ptr<string>(new string());
		hashSendMsg[_lpIocp] = sendMsg;
	}
	sendMsg->append(msg,len);
}

bool NetWorkSession::Deal( char* i_lpChar,int i_charArrLength )
{
	MsgPackageManage packageManage(i_lpChar,i_charArrLength);
	MsgPackageArr packages = packageManage.GetMsgPackages();
	size_t length = packages.size();
	
	shared_ptr<NetWorkDelegate> tempNetWorkDelegate = _sPtrNetWorkDelegate;
	if(!tempNetWorkDelegate) return false;

	for (size_t i=0;i< length;i++)
	{
		if(_isQuit) break;
		shared_ptr<MsgPackage> packageMsg = shared_ptr<MsgPackage>(new MsgPackage());
		packageMsg->UnPackage(packages[i]->lpcMsgPackage,packages[i]->uiPackageLength);
		tempNetWorkDelegate->ExecuteReciveDelegateHandle(packageMsg,shared_from_this());
	}
	return true;
}

bool SevenSmile::GameFramework::NetWorkSession::Deal( VctMsgPkg& i_msgPkg )
{
	size_t length = i_msgPkg->size();

	shared_ptr<NetWorkDelegate> tempNetWorkDelegate = _sPtrNetWorkDelegate;
	if(!tempNetWorkDelegate) return false;

	for (size_t i=0;i< length;i++)
	{
		if(_isQuit) break;
		tempNetWorkDelegate->ExecuteReciveDelegateHandle((*i_msgPkg)[i],shared_from_this());
	}
	return true;
}

NetWorkSession::~NetWorkSession( void )
{
	if(_expand)
	{
		_expand->SetNetWorkSession(NullShardPtr(INetWorkSession));
	}
	SetNetWorkDelegate(NullShardPtr(NetWorkDelegate));
}

NetWorkSession::NetWorkSession( )
:_isQuit(false),
_lpIocp(0),
_sPtrNetWorkDelegate(NullShardPtr(NetWorkDelegate))
{
}

void SevenSmile::GameFramework::NetWorkSession::SendToLocal( const char* msg,const int len )
{
	do 
	{
		if(!_lpIocp) break;
		if(!_sPtrNetWorkDelegate) break;
		if(!_sPtrNetWorkDelegate->GetLpMsgDeal()) break;
		if(!msg)break;
		_sPtrNetWorkDelegate->GetLpMsgDeal()->MsgDeal(
			this->_lpIocp,(char*)msg,len);
	} while (0);
}

void SevenSmile::GameFramework::NetWorkSession::AddToObjectManage()
{
	//shared_ptr<INetWorkSession> oldSession = ObjectManage<INetWorkSession>::GetObj(GetObjectKey());
	//if(oldSession) { oldSession->DelFromObjectManage(); }
	ObjectManage<INetWorkSession>::DelObj(GetObjectKey());
	ObjectManage<INetWorkSession>::AddObj(GetObjectKey(),shared_from_this());
}

void SevenSmile::GameFramework::NetWorkSession::DelFromObjectManage()
{
	shared_ptr<INetWorkSession> session = ObjectManage<INetWorkSession>::GetObj(GetObjectKey());
	if(session && session.get() != this) return;
	ObjectManage<INetWorkSession>::DelObj(GetObjectKey());
}

void SevenSmile::GameFramework::NetWorkSession::SetObjectKey( std::string i_key )
{
	_key = i_key; 
	_isQuit = false;
	AddToObjectManage();
}

void SevenSmile::GameFramework::NetWorkSession::SendGroupS( Group_enum _GroupEnum , const char* msg ,const int len )
{
	if(!_sPtrNetWorkDelegate) return;
	map<Group_enum , string>::iterator it = m_GroupKey.find(_GroupEnum);
	if (it  == m_GroupKey.end())
		return;
	NetWorkDelegate::LMapGroup::Lock lock(_sPtrNetWorkDelegate->m_MapGroupManager);
	shared_ptr<GroupManager<INetWorkSession>> sPtrGroup = _sPtrNetWorkDelegate->m_MapGroupManager[it->second];
	GroupManager<INetWorkSession>::MapGroups::iterator INWSit =  sPtrGroup->GetGroups().begin();
	for ( ; INWSit !=  sPtrGroup->GetGroups().end() ; INWSit++ )
	{
		shared_ptr<INetWorkSession> INWS = INWSit->second;
		INWS->Send(msg,len);
	}
}

bool SevenSmile::GameFramework::NetWorkSession::PushGroupS( Group_enum _GroupEnum , std::string i_key , shared_ptr<INetWorkSession> session )
{
	if(!_sPtrNetWorkDelegate) return false;
	//if(session == shared_from_this()) return false;
	if (m_GroupKey.find(_GroupEnum)!= m_GroupKey.end())
		return false;
	m_GroupKey[_GroupEnum] = i_key;
	NetWorkDelegate::LMapGroup::Lock lock(_sPtrNetWorkDelegate->m_MapGroupManager);
	shared_ptr<GroupManager<INetWorkSession>> sPtrGroup = _sPtrNetWorkDelegate->m_MapGroupManager[i_key];
	if(!sPtrGroup)
	{
		_sPtrNetWorkDelegate->m_MapGroupManager[i_key] = shared_ptr<GroupManager<INetWorkSession>>(new GroupManager<INetWorkSession>);
		sPtrGroup = _sPtrNetWorkDelegate->m_MapGroupManager[i_key];
	}
	return sPtrGroup->AddGroupRtKey(session) != 0;
}

bool SevenSmile::GameFramework::NetWorkSession::EraseGroupS( Group_enum _GroupEnum , shared_ptr<INetWorkSession> session )
{
	if(!_sPtrNetWorkDelegate) return false;
	map<Group_enum , string>::iterator it = m_GroupKey.find(_GroupEnum);
	if (it == m_GroupKey.end())
		return false;
	NetWorkDelegate::LMapGroup::Lock lock(_sPtrNetWorkDelegate->m_MapGroupManager);
	shared_ptr<GroupManager<INetWorkSession>> sPtrGroup = _sPtrNetWorkDelegate->m_MapGroupManager[it->second];
	m_GroupKey.erase(it);
	return sPtrGroup->DelGroupByKey(session);
}

// void SevenSmile::GameFramework::NetWorkSession::SetGroupMananger( Group_enum _GroupEnum , std::string i_key )
// {
// 	if(!_sPtrNetWorkDelegate) return;
// 	NetWorkDelegate::LMapGroup::Lock lock(_sPtrNetWorkDelegate->m_MapGroupManager);
// 	NetWorkDelegate::LMapGroup::iterator it = _sPtrNetWorkDelegate->m_MapGroupManager.find(i_key);
// 	if (it == _sPtrNetWorkDelegate->m_MapGroupManager.end())
// 		_sPtrNetWorkDelegate->m_MapGroupManager[i_key] = shared_ptr<GroupManager<INetWorkSession>>(new GroupManager<INetWorkSession>);
// 	PushGroupS(_GroupEnum , i_key , shared_from_this() );
// }

void SevenSmile::GameFramework::NetWorkSession::SetIOCP( IOCP_IO* i_iocp )
{
	//TODO FIX
	//该函属会被ClearSessionThreadJob.do()调用
	//将导致判断_lpIocp!=NULL后的代码出问题
	_lpIocp = i_iocp; 
	if(!_lpIocp) return;

	sockaddr_in addrMy;
	memset(&addrMy,0,sizeof(addrMy));
	int len = sizeof(addrMy);

	//int ret = getsockname(_lpIocp->socket,(sockaddr*)&addrMy,&len);
	int ret = getpeername(_lpIocp->socket,(sockaddr*)&addrMy,&len);

	if (ret == 0)
	{
		m_port		= addrMy.sin_port;
		m_addr.append(inet_ntoa(addrMy.sin_addr));
		m_addr.append(ToString(m_port));
		m_ip			= inet_ntoa(addrMy.sin_addr);
	}
}

void SevenSmile::GameFramework::NetWorkSession::Kick()
{
	SetExpend(NullShardPtr(INetWorkExpend));
	Quit();
	//if(_lpIocp && _lpIocp->socket)
	//{
	//	printf("NetWorkSession Close %s! \n",m_addr.c_str());
	//	int res = shutdown(_lpIocp->socket,SD_SEND);
	//	if(res != 0) printf("CloseSocket Field %d!\n",WSAGetLastError());
	//}
}