#include "WorldSession.h"
#include "../Object/Element.h"
#include "../Server/MsgDealImp.h"
#include "../Object/Player.h"
#include "../World/WorldMap.h"
#include "MsgPackageManage.h"
#include "TCP_PROTOCOL.h"
#include "Timer.h"
#include "MsgPackage.h"

//#define ViewSendMsg

WorldSession::WorldSession(MsgDealImp* i_deal)
	:_player(0),
	_deal(i_deal),
	_quit(SESSION_STATE_NORMAL),
	_bLoadPlayer(false)
{
	CThreadSync sync;
	/*init read handle*/
	if(_ReadHandles.empty())
	{
		InitReadHandle();
	}
	_interval.SetCurrent(0);
	time_t time = 1000 * 60 * 10;
	_interval.SetInterval(time);
}

WorldSession::~WorldSession(void)
{
	CThreadSync sync;

	SetPlayer(0);
	_iocp		= 0;
	_deal		= 0;
	_bLoadPlayer = true;
	_dDealList.clear();
	_dSendList.clear();
}

void WorldSession::FreeWritePackage(MsgPackage* myPackage)
{
	if (0 != myPackage)
	{
		delete myPackage;
		myPackage = 0;
	}
	
}

void WorldSession::InitReadHandle()
{
	/*INIT READ PLAYER HANDLE*/
	_ReadHandles[PTP_PLAYER_LAND]					=				&WorldSession::ReadPlayerLandHandle;
	_ReadHandles[PTP_PLAYER_QUIT]					=				&WorldSession::ReadPlayerQuitHandle;
	_ReadHandles[PTP_PLAYER_MOVE]					=				&WorldSession::ReadPlayerMoveHandle;
	_ReadHandles[PTP_PLAYER_ENTER_MAP]				=				&WorldSession::ReadPlayerEnterMapHandle;
	_ReadHandles[PTP_PLAYER_LEAVE_MAP]				=				&WorldSession::ReadPlayerExitMapHandle;
	_ReadHandles[PTP_PLAYER_CHANGE_EQUIP]			=				&WorldSession::ReadPlayerChangeEquipHandle;
	_ReadHandles[PTP_PLAYER_CHAT]					=				&WorldSession::ReadPlayerChatHandle;
	_ReadHandles[PTP_PLAYER_CREATE]					=				&WorldSession::ReadPlayerCreateHandle;
	_ReadHandles[PTP_PLAYER_ADD_EQUIPEXP]			=				&WorldSession::ReadPlayerAddEquipExp;
	_ReadHandles[PTP_PLAYER_UPDATE]					=				&WorldSession::ReadPlayerUpdataHandle;

	/*INIT REAED ELEMENT HANDLE*/
	_ReadHandles[ETP_ELEMENT_CREATE]				=				&WorldSession::ReadElementCreateHandle;
	_ReadHandles[ETP_ELEMENT_DESTRORY]				=				&WorldSession::ReadElementDestroyHandle;
	_ReadHandles[PTP_PLAYER_ADMIT_ENERGY]			=				&WorldSession::ReadPlayerAdmitElementEnergyHandle;

	/*INIT REAED ITEM HANDLE*/
	_ReadHandles[ITP_ITEM_BREED]					=				&WorldSession::ReadItemBreedHandle;
	_ReadHandles[ITP_ITEM_EXCHANGE]					=				&WorldSession::ReadItemExchangeHandle;
	_ReadHandles[ITP_ITEM_USE]						=				&WorldSession::ReadItemUseHandle;
	_ReadHandles[ITP_ITEM_BREED_INFO]				=				&WorldSession::ReadItemGetBreedInfoHandle;
	
	_ReadHandles[TPP_TEMPITEM_CONVERSION]			=				&WorldSession::ReadTtemConvTempItemHandle;
	_ReadHandles[TPP_TEMPITEM_CLEAR]				=				&WorldSession::ReadClearTempBagHandle;

	/*INIT READ MAP HANDLE*/
	_ReadHandles[MTP_TILE_TURN]						=				&WorldSession::ReadTileTurnHandle;

	/*PROFESSION HANDLE*/
	_ReadHandles[PTP_PROFESSION_OPENNEW]			=				&WorldSession::ReadOpenNewProHandle;
	_ReadHandles[PTP_PROFESSION_ADDEXP]				=				&WorldSession::ReadAddCurProExpHandle;

	_ReadHandles[PTP_PLAYER_THROW_ITEM]				=				&WorldSession::ReadPlayerThrowItemToGroundHandle;
	_ReadHandles[PTP_PLAYER_THROW_GOLD]				=				&WorldSession::ReadPlayerThrowGoldToGroundHandle;
	_ReadHandles[PTP_PLAYER_PICKUP_ITEM]			=				&WorldSession::ReadPlayerPickUpGroundItemHandle;
	_ReadHandles[PTP_PLAYER_HARVEST_ELM]			=				&WorldSession::ReadPlayerHarvestElmHandle;
	_ReadHandles[PTP_PLAYER_STALL_ITEM]				=				&WorldSession::ReadPlayerStallItemHandle;
	_ReadHandles[PTP_PLAYER_MERCHANDISE]			=				&WorldSession::ReadPlayerMerchandiseHandle;
	_ReadHandles[PTP_PLAYER_BUY_STALLITEM]			=				&WorldSession::ReadPlayerBuyStallItemHandle;
	_ReadHandles[PTP_PLAYER_TALK]					=				&WorldSession::ReadPlayerTalkHandle;
	_ReadHandles[PTP_PLAYER_ADDMOOD_ELE]			=				&WorldSession::ReadPlayerAddEleMoodHandle;
	_ReadHandles[PTP_PLAYER_REQUEST]				=				&WorldSession::ReadPlayerRequestHandle;

	_ReadHandles[FTP_PLAYERFRIEND_ADD]				=				&WorldSession::ReadPlayerAddFriendHandle;
	_ReadHandles[FTP_PLAYERFRIEND_DEL]				=				&WorldSession::ReadPlayerDelFriendHandle;
	_ReadHandles[FTP_PlAYERFRIEND_GOTOHOME]			=				&WorldSession::ReadPlayerGotoFriendMapHandle;

	//quest
	_ReadHandles[QTP_QUEST_ACCEPTED]				=				&WorldSession::ReadQuestAcceptHandle;
	_ReadHandles[QTP_QUEST_FIELD]					=				&WorldSession::ReadQuestFieldHandle;
	_ReadHandles[QTP_QUEST_REWARD]					=				&WorldSession::ReadQuestRewardHandle;
	_ReadHandles[QTP_QUEST_BATTLE_RECORD]			=				&WorldSession::ReadQuestBattleRecordHandle;
	_ReadHandles[QTP_QUEST_UPDATE]					=				&WorldSession::ReadQuestBattleEvaluateHandle;
	_ReadHandles[QTP_CONTINUOUS_ACCEPT]				=				&WorldSession::ReadContinuousQuestAcceptHandle;
	_ReadHandles[QTP_CONTINUOUS_REWARD]				=				&WorldSession::ReadContinuousQuestRewardHandle;
	_ReadHandles[QTP_QUESTRECORD_GETINFO]			=				&WorldSession::ReadGetQuestRecordHandle;
	_ReadHandles[QTP_QUESTRECORD_UPDATA]			=				&WorldSession::ReadUpQuestRecordHandle;

	_ReadHandles[FTP_MAKEFORMULA_ADDGEM]			=				&WorldSession::ReadPlayerAddGemToEquipmentHandle;
	_ReadHandles[FTP_MAKEFORMULA_PRODUCTION]		=				&WorldSession::ReadPlayerProductionHandle;
	_ReadHandles[FTP_MAKEFORMULA_UPSKILL]			=				&WorldSession::ReadPlayerUpSkillHandle;
	_ReadHandles[FTP_MAKEFORMULA_UPSLOTLEVEL]		=				&WorldSession::ReadPlayerUpSlotLevelHandle;
	_ReadHandles[FTP_MAKEFORMULA_SYNTHESIS]			=				&WorldSession::ReadPlayerSynthesisItemHandle;
	_ReadHandles[PTP_PLAYER_SHOPPING]				=				&WorldSession::ReadPlayerShoppingHandle;
	_ReadHandles[PTP_PLAYER_BUY_FORMULARESULT]		=				&WorldSession::ReadPlayerBuyFormulaResult;
	_ReadHandles[PTP_PLAYER_SELLTO_SYSTEM]			=				&WorldSession::ReadPlayerStllItemsToSystemHandle;

	_ReadHandles[LTP_LOTTERY_START]					=				&WorldSession::ReadPlayerLotteryStart;
	_ReadHandles[LTP_LOTTERY_SINGLEREWARD]			=				&WorldSession::ReadPlayerLotteryReward;

	_ReadHandles[PTP_PLAYER_GETINFO]				=				&WorldSession::ReadPlayerGetInfoHandle;
	//Sever Function
	_ReadHandles[SEVER_PING]						=				&WorldSession::ReadSeverPingHandle;
}


bool WorldSession::Deal( char* i_lpChar,int i_charArrLength )
{
	CThreadSync sync;
	_DealMessage(i_lpChar,i_charArrLength);
	if(_quit == SESSION_STATE_QUIT)
	{
		return false;
	}
	return true;
}

bool WorldSession::_DealMessage( const char* i_lpConstChar,int i_charArrLength )
{
	//解大包
	char* i_lpChar = const_cast<char*>(i_lpConstChar);
	//取包长
	unsigned int uintPackageTotalLength = 
		SevenSmile::Net::Util::BytesToUShortForAs(i_lpChar);

	
	if(uintPackageTotalLength == i_charArrLength)
	{
		CThreadSync sync;
		_Deal(i_lpChar,i_charArrLength);
	}else if (uintPackageTotalLength< i_charArrLength)
	{
		SevenSmile::Net::MsgPackageManage mpm(i_lpChar,i_charArrLength);
		SevenSmile::Net::MsgPackageArr mpa=mpm.GetMsgPackages();
		int length=mpa.size();
		CThreadSync sync;
		for (unsigned int i=0;i<length;i++)
		{
			_Deal(
				mpa[i]->lpcMsgPackage,
				mpa[i]->uiPackageLength
				);
		}
	}else{
		errorlog printf(
			"uintPackageTotalLength %d > i_charArrLength %d",
			uintPackageTotalLength,i_charArrLength);
	}
	return true;
}


bool WorldSession::_Deal(const char* i_lpChar,int i_charArrLength)
{
	//MsgPackage myPackage;
	shared_ptr<MsgPackage> myPackage =
		shared_ptr<MsgPackage>(new MsgPackage());

	myPackage->UnPackage(const_cast<char*>(i_lpChar),i_charArrLength);

	TCP_PROTOCOL recvAct=(TCP_PROTOCOL)myPackage->GetPackageAction();
	if (recvAct == PTP_PLAYER_LAND)
	{
		_bLoadPlayer = true;
	}

	CThreadSync sync;
	_dDealList.push_back(myPackage);
	return true;
}

bool WorldSession::ExecuteReadHandle(shared_ptr<MsgPackage> i_myPackage)
{
	//判断包头长度 
	int totalLen = i_myPackage->GetPackageTotalLength();
	
// 	不做检验
// 	if(totalLen != i_charArrLength)
// 	{
// 		errorlog printf("ReadHandleError:ReadHandle PackageLength Error: CurLength %d, TotalLength %d!\n",
// 			i_charArrLength,totalLen);
// 		return false;
// 	}

	/*解析*/
	TCP_PROTOCOL recvAct=(TCP_PROTOCOL)i_myPackage->GetPackageAction();
	if(_ReadHandles.find(recvAct) == _ReadHandles.end())
	{
		//errorlog printf("ReadHandleError:ReadHandle Not Find Action: %d!\n",recvAct);
		return false;
	}
	
	msglog
	printf("%s",SevenSmile::Util::Time::GetCurentDataAndTime());
	printf("Socket:%d	<<	ReadHandle:%d	<<	%s!\n",
	_iocp->socket,recvAct,
	TcpProtocolUtil::instance()->PrintTcpProtocol(recvAct));

	if(0 == GetPlayer() && 
		recvAct != PTP_PLAYER_LAND &&
		recvAct != PTP_PLAYER_CREATE &&
		recvAct != PTP_PLAYER_QUIT
	){
		//errorlog printf("Session Player Not Have!");
		Quit(SESSION_STATE_QUIT);
		return false;
	}

	if (recvAct == PTP_PLAYER_LAND)
	{
		CThreadSync sync;
		_bLoadPlayerSingle = true;
		_bLoadPlayer = false;
	}
	if (!(*_ReadHandles[recvAct])(i_myPackage.get(),this))
	{
		//errorlog printf("ReadHandleError:ReadHandle Action: %d!\n",recvAct);
		return false;
	}
	return true;
}

void WorldSession::Send(const char* msg ,const int len )
{
	CThreadSync sync;
	//发送信息长度为0
	//并且不在正常状态下 退出
	if (len <= 0 
		|| Quit() != SESSION_STATE_NORMAL)
	{
		return;
	}
	shared_ptr<string> str = shared_ptr<string>(
		new string(msg,len));
	_dSendList.push_back(str);
	
	//_deal->Send(_iocp,const_cast<char*>(msg),len);
	
}

void WorldSession::SendToClient()
{
	CThreadSync sync;
	//在发送主类 存在
	//及处在正常状态 进行发送
	if(0 != _deal 
		&& _quit == SESSION_STATE_NORMAL
		&& _dSendList.size()>0)
	{
		shared_ptr<string> ptrStr;
		std::vector<shared_ptr<string>>::iterator it;
		for(it = _dSendList.begin(); it != _dSendList.end(); it++)
		{
			ptrStr = *it;
			//发送长度不能超过BUFFER_SIZE
			if(ptrStr->length() > 0 && 
				!_deal->Send(
					_iocp,
					const_cast<char*>(ptrStr->c_str()),
					ptrStr->length()))
			{
				SessionQuit();
				break;
			}
		}
	}
	if (!_dSendList.empty())
	{
		//_dSendList.swap(std::vector<shared_ptr<string>>());
		_dSendList.clear();
	}
}

void WorldSession::DealFromClient()
{
	CThreadSync sync;
	if (_quit == SESSION_STATE_QUIT
		||(_bLoadPlayer && _bLoadPlayerSingle))
	{
		return;
	}
	if(0 != _deal)
	{
		
		for (size_t i = 0 ; i<_dDealList.size(); i++)
		{
			//改为只解析小包
			ExecuteReadHandle(_dDealList[i]);
			if(_quit == SESSION_STATE_QUIT)
			{
				SendToClient();
				break;
			}
		}
	}
	if (!_dDealList.empty())
	{
		_dDealList.clear();
	}
}

stdext::hash_map<TCP_PROTOCOL,CallBack> WorldSession::_ReadHandles;


shared_ptr<string> WorldSession::PackageSeverPingHandle()
{
	MsgPackage sendPackage;
	sendPackage.SetPackageAction(SEVER_PING);
	//time_t curtime = time(0);
	//sendPackage.AddMsgData(0,(unsigned int)curtime);

	char* msg = 0;
	int len = sendPackage.Package(msg);
	shared_ptr<string> str(new string(msg,len));
	return str;
}


shared_ptr<string> WorldSession::PackageSeverCloseHandle()
{
	MsgPackage sendPackage;
	sendPackage.SetPackageAction(SEVER_CLOSE);
	char* msg = 0;
	int len = sendPackage.Package(msg);
	shared_ptr<string> str(new string(msg,len));
	return str;
}

bool WorldSession::ReadSeverPingHandle( MsgPackage* myPackage ,WorldSession* const i_session )
{
	return true;
}

void WorldSession::UpTime( time_t diff )
{
	
	if(_interval.GetCurrent()>=0)
	{
		_interval.Update(diff);
	}
	else 
	{
		_interval.SetCurrent(0);
	}
	if (_interval.Passed())
	{
		_interval.Reset();
		shared_ptr<string> pingStr = PackageSeverPingHandle();
		this->Send(pingStr->c_str(),pingStr->length());
	}
}

void WorldSession::SetPlayer( Player* val )
{
	//CThreadSync sync;
	if(_player){ _player->SetSession(0); }
	_player = val;
	if(_player){ _player->SetSession(this); }
}

void WorldSession::SessionQuit()
{
	
	MsgPackage quitMsg;
	quitMsg.SetPackageAction(PTP_PLAYER_QUIT);
	char* s = 0;
	int le = quitMsg.Package(s);

	CThreadSync sync;
	if (_bLoadPlayer == true)
	{
		_bLoadPlayer = false;
		if (!_dDealList.empty())
		{
			_dDealList.clear();
		}
	}
	this->Deal(s,le);
}

shared_ptr<string> WorldSession::PackageChampionTeamInfoHandle( DB_CHAMPION_TEAM& i_info )
{
	MsgPackage sendPackage;
	sendPackage.SetPackageAction(WTP_CHAMPIONTEAM_INFO);
	sendPackage.AddMsgData(0,	i_info.id);      
	sendPackage.AddMsgData(1,	(unsigned int)i_info.fasttime);
	sendPackage.AddMsgData(2,	i_info.killcount);
	sendPackage.AddMsgData(3,	(unsigned int)i_info.fastdate);
	sendPackage.AddMsgData(4,	(unsigned int)i_info.killdate);
	sendPackage.AddMsgData(5,	i_info.killid);
	sendPackage.AddMsgData(6,	i_info.fastid_a);
	sendPackage.AddMsgData(7,	i_info.fastid_b);
	sendPackage.AddMsgData(8,	i_info.fastid_c);

	char* msg = 0;
	int le = sendPackage.Package(msg);

	shared_ptr<string> res(new string);
	res->append(msg,le);
	return res;
}
bool WorldSession::_bLoadPlayerSingle = false;
