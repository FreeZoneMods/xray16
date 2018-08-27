#include "stdafx.h"
#include "game_cl_roleplay.h"
#include "xrMessages.h"
#include "level.h"
#include "UIGameRP.h"
#include "xr_level_controller.h"
#include "map_manager.h"
#include "map_location.h"
#include "actor.h"
#include "ui/UIMainIngameWnd.h"
#include "ui/UISkinSelectorRP.h"
#include "ui/UIPDAWnd.h"
#include "ui/UIMapDesc.h"
#include "game_base_menu_events.h"
#include "ui/TeamInfo.h"
#include "string_table.h"
#include "clsid_game.h"
#include "ui/UIActorMenu.h"

#include "MainMenu.h"

#define MESSAGE_MENUS	"rp_messages_menu"

#include "game_cl_teamdeathmatch_snd_messages.h"
#include "reward_event_generator.h"

const shared_str game_cl_RolePlay::GetTeamMenu(s16 team)
{
	switch (team)
	{
		case 0: return "roleplay_team0"; break;
		case 1: return "roleplay_team1"; break;
		case 2: return "roleplay_team2"; break;
		case 3: return "roleplay_team3"; break;
		case 4: return "roleplay_team4"; break;
		case 5: return "roleplay_team5"; break;
		case 6: return "roleplay_team6"; break;
		case 7: return "roleplay_team7"; break;
		case 8: return "roleplay_team8"; break;
		case 9: return "roleplay_team9"; break;
		default: NODEFAULT;
	};
#ifdef DEBUG
	return NULL;
#endif// DEBUG
}

game_cl_RolePlay::game_cl_RolePlay()
{
	PresetItemsTeam1.clear();
	PresetItemsTeam2.clear();
	//--------------------------m4d_RP
	PresetItemsTeam3.clear();
	PresetItemsTeam4.clear();
	PresetItemsTeam5.clear();
	PresetItemsTeam6.clear();
	PresetItemsTeam7.clear();
	PresetItemsTeam8.clear();
	PresetItemsTeam9.clear();

	//---m4d_SkinSelector
	pCurSkinMenu = NULL;

	m_bTeamSelected = FALSE;
	m_game_ui = NULL;

	m_bShowPlayersNames = false;
	m_bFriendlyIndicators = false;
	m_bFriendlyNames = false;

	LoadSndMessages();

	//-------m4d_pda
	pPdaMenu = nullptr;
}
void game_cl_RolePlay::Init()
{
	//	pInventoryMenu	= xr_new<CUIInventoryWnd>();
	//	pPdaMenu = xr_new<CUIPdaWnd>();
	//	pMapDesc = xr_new<CUIMapDesc>();

	//----m4d_pda
	if (!g_dedicated_server)
	{
		pPdaMenu = new CUIPdaWnd();
	}
	//-----------------------------------------------------------
	LoadTeamData(GetTeamMenu(1));
	LoadTeamData(GetTeamMenu(2));
	//-------------------------------------------m4d
	LoadTeamData(GetTeamMenu(3));
	LoadTeamData(GetTeamMenu(4));
	LoadTeamData(GetTeamMenu(5));
	LoadTeamData(GetTeamMenu(6));
	LoadTeamData(GetTeamMenu(7));
	LoadTeamData(GetTeamMenu(8));
	LoadTeamData(GetTeamMenu(9));
}

game_cl_RolePlay::~game_cl_RolePlay()
{
	PresetItemsTeam1.clear();
	PresetItemsTeam2.clear();
	//-------------------------------------------m4d
	PresetItemsTeam3.clear();
	PresetItemsTeam4.clear();
	PresetItemsTeam5.clear();
	PresetItemsTeam6.clear();
	PresetItemsTeam7.clear();
	PresetItemsTeam8.clear();
	PresetItemsTeam9.clear();

	//---m4d_SkinSelector
	xr_delete(pCurSkinMenu);

	xr_delete(pCurBuyMenu);
	xr_delete(pCurSkinMenu);

	//------m4d_pda
	xr_delete(pPdaMenu);

	//	xr_delete(pInventoryMenu);
}

void game_cl_RolePlay::net_import_state(NET_Packet& P)
{
	//bool teamsEqual = (!teams.empty()) ? (teams[0].score == teams[1].score) : false;
	u16 old_phase = Phase();
	inherited::net_import_state(P);
	u16 new_phase = Phase();
	m_bFriendlyIndicators = !!P.r_u8();
	m_bFriendlyNames = !!P.r_u8();
	/*if (!teams.empty())
	{
		if (teamsEqual)
		{
			if (teams[0].score != teams[1].score)
			{
				if (Level().CurrentViewEntity())
				{
					if (teams[0].score > teams[1].score)
						PlaySndMessage(ID_TEAM1_LEAD);
					else
						PlaySndMessage(ID_TEAM2_LEAD);
				}
			}
		}
		else
		{
			if (teams[0].score == teams[1].score)
				if (Level().CurrentViewEntity())
					PlaySndMessage(ID_TEAMS_EQUAL);
		}
	};
	if ((old_phase != new_phase) &&
		(
		(new_phase == GAME_PHASE_TEAM1_SCORES) ||
			(new_phase == GAME_PHASE_TEAM2_SCORES)
			))
	{
		if (m_reward_generator)
			m_reward_generator->OnRoundEnd();
	}*/
}
void game_cl_RolePlay::TranslateGameMessage(u32 msg, NET_Packet& P)
{
	CStringTable st;
	string512 Text;
	//	LPSTR	Color_Teams[3]	= {"%c[255,255,255,255]", "%c[255,64,255,64]", "%c[255,64,64,255]"};
	char	Color_Main[] = "%c[255,192,192,192]";
	//	LPSTR	TeamsNames[3]	= {"Zero Team", "Team Green", "Team Blue"};

	switch (msg) {
	case GAME_EVENT_PLAYER_JOIN_TEAM: //tdm
	{
		string64 PlayerName;
		P.r_stringZ(PlayerName);
		u16 Team;
		P.r_u16(Team);

		xr_sprintf(Text, "%s%s %s%s %s%s",
			"",//no color
			PlayerName,
			Color_Main,
			*st.translate("mp_joined"),
			CTeamInfo::GetTeam_color_tag(int(Team)),
			CTeamInfo::GetTeam_name(int(Team)));
		if (CurrentGameUI()) CurrentGameUI()->CommonMessageOut(Text);
		//---------------------------------------
		Msg("%s %s %s", PlayerName, *st.translate("mp_joined"),
			CTeamInfo::GetTeam_name(int(Team)));
	}break;

	case PLAYER_CHANGE_TEAM://tdm
	{
		u16 PlayerID, OldTeam, NewTeam;
		P.r_u16(PlayerID);
		P.r_u16(OldTeam);
		P.r_u16(NewTeam);

		game_PlayerState* pPlayer = GetPlayerByGameID(PlayerID);
		if (!pPlayer) break;

		xr_sprintf(Text, "%s%s %s%s %s%s",
			CTeamInfo::GetTeam_color_tag(int(OldTeam)),
			pPlayer->getName(),
			Color_Main,
			*st.translate("mp_switched_to"),
			CTeamInfo::GetTeam_color_tag(int(NewTeam)),
			CTeamInfo::GetTeam_name(int(NewTeam)));
		if (CurrentGameUI()) CurrentGameUI()->CommonMessageOut(Text);
		//---------------------------------------
		Msg("%s *s %s", pPlayer->getName(), *st.translate("mp_switched_to"), CTeamInfo::GetTeam_name(int(NewTeam)));
	}break;

	default:
		inherited::TranslateGameMessage(msg, P);
	};
}

void game_cl_RolePlay::SetGameUI(CUIGameCustom* uigame)
{
	inherited::SetGameUI(uigame);
	m_game_ui = smart_cast<CUIGameRP*>(uigame);
	R_ASSERT(m_game_ui);
};

CUIGameCustom* game_cl_RolePlay::createGameUI()
{
	if (g_dedicated_server)
		return NULL;

	CLASS_ID clsid = CLSID_GAME_UI_ROLEPLAY;
	m_game_ui = smart_cast<CUIGameRP*> (NEW_INSTANCE(clsid));
	R_ASSERT(m_game_ui);
	m_game_ui->Load();
	m_game_ui->SetClGame(this);
	LoadMessagesMenu(MESSAGE_MENUS);
	return					m_game_ui;
}

void game_cl_RolePlay::GetMapEntities(xr_vector<SZoneMapEntityData>& dst)
{
	SZoneMapEntityData D;
	u32 color_self_team = 0xff00ff00;
	D.color = color_self_team;

	s16 local_team = local_player->team;

	PLAYERS_MAP_IT it = players.begin();
	for (; it != players.end(); ++it) {
		if (local_team == it->second->team) {
			u16 id = it->second->GameID;
			CObject* pObject = Level().Objects.net_Find(id);
			if (!pObject) continue;
			if (!pObject || !smart_cast<CActor*>(pObject)) continue;

			VERIFY(pObject);
			D.pos = pObject->Position();
			dst.push_back(D);
		}
	}
}

void game_cl_RolePlay::OnMapInfoAccept()
{
	if (CanCallTeamSelectMenu())
		m_game_ui->m_pUITeamSelectWnd->ShowDialog(true);
	//.		m_game_ui->StartStopMenu(m_game_ui->m_pUITeamSelectWnd, true);
};

void game_cl_RolePlay::OnTeamMenu_Cancel()
{
	m_game_ui->m_pUITeamSelectWnd->HideDialog();
	//.	m_game_ui->StartStopMenu(m_game_ui->m_pUITeamSelectWnd, true);

	if (!m_bTeamSelected && !m_bSpectatorSelected)
	{
		if (CanCallTeamSelectMenu() && !m_game_ui->m_pUITeamSelectWnd->IsShown())
		{
			m_game_ui->m_pUITeamSelectWnd->ShowDialog(true);
			//.			m_game_ui->StartStopMenu(m_game_ui->m_pUITeamSelectWnd, true);
			return;
		}
	}
	m_bMenuCalledFromReady = FALSE;
};

void game_cl_RolePlay::OnTeamMenuBack() //------------------------------------------------����� �� ������ �������---------���������� ���� � ���������� ���������: 
{
	//if (local_player->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
	//{
	//	m_game_ui->ShowServerInfo();
	//	//.		m_game_ui->StartStopMenu(m_game_ui->m_pMapDesc, true);
	//}
	if (Level().CurrentControlEntity() && !smart_cast<CActor*>(Level().CurrentControlEntity()) && !m_bSpectatorSelected)
	{
		MainMenu()->Activate(true);
	}
};

void game_cl_RolePlay::OnSkinMenuBack() //--------------------------------------------------����� �� ������---------���������� ���� � ���������� ���������: 
{
	//if (CanCallTeamSelectMenu())
	//	m_game_ui->m_pUITeamSelectWnd->ShowDialog(true);
	////.		m_game_ui->StartStopMenu(m_game_ui->m_pUITeamSelectWnd, true);
	if (CanCallTeamSelectMenu())
	{
		if (Level().CurrentControlEntity() && !smart_cast<CActor*>(Level().CurrentControlEntity()))
		{
			m_game_ui->m_pUITeamSelectWnd->ShowDialog(true);
		}
	}
};

void game_cl_RolePlay::OnSpectatorSelect()
{
	m_bTeamSelected = FALSE;
	inherited::OnSpectatorSelect();
}

void game_cl_RolePlay::OnTeamSelect(int Team)
{
	bool NeedToSendTeamSelect = true;
	if (Team != -1)
	{
		if (Team + 1 == local_player->team && m_bSkinSelected)
			NeedToSendTeamSelect = false;
		else
		{
			NeedToSendTeamSelect = true;
		}
	}

	if (NeedToSendTeamSelect)
	{
		CObject *l_pObj = Level().CurrentEntity();

		CGameObject *l_pPlayer = smart_cast<CGameObject*>(l_pObj);
		if (!l_pPlayer) return;

		NET_Packet		P;
		l_pPlayer->u_EventGen(P, GE_GAME_EVENT, l_pPlayer->ID());
		P.w_u16(GAME_EVENT_PLAYER_GAME_MENU);
		P.w_u8(PLAYER_CHANGE_TEAM);

		P.w_s16(s16(Team + 1));
		//P.w_u32			(0);
		l_pPlayer->u_EventSend(P);
		//-----------------------------------------------------------------
		m_bSkinSelected = FALSE;
	};
	//-----------------------------------------------------------------
	m_bTeamSelected = TRUE;
	//---------------------------
	//	if (m_bMenuCalledFromReady)
	//	{
	//		OnKeyboardPress(kJUMP);
	//	}
};
//-----------------------------------------------------------------
void game_cl_RolePlay::SetCurrentBuyMenu()
{
	if (!local_player) return;
	if (!local_player->team || local_player->skin == -1) return;
	if (g_dedicated_server) return;

	if (!pCurBuyMenu)
	{
		//-----m4d_RP
		s16 team_index = local_player->team;
		pCurBuyMenu = InitBuyMenu(GetBaseCostSect(), team_index);
		switch (team_index)
		{
			case 1:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam1);
				pCurPresetItems = &PresetItemsTeam1;
			}
			break;
			case 2:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam2);
				pCurPresetItems = &PresetItemsTeam2;
			}
			break;
			case 3:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam3);
				pCurPresetItems = &PresetItemsTeam3;
			}
			break;
			case 4:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam4);
				pCurPresetItems = &PresetItemsTeam4;
			}
			break;
			case 5:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam5);
				pCurPresetItems = &PresetItemsTeam5;
			}
			break;
			case 6:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam6);
				pCurPresetItems = &PresetItemsTeam6;
			}
			break;
			case 7:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam7);
				pCurPresetItems = &PresetItemsTeam7;
			}
			break;
			case 8:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam8);
				pCurPresetItems = &PresetItemsTeam8;
			}
			break;
			case 9:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam9);
				pCurPresetItems = &PresetItemsTeam9;
			}
			break;
			default:
			{
				LoadTeamDefaultPresetItems(GetTeamMenu(team_index), pCurBuyMenu, &PresetItemsTeam2);
				pCurPresetItems = &PresetItemsTeam2;
			}
			break;
		};
		LoadDefItemsForRank(pCurBuyMenu);
	};

	if (!pCurBuyMenu) return;

	//-----------------------------------
	if (m_cl_dwWarmUp_Time != 0)
		pCurBuyMenu->IgnoreMoneyAndRank(true);
	else
		pCurBuyMenu->IgnoreMoneyAndRank(false);
	//-----------------------------------
};

//---m4d_SkinSelector
CUISkinSelectorWndRP* game_cl_RolePlay::InitSkinMenu(s16 Team)
{
	if (Team == -1)
	{
		Team = local_player->team;
	};

	cl_TeamStruct *pTeamSect = &TeamList[ModifyTeam(Team)];

	CUISkinSelectorWndRP* pMenu = xr_new<CUISkinSelectorWndRP>((char*)pTeamSect->caSection.c_str(), Team);
	return							pMenu;
};

void game_cl_RolePlay::SetCurrentSkinMenu()
{
	s16 new_team;
	if (!local_player) return;

	//----m4d_RP
	//new_team = local_player->team;
	switch (local_player->team)
	{
	    case 1:
	    {
	        new_team = 1;
	    }
	    break;
	    case 2:
	    {
	        new_team = 2;
	    }
	    break;
	    case 3:
	    {
	        new_team = 3;
	    }
	    break;
	    case 4:
	    {
	        new_team = 4;
	    }
	    break;
	    case 5:
	    {
	        new_team = 5;
	    }
	    break;
	    case 6:
	    {
	        new_team = 6;
	    }
	    break;
	    case 7:
	    {
	        new_team = 7;
	    }
	    break;
	    case 8:
	    {
	        new_team = 8;
	    }
	    break;
	    case 9:
	    {
	        new_team = 9;
	    }
	    break;
	    default:
	    {
	        new_team = 2;
	    }
	    break;
	}

	if (pCurSkinMenu && pCurSkinMenu->GetTeam() == new_team)
		return;

	if (pCurSkinMenu && new_team != pCurSkinMenu->GetTeam())
		if (pCurSkinMenu->IsShown())
			pCurSkinMenu->HideDialog();
	//.			m_game_ui->StartStopMenu(pCurSkinMenu,true);

	xr_delete(pCurSkinMenu);
	pCurSkinMenu = InitSkinMenu(new_team);
};

bool game_cl_RolePlay::CanBeReady()
{
	if (!local_player) return false;

	m_bMenuCalledFromReady = TRUE;

	if (!m_bTeamSelected)
	{
		m_bMenuCalledFromReady = FALSE;
		if (CanCallTeamSelectMenu())
			m_game_ui->m_pUITeamSelectWnd->ShowDialog(true);
		//.			m_game_ui->StartStopMenu(m_game_ui->m_pUITeamSelectWnd,true);

		return false;
	}

	//---m4d_SkinSelector
	SetCurrentSkinMenu();

	SetCurrentBuyMenu();

	if (pCurBuyMenu && !pCurBuyMenu->IsShown())
	{
		pCurBuyMenu->ResetItems();
		SetBuyMenuItems(&PlayerDefItems);
	}

	if (!m_bSkinSelected)
	{
		m_bMenuCalledFromReady = FALSE;
		if (CanCallSkinMenu())
			pCurSkinMenu->ShowDialog(true);

		return false;
	}

	if (pCurBuyMenu)
	{
		const preset_items& _p = pCurBuyMenu->GetPreset(_preset_idx_last);
		bool Passed = false;
		Passed = (_p.size() == 0) ? 1 : (s32(pCurBuyMenu->GetPresetCost(_preset_idx_last)) <= local_player->money_for_round);
		Passed |= pCurBuyMenu->IsIgnoreMoneyAndRank();
		if (!Passed)
		{
			if (CanCallBuyMenu())
			{
				ShowBuyMenu();
			}
			return false;
		}
		m_bMenuCalledFromReady = FALSE;
		OnBuyMenu_Ok();
		return true;
	};
	//m_bMenuCalledFromReady = FALSE;
	return true;

	//return inherited::CanBeReady();
};

char* game_cl_RolePlay::getTeamSection(int Team)
{
	//---m4d_RP
	switch (Team)
	{
		case 1: return "roleplay_team1";
		case 2: return "roleplay_team2";
		case 3: return "roleplay_team3";
		case 4: return "roleplay_team4";
		case 5: return "roleplay_team5";
		case 6: return "roleplay_team6";
		case 7: return "roleplay_team7";
		case 8: return "roleplay_team8";
		case 9: return "roleplay_team9";
	default: return NULL;
	};
};

#include "string_table.h"
#include "ui/teaminfo.h"

#include "game_cl_deathmatch_snd_messages.h" //---m4d_SkinSelector

void game_cl_RolePlay::shedule_Update(u32 dt)
{
	CStringTable st;
	string512	msg;

	//---m4d_SkinSelector~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//inherited::shedule_Update(dt);
	game_cl_mp::shedule_Update(dt);

	if (g_dedicated_server)	return;

	//fake	
	if (m_game_ui)
	{
		m_game_ui->SetTimeMsgCaption(NULL);
		m_game_ui->SetRoundResultCaption(NULL);
		m_game_ui->SetSpectatorMsgCaption(NULL);
		m_game_ui->SetPressJumpMsgCaption(NULL);
		m_game_ui->SetPressBuyMsgCaption(NULL);
		m_game_ui->SetForceRespawnTimeCaption(NULL);
		m_game_ui->SetWarmUpCaption(NULL);
	};
	//	if (CurrentGameUI() && CurrentGameUI()->UIMainIngameWnd)
	//		CurrentGameUI()->UIMainIngameWnd->ZoneCounter().SetText("");

	switch (Phase())
	{
	case GAME_PHASE_INPROGRESS:
	{
		//m_game_ui->ShowPlayersList(false);

		Check_Invincible_Players();

		if (!m_game_ui)
			break;

		if (m_s32TimeLimit && m_cl_dwWarmUp_Time == 0)
		{
			if (Level().timeServer()<(m_start_time + m_s32TimeLimit))
			{
				u32 lts = Level().timeServer();
				u32 Rest = (m_start_time + m_s32TimeLimit) - lts;
				string64 S;
				ConvertTime2String(&S, Rest);
				m_game_ui->SetTimeMsgCaption(S);
			}
			else
			{
				m_game_ui->SetTimeMsgCaption("00:00:00");
			}
		};
		game_PlayerState* lookat_player = Game().lookat_player();
		if (local_player && !local_player->IsSkip())
		{
			if (m_bFirstRun)
			{
				m_bFirstRun = FALSE;
				if (!Level().IsDemoPlayStarted() && Level().CurrentEntity())
				{
					VERIFY(m_game_ui);
					m_bFirstRun = m_game_ui->ShowServerInfo() ? FALSE : TRUE;
				}

				GetActiveVoting();
			};

			if (lookat_player)
			{
				string256 MoneyStr;
				xr_sprintf(MoneyStr, "%d", lookat_player->money_for_round);
				m_game_ui->ChangeTotalMoneyIndicator(MoneyStr);
			}

			m_game_ui->SetPressJumpMsgCaption(NULL);
			m_game_ui->SetPressBuyMsgCaption(NULL);

			if (m_cl_dwWarmUp_Time > Level().timeServer())
			{
				u32 TimeRemains = m_cl_dwWarmUp_Time - Level().timeServer();
				string64 S;
				ConvertTime2String(&S, TimeRemains);
				string1024 tmpStr = "";
				if (TimeRemains > 10000)
					strconcat(sizeof(tmpStr), tmpStr, *st.translate("mp_time2start"), " ", S);
				else
				{
					if (TimeRemains < 1000)
						strconcat(sizeof(tmpStr), tmpStr, *st.translate("mp_go"), "");
					else
					{
						static u32 dwLastTimeRemains = 10;
						u32 dwCurTimeRemains = TimeRemains / 1000;
						if (dwLastTimeRemains != dwCurTimeRemains)
						{
							if (dwCurTimeRemains > 0 && dwCurTimeRemains <= 5)
								PlaySndMessage(ID_COUNTDOWN_1 + dwCurTimeRemains - 1);
						}
						dwLastTimeRemains = dwCurTimeRemains;
						_itoa(dwCurTimeRemains, S, 10);
						strconcat(sizeof(tmpStr), tmpStr, *st.translate("mp_ready"), "...", S);
					}
				};

				m_game_ui->SetWarmUpCaption(tmpStr);
			}

			if (Level().CurrentEntity() && smart_cast<CSpectator*>(Level().CurrentEntity()))
			{
				if (!(pCurBuyMenu && pCurBuyMenu->IsShown()) &&
					!(pCurSkinMenu && pCurSkinMenu->IsShown()) &&
					!m_game_ui->IsServerInfoShown() &&
					(CurrentGameUI() && CurrentGameUI()->GameIndicatorsShown())
					)
				{
					if (!m_bSkinSelected)
						m_game_ui->SetPressJumpMsgCaption("mp_press_jump2select_skin");
					else
						m_game_ui->SetPressJumpMsgCaption("mp_press_jump2start");

					if (CanCallBuyMenu())
						m_game_ui->SetPressBuyMsgCaption("mp_press_to_buy");
				};
			};

			if (Level().CurrentControlEntity() &&
				smart_cast<CSpectator*>(Level().CurrentControlEntity()) &&
				(CurrentGameUI()->GameIndicatorsShown())
				)
			{

				CSpectator* pSpectator = smart_cast<CSpectator*>(Level().CurrentControlEntity());
				if (pSpectator)
				{
					string1024 SpectatorStr = "";
					pSpectator->GetSpectatorString(SpectatorStr);
					m_game_ui->SetSpectatorMsgCaption(SpectatorStr);
				}
			}

			u32 CurTime = Level().timeServer();
			if (IsVotingEnabled() && IsVotingActive() && m_dwVoteEndTime >= CurTime)
			{
				u32 TimeLeft = m_dwVoteEndTime - Level().timeServer();
				string1024 VoteTimeResStr;
				u32 SecsLeft = (TimeLeft % 60000) / 1000;
				u32 MinitsLeft = (TimeLeft - SecsLeft) / 60000;

				u32 NumAgreed = 0;
				PLAYERS_MAP_IT I;
				I = players.begin();
				for (; I != players.end(); ++I)
				{
					game_PlayerState* ps = I->second;
					if (ps->m_bCurrentVoteAgreed == 1) NumAgreed++;
				}

				xr_sprintf(VoteTimeResStr, st.translate("mp_timeleft").c_str(), MinitsLeft, SecsLeft, float(NumAgreed) / players.size());
				m_game_ui->SetVoteTimeResultMsg(VoteTimeResStr);
			};

			if (local_player->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD) &&
				m_u32ForceRespawn &&
				!local_player->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
			{
				u32 Rest = m_u32ForceRespawn - local_player->DeathTime;
				string64			S;
				ConvertTime2String(&S, Rest);
				string128			FullS;
				xr_sprintf(FullS, "%s : %s", *st.translate("mp_time2respawn"), S);

				m_game_ui->SetForceRespawnTimeCaption(FullS);
			};


			if (Level().CurrentViewEntity())
			{
				game_PlayerState* ps = GetPlayerByGameID(Level().CurrentViewEntity()->ID());

				if (ps && m_game_ui)
					m_game_ui->SetRank(ps->team, ps->rank);

				if (ps && m_game_ui)
					m_game_ui->SetFraglimit(ps->frags(), m_s32FragLimit);
			}
		};
	}break;
	case GAME_PHASE_PENDING:
	{
		if (!m_game_ui)
			break;

		m_game_ui->UpdateTeamPanels();
		m_game_ui->ShowPlayersList(true);
	}break;
	case GAME_PHASE_PLAYER_SCORES:
	{
		if (!m_game_ui)
			break;

		string128 resstring;
		xr_sprintf(resstring, st.translate("mp_player_wins").c_str(), WinnerName);
		m_game_ui->SetRoundResultCaption(resstring);

		SetScore();
		m_game_ui->UpdateTeamPanels();
		m_game_ui->ShowPlayersList(true);
	}break;
	};

	//-----------------------------------------
	if (!CanCallBuyMenu()) HideBuyMenu();

	if (pCurSkinMenu && pCurSkinMenu->IsShown() && !CanCallSkinMenu())
		pCurSkinMenu->HideDialog();
	//-----------------------------------------------

	//-----------------------------------------------
	//if (m_game_ui->m_pInventoryMenu && m_game_ui->m_pInventoryMenu->IsShown() && !CanCallInventoryMenu())
	//	StartStopMenu(m_game_ui->m_pInventoryMenu,true);
	if (m_game_ui && m_game_ui->ActorMenu().IsShown() && !CanCallInventoryMenu())
	{
		m_game_ui->HideActorMenu();
	}

	//-----------------------------------------

	u32 cur_game_state = Phase();
	//if(m_game_ui->m_pMapDesc && m_game_ui->m_pMapDesc->IsShown() && cur_game_state!=GAME_PHASE_INPROGRESS)
	//{
	//	m_game_ui->m_pMapDesc->HideDialog();
	//}

	if (pCurSkinMenu && pCurSkinMenu->IsShown() && cur_game_state != GAME_PHASE_INPROGRESS)
	{
		pCurSkinMenu->HideDialog();
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (!m_game_ui) return;
	//---------------------------------------------------------
	if (m_game_ui->m_pUITeamSelectWnd && m_game_ui->m_pUITeamSelectWnd->IsShown() && !CanCallTeamSelectMenu())
		m_game_ui->m_pUITeamSelectWnd->HideDialog();
	//.		m_game_ui->StartStopMenu(m_game_ui->m_pUITeamSelectWnd,true);
	//---------------------------------------------------------

	if (m_game_ui)
		m_game_ui->SetBuyMsgCaption(NULL);

	switch (m_phase)
	{
		case GAME_PHASE_TEAM1_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(1));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM2_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(2));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();

		}
		break;
		//-----------m4d_RP
		case GAME_PHASE_TEAM3_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(3));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM4_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(4));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM5_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(5));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM6_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(6));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM7_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(7));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM8_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(8));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_TEAM9_SCORES:
		{
			xr_sprintf(msg, st.translate("mp_team_wins").c_str(), CTeamInfo::GetTeam_name(9));
			m_game_ui->SetRoundResultCaption(msg);

			m_game_ui->UpdateTeamPanels();
			m_game_ui->ShowPlayersList(true);

			SetScore();
		}
		break;
		case GAME_PHASE_INPROGRESS:
		{
			if (local_player && !local_player->IsSkip())
			{
				if (Level().CurrentEntity() && smart_cast<CSpectator*>(Level().CurrentEntity()))
				{
					if (!(pCurBuyMenu && pCurBuyMenu->IsShown()) &&
						!(pCurSkinMenu && pCurSkinMenu->IsShown()) &&
						!m_game_ui->IsServerInfoShown() &&
						(CurrentGameUI() && CurrentGameUI()->GameIndicatorsShown())
						)
					{
						if (!m_bTeamSelected)
							m_game_ui->SetPressJumpMsgCaption("mp_press_jump2select_team");
					};
				};
				SetScore();

				if (local_player->testFlag(GAME_PLAYER_FLAG_ONBASE) &&
					!local_player->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
				{
					string1024 msg;
					xr_sprintf(msg, *st.translate("mp_press_to_buy"), "B");
					if (m_game_ui)
						m_game_ui->SetBuyMsgCaption(msg);
					m_bBuyEnabled = true;
				}
				else if (!local_player->testFlag(GAME_PLAYER_FLAG_ONBASE) &&
					!local_player->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
				{
					m_bBuyEnabled = false;
				}
				else if (local_player->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
				{
					m_bBuyEnabled = true;
				}
			};
		}break;
		default:
		{
		}
		break;
	};

}

void game_cl_RolePlay::SetScore()
{
	if (local_player)
	{
		s16 lt = local_player->team;
		if (lt >= 0)
		{
			//--------------m4d_RP
			if (m_game_ui)
				m_game_ui->SetScoreCaption(teams[0].score, teams[1].score, teams[2].score, teams[3].score, teams[4].score, teams[5].score, teams[6].score, teams[7].score, teams[8].score); 
		}
	}
};

bool game_cl_RolePlay::OnKeyboardPress(int key)
{
	if (inherited::OnKeyboardPress(key))	return true;
	if (kTEAM == key)
	{
		if (m_game_ui && CanCallTeamSelectMenu())
			m_game_ui->m_pUITeamSelectWnd->ShowDialog(true);

		return true;
	};

	//---m4d_SkinSelector
	if (kSKIN == key)
	{
		if (pCurSkinMenu && pCurSkinMenu->IsShown())
			pCurSkinMenu->HideDialog();
		else
		{
			if (CanCallSkinMenu())
			{
				SetCurrentSkinMenu();
				pCurSkinMenu->ShowDialog(true);
			}
		}
		return true;
	};
	//----------------------------------------------------------------m4d_pda
	if (kACTIVE_JOBS == key)
	{
		if (Level().CurrentControlEntity() && smart_cast<CActor*>(Level().CurrentControlEntity()))
		{
			if (m_game_ui)
			{
				if (pPdaMenu && pPdaMenu->IsShown())
					pPdaMenu->HideDialog();
				else
				{
					pPdaMenu->SetActiveSubdialog("eptTasks");
					pPdaMenu->ShowDialog(true);
				};
				return true;
			}
		}
	};

	return false;
}

bool game_cl_RolePlay::IsEnemy(game_PlayerState* ps) //---------m4d_? ��� ����
{
	if (!local_player) return false;
	return local_player->team != ps->team;
};

bool game_cl_RolePlay::IsEnemy(CEntityAlive* ea1, CEntityAlive* ea2)
{
	return (ea1->g_Team() != ea2->g_Team());
};

#define PLAYER_NAME_COLOR 0xff40ff40

void game_cl_RolePlay::OnRender()
{
	game_PlayerState* lookat_player = Game().lookat_player();
	if (local_player && (local_player == lookat_player) &&
		(m_bShowPlayersNames || m_bFriendlyIndicators))
	{
		cl_TeamStruct *pTS = &TeamList[ModifyTeam(local_player->team)];
		PLAYERS_MAP_IT it = players.begin();
		for (; it != players.end(); ++it)
		{
			game_PlayerState* ps = it->second;
			u16 id = ps->GameID;
			if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD)) continue;
			CObject* pObject = Level().Objects.net_Find(id);
			if (!pObject) continue;
			if (!pObject || !smart_cast<CActor*>(pObject)) continue;
			if (IsEnemy(ps)) continue;


			if (ps == local_player)
				continue;


			float dup = 0.0f;
			if (/*m_bFriendlyNames &&*/ m_bShowPlayersNames)
			{
				VERIFY(pObject);
				CActor* pActor = smart_cast<CActor*>(pObject);
				VERIFY(pActor);
				Fvector IPos = pTS->IndicatorPos;
				IPos.y -= pTS->Indicator_r2;
				pActor->RenderText(ps->getName(), IPos, &dup, PLAYER_NAME_COLOR);
			}
			if (m_bFriendlyIndicators)
			{
				VERIFY(pObject);
				CActor* pActor = smart_cast<CActor*>(pObject);
				VERIFY(pActor);
				Fvector IPos = pTS->IndicatorPos;
				IPos.y += dup;
				pActor->RenderIndicator(IPos, pTS->Indicator_r1, pTS->Indicator_r2, pTS->IndicatorShader);
			};
		}
	};
	inherited::OnRender();
}

BOOL game_cl_RolePlay::CanCallBuyMenu()
{
	if (Phase() != GAME_PHASE_INPROGRESS)
		return FALSE;

	if (!m_game_ui)
		return FALSE;

	if (m_game_ui->m_pUITeamSelectWnd && m_game_ui->m_pUITeamSelectWnd->IsShown())
		return FALSE;

	if (!m_bTeamSelected)
		return FALSE;

	if (!m_bSkinSelected)
		return FALSE;

	if (!is_buy_menu_ready())
		return FALSE;

	if (!m_bSkinSelected || m_bSpectatorSelected)
		return FALSE;

	if (pCurSkinMenu && pCurSkinMenu->IsShown())
		return FALSE;

	if (m_game_ui && m_game_ui->ActorMenu().IsShown())
		return FALSE;

	return m_bBuyEnabled;
};

BOOL game_cl_RolePlay::CanCallSkinMenu()
{
	if (!m_game_ui)	return FALSE;
	if (m_game_ui->m_pUITeamSelectWnd && m_game_ui->m_pUITeamSelectWnd->IsShown())
		return FALSE;
	if (!m_bTeamSelected) return FALSE;

	//---m4d_SkinSelector
	//return inherited::CanCallSkinMenu();
	if (Phase() != GAME_PHASE_INPROGRESS) return false;
	if (m_game_ui && m_game_ui->ActorMenu().IsShown())
	{
		return FALSE;
	}
	if (pCurBuyMenu && pCurBuyMenu->IsShown())
	{
		return FALSE;
	};
	SetCurrentSkinMenu();
	if (!pCurSkinMenu)	return FALSE;
	if (!pCurSkinMenu->IsShown())
		pCurSkinMenu->SetCurSkin(local_player->skin);
	return TRUE;
};

BOOL game_cl_RolePlay::CanCallInventoryMenu()
{
	if (!m_game_ui)	return FALSE;
	if (m_game_ui->m_pUITeamSelectWnd && m_game_ui->m_pUITeamSelectWnd->IsShown())
		return FALSE;
	
	//---m4d_SkinSelector
	//return inherited::CanCallInventoryMenu();
	if (Phase() != GAME_PHASE_INPROGRESS) return false;
	if (Level().CurrentEntity() && !smart_cast<CActor*>(Level().CurrentEntity()))
	{
		return FALSE;
	}
	if (pCurSkinMenu && pCurSkinMenu->IsShown())
	{
		return FALSE;
	};
	if (local_player->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
	{
		return FALSE;
	}
	return TRUE;
};

BOOL game_cl_RolePlay::CanCallTeamSelectMenu()
{
	if (Phase() != GAME_PHASE_INPROGRESS) return false;
	if (!local_player) return false;
	if (m_game_ui && m_game_ui->ActorMenu().IsShown())
	{
		return FALSE;
	}
	//------m4d_pda
	if (pPdaMenu && pPdaMenu->IsShown())
	{
		return FALSE;
	};
	if (pCurBuyMenu && pCurBuyMenu->IsShown())
	{
		return FALSE;
	};
	//----m4d_delete (skin_menu)
	//if (pCurSkinMenu && pCurSkinMenu->IsShown())
	//{
	//	return FALSE;
	//};

	m_game_ui->m_pUITeamSelectWnd->SetCurTeam(ModifyTeam(local_player->team));
	return TRUE;
};

#define FRIEND_LOCATION	"mp_friend_location"


void game_cl_RolePlay::UpdateMapLocations()
{
	inherited::UpdateMapLocations();
	if (local_player && !Level().CurrentEntity()) //-----m4d_pda (fix: �� ����� self ����������� ��� friend)
	{
		PLAYERS_MAP_IT it = players.begin();
		for (; it != players.end(); ++it)
		{
			game_PlayerState* ps = it->second;
			u16 id = ps->GameID;
			if (ps->testFlag(GAME_PLAYER_FLAG_VERY_VERY_DEAD))
			{
				Level().MapManager().RemoveMapLocation(FRIEND_LOCATION, id);
				continue;
			};

			CObject* pObject = Level().Objects.net_Find(id);
			if (!pObject || !smart_cast<CActor*>(pObject)) continue;
			if (IsEnemy(ps))
			{
				if (Level().MapManager().HasMapLocation(FRIEND_LOCATION, id))
				{
					Level().MapManager().RemoveMapLocation(FRIEND_LOCATION, id);
				};
				continue;
			};
			if (!Level().MapManager().HasMapLocation(FRIEND_LOCATION, id))
			{
				(Level().MapManager().AddMapLocation(FRIEND_LOCATION, id))->EnablePointer();
			}
		}
	};
};

void game_cl_RolePlay::LoadSndMessages()
{
	//	LoadSndMessage("dm_snd_messages", "you_won", ID_YOU_WON);
	LoadSndMessage("tdm_snd_messages", "team1_win", ID_TEAM1_WIN);
	LoadSndMessage("tdm_snd_messages", "team2_win", ID_TEAM2_WIN);
	LoadSndMessage("tdm_snd_messages", "teams_equal", ID_TEAMS_EQUAL);
	LoadSndMessage("tdm_snd_messages", "team1_lead", ID_TEAM1_LEAD);
	LoadSndMessage("tdm_snd_messages", "team2_lead", ID_TEAM2_LEAD);

	//-----m4d_RP
	LoadSndMessage("rp_snd_messages", "team_rank1", ID_RP_RANK_1);
	LoadSndMessage("rp_snd_messages", "team_rank2", ID_RP_RANK_2);
	LoadSndMessage("rp_snd_messages", "team_rank3", ID_RP_RANK_3);
	LoadSndMessage("rp_snd_messages", "team_rank4", ID_RP_RANK_4);
};

void game_cl_RolePlay::OnSwitchPhase_InProgress()
{
	HideBuyMenu();
	if (!m_bSkinSelected) m_bTeamSelected = FALSE;
};

void game_cl_RolePlay::OnSwitchPhase(u32 old_phase, u32 new_phase)
{
	inherited::OnSwitchPhase(old_phase, new_phase);
	switch (new_phase)
	{
	case GAME_PHASE_TEAM1_SCORES:
	{
		if (Level().CurrentViewEntity())
			PlaySndMessage(ID_TEAM1_WIN);
	}break;
	case GAME_PHASE_TEAM2_SCORES:
	{
		if (Level().CurrentViewEntity())
			PlaySndMessage(ID_TEAM2_WIN);
	}break;
	default:
	{
	}break;
	};
}

void game_cl_RolePlay::OnTeamChanged()
{
	xr_delete(pCurBuyMenu);
	SetCurrentBuyMenu();
	if (pCurBuyMenu)
	{
		ReInitRewardGenerator(local_player);
	}
	inherited::OnTeamChanged();
};

void game_cl_RolePlay::PlayRankChangesSndMessage()
{
	if (local_player)
	{
		switch (local_player->rank)
		{
		case 0: break;
		default:
		{
			//--------------------m4d_RP
			PlaySndMessage(ID_RP_RANK_0 + local_player->rank);
			break;
		}
		}
	}
};

void game_cl_RolePlay::OnGameMenuRespond_ChangeTeam(NET_Packet& P)
{
	s16 OldTeam = local_player->team;
	local_player->team = u8(P.r_s16() & 0x00ff);
	if (OldTeam != local_player->team)
	{
		OnTeamChanged();
		if (m_reward_generator)
			m_reward_generator->OnPlayerChangeTeam(local_player->team);
	}

	SetCurrentSkinMenu();
	if (pCurSkinMenu)
	{
		pCurSkinMenu->SetCurSkin(local_player->skin);
		if (CanCallSkinMenu())
			pCurSkinMenu->ShowDialog(true);
		//.			m_game_ui->StartStopMenu(pCurSkinMenu, true);
	}
};

bool game_cl_RolePlay::IsPlayerInTeam(game_PlayerState* ps, ETeam team)
{
	if (ps->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
	{
		if (team == etSpectatorsTeam)
		{
			return true;
		}
		return false;
	}
	return (ModifyTeam(s16(ps->team)) == s16(team));
}

LPCSTR game_cl_RolePlay::GetGameScore(string32&	score_dest)
{
	//------------------------------------------------------------------------------------------------------------m4d_RP
	xr_sprintf(score_dest, "[%d:%d:%d:%d:%d:%d:%d:%d:%d]", teams[0].score, teams[1].score, teams[2].score, teams[3].score, teams[4].score, teams[5].score, teams[6].score, teams[7].score, teams[8].score);
	return score_dest;
}

void game_cl_RolePlay::OnConnected()
{
	inherited::OnConnected();
	m_game_ui = smart_cast<CUIGameRP*>(CurrentGameUI());
}

//---m4d_SkinSelector
void game_cl_RolePlay::OnSkinMenu_Ok()
{
	CObject *l_pObj = Level().CurrentEntity();

	CGameObject *l_pPlayer = smart_cast<CGameObject*>(l_pObj);
	if (!l_pPlayer) return;

	NET_Packet		P;
	l_pPlayer->u_EventGen(P, GE_GAME_EVENT, l_pPlayer->ID());
	P.w_u16(GAME_EVENT_PLAYER_GAME_MENU);
	P.w_u8(PLAYER_CHANGE_SKIN);



	P.w_s8((u8)pCurSkinMenu->GetActiveIndex());
	l_pPlayer->u_EventSend(P);
	//-----------------------------------------------------------------
	m_bSkinSelected = TRUE;

};

void game_cl_RolePlay::OnGameMenuRespond_ChangeSkin(NET_Packet& P)
{
	s8 NewSkin = P.r_s8();
	local_player->skin = NewSkin;

	if (pCurSkinMenu && pCurSkinMenu->IsShown())
		pCurSkinMenu->HideDialog();

	//if (m_game_ui->m_pMapDesc && m_game_ui->m_pMapDesc->IsShown())
	//	m_game_ui->m_pMapDesc->HideDialog();


	SetCurrentSkinMenu();
	if (pCurSkinMenu)				pCurSkinMenu->SetCurSkin(local_player->skin);
	SetCurrentBuyMenu();
	ReInitRewardGenerator(local_player);
	m_bSpectatorSelected = FALSE;

	if (m_bMenuCalledFromReady)
	{
		OnKeyboardPress(kJUMP);
	}
};