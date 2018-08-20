#include "StdAfx.h"
#include "TeamInfo.h"
#include "../string_table.h"

u32			CTeamInfo::team1_color;
u32			CTeamInfo::team2_color;
//------------------------m4d_RP
u32			CTeamInfo::team3_color;
u32			CTeamInfo::team4_color;
u32			CTeamInfo::team5_color;
u32			CTeamInfo::team6_color;
u32			CTeamInfo::team7_color;
u32			CTeamInfo::team8_color;
u32			CTeamInfo::team9_color;

shared_str	CTeamInfo::team1_name;
shared_str	CTeamInfo::team2_name;
shared_str	CTeamInfo::team3_name;
shared_str	CTeamInfo::team4_name;
shared_str	CTeamInfo::team5_name;
shared_str	CTeamInfo::team6_name;
shared_str	CTeamInfo::team7_name;
shared_str	CTeamInfo::team8_name;
shared_str	CTeamInfo::team9_name;

shared_str	CTeamInfo::team1_color_tag;
shared_str	CTeamInfo::team2_color_tag;
shared_str	CTeamInfo::team3_color_tag;
shared_str	CTeamInfo::team4_color_tag;
shared_str	CTeamInfo::team5_color_tag;
shared_str	CTeamInfo::team6_color_tag;
shared_str	CTeamInfo::team7_color_tag;
shared_str	CTeamInfo::team8_color_tag;
shared_str	CTeamInfo::team9_color_tag;

Flags32		CTeamInfo::flags;

u32 CTeamInfo::GetTeam1_color(){
	if (flags.test(flTeam1_color))
		return team1_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team1","color");
	team1_color = color_argb(155,atoi(_GetItem(tm_col, 0, _buff)),atoi(_GetItem(tm_col, 1, _buff)),atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam1_color, true);

	return team1_color;
}

u32 CTeamInfo::GetTeam2_color(){
	if (flags.test(flTeam2_color))
		return team2_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team2","color");
	team2_color = color_argb(155,atoi(_GetItem(tm_col, 0, _buff)),atoi(_GetItem(tm_col, 1, _buff)),atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam2_color, true);

	return team2_color;
}

u32 CTeamInfo::GetTeam3_color()
{
	if (flags.test(flTeam3_color))
		return team3_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team3", "color");
	team3_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam3_color, true);

	return team3_color;
}

u32 CTeamInfo::GetTeam4_color()
{
	if (flags.test(flTeam4_color))
		return team4_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team4", "color");
	team4_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam4_color, true);

	return team4_color;
}

u32 CTeamInfo::GetTeam5_color()
{
	if (flags.test(flTeam5_color))
		return team5_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team5", "color");
	team5_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam5_color, true);

	return team5_color;
}

u32 CTeamInfo::GetTeam6_color()
{
	if (flags.test(flTeam6_color))
		return team6_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team6", "color");
	team6_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam6_color, true);

	return team6_color;
}

u32 CTeamInfo::GetTeam7_color()
{
	if (flags.test(flTeam7_color))
		return team7_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team7", "color");
	team7_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam7_color, true);

	return team7_color;
}

u32 CTeamInfo::GetTeam8_color()
{
	if (flags.test(flTeam8_color))
		return team8_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team8", "color");
	team8_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam8_color, true);

	return team8_color;
}

u32 CTeamInfo::GetTeam9_color()
{
	if (flags.test(flTeam9_color))
		return team9_color;

	string256 _buff;

	LPCSTR tm_col = pSettings->r_string("team9", "color");
	team9_color = color_argb(
		155, atoi(_GetItem(tm_col, 0, _buff)), atoi(_GetItem(tm_col, 1, _buff)), atoi(_GetItem(tm_col, 2, _buff)));
	flags.set(flTeam9_color, true);

	return team9_color;
}

shared_str	CTeamInfo::GetTeam1_name(){
	if (flags.test(flTeam1_name))
		return team1_name;

	CStringTable st;

	team1_name = st.translate(pSettings->r_string_wb("team1","name"));
	flags.set(flTeam1_name,true);

    return team1_name;
}

shared_str	CTeamInfo::GetTeam2_name(){
	if (flags.test(flTeam2_name))
		return team2_name;

	CStringTable st;

	team2_name = st.translate(pSettings->r_string_wb("team2","name"));
	flags.set(flTeam2_name,true);

    return team2_name;
}

shared_str CTeamInfo::GetTeam3_name()
{
	if (flags.test(flTeam3_name))
		return team3_name;

	CStringTable st;

	team3_name = st.translate(pSettings->r_string_wb("team3", "name"));
	flags.set(flTeam3_name, true);

	return team3_name;
}

shared_str CTeamInfo::GetTeam4_name()
{
	if (flags.test(flTeam4_name))
		return team4_name;

	CStringTable st;

	team4_name = st.translate(pSettings->r_string_wb("team4", "name"));
	flags.set(flTeam4_name, true);

	return team4_name;
}

shared_str CTeamInfo::GetTeam5_name()
{
	if (flags.test(flTeam5_name))
		return team5_name;

	CStringTable st;

	team5_name = st.translate(pSettings->r_string_wb("team5", "name"));
	flags.set(flTeam5_name, true);

	return team5_name;
}

shared_str CTeamInfo::GetTeam6_name()
{
	if (flags.test(flTeam6_name))
		return team6_name;

	CStringTable st;

	team6_name = st.translate(pSettings->r_string_wb("team6", "name"));
	flags.set(flTeam6_name, true);

	return team6_name;
}

shared_str CTeamInfo::GetTeam7_name()
{
	if (flags.test(flTeam7_name))
		return team7_name;

	CStringTable st;

	team7_name = st.translate(pSettings->r_string_wb("team7", "name"));
	flags.set(flTeam7_name, true);

	return team7_name;
}

shared_str CTeamInfo::GetTeam8_name()
{
	if (flags.test(flTeam8_name))
		return team8_name;

	CStringTable st;

	team8_name = st.translate(pSettings->r_string_wb("team8", "name"));
	flags.set(flTeam8_name, true);

	return team8_name;
}

shared_str CTeamInfo::GetTeam9_name()
{
	if (flags.test(flTeam9_name))
		return team9_name;

	CStringTable st;

	team9_name = st.translate(pSettings->r_string_wb("team9", "name"));
	flags.set(flTeam9_name, true);

	return team9_name;
}

LPCSTR	CTeamInfo::GetTeam_name(int team)
{
	string32 tmp;
	//---m4d_RP
	R_ASSERT2(team == 1 || team == 2 || team == 3 || team == 4 || team == 5 || team == 6 || team == 7 || team == 8 || team == 9 || team == 10, itoa(team, tmp, 10) );
	switch (team)
	{
		case 1:
		{
			return *GetTeam1_name();
		}
		break;
		case 2:
		{
			return *GetTeam2_name();
		}
		break;
		case 3:
		{
			return *GetTeam3_name();
		}
		break;
		case 4:
		{
			return *GetTeam4_name();
		}
		break;
		case 5:
		{
			return *GetTeam5_name();
		}
		break;
		case 6:
		{
			return *GetTeam6_name();
		}
		break;
		case 7:
		{
			return *GetTeam7_name();
		}
		break;
		case 8:
		{
			return *GetTeam8_name();
		}
		break;
		case 9:
		{
			return *GetTeam9_name();
		}
		break;
		default:
		{
			return *GetTeam2_name();
		}
		break;
	}
}

LPCSTR	CTeamInfo::GetTeam_color_tag(int team)
{
	string32 tmp;
	//---m4d_RP
	R_ASSERT2(team == 1 || team == 2 || team == 3 || team == 4 || team == 5 || team == 6 || team == 7 || team == 8 || team == 9 || team == 10, itoa(team, tmp, 10) );
//	if (flags.test(flTeam1_col_t)) return *team1_color_tag;
//	if (flags.test(flTeam2_col_t)) return *team2_color_tag;
	if (team == 10)
	{
		team = 9;
	}

	string256 _buff;

	//"%c[255,64,255,64]", "%c[255,64,64,255]"

	LPCSTR tm_col;
	switch (team)
	{
		case 1:
		{
			tm_col = pSettings->r_string("team1", "color");
		}
		break;
		case 2:
		{
			tm_col = pSettings->r_string("team2", "color");
		}
		break;
		case 3:
		{
			tm_col = pSettings->r_string("team3", "color");
		}
		break;
		case 4:
		{
			tm_col = pSettings->r_string("team4", "color");
		}
		break;
		case 5:
		{
			tm_col = pSettings->r_string("team5", "color");
		}
		break;
		case 6:
		{
			tm_col = pSettings->r_string("team6", "color");
		}
		break;
		case 7:
		{
			tm_col = pSettings->r_string("team7", "color");
		}
		break;
		case 8:
		{
			tm_col = pSettings->r_string("team8", "color");
		}
		break;
		case 9:
		{
			tm_col = pSettings->r_string("team9", "color");
		}
		break;
		default:
		{
			tm_col = pSettings->r_string("team2", "color");
		}
		break;
	}

	//team1_color = color_argb(155,atoi(_GetItem(tm_col, 0, _buff)),atoi(_GetItem(tm_col, 1, _buff)),atoi(_GetItem(tm_col, 2, _buff)));
	xr_string str;
	str = "%c[255,";
	str += _GetItem(tm_col, 0, _buff);
	str += ",";
	str += _GetItem(tm_col, 1, _buff);
	str += ",";
	str += _GetItem(tm_col, 2, _buff);
	str += "]";

	switch (team)
	{
		case 1:
		{
			flags.set(flTeam1_col_t, true);
			team1_color_tag = str.c_str();
			return *team1_color_tag;
		}
		break;
		case 2:
		{
			flags.set(flTeam2_col_t, true);
			team2_color_tag = str.c_str();
			return *team2_color_tag;
		}
		break;
		case 3:
		{
			flags.set(flTeam3_col_t, true);
			team3_color_tag = str.c_str();
			return *team3_color_tag;
		}
		break;
		case 4:
		{
			flags.set(flTeam4_col_t, true);
			team4_color_tag = str.c_str();
			return *team4_color_tag;
		}
		break;
		case 5:
		{
			flags.set(flTeam5_col_t, true);
			team5_color_tag = str.c_str();
			return *team5_color_tag;
		}
		break;
		case 6:
		{
			flags.set(flTeam6_col_t, true);
			team6_color_tag = str.c_str();
			return *team6_color_tag;
		}
		break;
		case 7:
		{
			flags.set(flTeam7_col_t, true);
			team7_color_tag = str.c_str();
			return *team7_color_tag;
		}
		break;
		case 8:
		{
			flags.set(flTeam8_col_t, true);
			team8_color_tag = str.c_str();
			return *team8_color_tag;
		}
		break;
		case 9:
		{
			flags.set(flTeam9_col_t, true);
			team9_color_tag = str.c_str();
			return *team9_color_tag;
		}
		break;
		default:
		{
			flags.set(flTeam2_col_t, true);
			team2_color_tag = str.c_str();
			return *team2_color_tag;
		}
		break;
	}
}