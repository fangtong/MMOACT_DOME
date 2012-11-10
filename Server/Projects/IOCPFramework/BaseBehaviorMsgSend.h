#include "stdafx.h"
#pragma once
#include "DataStruct.h"

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace BaseBehavior{
				class BaseBehaviorMsgSend
				{
				public:
					//IO_GROUP _ioSendGroup;
					BaseBehaviorMsgSend();
					virtual ~BaseBehaviorMsgSend();
					bool Send(
						IOCP_IO* i_lp_io,
						char* i_lpChar,
						int i_charArrLength
						);
					void Remove(IOCP_IO* lp_io);

				private:
					
					IO_GROUP _ioSendGroup;
					//IOCP_IO* _lpIocpIO;
				};

			}
		}
	}
}