#pragma once

#include "game_sv_deathmatch.h"

class game_sv_Defence: public game_sv_Deathmatch
{
	typedef game_sv_Deathmatch inherited;
public:
	game_sv_Defence() { m_type = eGameIDDefence; }
	virtual		LPCSTR				type_name() const { return "defence"; };

	virtual		BOOL				isFriendlyFireEnabled();
	virtual		float				GetFriendlyFire();

	virtual		BOOL				Get_FriendlyIndicators();
	virtual		BOOL				Get_FriendlyNames();

	virtual		KILL_RES			GetKillResult(game_PlayerState* pKiller, game_PlayerState* pVictim);
	virtual		bool				OnKillResult(KILL_RES KillResult, game_PlayerState* pKiller, game_PlayerState* pVictim);
	virtual		void				OnPlayerKillPlayer(game_PlayerState* ps_killer, game_PlayerState* ps_killed, KILL_TYPE KillType, SPECIAL_KILL_TYPE SpecialKillType, CSE_Abstract* pWeaponA);
	virtual		int					Get_TeamKillLimit();
	virtual		BOOL				Get_TeamKillPunishment();


	//virtual		void				OnPlayerHitPlayer(u16 id_hitter, u16 id_hitted, NET_Packet& P);
	//virtual		void				OnPlayerHitPlayer_Case(game_PlayerState* ps_hitter, game_PlayerState* ps_hitted, SHit* pHitS);
protected:
	virtual		void				ReadOptions(shared_str &options);
};