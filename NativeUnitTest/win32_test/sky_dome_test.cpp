#include "StdAfx.h"
#include "sky_dome_test.h"
#include <sstream>

nDEFINE_CLASS(sky_dome_test, nrender_test)

sky_dome_test::sky_dome_test(void)
{
	m_theta = 75/180*nPI;
	m_phi = 0;
	m_turbidity = 2.5f;
	m_overcast = 0;
	m_expose = 20;
	m_gama = 1;

	m_lgt = NULL;
}

sky_dome_test::~sky_dome_test(void)
{
}

extern nspeed_tree_component::ptr _load_spt(nlevel::ptr level_ptr, const vector3& pos, float scale);
extern nterrain_actor::ptr _generate_terrain(nlevel::ptr level_ptr);
void sky_dome_test::open(perspective_camera_controller* cam)
{
	nrender_test::open(cam);
	
	//nsky_actor::ptr sky = m_level_ptr->create_actor_t<nsky_actor>(_T("sky_dome_test"));
	//sky = sky->create_sky_dome(18000, 50);


	cam->set_look_at( vector3::zero );


#if 1
	nterrain_actor::ptr trn = _generate_terrain(m_level_ptr);

	int trn_w = trn->get_map_width();
	int trn_h = trn->get_map_height();
	for (int i=0; i<256; i++)
	{
		int x = rand()%trn_w;
		int y = rand()%trn_h;
		vector3 tree_pos = trn->tile2world(x,y);

		float s = rand()%10+5;
		
		_load_spt(m_level_ptr, tree_pos, s);
	}	
#endif

	//--
	vector3 dir(-1, -1,-1);
	m_light_anim_radius = 10000;
	add_direct_light(vec_normalize(dir));

#ifdef ENABLE_LEVEL_SAVE_TEST	// ˳�����һ��level���̴���
	resource_location lv_loc(_T("game_data"), _T("level"));
	nengine::instance()->save_level(m_level_ptr->get_name(), lv_loc);
#endif
}

void sky_dome_test::tick(float run_time,float delta_time)
{
	nrender_test::tick(run_time, delta_time);

	float r = sin(run_time*0.1f);
	r = (r+1)*0.5f;

#if 0
	m_gama = 0.0001+r*8;
	m_sky->set_gama(1/m_gama);
#endif 

#if 0
	m_expose = 20*r+10;
	m_sky->set_exposure(true, 1/m_expose);
#endif

#if 0
	m_overcast = r;
	m_sky->set_overcast(m_overcast);
#endif

#if 0
	m_turbidity = 2.5f+2.5f*r;
	m_sky->set_turbidity(m_turbidity);
#endif

#if 1
	m_theta = nPI/2*r;
	m_phi = nPI*r;
	//m_sky->set_sun_dir(m_theta, m_phi);
#endif

	//m_sky->rebuild_colors();
	//m_lgt->local_to_world.set_axis_z(m_sky->get_sun_dir());
	//m_lgt->m_diffuse_color = m_sky->get_sun_color();
}

void sky_dome_test::debug_draw(nrender_primitive_draw_interface* PDI)
{
	std::wostringstream ss;
	ss << _T("theta:") << m_theta/nPI*180
		<< _T(", phi:") << m_phi/nPI*180
		<< _T(",turbidity:") << m_turbidity
		<< _T(", overcast:") << m_overcast
		<< _T(", expose:") << m_expose
		<< _T(", gama:") << m_gama
		;
	PDI->draw_debug_string(2, 18, ss.str(), color4ub(5,255,255,255));
	ntest_case::debug_draw(PDI);
}