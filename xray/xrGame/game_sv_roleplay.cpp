#include "stdafx.h"
#include "game_sv_roleplay.h"
#include "xrserver_objects_alife_monsters.h"
#include "xrserver.h"
#include "Level.h"
#include "game_cl_mp.h"
#include "string_table.h"
#include "clsid_game.h"
#include <functional>

#include "ui\UIBuyWndShared.h"

//-------------------------------------------------------
extern	s32			g_sv_dm_dwFragLimit;
extern	BOOL		g_sv_dm_bPDAHunt;
//-------------------------------------------------------
extern	BOOL		g_sv_tdm_bAutoTeamBalance;
extern	BOOL		g_sv_tdm_bAutoTeamSwap;
extern	BOOL		g_sv_tdm_bFriendlyIndicators;
extern	BOOL		g_sv_tdm_bFriendlyNames;
extern	float		g_sv_tdm_fFriendlyFireModifier;
//-------------------------------------------------------
extern	int			g_sv_tdm_iTeamKillLimit;
extern	int			g_sv_tdm_bTeamKillPunishment;
//-------------------------------------------------------
BOOL	game_sv_RolePlay::isFriendlyFireEnabled() { return (int(g_sv_tdm_fFriendlyFireModifier*100.0f) > 0); };
float	game_sv_RolePlay::GetFriendlyFire() { return (int(g_sv_tdm_fFriendlyFireModifier*100.0f) > 0) ? g_sv_tdm_fFriendlyFireModifier : 0.0f; };
BOOL	game_sv_RolePlay::Get_AutoTeamBalance() { return g_sv_tdm_bAutoTeamBalance; };
BOOL	game_sv_RolePlay::Get_AutoTeamSwap() { return g_sv_tdm_bAutoTeamSwap; };
BOOL	game_sv_RolePlay::Get_FriendlyIndicators() { return g_sv_tdm_bFriendlyIndicators; };
BOOL	game_sv_RolePlay::Get_FriendlyNames() { return g_sv_tdm_bFriendlyNames; };
int		game_sv_RolePlay::Get_TeamKillLimit() { return g_sv_tdm_iTeamKillLimit; };
BOOL	game_sv_RolePlay::Get_TeamKillPunishment() { return g_sv_tdm_bTeamKillPunishment; };
//-------------------------------------------------------
void	game_sv_RolePlay::Create(shared_str& options)
{
	inherited::Create(options);
	R_ASSERT2(rpoints[0].size(), "rpoints for specators not found");

	switch_Phase(GAME_PHASE_PENDING);
	game_TeamState td;
	td.score = 0;
	td.num_targets = 0;

	teams.push_back(td);//freedom
	teams.push_back(td);//merc
	//--------------------------------------------------------m4d_RP
	teams.push_back(td);//stalker
	teams.push_back(td);//bandit
	teams.push_back(td);//csky
	teams.push_back(td);//dolg
	teams.push_back(td);//monolith
	teams.push_back(td);//ecolog
	teams.push_back(td);//army

	teams_swaped = false;
	round_end_reason = eRoundEnd_Force;
}

void game_sv_RolePlay::net_Export_State(NET_Packet& P, ClientID to)
{
	inherited::net_Export_State(P, to);
	P.w_u8(u8(Get_FriendlyIndicators()));
	P.w_u8(u8(Get_FriendlyNames()));
}

u8 game_sv_RolePlay::AutoTeam()
{
	struct TeamPlayersCalculator
	{
		//------------m4d_RP
		u32 m_teams[9];
		TeamPlayersCalculator()
		{
			m_teams[0] = 0;
			m_teams[1] = 0;
			m_teams[2] = 0;
			m_teams[3] = 0;
			m_teams[4] = 0;
			m_teams[5] = 0;
			m_teams[6] = 0;
			m_teams[7] = 0;
			m_teams[8] = 0;
		}
		void operator()(IClient* client)
		{
			xrClientData *l_pC = static_cast<xrClientData*>(client);
			game_PlayerState* ps = l_pC->ps;
			if (!ps)
				return;
			if (!l_pC->net_Ready)
				return;
			if (ps->IsSkip() || ps->team == 0 || ps->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
				return;

			if (ps->team >= 1) ++(m_teams[ps->team - 1]);
		}
	};
	TeamPlayersCalculator team_calculator;
	m_server->ForEachClientDo(team_calculator);

	return (team_calculator.m_teams[0] > team_calculator.m_teams[1]) ? 2 : 1;
}

u32 game_sv_RolePlay::GetPlayersCountInTeams(u8 team)
{

	struct team_players_calculator
	{
		u8 team;
		u32 count;
		void operator()(IClient* client)
		{
			xrClientData* tmp_client = static_cast<xrClientData*>(client);
			game_PlayerState* ps = tmp_client->ps;
			if (!ps)
				return;
			if (tmp_client->net_Ready)
				return;
			if (ps->IsSkip() || ps->team == 0 || ps->testFlag(GAME_PLAYER_FLAG_SPECTATOR))
				return;
			if (ps->team >= team) ++count;
		}
	};
	team_players_calculator tmp_functor;
	tmp_functor.team = team;
	tmp_functor.count = 0;
	m_server->ForEachClientDo(tmp_functor);
	return tmp_functor.count;
};

bool game_sv_RolePlay::TeamSizeEqual()
{
	//-----m4d_RP
	return GetPlayersCountInTeams(1) == GetPlayersCountInTeams(2) == GetPlayersCountInTeams(3) == GetPlayersCountInTeams(4) == GetPlayersCountInTeams(5) == GetPlayersCountInTeams(6) == GetPlayersCountInTeams(7) == GetPlayersCountInTeams(8) == GetPlayersCountInTeams(9);
}

struct lowest_player_functor		//for autoteam balance
{
	s16 lowest_score;
	s16 MaxTeam;
	xrClientData* lowest_player;
	lowest_player_functor()
	{
		lowest_score = 32767;
		lowest_player = NULL;
	}
	void operator()(IClient* client)
	{
		xrClientData *l_pC = static_cast<xrClientData*>(client);
		game_PlayerState* ps = l_pC->ps;
		if (!ps) return;
		if (!l_pC->net_Ready) return;
		if (ps->IsSkip()) return;
		if (ps->team - 1 != MaxTeam) return;

		if (ps->frags() < lowest_score)
		{
			lowest_score = ps->frags();
			lowest_player = l_pC;
		};
	}
};

void game_sv_RolePlay::AutoBalanceTeams()
{
	return; // =)
	//----------------m4d_RP (хз)
	//if (!Get_AutoTeamBalance()) return;
	////calc team count
	//s16 MinTeam, MaxTeam;
	//u32 NumToMove;
	//struct team_counter_functor
	//{
	//	u8 l_teams[2];
	//	team_counter_functor()
	//	{
	//		l_teams[0] = 0;
	//		l_teams[1] = 0;
	//	}
	//	void operator()(IClient* client)
	//	{
	//		xrClientData *l_pC = static_cast<xrClientData*>(client);
	//		game_PlayerState* ps = l_pC->ps;
	//		if (!ps) return;
	//		if (!l_pC->net_Ready) return;
	//		if (ps->IsSkip()) return;
	//		if (ps->team >= 1) ++(l_teams[ps->team - 1]);
	//	}
	//};
	//team_counter_functor tmp_functor;
	//m_server->ForEachClientDo(tmp_functor);

	//if (tmp_functor.l_teams[0] == tmp_functor.l_teams[1]) return;

	//if (tmp_functor.l_teams[0] > tmp_functor.l_teams[1])
	//{
	//	MinTeam = 1;
	//	MaxTeam = 0;
	//}
	//else
	//{
	//	MinTeam = 0;
	//	MaxTeam = 1;
	//};

	//NumToMove = (tmp_functor.l_teams[MaxTeam] - tmp_functor.l_teams[MinTeam]) / 2;
	//if (!NumToMove) return;
	/////////////////////////////////////////////////////////////////////////
	//while (NumToMove)
	//{
	//	///////// get lowest score player from MaxTeam
	//	lowest_player_functor autob_functor;
	//	autob_functor.MaxTeam = MaxTeam;
	//	m_server->ForEachClientDo(autob_functor);
	//	R_ASSERT(autob_functor.lowest_player && autob_functor.lowest_player->ps);
	//	game_PlayerState* ps = autob_functor.lowest_player->ps;
	//	ps->team = u8((MinTeam + 1) & 0x00ff);
	//	NumToMove--;
	//}
};

void game_sv_RolePlay::OnRoundStart()
{
	if ((round_end_reason != eRoundEnd_Force) &&
		(round_end_reason != eRoundEnd_GameRestartedFast))
	{
		AutoBalanceTeams();
		AutoSwapTeams();
	}
	if (round_end_reason == eRoundEnd_GameRestarted)
	{
		teams_swaped = false;
	}
	inherited::OnRoundStart();
};

void game_sv_RolePlay::OnRoundEnd()
{
	inherited::OnRoundEnd();
	if (m_bMapNeedRotation)
	{
		if (Get_AutoTeamSwap() && !teams_swaped)
		{
			m_bMapNeedRotation = false;
		}
	}
};


void game_sv_RolePlay::OnPlayerConnect(ClientID id_who)
{
	inherited::OnPlayerConnect(id_who);

	xrClientData* xrCData = m_server->ID_to_client(id_who);
	game_PlayerState*	ps_who = get_id(id_who);
	//	LPCSTR	options				=	get_name_id	(id_who);
	ps_who->team = AutoTeam();//u8(get_option_i(options,"team",AutoTeam()));


	if (ps_who->IsSkip()) return;

	if (!xrCData->flags.bReconnect) Money_SetStart(id_who);
	SetPlayersDefItems(ps_who);
}

void game_sv_RolePlay::OnPlayerConnectFinished(ClientID id_who)
{
	xrClientData* xrCData = m_server->ID_to_client(id_who);
	VERIFY(xrCData && xrCData->ps);

	NET_Packet					P;
	GenerateGameMessage(P);
	P.w_u32(GAME_EVENT_PLAYER_CONNECTED);
	P.w_clientID(id_who);
	xrCData->ps->team = 3; //------------m4d_RP (дефолтная тима)
	xrCData->ps->m_iTeamKills = 0;
	xrCData->ps->setFlag(GAME_PLAYER_FLAG_SPECTATOR);
	xrCData->ps->setFlag(GAME_PLAYER_FLAG_READY);
	xrCData->ps->net_Export(P, true);
	u_EventSend(P);

	// Send Message About Client join Team
	GenerateGameMessage(P);
	P.w_u32(GAME_EVENT_PLAYER_JOIN_TEAM);
	P.w_stringZ(xrCData->ps->getName());
	P.w_u16(xrCData->ps->team);
	u_EventSend(P);

	SpawnPlayer(id_who, "spectator");
	Send_Anomaly_States(id_who);

	xrCData->net_Ready = TRUE;
};

void game_sv_RolePlay::OnPlayerSelectTeam(NET_Packet& P, ClientID sender)
{
	xrClientData *l_pC = m_server->ID_to_client(sender);
	s16 l_team;
	P.r_s16(l_team);
	OnPlayerChangeTeam(l_pC->ID, l_team);
	//-------------------------------------------------
};

void game_sv_RolePlay::OnPlayerChangeTeam(ClientID id_who, s16 team)
{
	game_PlayerState*	ps_who = get_id(id_who);
	if (!ps_who) return;
	if (!team)
	{
		if (!ps_who->team)
			team = AutoTeam();
		else
			if (TeamSizeEqual())
			{
				team = ps_who->team;
			}
			else
			{
				team = AutoTeam();
			}
	};
	//-----------------------------------------------------	
	NET_Packet Px;
	GenerateGameMessage(Px);
	Px.w_u32(GAME_EVENT_PLAYER_GAME_MENU_RESPOND);
	Px.w_u8(PLAYER_CHANGE_TEAM);
	Px.w_s16(team);
	m_server->SendTo(id_who, Px, net_flags(TRUE, TRUE));
	//-----------------------------------------------------
	if (ps_who->team == team) return;
	//-----------------------------------------------------
	KillPlayer(id_who, ps_who->GameID);
	//-----------------------------------------------------	
	ps_who->setFlag(GAME_PLAYER_FLAG_SPECTATOR);
	//-----------------------------------------------------
	s16 OldTeam = ps_who->team;
	ps_who->team = u8(team & 0x00ff);
	TeamStruct* pTS = GetTeamData(team);
	if (pTS)
	{
		if ((ps_who->money_for_round < pTS->m_iM_Start) || (OldTeam == 0))
			Money_SetStart(id_who);
	}

	/////////////////////////////////////////////////////////
	//Send Switch team message
	NET_Packet			P;
	//	P.w_begin			(M_GAMEMESSAGE);
	GenerateGameMessage(P);
	P.w_u32(PLAYER_CHANGE_TEAM);
	P.w_u16(ps_who->GameID);
	P.w_u16(ps_who->team);
	P.w_u16(team);
	u_EventSend(P);
	/////////////////////////////////////////////////////////



	SetPlayersDefItems(ps_who);

}

void game_sv_RolePlay::OnPlayerKillPlayer(game_PlayerState* ps_killer, game_PlayerState* ps_killed, KILL_TYPE KillType, SPECIAL_KILL_TYPE SpecialKillType, CSE_Abstract* pWeaponA)
{
	s16 OldKillsKiller = 0;
	s16 OldKillsVictim = 0;

	if (ps_killer)
	{
		//.		OldKillsKiller = ps_killer->kills;
		OldKillsKiller = ps_killer->frags();
	}

	if (ps_killed)
	{
		//.		OldKillsVictim = ps_killed->kills;
		OldKillsVictim = ps_killed->frags();
	}

	inherited::OnPlayerKillPlayer(ps_killer, ps_killed, KillType, SpecialKillType, pWeaponA);

	UpdateTeamScore(ps_killer, OldKillsKiller);

	if (ps_killer != ps_killed)
		UpdateTeamScore(ps_killed, OldKillsVictim);


	//-------------------------------------------------------------------	
	if (ps_killed && ps_killer)
	{
		if (ps_killed != ps_killer && ps_killer->team == ps_killed->team)
		{
			//.			ps_killer->m_iTeamKills++;

			//Check for TeamKill
			if (Get_TeamKillPunishment())
			{
				if (ps_killer->m_iTeamKills >= Get_TeamKillLimit())
				{
					struct player_state_searcher
					{
						game_PlayerState* ps_killer;
						IClient* server_client;

						bool operator()(IClient* client)
						{
							xrClientData* pCL = (xrClientData*)client;
							if (!pCL || pCL == server_client) return false;
							if (!pCL->ps || pCL->ps != ps_killer) return false;
							return true;
						}
					};
					player_state_searcher tmp_predicate;
					tmp_predicate.ps_killer = ps_killer;
					tmp_predicate.server_client = m_server->GetServerClient();
					xrClientData* tmp_client = static_cast<xrClientData*>(
						m_server->FindClient(tmp_predicate)
						);
					if (tmp_client)
					{
#ifdef DEBUG
						Msg("--- Kicking player %s", tmp_client->ps->getName());
#endif
						LPSTR	reason;
						STRCONCAT(reason, CStringTable().translate("st_kicked_by_server").c_str());
						m_server->DisconnectClient(tmp_client, reason);
					}
				}
			}
		}
	}
}

void game_sv_RolePlay::UpdateTeamScore(game_PlayerState* ps_killer, s16 OldKills)
{
	if (!ps_killer) return;
	SetTeamScore(ps_killer->team - 1, GetTeamScore(ps_killer->team - 1) + ps_killer->frags() - OldKills);
}

KILL_RES game_sv_RolePlay::GetKillResult(game_PlayerState* pKiller, game_PlayerState* pVictim)
{
	KILL_RES Res = inherited::GetKillResult(pKiller, pVictim);
	switch (Res)
	{
	case KR_RIVAL:
	{
		if (pKiller->team == pVictim->team)
			Res = KR_TEAMMATE;
	}break;
	default:
	{
	}break;
	};
	return Res;
};

bool game_sv_RolePlay::OnKillResult(KILL_RES KillResult, game_PlayerState* pKiller, game_PlayerState* pVictim)
{
	bool res = true;
	TeamStruct* pTeam = GetTeamData(u8(pKiller->team));
	switch (KillResult)
	{
	case KR_TEAMMATE:
	{
		//.			pKiller->kills -= 1;
		pKiller->m_iTeamKills++;
		if (pTeam)
			Player_AddMoney(pKiller, pTeam->m_iM_KillTeam);
		res = false;
	}break;
	default:
	{
		res = inherited::OnKillResult(KillResult, pKiller, pVictim);
	}break;
	}
	return res;
};


bool game_sv_RolePlay::checkForFragLimit()
{
	//------m4d_RP
	if (g_sv_dm_dwFragLimit && ((teams[0].score >= g_sv_dm_dwFragLimit)
		|| (teams[1].score >= g_sv_dm_dwFragLimit)
		|| (teams[2].score >= g_sv_dm_dwFragLimit)
		|| (teams[3].score >= g_sv_dm_dwFragLimit)
		|| (teams[4].score >= g_sv_dm_dwFragLimit)
		|| (teams[5].score >= g_sv_dm_dwFragLimit)
		|| (teams[6].score >= g_sv_dm_dwFragLimit)
		|| (teams[7].score >= g_sv_dm_dwFragLimit)
		|| (teams[8].score >= g_sv_dm_dwFragLimit)))
	{
		OnFraglimitExceed();
		return true;
	};
	return false;
}

u32 game_sv_RolePlay::RP_2_Use(CSE_Abstract* E)
{
	CSE_ALifeCreatureActor	*pA = smart_cast<CSE_ALifeCreatureActor*>(E);
	if (!pA) return 0;


	u32 Team = u32(pA->g_team());
	return (rpoints[Team].size()) ? Team : 0;
};

void game_sv_RolePlay::OnPlayerHitPlayer_Case(game_PlayerState* ps_hitter, game_PlayerState* ps_hitted, SHit* pHitS)
{
	//if (pHitS->hit_type != ALife::eHitTypePhysicStrike)
	//{
	if (ps_hitter && ps_hitted)
	{
		if (ps_hitter->team == ps_hitted->team && ps_hitter != ps_hitted)
		{
			pHitS->power *= GetFriendlyFire();
			pHitS->impulse *= (GetFriendlyFire()>1.0f) ? GetFriendlyFire() : 1.0f;
		}
	}
	//	}	
	inherited::OnPlayerHitPlayer_Case(ps_hitter, ps_hitted, pHitS);
};

void game_sv_RolePlay::OnPlayerHitPlayer(u16 id_hitter, u16 id_hitted, NET_Packet& P)
{
	inherited::OnPlayerHitPlayer(id_hitter, id_hitted, P);
};

void game_sv_RolePlay::LoadTeams()
{
	m_sBaseWeaponCostSection._set("roleplay_base_cost");
	if (!pSettings->section_exist(m_sBaseWeaponCostSection))
	{
		R_ASSERT2(0, "No section for base weapon cost for this type of the Game!");
		return;
	};
	m_strWeaponsData->Load(m_sBaseWeaponCostSection);

	LoadTeamData("roleplay_team0");
	LoadTeamData("roleplay_team1");
	LoadTeamData("roleplay_team2");
	//-----------------------m4d_RP
	LoadTeamData("roleplay_team3");
	LoadTeamData("roleplay_team4");
	LoadTeamData("roleplay_team5");
	LoadTeamData("roleplay_team6");
	LoadTeamData("roleplay_team7");
	LoadTeamData("roleplay_team8");
	LoadTeamData("roleplay_team9");
};

void game_sv_RolePlay::Update()
{
	inherited::Update();
	switch (Phase())
	{
		//-------------m4d_RP
	case GAME_PHASE_TEAM1_SCORES:
	case GAME_PHASE_TEAM2_SCORES:
	case GAME_PHASE_TEAM3_SCORES:
	case GAME_PHASE_TEAM4_SCORES:
	case GAME_PHASE_TEAM5_SCORES:
	case GAME_PHASE_TEAM6_SCORES:
	case GAME_PHASE_TEAM7_SCORES:
	case GAME_PHASE_TEAM8_SCORES:
	case GAME_PHASE_TEAM9_SCORES:
	case GAME_PHASE_TEAMS_IN_A_DRAW:
	{
		if (m_delayedRoundEnd && m_roundEndDelay < Device.TimerAsync())
		{
			OnRoundEnd(); // eRoundEnd_Finish 
		}
	} break;
	};
}
extern INT g_sv_Skip_Winner_Waiting;
bool game_sv_RolePlay::HasChampion()
{
	return (GetTeamScore(0) != GetTeamScore(1) || g_sv_Skip_Winner_Waiting);
}

void game_sv_RolePlay::OnTimelimitExceed()
{
	u8 winning_team = (GetTeamScore(0) < GetTeamScore(1)) ? 1 : 0;
	OnTeamScore(winning_team, false);
	m_phase = u16((winning_team) ? GAME_PHASE_TEAM2_SCORES : GAME_PHASE_TEAM1_SCORES);
	switch_Phase(m_phase);

	OnDelayedRoundEnd(eRoundEnd_TimeLimit); //"TIME_limit"
}
void game_sv_RolePlay::OnFraglimitExceed()
{
	u8 winning_team = (GetTeamScore(0) < GetTeamScore(1)) ? 1 : 0;
	OnTeamScore(winning_team, false);
	m_phase = u16((winning_team) ? GAME_PHASE_TEAM2_SCORES : GAME_PHASE_TEAM1_SCORES);
	switch_Phase(m_phase);

	OnDelayedRoundEnd(eRoundEnd_FragLimit); //"FRAG_limit"
}
//-----------------------------------------------
void game_sv_RolePlay::ReadOptions(shared_str &options)
{
	inherited::ReadOptions(options);
	//-------------------------------
	g_sv_tdm_bAutoTeamBalance = get_option_i(*options, "abalance", (g_sv_tdm_bAutoTeamBalance ? 1 : 0)) != 0;
	g_sv_tdm_bAutoTeamSwap = get_option_i(*options, "aswap", (g_sv_tdm_bAutoTeamSwap ? 1 : 0)) != 0;
	g_sv_tdm_bFriendlyIndicators = get_option_i(*options, "fi", (g_sv_tdm_bFriendlyIndicators ? 1 : 0)) != 0;
	g_sv_tdm_bFriendlyNames = get_option_i(*options, "fn", (g_sv_tdm_bFriendlyNames ? 1 : 0)) != 0;

	float fFF = get_option_f(*options, "ffire", g_sv_tdm_fFriendlyFireModifier);
	g_sv_tdm_fFriendlyFireModifier = fFF;
}

static bool g_bConsoleCommandsCreated_TDM = false;
void game_sv_RolePlay::ConsoleCommands_Create()
{
};

void game_sv_RolePlay::ConsoleCommands_Clear()
{
	inherited::ConsoleCommands_Clear();
};

void game_sv_RolePlay::AutoSwapTeams()
{
	if (!Get_AutoTeamSwap()) return;

	struct auto_team_swaper
	{
		void operator()(IClient* client)
		{
			xrClientData *l_pC = static_cast<xrClientData*>(client);
			if (!l_pC || !l_pC->net_Ready || !l_pC->ps) return;
			game_PlayerState* ps = l_pC->ps;
			if (!ps || ps->IsSkip()) return;

			if (ps->team != 0)
				ps->team = (ps->team == 1) ? 2 : 1;
		}
	};
	auto_team_swaper tmp_functor;
	m_server->ForEachClientDo(tmp_functor);
	teams_swaped = true;
}

void game_sv_RolePlay::WriteGameState(CInifile& ini, LPCSTR sect, bool bRoundResult)
{
	inherited::WriteGameState(ini, sect, bRoundResult);

	for (u32 i = 0; i<teams.size(); ++i)
	{
		string16		buf_name;
		xr_sprintf(buf_name, "team_%d_score", i);
		ini.w_u32(sect, buf_name, GetTeamScore(i));
	}
}

//--------------------------------------m4d_? еще чота с передачей итемов на серве 
BOOL game_sv_RolePlay::OnTouchItem(CSE_ActorMP *actor, CSE_Abstract *item)
{
	VERIFY(actor);
	VERIFY(item);

	if ((item->m_tClassID == CLSID_OBJECT_PLAYERS_BAG) && (item->ID_Parent == 0xffff))
	{
		//-------------------------------
		//move all items from rukzak to player
		if (!item->children.empty())
		{
			NET_Packet	EventPack;
			NET_Packet	PacketReject;
			NET_Packet	PacketTake;

			EventPack.w_begin(M_EVENT_PACK);

			while (!item->children.empty())
			{
				CSE_Abstract *e_child_item = get_entity_from_eid(item->children.back());
				if (e_child_item)
				{
					if (!OnTouch(actor->ID, e_child_item->ID, FALSE))
					{
						NET_Packet P;
						u_EventGen(P, GE_OWNERSHIP_REJECT, item->ID);
						P.w_u16(e_child_item->ID);

						m_server->Process_event_reject(P, m_server->GetServerClient()->ID, 0, item->ID, e_child_item->ID);
						continue;
					}
				}

				m_server->Perform_transfer(PacketReject, PacketTake, e_child_item, item, actor);

				EventPack.w_u8(u8(PacketReject.B.count));
				EventPack.w(&PacketReject.B.data, PacketReject.B.count);
				EventPack.w_u8(u8(PacketTake.B.count));
				EventPack.w(&PacketTake.B.data, PacketTake.B.count);
			}
			if (EventPack.B.count > 2)
				u_EventSend(EventPack);
		}
		//-------------------------------
		//destroy the BAG
		DestroyGameItem(item);
		if (g_sv_dm_bPDAHunt && actor->owner && actor->owner->ps)
		{
			Player_AddBonusMoney(actor->owner->ps, READ_IF_EXISTS(pSettings, r_s32, "mp_bonus_money", "pda_taken", 0), SKT_PDA);
		};

		//-------------------------------
		return FALSE;

	};
	//---------------------------------------------------------------
	return TRUE;
}

void game_sv_RolePlay::OnDetachItem(CSE_ActorMP *actor, CSE_Abstract *item)
{
	R_ASSERT(actor);
	R_ASSERT(item);
	if (item->m_tClassID == CLSID_OBJECT_PLAYERS_BAG)
	{
		//move all items from player to rukzak
		xr_vector<u16>::const_iterator it_e = actor->children.end();

		xr_vector<CSE_Abstract*>			to_transfer;
		xr_vector<CSE_Abstract*>			to_destroy;
		xr_vector<CSE_Abstract*>			to_reject;
		// may be there is a sense to move next invokation into the ProcessDeath method...
		FillDeathActorRejectItems(actor, to_reject);

		for (xr_vector<u16>::const_iterator it = actor->children.begin();
			it != it_e; ++it)
		{
			u16 ItemID = *it;
			CSE_Abstract* e_item = get_entity_from_eid(ItemID);

			R_ASSERT(e_item->ID_Parent == actor->ID);

			if (std::find(to_reject.begin(), to_reject.end(), e_item) != to_reject.end())
				continue;

			if ((e_item->m_tClassID == CLSID_OBJECT_W_KNIFE) ||
				(e_item->m_tClassID == CLSID_DEVICE_TORCH))
			{
				to_destroy.push_back(e_item);
			}
			else if (m_strWeaponsData->GetItemIdx(e_item->s_name) != u32(-1))
			{
				if (!smart_cast<CSE_ALifeItemCustomOutfit*>(e_item))
				{
					to_transfer.push_back(e_item);
				}
			}
		}

		xr_vector<CSE_Abstract*>::const_iterator tr_it_e = to_transfer.end();

		NET_Packet							EventPack;
		NET_Packet							PacketReject;
		NET_Packet							PacketTake;
		EventPack.w_begin(M_EVENT_PACK);

		for (xr_vector<CSE_Abstract*>::const_iterator tr_it = to_transfer.begin();
			tr_it != tr_it_e; ++tr_it)
		{
			m_server->Perform_transfer(PacketReject, PacketTake, *tr_it, actor, item);
			EventPack.w_u8(u8(PacketReject.B.count));
			EventPack.w(&PacketReject.B.data, PacketReject.B.count);
			EventPack.w_u8(u8(PacketTake.B.count));
			EventPack.w(&PacketTake.B.data, PacketTake.B.count);
		}

		if (EventPack.B.count > 2)
			u_EventSend(EventPack);

		std::for_each(to_destroy.begin(), to_destroy.end(),
			std::bind1st(std::mem_fun<void, game_sv_mp, CSE_Abstract*>(
				&game_sv_mp::DestroyGameItem), this));

		std::for_each(to_reject.begin(), to_reject.end(),
			std::bind1st(std::mem_fun<void, game_sv_mp, CSE_Abstract*>(
				&game_sv_mp::RejectGameItem), this));

	};
}


BOOL game_sv_RolePlay::OnTouch(u16 eid_who, u16 eid_what, BOOL bForced)
{
	CSE_ActorMP *e_who = smart_cast<CSE_ActorMP*>(m_server->ID_to_entity(eid_who));
	if (!e_who)
		return FALSE;

	CSE_Abstract *e_entity = m_server->ID_to_entity(eid_what);
	if (!e_entity)
		return FALSE;

	return OnTouchItem(e_who, e_entity);
}

void game_sv_RolePlay::OnDetach(u16 eid_who, u16 eid_what)
{
	CSE_ActorMP *e_who = smart_cast<CSE_ActorMP*>(m_server->ID_to_entity(eid_who));
	if (!e_who)
		return;

	CSE_Abstract *e_entity = m_server->ID_to_entity(eid_what);
	if (!e_entity)
		return;

	OnDetachItem(e_who, e_entity);
}

void game_sv_RolePlay::OnObjectEnterTeamBase(u16 id, u16 zone_team)
{
	CSE_Abstract*			e_who = m_server->ID_to_entity(id);
	VERIFY(e_who);
	CSE_ALifeCreatureActor* eActor = smart_cast<CSE_ALifeCreatureActor*>(e_who);
	if (eActor)
	{
		game_cl_mp*	tmp_cl_game = smart_cast<game_cl_mp*>(&Game());
		s16 mteam = tmp_cl_game->ModifyTeam(s16(zone_team));
		game_PlayerState* ps = eActor->owner->ps;
		if (ps && (ps->team == mteam))
		{
			ps->setFlag(GAME_PLAYER_FLAG_ONBASE);
			signal_Syncronize();
		}
	}
}

void game_sv_RolePlay::OnObjectLeaveTeamBase(u16 id, u16 zone_team)
{
	CSE_Abstract*			e_who = m_server->ID_to_entity(id);
	if (!e_who)
		return;

	CSE_ALifeCreatureActor* eActor = smart_cast<CSE_ALifeCreatureActor*>(e_who);
	if (eActor)
	{
		game_cl_mp*	tmp_cl_game = smart_cast<game_cl_mp*>(&Game());
		s16 mteam = tmp_cl_game->ModifyTeam(s16(zone_team));
		game_PlayerState* ps = eActor->owner->ps;
		if (ps && (ps->team == mteam))
		{
			ps->resetFlag(GAME_PLAYER_FLAG_ONBASE);
			signal_Syncronize();
		}
	}
}

void game_sv_RolePlay::RespawnPlayer(ClientID id_who, bool NoSpectator)
{
	inherited::RespawnPlayer(id_who, NoSpectator);
	xrClientData*	xrCData = (xrClientData*)m_server->ID_to_client(id_who);
	VERIFY(xrCData);
	game_PlayerState* ps = xrCData->ps;
	VERIFY(ps);
	ps->resetFlag(GAME_PLAYER_FLAG_ONBASE);
}
