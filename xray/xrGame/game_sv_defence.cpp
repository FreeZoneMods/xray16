#include "stdafx.h"
#include "game_sv_defence.h"

BOOL		g_sv_df_bFriendlyIndicators = FALSE;
BOOL		g_sv_df_bFriendlyNames = FALSE;
float		g_sv_df_fFriendlyFireModifier = 1.0f;

BOOL	game_sv_Defence::isFriendlyFireEnabled() { return (int(g_sv_df_fFriendlyFireModifier*100.0f) > 0); };
float	game_sv_Defence::GetFriendlyFire() { return (int(g_sv_df_fFriendlyFireModifier*100.0f) > 0) ? g_sv_df_fFriendlyFireModifier : 0.0f; };
BOOL	game_sv_Defence::Get_FriendlyIndicators() { return g_sv_df_bFriendlyIndicators; };
BOOL	game_sv_Defence::Get_FriendlyNames() { return g_sv_df_bFriendlyNames; };

void game_sv_Defence::ReadOptions(shared_str &options)
{
	inherited::ReadOptions(options);

	g_sv_df_bFriendlyIndicators = get_option_i(*options, "fi", (g_sv_df_bFriendlyIndicators ? 1 : 0)) != 0;
	g_sv_df_bFriendlyNames = get_option_i(*options, "fn", (g_sv_df_bFriendlyNames ? 1 : 0)) != 0;

	float fFF = get_option_f(*options, "ffire", g_sv_df_fFriendlyFireModifier);
	g_sv_df_fFriendlyFireModifier = fFF;
}