#pragma once
//#include "../Object/Player.h"
#include <hash_map>
#include <vector>
#include <set>
#include <memory>

using namespace std::tr1;
using namespace std;

enum SESSION_QUIT_STATE
{
	SESSION_STATE_NORMAL,
	SESSION_STATE_READYQUIT,
	SESSION_STATE_QUIT,
};

template<class T>
class WorldSession
	:public SevenSmile::Util::ThreadHelper::ObjectLevelLockable<WorldSession>
{
public:
	WorldSession(MsgDealImp* i_deal);
	~WorldSession(void);
	
public:
	bool		Deal(char* i_lpChar,int i_charArrLength);
	void		Send(const char* msg,const int len);

	IOCP_IO*	GetIocp()	const				{ Lock(*this); return _iocp; };
	void		SetIocp(IOCP_IO* i_iocp)		{ Lock(*this); _iocp = i_iocp; };

	T*			GetExpand() const				{ Lock(*this); return _expand; }
	void		SetExpand(T* val)				{ Lock(*this); _expand = val; };
	void		SessionQuit();

	void		DealFromClient();
	void		SendToClient();
	void		UpTime(time_t diff);

	SESSION_QUIT_STATE Quit() const
	{
		return  _quit;
	}
	void Quit(SESSION_QUIT_STATE val) { Lock(*this); _quit = val; }

	static bool								_bLoadPlayerSingle;

private:

	MsgDealImp*								_deal;
	
	T*									_expand;

	IOCP_IO*								_iocp;

	std::vector<shared_ptr<string>>			_dSendList;
	std::vector<shared_ptr<MsgPackage>>		_dDealList;

	SESSION_QUIT_STATE						_quit;
	bool									_bLoadPlayer;
private:

	bool		_Deal(const char* i_lpChar,int i_charArrLength);
	bool		_DealMessage( const char* i_lpChar,int i_charArrLength );
	bool		ExecuteReadHandle(shared_ptr<MsgPackage> i_myPackage);

public:
	/************************************************************************/
	/* READ HANDLE                                                          */
	/************************************************************************/
	static void									InitReadHandle();

	/************************************************************************/
	/* Sever handle                                                         */
	/************************************************************************/
	static bool									ReadSeverPingHandle(MsgPackage* myPackage ,WorldSession* const i_session);

	/************************************************************************/
	/* Player Read Handle                                                   */
	/************************************************************************/
	static bool									ReadPlayerLandHandle(MsgPackage* myPackage , WorldSession* const i_session);			//登陆
	static bool									ReadPlayerQuitHandle(MsgPackage* myPackage , WorldSession* const i_session);			//退出
	static bool									ReadPlayerMoveHandle(MsgPackage* myPackage , WorldSession* const i_session);			//移动
	static bool									ReadPlayerEnterMapHandle(MsgPackage* myPackage , WorldSession* const i_session);		
	static bool									ReadPlayerExitMapHandle(MsgPackage* myPackage , WorldSession* const i_session);			//退出地图
	static bool									ReadPlayerThrowItemToGroundHandle(MsgPackage* myPackage ,WorldSession* const i_session);//将物品扔出
	static bool									ReadPlayerThrowGoldToGroundHandle(MsgPackage* myPackage ,WorldSession* const i_session);//将金钱扔出
	static bool									ReadPlayerPickUpGroundItemHandle(MsgPackage* myPackage ,WorldSession* const i_session);	//捡起物品
	static bool									ReadPlayerHarvestElmHandle(MsgPackage* myPackage,WorldSession* const i_session);		//收获元素
	static bool									ReadPlayerStallItemHandle(MsgPackage* myPackage,WorldSession* const i_session);			//将物品摆摊
	static bool									ReadPlayerMerchandiseHandle(MsgPackage* myPackage,WorldSession* const i_session);		//交易
	static bool									ReadPlayerBuyStallItemHandle(MsgPackage* myPackage,WorldSession* const i_session);		//购买摆摊物品
	static bool									ReadPlayerChangeEquipHandle(MsgPackage* myPackage,WorldSession* const i_session);		//更换装备
	static bool									ReadPlayerAddEleMoodHandle( MsgPackage* myPackage,WorldSession* const i_session );		//增加元素心情值

	static bool									ReadPlayerTalkHandle(MsgPackage* myPackage,WorldSession* const i_session);				//玩家对话
	static bool									ReadPlayerRequestHandle(MsgPackage* myPackage,WorldSession* const i_session);			//信息反馈
	static bool									ReadPlayerChatHandle(MsgPackage* myPackage,WorldSession* const i_session);				//聊天
	static bool									ReadPlayerCreateHandle(MsgPackage* myPackage,WorldSession* const i_session);			//创建新角色

	static bool									ReadPlayerAddFriendHandle(MsgPackage* myPackage,WorldSession* const i_session);				//增加好友
	static bool									ReadPlayerDelFriendHandle(MsgPackage* myPackage,WorldSession* const i_session);				//删除好友
	static bool									ReadPlayerGotoFriendMapHandle(MsgPackage* myPackage,WorldSession* const i_session);

	static bool									ReadPlayerAdmitElementEnergyHandle(MsgPackage* myPackage,WorldSession* const i_session);	//吸收能量
	static bool									ReadPlayerAddEquipExp(MsgPackage* myPackage,WorldSession* const i_session);				//增加装备经验

	static bool									ReadPlayerGetInfoHandle(MsgPackage* myPackage,WorldSession* const i_session);			//获取玩家信息

	static bool									ReadPlayerAddGemToEquipmentHandle(MsgPackage* myPackage,WorldSession* const i_session);	//增加宝石到装备上
	static bool									ReadPlayerProductionHandle(MsgPackage* myPackage,WorldSession* const i_session);		//制作装备
	static bool									ReadPlayerUpSkillHandle(MsgPackage* myPackage,WorldSession* const i_session);			//升级技能
	static bool									ReadPlayerUpSlotLevelHandle(MsgPackage* myPackage,WorldSession* const i_session);		//升级槽等级
	static bool									ReadPlayerSynthesisItemHandle(MsgPackage* myPackage,WorldSession* const i_session);		//合成物品
	static bool									ReadPlayerShoppingHandle(MsgPackage* myPackage,WorldSession* const i_session);			//购物
	static bool									ReadPlayerLotteryStart(MsgPackage* myPackage,WorldSession* const i_session);			//玩家抽奖准备
	static bool									ReadPlayerLotteryReward(MsgPackage* myPackage,WorldSession* const i_session);			//玩家抽奖奖励
	static bool									ReadPlayerBuyFormulaResult(MsgPackage* myPackage,WorldSession* const i_session);		//用配方直接购买成品
	static bool									ReadPlayerStllItemsToSystemHandle(MsgPackage* myPackage,WorldSession* const i_session);	//向系统出售物品
	static bool									ReadPlayerUpdataHandle( MsgPackage* myPackage,WorldSession* const i_session );			//更新部分player属性
	/************************************************************************/
	/* Element Read Handle                                                  */
	/************************************************************************/
	static bool									ReadElementCreateHandle(MsgPackage* myPackage ,WorldSession* const i_session);		//创建
	static bool									ReadElementDestroyHandle(MsgPackage* myPackage , WorldSession* const i_session);	//销毁
	
	/************************************************************************/
	/* Item Read Handle                                                     */
	/************************************************************************/
	static bool									ReadItemUseHandle(MsgPackage* myPackage,WorldSession* const i_session);			//物品使用
	static bool									ReadItemBreedHandle(MsgPackage* myPackage ,WorldSession* const i_session);		//物品种植
	static bool									ReadItemExchangeHandle(MsgPackage* myPackage ,WorldSession* const i_session);	//物品交换
	static bool									ReadItemGetBreedInfoHandle(MsgPackage* myPackage,WorldSession* const i_session);//是否能种植

	/************************************************************************/
	/* TempBag Read Handle                                                  */
	/************************************************************************/
	static bool									ReadTtemConvTempItemHandle(MsgPackage* myPackage,WorldSession* const i_session);	//改变临时物品
	static bool									ReadClearTempBagHandle(MsgPackage* myPackage,WorldSession* const i_session);	//清除所有临时物品
	
	/************************************************************************/
	/* Map Read Handle                                                      */
	/************************************************************************/
	static bool									ReadTileTurnHandle(MsgPackage* myPackage,WorldSession* const i_session);		//翻格子

	/************************************************************************/
	/* Quest Read Handle                                                    */
	/************************************************************************/
	static bool									ReadQuestAcceptHandle(	MsgPackage* myPackage,WorldSession* const i_session);		//接受任务
	//static bool									ReadQuestCompleteHandle(MsgPackage* myPackage,WorldSession* const i_session);	//完成任务
	static bool									ReadQuestFieldHandle(	MsgPackage* myPackage,WorldSession* const i_session);		//放弃任务
	static bool									ReadQuestRewardHandle(	MsgPackage* myPackage,WorldSession* const i_session);		//交还任务
	static bool									ReadQuestBattleRecordHandle( MsgPackage* myPackage,WorldSession* const i_session );	//战斗更新任务进度
	static bool									ReadQuestBattleEvaluateHandle( MsgPackage* myPackage,WorldSession* const i_session);//战斗评价更新
	static bool									ReadContinuousQuestAcceptHandle(MsgPackage* myPackage,WorldSession* const i_session);//接受连续任务
	static bool									ReadContinuousQuestRewardHandle(MsgPackage* myPackage,WorldSession* const i_session);//交还连续任务
	static bool									ReadGetQuestRecordHandle(MsgPackage* myPackage,WorldSession* const i_session);		//获取任务记录信息
	static bool									ReadUpQuestRecordHandle(MsgPackage* myPackage,WorldSession* const i_session);		//更新任务记录
	/************************************************************************/
	/* PLAYER PROFESSION HANDLE                                             */
	/************************************************************************/
	static bool									ReadAddCurProExpHandle(MsgPackage* myPackage,WorldSession* const i_session);
	static bool									ReadOpenNewProHandle(MsgPackage* myPackage,WorldSession* const i_session);
public:
	/************************************************************************/
	/* PACKAGE HANDLE	                                                    */
	/************************************************************************/
	static void									FreeWritePackage(MsgPackage* myPackage);

	/************************************************************************/
	/* Sever handle                                                         */
	/************************************************************************/
	static shared_ptr<string>					PackageSeverPingHandle();
	static shared_ptr<string>					PackageSeverCloseHandle();

	/************************************************************************/
	/* Element  Send Handle                                                 */
	/************************************************************************/

	static string								PackageElementCreateHandle( Element* ele );	//创建
	static string								PackageElementDestroyHandle(Element* ele); //销毁
	static string								PackageBehaviorGrowHandle( Element* ele); //生长行为
	static shared_ptr<string>					PackageElementUpdate(Element* i_ele,std::set<int> &flag);//元素属性更新
	static shared_ptr<string>					PackageElementOutValue(Element* i_ele);

	/************************************************************************/
	/* Player Send  Handle                                                  */
	/************************************************************************/
	static string								PackagePlayerInitHandle(Player* i_player);
	static shared_ptr<string>					PackagePlayerLandCompleteHandle();
	static shared_ptr<string>					PackagePlayerEnterMapHandle(Player* i_player);
	static void									PackagePlayerQuitHandle(MsgPackage* sendPackage,Player* i_player);
	static string								PackagePlayerMoveHandle(Player* i_player);
	//static string								PackagePlayerEquipViewInfoHandle(Player* i_player);
	static string								PackagePlayerUpdate(Player* i_player,std::set<int> &flag);
	static string								PackageElementArrivalHandle(Creature* i_creature);

	static shared_ptr<string>					PackagePlayerTalkHandle(
		TALKSTATE i_state,Element* i_talk_target = 0,
		shared_ptr<std::set<unsigned int>> i_talklist = shared_ptr<std::set<unsigned int>>((std::set<unsigned int>*)0),
		shared_ptr<VecSpQuest> i_complete_quests = shared_ptr<VecSpQuest>((VecSpQuest*)0));
	static shared_ptr<string>					PackagePlayerRequestHandle(
		unsigned int i_playerid,PLAYER_REQUEST i_mark);
	static shared_ptr<string>					PackagePlayerLandErrorHandle();
	static shared_ptr<string>					PackagePlayerCreateHandle(unsigned int i_guid);
	static shared_ptr<string>					PackagePlayerAddFriendHandle(PlayerFriend* i_friend);
	static shared_ptr<string>					PackagePlayerDelFriendHandle(unsigned int i_friendid);
	static shared_ptr<string>					PackagePlayerFriendUpdata(PlayerFriend* i_friend,vector<int> &flag);
	static shared_ptr<string>					PackagePlayeAdmitElementEnergy(Element* i_ele);
	static shared_ptr<string>					PackagePlayerMakeFormulaResultHandle(							//制作配方
		FORMULA_FACTORY::FormulaResult &i_result,
		FORMULA_FACTORY::FORMULA_TYPE i_type);
	static shared_ptr<string>					PackagePlayerShopping(PLAYER_SHOPPING_STATE i_state);

	static shared_ptr<string>					PackagePlayerLotteryStart(								//玩家抽奖准备
		std::vector<LOTTERY_FACTORY::LotteryIdAndCount>& const i_list);			
	static shared_ptr<string>					PackagePlayerLotteryReward(unsigned int i_pos);			//玩家抽奖奖励
	static shared_ptr<string>					PackagePlayerLotteryError(								//玩家抽奖错误信息
		LOTTERY_FACTORY::LOTTERY_ERROR i_lotteryError);
	/************************************************************************/
	/* Map Send  Handle                                                     */
	/************************************************************************/
	//static void									SendSceneInitHandle(WorldSession* i_session , Scene* i_scene);
	//static void									SendMapInitHandle(WorldSession* i_session , WorldMap* i_map);
	static void									SendMapInitHandle(WorldSession* i_session , WorldMap* i_map);
	static shared_ptr<string>					PackageMapExitHandle(unsigned int i_playerid,unsigned int i_mapId);				

	static string								PackageTileInitHandle(Tile* i_tile);
	static string								PackageTileTurnHandle(Tile* i_tile);
	

	/************************************************************************/
	/* Item Send Handle	                                                    */
	/************************************************************************/
	static string								PackageItemInitHandle( Item* i_item);
	static string								PackageBagInitHandle( Bag* i_bag);
	static string								PackageEquipListInitHandle( EquipList* i_equip);
	static string								PackageFashionListInitHandle( FashionList* i_fashions);
	static string								PackageItemUseHandle( Item* i_item);
	static string								PackageItemBreedInfoHandle(Item* i_item);
	static string								PackagePlayerMerchandiseHandle(Player* i_player,bool start); //交易物品 状态
	static string								PackagePlayerAddorDelMerchandiseItemHandle(Player* i_player,MerchandiseItem* i_item,bool add); //增加/删除交易物品
	static string								PackageItemUpdate(Item* i_item,vector<int> &flag);		//物品更新方法
	static string								PackageItemDestroy(Item* i_item);//物品删除方法

	//static string								PackageItemThrowToGroundHandle(Item* i_item,Item* i_gitem);

	/************************************************************************/
	/* Quest Send Handle	                                                */
	/************************************************************************/
	static shared_ptr<string>					PackageQuestAcceptMarkedHandle(			//任务可接受标示
		shared_ptr<vector<Element*>> i_list);								
	static shared_ptr<string>					PackageQuestCompleteMarkedHandle(		//任务可完成标示
		shared_ptr<vector<Element*>> i_list);
	
	static shared_ptr<string>					PackageQuestAcceptHandle(unsigned int i_questid);	//接受任务
	static shared_ptr<string>					PackageQuestCompleteHandle(							//完成任务
		shared_ptr<VecSpQuest> completelist);	
	static shared_ptr<string>					PackageQuestFieldHandle(unsigned int i_questid);	//放弃任务
	static shared_ptr<string>					PackageQuestRewardHandle(unsigned int i_questid);	//交还任务
	static shared_ptr<string>					PackageQuestProgressHandle(							//进度任务
		SpQuest i_quest,shared_ptr<vector<IdAndCount>> list);
	static shared_ptr<string>					PackageQuestInitHandle(								//玩家已接受任务列表
		Player* i_player,
		const VecSpQuest* i_vecSpQuest);

	static shared_ptr<string>					PackageGlobalQuestsHandle(							//获取所有全局任务列表
		shared_ptr<VecSpQuest> i_vecSpQuest,
		Player* const i_player);
	static shared_ptr<string>					PackageQuestCompleteBonusHandle(					//任务完成奖励
		IdAndCount* const i_idAndCount,unsigned int const i_num);
	static shared_ptr<string>					PackageQuestUpdate(									//任务状态更新
		SpQuest i_spQuest,vector<int>& i_flag);
	static shared_ptr<string>					PackageContinuousQuestAcceptHandle(					//接受连续任务
		unsigned int i_questId,
		unsigned int i_count,
		unsigned int i_time,
		CONTINUOUSQUEST_ACCEPT_ERROR error);
	static shared_ptr<string>					PackageContinuousQuestRewardHandle(					//交还连续任务
		unsigned int i_questId,
		IdAndCount* i_icBonus,
		unsigned int i_completeCount);
	static shared_ptr<string>					PackageUpQuestRecordHandle(							//发送任务记录/更新
		DB_QUEST_RECORD* const i_record);

	static shared_ptr<string>					PackageChatHandle(
		string* i_name,CHAT_MARK i_mark,shared_ptr<string> i_msg,unsigned int i_privateid = 0 );
	
	/************************************************************************/
	/* Achievement	Send Handle	                                            */
	/************************************************************************/

	static shared_ptr<string>					PackageAchievementInitHandle(
			Player* i_player);
	static shared_ptr<string>					PackageAchievementAddHandle(
		unsigned int i_achievement_id,
		unsigned int i_curnum);

	/************************************************************************/
	/* PLAYER PROFESSION HANDLE                                             */
	/************************************************************************/
	static shared_ptr<string>					PackageOpenNewProHandle(
		DB_USER_CHARACTER_PROFESSION& i_pro);
	static shared_ptr<string>					PackageProUpDateHandle(
		DB_USER_CHARACTER_PROFESSION* i_pro);

	/************************************************************************/
	/* WORLD HANDLE		                                                    */
	/************************************************************************/	
	static shared_ptr<string>					PackageChampionTeamInfoHandle(
		DB_CHAMPION_TEAM& i_info);
	static shared_ptr<string>					PackageDBBaseDateHandle();
};