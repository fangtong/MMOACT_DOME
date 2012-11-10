#pragma once
#include "inetworksession.h"
#include <memory>
#include "ThreadingModel.h"
#include "DataStruct.h"
#include "MsgPackage.h"
#include <vector>
#include "FinalClass.h"
#include "INetWorkExpend.h"
#include "LockedMap.h"
#include "GroupManager.h"
#include "networksessiondefind.h"

using namespace SevenSmile::GameFramework;
using namespace SevenSmile::Util::Container;
using namespace std::tr1;
using namespace SevenSmile::Manage;

namespace SevenSmile
{
	namespace GameFramework
	{
		class NetWorkSession :
			public SevenSmile::GameFramework::INetWorkSession
		{
		public:
			NetWorkSession();
			~NetWorkSession(void);

		public:

			bool			Deal(char* i_lpChar,int i_charArrLength);
			bool			Deal(VctMsgPkg& i_msgPkg);
			void			Send(const char* msg,const int len);
			void			SendToLocal(const char* msg,const int len);

			virtual void	Kick();
			virtual void	Quit();
			bool			isQuit(){ return _isQuit; }

			void			SetExpend(shared_ptr<INetWorkExpend> i_sPtr);
			shared_ptr<INetWorkExpend>	GetExpend() { return _expand; }

			virtual IOCP_IO*	GetIOCP() { return _lpIocp; }
			virtual void		SetIOCP(IOCP_IO* i_iocp);

			virtual shared_ptr<NetWorkDelegate> GetNetWorkDelegate(){ return _sPtrNetWorkDelegate;}
			virtual void		SetNetWorkDelegate(shared_ptr<NetWorkDelegate> i_delegate){ _sPtrNetWorkDelegate = i_delegate; }

			virtual void AddToObjectManage();
			virtual void DelFromObjectManage();

			virtual void SetObjectKey(std::string i_key);
			virtual std::string GetObjectKey() { return _key; }

			virtual void SendGroupS( Group_enum _GroupEnum , const char* msg ,const int len );
			virtual bool PushGroupS( Group_enum _GroupEnum , string i_key , shared_ptr<INetWorkSession> session);
			virtual bool EraseGroupS( Group_enum _GroupEnum , shared_ptr<INetWorkSession> session);
/*			virtual void SetGroupMananger( Group_enum _GroupEnum , string i_key );*/

			virtual unsigned short	GetSessionPort() { return m_port; };
			virtual std::string& GetSessionAddr() { return m_addr; };
			virtual std::string	GetSessionIp() { return m_ip; };
		protected:
			IOCP_IO* _lpIocp;
			shared_ptr<INetWorkExpend> _expand;
			LockedStdVector<shared_ptr<MsgPackage>> _vctReciveList;
			shared_ptr<NetWorkDelegate> _sPtrNetWorkDelegate;
			bool _isQuit;
			std::string _key;
			std::map<Group_enum , string> m_GroupKey;
			unsigned short					m_port;
			std::string							m_addr;
			string								m_ip;
		};
	}
}
