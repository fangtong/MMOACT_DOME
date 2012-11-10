#include "MsgDealImp.h"
#include "NetWorkDelegate.h"
#include "DataStruct.h"
#include "MsgPackageManage.h"
#include "MsgPackage.h"

using namespace SevenSmile::GameFramework;
using namespace SevenSmile::Net;

MsgDealImp::MsgDealImp(NetWorkDelegate* i_netWorkDelegate)
:_lpNetWorkDelegate(i_netWorkDelegate)
{

}

MsgDealImp::~MsgDealImp(void)
{
 	_lpNetWorkDelegate = 0;
}

bool MsgDealImp::MsgDeal( IOCP_IO* i_iocpIO,char* i_lpChar,int i_charArrLength )
{
	bool res = false;
	if(_lpNetWorkDelegate)
	{
		do 
		{
			//{//是否退出 踢出
			//	NetWorkDelegate::HashIocpToSession& hashIocpToSession = _lpNetWorkDelegate->GetSessionList();
			//	NetWorkDelegate::HashIocpToSession::Lock lock(hashIocpToSession);
			//	if(hashIocpToSession.end() ==hashIocpToSession.find(i_iocpIO))
			//	{
			//		break;
			//	}
			//}

			//解包操作
			std::vector<shared_ptr<MsgPackage>> vctMsg;
			{
				MsgPackageManage packageManage(i_lpChar,i_charArrLength);
				MsgPackageArr packages = packageManage.GetMsgPackages();
				size_t length = packages.size();
				for (size_t i=0;i< length;i++)
				{
					shared_ptr<MsgPackage> packageMsg = shared_ptr<MsgPackage>(new MsgPackage());
					packageMsg->UnPackage(packages[i]->lpcMsgPackage,packages[i]->uiPackageLength);
					vctMsg.push_back(packageMsg);
				}
			}

			shared_ptr<INetWorkSession> sPtrSession = _lpNetWorkDelegate->InsertSession(i_iocpIO);

			NetWorkDelegate::HashIocpToPkg& hashReciveMsg = 
				_lpNetWorkDelegate->GetHashToReciveMsg();
			NetWorkDelegate::HashIocpToPkg::Lock lock(hashReciveMsg);

			{	//消息拼接 不进行打包
				//NetWorkDelegate::HashIocpToMsg::iterator it =  hashReciveMsg.find(i_iocpIO);
				//shared_ptr<string> reciveMsg;
				//if(hashReciveMsg.end() != it)
				//{
				//	reciveMsg = it->second;	
				//}
				//if(!reciveMsg)
				//{
				//	reciveMsg = shared_ptr<string>(new string());
				//	hashReciveMsg[i_iocpIO] = reciveMsg;
				//}
				//reciveMsg->append(i_lpChar,i_charArrLength);
			}
			
			VctMsgPkg msgPkg = hashReciveMsg[i_iocpIO];
			if(!msgPkg)
			{
				msgPkg = VctMsgPkg(new std::vector<shared_ptr<MsgPackage>>);
				hashReciveMsg[i_iocpIO] = msgPkg;
			}
			msgPkg->insert(msgPkg->end(),vctMsg.begin(),vctMsg.end());
			res = true;
			_lpNetWorkDelegate->NotifyRecive();
		} while (0);
	}
	return 	res;
}


bool MsgDealImp::MsgQuit( IOCP_IO* i_iocpIO )
{
	if(!_lpNetWorkDelegate) return false;

	//shared_ptr<INetWorkSession> sPtrSession = _lpNetWorkDelegate->DelSession(i_iocpIO);
	shared_ptr<INetWorkSession> sPtrSession = _lpNetWorkDelegate->FindSession(i_iocpIO);
	if(!sPtrSession) return false;
	
	sPtrSession->Quit();

	
	printf("MsgQuit %s! \n",NetWorkDelegate::GetSocketStr(i_iocpIO).c_str());
	
	return true;
}
// 
bool MsgDealImp::Send( IOCP_IO* i_iocpIO,char* i_lpChar,unsigned int i_charArrLength )
{
	bool res = false;
	do 
	{
		if(0 == i_iocpIO)
		{
			break;
		}
		unsigned int head = 0;
		unsigned int sendLength = 0;
		while(head < i_charArrLength)
		{
			sendLength = (head + BUFFER_SIZE) < i_charArrLength? BUFFER_SIZE:i_charArrLength-head;
			res = BaseBehaviorMsgDeal::Send(i_iocpIO,i_lpChar+head,sendLength);
			if( !res ) break;
			head += sendLength;
		}
	} while (0);
	return res;
}

