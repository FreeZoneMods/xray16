#include "stdafx.h"
#pragma hdrstop

#include "CameraLook.h"
#include "../xrEngine/Cameramanager.h"
#include "xr_level_controller.h"
#include "actor.h"

CCameraLook::CCameraLook(CObject* p, u32 flags) :CCameraBase(p, flags)
{
}

void CCameraLook::Load(LPCSTR section)
{
	inherited::Load(section);
	style = csLookAt;
	lim_zoom = pSettings->r_fvector2(section, "lim_zoom");
	dist = (lim_zoom[0] + lim_zoom[1])*0.5f;
	prev_d = 0.0f;
	m_look_cam_fp_zoom = 1;
}

CCameraLook::~CCameraLook()
{
}

void CCameraLook::Update(Fvector& point, Fvector& /**noise_dangle/**/)
{
	vPosition.set(point);
	Fmatrix mR;
	mR.setHPB(-yaw, -pitch, -roll);

	vDirection.set(mR.k);
	vNormal.set(mR.j);

	if (m_Flags.is(flRelativeLink))
	{
		parent->XFORM().transform_dir(vDirection);
		parent->XFORM().transform_dir(vNormal);
	}
	UpdateDistance(point);
}

void CCameraLook::UpdateDistance(Fvector& point)
{
	Fvector vDir;
	vDir.invert(vDirection);

	collide::rq_result R;
	float covariance = VIEWPORT_NEAR * 6.0f;
	g_pGameLevel->ObjectSpace.RayPick(point, vDir, dist + covariance, collide::rqtBoth, R, parent);

	float d = psCamSlideInert * prev_d + (1.0f - psCamSlideInert)*(R.range - covariance);
	prev_d = d;

	vPosition.mul(vDirection, -d - VIEWPORT_NEAR);
	vPosition.add(point);
}

void CCameraLook::Move(int cmd, float val, float factor)
{
	switch (cmd) {
	case kDOWN:			pitch -= val ? val : (rot_speed.x*Device.fTimeDelta / factor);	break;
	case kUP:			pitch += val ? val : (rot_speed.x*Device.fTimeDelta / factor);	break;
	case kLEFT:			yaw -= val ? val : (rot_speed.y*Device.fTimeDelta / factor);	break;
	case kRIGHT:		yaw += val ? val : (rot_speed.y*Device.fTimeDelta / factor);	break;
	}
	if (bClampYaw)		clamp(yaw, lim_yaw[0], lim_yaw[1]);
	if (bClampPitch)	clamp(pitch, lim_pitch[0], lim_pitch[1]);
	clamp(dist, lim_zoom[0], lim_zoom[1]);
}

void CCameraLook::OnActivate(CCameraBase* old_cam)
{
	if (old_cam)
	{
		if (m_Flags.is(flRelativeLink) == old_cam->m_Flags.is(flRelativeLink))
			yaw = (old_cam)->yaw;

		if (m_Flags.is(flKeepPitch))
			pitch = (old_cam)->pitch;
	}
	if (yaw>PI_MUL_2) yaw -= PI_MUL_2;
	if (yaw<-PI_MUL_2)yaw += PI_MUL_2;
}

#include "../xrEngine/xr_input.h"
#include "visual_memory_manager.h"
#include "actor_memory.h"

int cam_dik = DIK_LSHIFT;

Fvector CCameraLook2::m_cam_offset;
void CCameraLook2::OnActivate(CCameraBase* old_cam)
{
	CCameraLook::OnActivate(old_cam);
}

void CCameraLook2::Update(Fvector& point, Fvector&)
{
	Fmatrix mR;
	mR.setHPB(-yaw, -pitch, -roll);

	vDirection.set(mR.k);
	vNormal.set(mR.j);

	Fmatrix							a_xform;
	a_xform.setXYZ(0, -yaw, 0);
	a_xform.translate_over(point);
	Fvector _off = m_cam_offset;
	a_xform.transform_tiny(_off);
	vPosition.set(_off);

	UpdateDistance(_off);
}

void CCameraLook2::UpdateAutoAim()
{

}

void CCameraLook2::Load(LPCSTR section)
{
	CCameraLook::Load(section);
	m_cam_offset = Fvector().set(0.314f, 0.2f, 0.0f);
	dist = 1.4f;
	prev_d = 0.0f;

	// hide aimbot hack at release
	if (strstr(Core.Params, "-dbg"))
	{
		m_autoaim_inertion_yaw = pSettings->r_fvector2(section, "autoaim_speed_y");
		m_autoaim_inertion_pitch = pSettings->r_fvector2(section, "autoaim_speed_x");
	}
	else
	{
		m_autoaim_inertion_yaw = Fvector2().set(0.0f, 0.0f);
		m_autoaim_inertion_pitch = Fvector2().set(0.0f, 0.0f);
	}
	m_look_cam_fp_zoom = 1;
}