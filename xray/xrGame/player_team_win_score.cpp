#include "stdafx.h"
#include "player_team_win_score.h"
#include "game_cl_capture_the_artefact.h"
#include "game_cl_artefacthunt.h"
#include "game_state_accumulator.h"

//---m4d_RP
#include "game_cl_roleplay.h"
//---m4d_Defence
#include "game_cl_defence.h"

namespace award_system
{

player_team_win_score::player_team_win_score(game_state_accumulator* owner) :
	inherited(owner)
{
	m_win_score		= 0;
};

void player_team_win_score::reset_game()
{
	m_win_score		= 0;
}

void player_team_win_score::OnRoundStart()
{
	reset_game();
}

void player_team_win_score::OnRoundEnd()
{
	save_round_scores();
}

void player_team_win_score::save_round_scores()
{
	m_green_team_score	= 0;
	m_blue_team_score	= 0;
	//----------------m4d_RP
	m_yellow_team_score = 0;
	m_black_team_score = 0;
	m_lightblue_team_score = 0;
	m_red_team_score = 0;
	m_brown_team_score = 0;
	m_lunar_team_score = 0;
	m_darkgreen_team_score = 0;

	m_player_team		= etSpectatorsTeam;

	game_PlayerState* tmp_local_player = m_owner->get_local_player();
	if (!tmp_local_player)
		return;

	switch (Game().Type())
	{
	case eGameIDCaptureTheArtefact:
		{
			game_cl_CaptureTheArtefact*	tmp_game	= smart_cast<game_cl_CaptureTheArtefact*>(Level().game);
			m_green_team_score						= tmp_game->GetGreenTeamScore();
			m_blue_team_score						= tmp_game->GetBlueTeamScore();
			m_player_team							= tmp_local_player->team;
		}break;
	case eGameIDArtefactHunt:
	case eGameIDTeamDeathmatch:
		{
			game_cl_TeamDeathmatch* tmp_game	= smart_cast<game_cl_TeamDeathmatch*>(Level().game);
			m_green_team_score					= tmp_game->GetGreenTeamScore();
			m_blue_team_score					= tmp_game->GetBlueTeamScore();
			if (tmp_local_player->team > 0)
			{
				m_player_team = static_cast<u8>(tmp_game->ModifyTeam(tmp_local_player->team));
			}
		}break;
		//----m4d_RP
	case eGameIDRolePlay:
		{
			game_cl_RolePlay* tmp_game = smart_cast<game_cl_RolePlay*>(Level().game);

			m_green_team_score = tmp_game->GetGreenTeamScore();
			m_blue_team_score = tmp_game->GetBlueTeamScore();
			m_yellow_team_score = tmp_game->GetYellowTeamScore();
			m_black_team_score = tmp_game->GetBlackTeamScore();
			m_lightblue_team_score = tmp_game->GetLightBlueTeamScore();
			m_red_team_score = tmp_game->GetRedTeamScore();
			m_brown_team_score = tmp_game->GetBrownTeamScore();
			m_lunar_team_score = tmp_game->GetLunarTeamScore();
			m_darkgreen_team_score = tmp_game->GetDarkGreenTeamScore();

			if (tmp_local_player->team > 0)
			{
				m_player_team = static_cast<u8>(tmp_game->ModifyTeam(tmp_local_player->team));
			}
		}break;
		//---m4d_Defence
	case eGameIDDefence:
		{
			game_cl_Defence* tmp_game = smart_cast<game_cl_Defence*>(Level().game);
			if (!xr_strcmp(tmp_local_player->getName(), tmp_game->WinnerName))
			{
				m_win_score = tmp_local_player->m_iRivalKills;
				return;
			}
		}break;
	case eGameIDDeathmatch:
		{
			game_cl_Deathmatch* tmp_game		= smart_cast<game_cl_Deathmatch*>(Level().game);
			if (!xr_strcmp(tmp_local_player->getName(), tmp_game->WinnerName))
			{
				m_win_score = tmp_local_player->m_iRivalKills;
				return;
			}
		}break;
	};//switch (Game().Type())
	if (static_cast<ETeam>(m_player_team) == etGreenTeam)
	{
		m_win_score = (m_green_team_score > m_blue_team_score) ? m_green_team_score : 0;
	} else if (static_cast<ETeam>(m_player_team) == etBlueTeam)
	{
		m_win_score = (m_blue_team_score > m_green_team_score) ? m_blue_team_score : 0;
	}
}

player_enemy_team_score::player_enemy_team_score(game_state_accumulator* owner) :
	inherited(owner)
{
	m_enemy_team_score = 0;
}

void player_enemy_team_score::reset_game()
{
	inherited::reset_game	();
	m_enemy_team_score		= 0;
}

void player_enemy_team_score::OnRoundEnd()
{
	save_round_scores		();
}

void player_enemy_team_score::save_round_scores()
{
	inherited::save_round_scores();

	//---m4d_RP
	switch (static_cast<ETeam>(m_player_team))
	{
		case etGreenTeam:
		{
			m_enemy_team_score = m_blue_team_score;
		}
		break;
		case etBlueTeam:
		{
			m_enemy_team_score = m_green_team_score;
		}
		break;
		case etYellowTeam:
		{
			m_enemy_team_score = m_black_team_score;
		}
		break;
		case etBlackTeam:
		{
			m_enemy_team_score = m_yellow_team_score;
		}
		break;
		case etLightBlueTeam:
		{
			m_enemy_team_score = m_blue_team_score;
		}
		break;
		case etRedTeam:
		{
			m_enemy_team_score = m_green_team_score;
		}
		break;
		case etBrownTeam:
		{
			m_enemy_team_score = m_red_team_score;
		}
		break;
		case etLunarTeam:
		{
			m_enemy_team_score = m_black_team_score;
		}
		break;
		case etDarkGreenTeam:
		{
			m_enemy_team_score = m_yellow_team_score;
		}
		break;
	}
}

player_runtime_win_score::player_runtime_win_score(game_state_accumulator* owner) :
	inherited(owner)
{
}

u32 const player_runtime_win_score::get_u32_param()
{
	u32 ret_score = 0;

	switch (static_cast<ETeam>(m_player_team))
	{
		case etGreenTeam:
		{
			ret_score = m_green_team_score;
		}
		break;

		case etBlueTeam:
		{
			ret_score = m_blue_team_score;
		}
		break;

		case etYellowTeam:
		{
			ret_score = m_yellow_team_score;
		}
		break;

		case etBlackTeam:
		{
			ret_score = m_black_team_score;
		}
		break;

		case etLightBlueTeam:
		{
			ret_score = m_lightblue_team_score;
		}
		break;

		case etRedTeam:
		{
			ret_score = m_red_team_score;
		}
		break;

		case etBrownTeam:
		{
			ret_score = m_brown_team_score;
		}
		break;

		case etLunarTeam:
		{
			ret_score = m_lunar_team_score;
		}
		break;

		case etDarkGreenTeam:
		{
			ret_score = m_darkgreen_team_score;
		}
		break;
	}

	return ret_score;
}

void player_runtime_win_score::OnPlayerBringArtefact(game_PlayerState const * ps)
{
	save_round_scores();
}

player_runtime_enemy_team_score::player_runtime_enemy_team_score(game_state_accumulator* owner) :
	inherited(owner)
{
}

void player_runtime_enemy_team_score::OnPlayerBringArtefact(game_PlayerState const * ps)
{
	save_round_scores();
}

} //namespace award_system