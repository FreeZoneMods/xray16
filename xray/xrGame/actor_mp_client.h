#ifndef ACTOR_MP_CLIENT_H
#define ACTOR_MP_CLIENT_H

#include "actor.h"
#include "actor_mp_state.h"
#include "anticheat_dumpable_object.h"

class CActorMP :
	public IAnticheatDumpable,
	public CActor {
private:
	typedef CActor			inherited;

private:
	actor_mp_state_holder	m_state_holder;
	//bool					m_i_am_dead;
	float					prev_cam_inert_value;
	static const float		cam_inert_value;

	//variables for CharacterInfo
	shared_str				CharacterName_;
	u16						CharacterRank_;
	shared_str				CharacterCommunity_;
	u16						CharacterReputation_;
	shared_str				CharacterIcon_;
	u16						IsGotCharacterInfo;


private:
			void			fill_state			(actor_mp_state &state);
			void			process_packet		(net_update		&N);
			void			postprocess_packet	(net_update_A	&packet);

public:
							CActorMP			();
	virtual void			net_Export			(NET_Packet		&packet);
	virtual void			net_Import			(NET_Packet		&packet);
	virtual BOOL			net_Relevant		();
	virtual void			OnEvent				(NET_Packet		&packet, u16 type);
	virtual void			Die					(CObject		*killer);
	virtual void			DumpActiveParams	(shared_str const & section_name, CInifile & dst_ini) const;
	shared_str const 		GetAnticheatSectionName	() const { return "mp_actor"; };
	virtual void			On_SetEntity		();
	virtual void			On_LostEntity		();

protected:
			void			use_booster				(NET_Packet &packet);
};

#endif // ACTOR_MP_CLIENT_H