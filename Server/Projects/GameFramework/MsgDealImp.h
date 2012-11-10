#pragma once
#include <hash_map>
#include "basebehaviormsgdeal.h"
#include "ThreadingModel.h"
#include "DataStruct.h"


using namespace SevenSmile::Net;
using namespace SevenSmile::Net::IOCPFramework;
using namespace SevenSmile::Util::ThreadHelper;
using namespace std::tr1;

namespace SevenSmile{
	namespace GameFramework
	{
		class NetWorkDelegate;

		class MsgDealImp:
			public SevenSmile::Net::IOCPFramework::BaseBehavior::BaseBehaviorMsgDeal
			//public SevenSmile::Util::ThreadHelper::ObjectLevelLockable<MsgDealImp>
		{
		public:
			explicit MsgDealImp(NetWorkDelegate* i_netWorkDelegate);
			~MsgDealImp(void);
		public:
			bool MsgDeal(IOCP_IO* i_iocpIO,char* i_lpChar,int i_charArrLength);
			//收信息
			bool MsgQuit(IOCP_IO* i_iocpIO);
			//退出

		public:
			bool Send(IOCP_IO* i_iocpIO,char* i_lpChar,unsigned int i_charArrLength);

		private:
			NetWorkDelegate* _lpNetWorkDelegate;
		};
	}
}

