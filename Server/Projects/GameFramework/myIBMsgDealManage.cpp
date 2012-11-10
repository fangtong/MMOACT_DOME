#include "myIBMsgDealManage.h"
#include "NetWorkDelegate.h"
#include "MsgPackageManage.h"
#include <vector>

using namespace SevenSmile::GameFramework;

myIBMsgDealManage::myIBMsgDealManage(unsigned int i_point)
{
	point = i_point;
}

myIBMsgDealManage::~myIBMsgDealManage(void)
{
}
bool myIBMsgDealManage::MsgDealManage( 
									  SevenSmile::Net::IOCPFramework::BaseBehavior::BaseBehaviorMainThread* lpIBMainThread, 
									  IOCP_IO* i_iocpIO, 
									  char* i_lpChar, 
									  int i_charArrLength
									  )
{
 	if(strcmp(i_lpChar,"<policy-file-request/>")==0){
		char res[1024];
		memset(res, '\0', sizeof(res));
		sprintf_s(res,"<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"%d\" /></cross-domain-policy>",point);
		//strcpy_s(xml,"<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"7100\" /></cross-domain-policy>");
		lpIBMainThread->_lpBaseBehaviorMsgDeal->Send(i_iocpIO,res,strlen(res)+1);
		i_iocpIO->player=NULL;
		printf("check flashplayer policy:%s!\n",NetWorkDelegate::GetSocketStr(i_iocpIO).c_str());
		return true;
	}
	//if(strcmp(i_lpChar,"tgw_l7_forward\r\nHost: app100629283-1.qzonapp.com:8002\r\n\r\n") == 0)
	if(strstr(i_lpChar,"tgw_l7_forward"))
	{
		printf("check tgw:%s!\n",NetWorkDelegate::GetSocketStr(i_iocpIO).c_str());
		char res[1024];
		memset(res, '\0', sizeof(res));
		strcpy_s(res, "tgw_l7_forward");
		lpIBMainThread->_lpBaseBehaviorMsgDeal->Send(i_iocpIO,res,strlen(res)+1);
		i_iocpIO->player=NULL;
		return true;
	}

	return lpIBMainThread->_lpBaseBehaviorMsgDeal->MsgDeal(
		i_iocpIO,
		i_lpChar,
		i_charArrLength
		);

	/*unsigned int uintPackageTotalLength = SevenSmile::Net::Util::BytesToUShortForAs(i_lpChar);

	if (uintPackageTotalLength == i_charArrLength)
	{
		return lpIBMainThread->_lpBaseBehaviorMsgDeal->MsgDeal(
			i_iocpIO,
			i_lpChar,
			i_charArrLength
			);
	}
	else if (uintPackageTotalLength< i_charArrLength)
	{
		SevenSmile::Net::MsgPackageManage mpm(i_lpChar,i_charArrLength);
		SevenSmile::Net::MsgPackageArr mpa=mpm.GetMsgPackages();

		std::vector<bool> vReturn;

		int length=mpa.size();
		for (unsigned int i=0;i<length;i++)
		{
			bool bRes=lpIBMainThread->_lpBaseBehaviorMsgDeal->MsgDeal(
				i_iocpIO,
				mpa[i]->lpcMsgPackage,
				mpa[i]->uiPackageLength
				);

			if(!bRes)
			{
				return false;
			}
		}
	}else{
		printf("ERROR: 解出包头长度 > 收到数据长度 MSG:%s \n ",i_lpChar);
	}*/
	return true;
}