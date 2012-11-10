#pragma once

#include <memory>
#include <hash_map>
#include "LockedSet.h"
#include "ThreadingModel.h"
#include "LockedHashMap.h"
#include "INetWorkSession.h"
#include "Singleton.h"
#include "GroupManager.h"
#include "LockedMap.h"



namespace SevenSmile
{
	namespace Net
	{
		class MsgPackage;
		namespace IOCPFramework
		{
			class IOCP;
			namespace BaseBehavior
			{
				class BaseBehaviorMsgDeal;
				class IBMsgDealManage;
			}
		}
	}
}
using namespace SevenSmile::Util::Container;
using namespace SevenSmile::Net::IOCPFramework ;
using namespace SevenSmile::Net::IOCPFramework::BaseBehavior;
using namespace SevenSmile::Net;
using namespace std::tr1;
using namespace SevenSmile::Util::ThreadHelper;
using namespace SevenSmile::Manage;
using namespace SevenSmile::Util::Container;
namespace SevenSmile{
	namespace GameFramework{

		class NetWorkDelegate
			:public SevenSmile::Util::ThreadHelper::ObjectLevelLockable<NetWorkDelegate>,
			public enable_shared_from_this<NetWorkDelegate>
		{
			friend class NetWorkSession;
		public:
			typedef LockedMap<string ,shared_ptr<GroupManager<INetWorkSession>>> LMapGroup;
		public:
			typedef LockedHashMap<IOCP_IO*,shared_ptr<INetWorkSession>> HashIocpToSession;//×éÓ³Éä
			typedef bool (*ReciveDelegateHandle)(shared_ptr<MsgPackage> const i_sPtrData,shared_ptr<INetWorkSession> const i_sPtrSession);
			typedef stdext::hash_map<unsigned int,ReciveDelegateHandle> HashUintToReciveDelegateHandle;
			typedef LockedSet<shared_ptr<INetWorkSession>> LockedSetLpSession;

			typedef LockedHashMap<IOCP_IO*,shared_ptr<string>> HashIocpToMsg;
			typedef LockedHashMap<IOCP_IO*,VctMsgPkg>				HashIocpToPkg;

			NetWorkDelegate(HANDLE hEventRecive);
			virtual ~NetWorkDelegate(void);

		public:
			bool Connect(const char* i_cIOCPAddress, unsigned int i_uiIOCPPort);

			void RegisterReciveDelegateHandle(unsigned int i_action,ReciveDelegateHandle i_handle);
			void RemoveReciveDelegateHandle(unsigned int i_action,ReciveDelegateHandle i_handle);
			void ExecuteReciveDelegateHandle(shared_ptr<MsgPackage> const i_sPtrData,shared_ptr<INetWorkSession> const i_sPtrSession);

			unsigned int ReciveAll();
			void SendAll();
			void ClearExitSession();
			void SendToLocalGlobal(const char* i_msg, unsigned int i_len);

			bool							RemoveSession(IOCP_IO* i_iocp);
			shared_ptr<INetWorkSession>		InsertSession(IOCP_IO* i_iocp);
			shared_ptr<INetWorkSession>		FindSession(IOCP_IO* i_iocp);
			shared_ptr<INetWorkSession>		DelSession( IOCP_IO* i_iocp );

			void CloseAllSession();

			virtual shared_ptr<INetWorkSession> CreateSession(IOCP_IO* i_iocp);

			HashIocpToSession&						GetSessionList()	{ //Lock lock(*this); 
				return _hashIocpToSession; }
			HashIocpToMsg& GetHashIocpToSendMsg()	{ return _hashIocpToSendMsg; }
			HashIocpToPkg& GetHashToReciveMsg()		{ return _hashIocpToReciveMsg; }
			LockedSetLpSession& GetSetQuitSession() { return _setQuitSession; }

			BaseBehaviorMsgDeal* GetLpMsgDeal() const { return _lpMsgDeal; }

			std::string GetIP() const { return m_ip; }
			static std::string GetSocketStr(IOCP_IO* i_io);

			unsigned int GetDisConnSessionCount();
			unsigned int GetSessionCount();

			void NotifyRecive();
		private:
			shared_ptr<IOCP> _sPtrIocp;
			BaseBehaviorMsgDeal* _lpMsgDeal;
			IBMsgDealManage* _lpMsgDealManage;

			HashUintToReciveDelegateHandle  _hashUintToReciveDelegateHandle;
			HashIocpToSession				_hashIocpToSession;

			HashIocpToMsg		_hashIocpToSendMsg;
			HashIocpToPkg		_hashIocpToReciveMsg;

			LockedSetLpSession		_setQuitSession;

			std::string m_ip;

			LMapGroup m_MapGroupManager;

			HANDLE    _hEventRecive;
		};

	}
}

#define SPtrNetWorkManager SevenSmile::Util::DesignPatterns::Singleton<stdext::shared_ptr<NetWorkDelegate>>::instance()
