#pragma once
#include <memory>
#include "AbsObject.h"
#include "imanageobject.h"
#include "iobject.h"
#include <xstring>
#include "networksessiondefind.h"

using namespace std::tr1;
namespace SevenSmile
{
	namespace GameFramework
	{
		class INetWorkSession;
		class INetWorkExpend
			//:public SevenSmile::Core::AbsObject
			:public virtual SevenSmile::Core::IObject,
			public virtual SevenSmile::Core::IManagedObject
		{
		public:
			virtual ~INetWorkExpend(void){};

			virtual void SetNetWorkSession(shared_ptr<INetWorkSession> i_session) = 0;
			virtual shared_ptr<INetWorkSession> GetNetWorkSession() = 0;
			virtual void Send(const char* i_msg,unsigned int i_le) = 0;
			virtual void SendToLocal(const char* i_msg,unsigned int i_le) = 0;
			virtual void SendGroupS( Group_enum _GroupEnum , const char* i_msg , unsigned int i_le) = 0;
/*			virtual void SetGroupMananger(Group_enum _GroupEnum , std::string i_key ) = 0;*/
		};
	}
}

