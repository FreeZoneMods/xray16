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
protected:
	virtual		void				ReadOptions(shared_str &options);
};