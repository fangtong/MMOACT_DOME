#include "BaseBehaviorMsgDeal.h"

namespace SevenSmile{
	namespace Net{
		namespace IOCPFramework{
			namespace BaseBehavior{
				void BaseBehaviorMsgDeal::SetBehaviorMsgSend(BaseBehaviorMsgSend* obj){
					this->interfaceBehaviorMsgSend=obj;
				}

				bool BaseBehaviorMsgDeal::Send(
					IOCP_IO* i_iocpIO,
					char* i_lpChar,
					unsigned int i_charArrLength,
					bool b_mainLogic)
				{
					if(!i_iocpIO) return false;
					bool res=true;
					unsigned int startPos = 0;
					unsigned int msgLength = i_charArrLength;
					while (startPos < i_charArrLength)
					{
						if(msgLength >= BUFFER_SIZE)
						{
							msgLength = BUFFER_SIZE;
						}
						res=this->interfaceBehaviorMsgSend->Send(i_iocpIO,i_lpChar+startPos,msgLength);
						if(!res) break;
						startPos += msgLength;
						msgLength = i_charArrLength - startPos;
					}
					

					if(res==false){
						if(b_mainLogic==false){
							this->MsgQuit(i_iocpIO);
						}
					}
					return res;
				}

			}
		}
	}
}