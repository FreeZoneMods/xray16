#pragma once
#include "game_cl_deathmatch.h"

class CUISpawnWndRP;
class CUISkinSelectorWndRP;
class CUIGameRP;

class game_cl_RolePlay: public game_cl_Deathmatch
{
	typedef game_cl_Deathmatch inherited;

	CUIGameRP*							m_game_ui;
	//---m4d_pda
	CUIPdaWnd*							pPdaMenu;

protected:
	bool								m_bFriendlyIndicators;
	bool								m_bFriendlyNames;

	virtual			void				shedule_Update(u32 dt);
	virtual			void				TranslateGameMessage(u32 msg, NET_Packet& P);

	virtual			void				LoadSndMessages();

	//---m4d_RP
	CUISkinSelectorWndRP*				pCurSkinMenu;

	CUISkinSelectorWndRP*				InitSkinMenu(s16 Team = -1);
public:
	game_cl_RolePlay();

	virtual			void				OnGameMenuRespond_ChangeSkin(NET_Packet& P);

	virtual								~game_cl_RolePlay();
	virtual			void				Init();
	virtual			void				net_import_state(NET_Packet& P);
	virtual			CUIGameCustom*		createGameUI();
	virtual			void				SetGameUI(CUIGameCustom*);
	virtual			void				GetMapEntities(xr_vector<SZoneMapEntityData>& dst);

	//---m4d_RP
	virtual CUIDialogWnd*			GetSkinWnd() { return (CUIDialogWnd*)pCurSkinMenu; };

	virtual BOOL					CanCallBuyMenu();
	virtual BOOL					CanCallSkinMenu();
	virtual	BOOL					CanCallInventoryMenu();
	virtual	BOOL					CanCallTeamSelectMenu();


	virtual		void				OnSpectatorSelect();
	virtual		void				OnSkinMenuBack();
	virtual		void				OnTeamMenuBack();
	virtual		void				OnMapInfoAccept();
	virtual		void				OnSkinMenu_Ok();

	virtual		void				OnGameMenuRespond_ChangeTeam(NET_Packet& P);

	virtual			void				OnTeamSelect(int Result);
	virtual			char*				getTeamSection(int Team);
	virtual			void				OnTeamChanged();
	virtual			void				PlayRankChangesSndMessage();
	virtual			void				OnTeamMenu_Cancel();

	virtual			void				Set_ShowPlayerNames(bool Show) { m_bShowPlayersNames = Show; };
	virtual			bool				Get_ShowPlayerNames() { return m_bShowPlayersNames; };
	virtual			s16					ModifyTeam(s16 Team) { return (Team != -1) ? Team - 1 : Team; };
	virtual			bool				Get_ShowPlayerNamesEnabled() { return m_bFriendlyNames; };

	virtual			bool				IsPlayerInTeam(game_PlayerState* ps, ETeam team);
	virtual			LPCSTR				GetGameScore(string32&	score_dest);
	s32									GetGreenTeamScore() const { return teams[0].score; }; //freedom
	s32									GetBlueTeamScore() const { return teams[1].score; }; //merc
	//------------------------------------------------------------------------------------------------------------m4d_RP
	s32									GetYellowTeamScore() const { return teams[2].score; }; //stalker
	s32									GetBlackTeamScore() const { return teams[3].score; }; //bandit
	s32									GetLightBlueTeamScore() const { return teams[4].score; }; //clear_sky
	s32									GetRedTeamScore() const { return teams[5].score; }; //dolg
	s32									GetBrownTeamScore() const { return teams[6].score; }; //monolith
	s32									GetLunarTeamScore() const { return teams[7].score; }; //ecolog
	s32									GetDarkGreenTeamScore() const { return teams[8].score; }; //army

protected:
	virtual const shared_str			GetBaseCostSect() { return "roleplay_base_cost"; }
	virtual const shared_str			GetTeamMenu(s16 team);

	PRESET_ITEMS						PresetItemsTeam1;
	PRESET_ITEMS						PresetItemsTeam2;
	//---------------------------------------------------------------------m4d_RP
	PRESET_ITEMS						PresetItemsTeam3;
	PRESET_ITEMS						PresetItemsTeam4;
	PRESET_ITEMS						PresetItemsTeam5;
	PRESET_ITEMS						PresetItemsTeam6;
	PRESET_ITEMS						PresetItemsTeam7;
	PRESET_ITEMS						PresetItemsTeam8;
	PRESET_ITEMS						PresetItemsTeam9;

	BOOL								m_bTeamSelected;
	bool								m_bShowPlayersNames;

	virtual bool						CanBeReady();
	virtual	void						SetCurrentBuyMenu();
	virtual	void						SetCurrentSkinMenu();

	virtual	bool						OnKeyboardPress(int key);

	virtual		void					OnRender();
	virtual		bool					IsEnemy(game_PlayerState* ps);
	virtual		bool					IsEnemy(CEntityAlive* ea1, CEntityAlive* ea2);

	virtual void						UpdateMapLocations();
	virtual	void						OnSwitchPhase(u32 old_phase, u32 new_phase);

	virtual	void						SetScore();
	virtual	void						OnSwitchPhase_InProgress();

	virtual u8 GetTeamCount() { return 9; };//m4d_RP
	virtual		void					OnConnected();
};

IC bool	RP_Compare_Players(game_PlayerState* p1, game_PlayerState* p2)
{
	return DM_Compare_Players(p1, p2);
};