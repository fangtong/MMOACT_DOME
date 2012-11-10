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
	static bool									ReadPlayerLandHandle(MsgPackage* myPackage , WorldSession* const i_session);			//��½
	static bool									ReadPlayerQuitHandle(MsgPackage* myPackage , WorldSession* const i_session);			//�˳�
	static bool									ReadPlayerMoveHandle(MsgPackage* myPackage , WorldSession* const i_session);			//�ƶ�
	static bool									ReadPlayerEnterMapHandle(MsgPackage* myPackage , WorldSession* const i_session);		
	static bool									ReadPlayerExitMapHandle(MsgPackage* myPackage , WorldSession* const i_session);			//�˳���ͼ
	static bool									ReadPlayerThrowItemToGroundHandle(MsgPackage* myPackage ,WorldSession* const i_session);//����Ʒ�ӳ�
	static bool									ReadPlayerThrowGoldToGroundHandle(MsgPackage* myPackage ,WorldSession* const i_session);//����Ǯ�ӳ�
	static bool									ReadPlayerPickUpGroundItemHandle(MsgPackage* myPackage ,WorldSession* const i_session);	//������Ʒ
	static bool									ReadPlayerHarvestElmHandle(MsgPackage* myPackage,WorldSession* const i_session);		//�ջ�Ԫ��
	static bool									ReadPlayerStallItemHandle(MsgPackage* myPackage,WorldSession* const i_session);			//����Ʒ��̯
	static bool									ReadPlayerMerchandiseHandle(MsgPackage* myPackage,WorldSession* const i_session);		//����
	static bool									ReadPlayerBuyStallItemHandle(MsgPackage* myPackage,WorldSession* const i_session);		//�����̯��Ʒ
	static bool									ReadPlayerChangeEquipHandle(MsgPackage* myPackage,WorldSession* const i_session);		//����װ��
	static bool									ReadPlayerAddEleMoodHandle( MsgPackage* myPackage,WorldSession* const i_session );		//����Ԫ������ֵ

	static bool									ReadPlayerTalkHandle(MsgPackage* myPackage,WorldSession* const i_session);				//��ҶԻ�
	static bool									ReadPlayerRequestHandle(MsgPackage* myPackage,WorldSession* const i_session);			//��Ϣ����
	static bool									ReadPlayerChatHandle(MsgPackage* myPackage,WorldSession* const i_session);				//����
	static bool									ReadPlayerCreateHandle(MsgPackage* myPackage,WorldSession* const i_session);			//�����½�ɫ

	static bool									ReadPlayerAddFriendHandle(MsgPackage* myPackage,WorldSession* const i_session);				//���Ӻ���
	static bool									ReadPlayerDelFriendHandle(MsgPackage* myPackage,WorldSession* const i_session);				//ɾ������
	static bool									ReadPlayerGotoFriendMapHandle(MsgPackage* myPackage,WorldSession* const i_session);

	static bool									ReadPlayerAdmitElementEnergyHandle(MsgPackage* myPackage,WorldSession* const i_session);	//��������
	static bool									ReadPlayerAddEquipExp(MsgPackage* myPackage,WorldSession* const i_session);				//����װ������

	static bool									ReadPlayerGetInfoHandle(MsgPackage* myPackage,WorldSession* const i_session);			//��ȡ�����Ϣ

	static bool									ReadPlayerAddGemToEquipmentHandle(MsgPackage* myPackage,WorldSession* const i_session);	//���ӱ�ʯ��װ����
	static bool									ReadPlayerProductionHandle(MsgPackage* myPackage,WorldSession* const i_session);		//����װ��
	static bool									ReadPlayerUpSkillHandle(MsgPackage* myPackage,WorldSession* const i_session);			//��������
	static bool									ReadPlayerUpSlotLevelHandle(MsgPackage* myPackage,WorldSession* const i_session);		//�����۵ȼ�
	static bool									ReadPlayerSynthesisItemHandle(MsgPackage* myPackage,WorldSession* const i_session);		//�ϳ���Ʒ
	static bool									ReadPlayerShoppingHandle(MsgPackage* myPackage,WorldSession* const i_session);			//����
	static bool									ReadPlayerLotteryStart(MsgPackage* myPackage,WorldSession* const i_session);			//��ҳ齱׼��
	static bool									ReadPlayerLotteryReward(MsgPackage* myPackage,WorldSession* const i_session);			//��ҳ齱����
	static bool									ReadPlayerBuyFormulaResult(MsgPackage* myPackage,WorldSession* const i_session);		//���䷽ֱ�ӹ����Ʒ
	static bool									ReadPlayerStllItemsToSystemHandle(MsgPackage* myPackage,WorldSession* const i_session);	//��ϵͳ������Ʒ
	static bool									ReadPlayerUpdataHandle( MsgPackage* myPackage,WorldSession* const i_session );			//���²���player����
	/************************************************************************/
	/* Element Read Handle                                                  */
	/************************************************************************/
	static bool									ReadElementCreateHandle(MsgPackage* myPackage ,WorldSession* const i_session);		//����
	static bool									ReadElementDestroyHandle(MsgPackage* myPackage , WorldSession* const i_session);	//����
	
	/************************************************************************/
	/* Item Read Handle                                                     */
	/************************************************************************/
	static bool									ReadItemUseHandle(MsgPackage* myPackage,WorldSession* const i_session);			//��Ʒʹ��
	static bool									ReadItemBreedHandle(MsgPackage* myPackage ,WorldSession* const i_session);		//��Ʒ��ֲ
	static bool									ReadItemExchangeHandle(MsgPackage* myPackage ,WorldSession* const i_session);	//��Ʒ����
	static bool									ReadItemGetBreedInfoHandle(MsgPackage* myPackage,WorldSession* const i_session);//�Ƿ�����ֲ

	/************************************************************************/
	/* TempBag Read Handle                                                  */
	/************************************************************************/
	static bool									ReadTtemConvTempItemHandle(MsgPackage* myPackage,WorldSession* const i_session);	//�ı���ʱ��Ʒ
	static bool									ReadClearTempBagHandle(MsgPackage* myPackage,WorldSession* const i_session);	//���������ʱ��Ʒ
	
	/************************************************************************/
	/* Map Read Handle                                                      */
	/************************************************************************/
	static bool									ReadTileTurnHandle(MsgPackage* myPackage,WorldSession* const i_session);		//������

	/************************************************************************/
	/* Quest Read Handle                                                    */
	/************************************************************************/
	static bool									ReadQuestAcceptHandle(	MsgPackage* myPackage,WorldSession* const i_session);		//��������
	//static bool									ReadQuestCompleteHandle(MsgPackage* myPackage,WorldSession* const i_session);	//�������
	static bool									ReadQuestFieldHandle(	MsgPackage* myPackage,WorldSession* const i_session);		//��������
	static bool									ReadQuestRewardHandle(	MsgPackage* myPackage,WorldSession* const i_session);		//��������
	static bool									ReadQuestBattleRecordHandle( MsgPackage* myPackage,WorldSession* const i_session );	//ս�������������
	static bool									ReadQuestBattleEvaluateHandle( MsgPackage* myPackage,WorldSession* const i_session);//ս�����۸���
	static bool									ReadContinuousQuestAcceptHandle(MsgPackage* myPackage,WorldSession* const i_session);//������������
	static bool									ReadContinuousQuestRewardHandle(MsgPackage* myPackage,WorldSession* const i_session);//������������
	static bool									ReadGetQuestRecordHandle(MsgPackage* myPackage,WorldSession* const i_session);		//��ȡ�����¼��Ϣ
	static bool									ReadUpQuestRecordHandle(MsgPackage* myPackage,WorldSession* const i_session);		//���������¼
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

	static string								PackageElementCreateHandle( Element* ele );	//����
	static string								PackageElementDestroyHandle(Element* ele); //����
	static string								PackageBehaviorGrowHandle( Element* ele); //������Ϊ
	static shared_ptr<string>					PackageElementUpdate(Element* i_ele,std::set<int> &flag);//Ԫ�����Ը���
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
	static shared_ptr<string>					PackagePlayerMakeFormulaResultHandle(							//�����䷽
		FORMULA_FACTORY::FormulaResult &i_result,
		FORMULA_FACTORY::FORMULA_TYPE i_type);
	static shared_ptr<string>					PackagePlayerShopping(PLAYER_SHOPPING_STATE i_state);

	static shared_ptr<string>					PackagePlayerLotteryStart(								//��ҳ齱׼��
		std::vector<LOTTERY_FACTORY::LotteryIdAndCount>& const i_list);			
	static shared_ptr<string>					PackagePlayerLotteryReward(unsigned int i_pos);			//��ҳ齱����
	static shared_ptr<string>					PackagePlayerLotteryError(								//��ҳ齱������Ϣ
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
	static string								PackagePlayerMerchandiseHandle(Player* i_player,bool start); //������Ʒ ״̬
	static string								PackagePlayerAddorDelMerchandiseItemHandle(Player* i_player,MerchandiseItem* i_item,bool add); //����/ɾ��������Ʒ
	static string								PackageItemUpdate(Item* i_item,vector<int> &flag);		//��Ʒ���·���
	static string								PackageItemDestroy(Item* i_item);//��Ʒɾ������

	//static string								PackageItemThrowToGroundHandle(Item* i_item,Item* i_gitem);

	/************************************************************************/
	/* Quest Send Handle	                                                */
	/************************************************************************/
	static shared_ptr<string>					PackageQuestAcceptMarkedHandle(			//����ɽ��ܱ�ʾ
		shared_ptr<vector<Element*>> i_list);								
	static shared_ptr<string>					PackageQuestCompleteMarkedHandle(		//�������ɱ�ʾ
		shared_ptr<vector<Element*>> i_list);
	
	static shared_ptr<string>					PackageQuestAcceptHandle(unsigned int i_questid);	//��������
	static shared_ptr<string>					PackageQuestCompleteHandle(							//�������
		shared_ptr<VecSpQuest> completelist);	
	static shared_ptr<string>					PackageQuestFieldHandle(unsigned int i_questid);	//��������
	static shared_ptr<string>					PackageQuestRewardHandle(unsigned int i_questid);	//��������
	static shared_ptr<string>					PackageQuestProgressHandle(							//��������
		SpQuest i_quest,shared_ptr<vector<IdAndCount>> list);
	static shared_ptr<string>					PackageQuestInitHandle(								//����ѽ��������б�
		Player* i_player,
		const VecSpQuest* i_vecSpQuest);

	static shared_ptr<string>					PackageGlobalQuestsHandle(							//��ȡ����ȫ�������б�
		shared_ptr<VecSpQuest> i_vecSpQuest,
		Player* const i_player);
	static shared_ptr<string>					PackageQuestCompleteBonusHandle(					//������ɽ���
		IdAndCount* const i_idAndCount,unsigned int const i_num);
	static shared_ptr<string>					PackageQuestUpdate(									//����״̬����
		SpQuest i_spQuest,vector<int>& i_flag);
	static shared_ptr<string>					PackageContinuousQuestAcceptHandle(					//������������
		unsigned int i_questId,
		unsigned int i_count,
		unsigned int i_time,
		CONTINUOUSQUEST_ACCEPT_ERROR error);
	static shared_ptr<string>					PackageContinuousQuestRewardHandle(					//������������
		unsigned int i_questId,
		IdAndCount* i_icBonus,
		unsigned int i_completeCount);
	static shared_ptr<string>					PackageUpQuestRecordHandle(							//���������¼/����
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