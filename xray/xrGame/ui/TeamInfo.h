#pragma once

class CTeamInfo {
public:
	//---m4d_RP
	static u32 GetTeam1_color();
	static u32 GetTeam2_color();
	static u32 GetTeam3_color();
	static u32 GetTeam4_color();
	static u32 GetTeam5_color();
	static u32 GetTeam6_color();
	static u32 GetTeam7_color();
	static u32 GetTeam8_color();
	static u32 GetTeam9_color();

	static shared_str GetTeam1_name();
	static shared_str GetTeam2_name();
	static shared_str GetTeam3_name();
	static shared_str GetTeam4_name();
	static shared_str GetTeam5_name();
	static shared_str GetTeam6_name();
	static shared_str GetTeam7_name();
	static shared_str GetTeam8_name();
	static shared_str GetTeam9_name();

	static LPCSTR		GetTeam_name(int team);
	static LPCSTR		GetTeam_color_tag(int team);

protected:
	static u32 team1_color;
	static u32 team2_color;
	static u32 team3_color;
	static u32 team4_color;
	static u32 team5_color;
	static u32 team6_color;
	static u32 team7_color;
	static u32 team8_color;
	static u32 team9_color;

	static shared_str team1_name;
	static shared_str team2_name;
	static shared_str team3_name;
	static shared_str team4_name;
	static shared_str team5_name;
	static shared_str team6_name;
	static shared_str team7_name;
	static shared_str team8_name;
	static shared_str team9_name;

	static shared_str team1_color_tag;
	static shared_str team2_color_tag;
	static shared_str team3_color_tag;
	static shared_str team4_color_tag;
	static shared_str team5_color_tag;
	static shared_str team6_color_tag;
	static shared_str team7_color_tag;
	static shared_str team8_color_tag;
	static shared_str team9_color_tag;

	enum {
		flTeam1_color = 1,
		flTeam2_color = 1 << 1,
		flTeam3_color = 1 << 2,
		flTeam4_color = 1 << 3,
		flTeam5_color = 1 << 4,
		flTeam6_color = 1 << 5,
		flTeam7_color = 1 << 6,
		flTeam8_color = 1 << 7,
		flTeam9_color = 1 << 8,

		flTeam1_name = 1 << 9,
		flTeam2_name = 1 << 10,
		flTeam3_name = 1 << 11,
		flTeam4_name = 1 << 12,
		flTeam5_name = 1 << 13,
		flTeam6_name = 1 << 14,
		flTeam7_name = 1 << 15,
		flTeam8_name = 1 << 16,
		flTeam9_name = 1 << 17,

		flTeam1_col_t = 1 << 18,
		flTeam2_col_t = 1 << 19,
		flTeam3_col_t = 1 << 20,
		flTeam4_col_t = 1 << 21,
		flTeam5_col_t = 1 << 22,
		flTeam6_col_t = 1 << 23,
		flTeam7_col_t = 1 << 24,
		flTeam8_col_t = 1 << 25,
		flTeam9_col_t = 1 << 26
	};
	static Flags32		flags;
};