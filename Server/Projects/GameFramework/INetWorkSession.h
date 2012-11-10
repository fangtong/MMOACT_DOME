#pragma once
#include "IOCP.h"
#include "DataStruct.h"
#include "MsgPackage.h"
#include <vector>
#include <memory>
#include "ThreadingModel.h"
#include "INetWorkExpend.h"
#include "LockedStdVector.h"
#include <xstring>
#include "networksessiondefind.h"

using namespace SevenSmile::Net::IOCPFramework ;
using namespace SevenSmile::Net;
using namespace std;
using namespace std::tr1;
using namespace SevenSmile::Util::Container;

namespace SevenSmile{
	namespace GameFramework{

		typedef shared_ptr<std::vector<shared_ptr<MsgPackage>>>				VctMsgPkg;

		class NetWorkDelegate;
		class INetWorkSession
			:public SevenSmile::Util::ThreadHelper::ObjectLevelLockable<INetWorkSession>,
			public SevenSmile::Core::IManagedObject,
			public enable_shared_from_this<INetWorkSession>
		{
		public:
			virtual ~INetWorkSession(void){};

			virtual bool		Deal(char* i_lpChar,int i_charArrLength) = 0;
			virtual bool		Deal(VctMsgPkg& i_msgPkg) = 0;
			virtual void		Send(const char* msg,const int len) = 0;
			virtual void		Quit() = 0;
			virtual void		SendToLocal(const char* msg,const int len) = 0;
			virtual void		Kick() = 0;

			virtual IOCP_IO*	GetIOCP() = 0;
			virtual void		SetIOCP(IOCP_IO* i_iocp) = 0;
			virtual bool		isQuit() = 0;

			virtual shared_ptr<NetWorkDelegate> GetNetWorkDelegate() = 0;
			virtual void		SetNetWorkDelegate(shared_ptr<NetWorkDelegate> i_delegate) = 0;

			virtual shared_ptr<INetWorkExpend> GetExpend() = 0;
			virtual void		SetExpend(shared_ptr<INetWorkExpend> i_expend) = 0;

			virtual void		SendGroupS( Group_enum _GroupEnum , const char* msg ,const int len ) = 0;
			virtual bool		PushGroupS( Group_enum _GroupEnum , std::string i_key , shared_ptr<INetWorkSession> session) = 0;
			virtual bool		EraseGroupS( Group_enum _GroupEnum , shared_ptr<INetWorkSession> session) = 0;
/*			virtual void		SetGroupMananger( Group_enum _GroupEnum , std::string i_key ) = 0;*/
			virtual unsigned short	GetSessionPort() = 0;
			virtual std::string&		GetSessionAddr() = 0;
			virtual std::string	GetSessionIp() = 0;
		};
	}
}