#include "StdAfx.h"
#include "UIPanelsClassFactory.h"
#include "game_base.h"
#include "UITeamPanels.h"

UIPanelsClassFactory::UIPanelsClassFactory()
{
}

UIPanelsClassFactory::~UIPanelsClassFactory()
{
}
	
UITeamState* UIPanelsClassFactory::CreateTeamPanel(shared_str const & teamName, UITeamPanels *teamPanels)
{
	//под кейсы потом -----------m4d_RP
	if (teamName == "greenteam")
	{
		return xr_new<UITeamState>(etGreenTeam, teamPanels);
	}
	else if (teamName == "blueteam")
	{
		return xr_new<UITeamState>(etBlueTeam, teamPanels);
	}
	else if (teamName == "spectatorsteam")
	{
		return xr_new<UITeamState>(etSpectatorsTeam, teamPanels);
	}
	else if (teamName == "yellowteam")
	{
		return xr_new<UITeamState>(etYellowTeam, teamPanels);
	}
	else if (teamName == "blackteam")
	{
		return xr_new<UITeamState>(etBlackTeam, teamPanels);
	}
	else if (teamName == "lightblueteam")
	{
		return xr_new<UITeamState>(etLightBlueTeam, teamPanels);
	}
	else if (teamName == "redteam")
	{
		return xr_new<UITeamState>(etRedTeam, teamPanels);
	}
	else if (teamName == "brownteam")
	{
		return xr_new<UITeamState>(etBrownTeam, teamPanels);
	}
	else if (teamName == "lunarteam")
	{
		return xr_new<UITeamState>(etLunarTeam, teamPanels);
	}
	else if (teamName == "darkgreenteam")
	{
		return xr_new<UITeamState>(etDarkGreenTeam, teamPanels);
	}
	else if (teamName == "greenteam_pending")
	{
		return xr_new<UITeamState>(etGreenTeam, teamPanels);
	}
	else if (teamName == "blueteam_pending")
	{
		return xr_new<UITeamState>(etBlueTeam, teamPanels);
	}
	else if (teamName == "yellowteam_pending")
	{
		return xr_new<UITeamState>(etYellowTeam, teamPanels);
	}
	else if (teamName == "blackteam_pending")
	{
		return xr_new<UITeamState>(etBlackTeam, teamPanels);
	}
	else if (teamName == "lightblueteam_pending")
	{
		return xr_new<UITeamState>(etLightBlueTeam, teamPanels);
	}
	else if (teamName == "redteam_pending")
	{
		return xr_new<UITeamState>(etRedTeam, teamPanels);
	}
	else if (teamName == "brownteam_pending")
	{
		return xr_new<UITeamState>(etBrownTeam, teamPanels);
	}
	else if (teamName == "lunarteam_pending")
	{
		return xr_new<UITeamState>(etLunarTeam, teamPanels);
	}
	else if (teamName == "darkgreenteam_pending")
	{
		return xr_new<UITeamState>(etDarkGreenTeam, teamPanels);
	}
	return NULL;
}