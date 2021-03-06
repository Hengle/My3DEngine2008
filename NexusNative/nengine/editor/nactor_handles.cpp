#include "StdAfx.h"
#include "nactor_handles.h"

namespace nexus
{
	nactor_handles::nactor_handles(void)
	{
	}

	nactor_handles::~nactor_handles(void)
	{
	}

	void nactor_handles::init()
	{	
		float c = 195/256.0f;
		vector4 colorX = vector4(c, 0, 0, 1);
		vector4 colorY = vector4(0, c, 0, 1);
		vector4 colorZ = vector4(0, 0, c, 1);

		//-- load move axis
		{
			nstatic_mesh_component::ptr moveX = load_axis_mesh(_T("ActorHandle_MoveX"), colorX);
			nstatic_mesh_component::ptr moveY = load_axis_mesh(_T("ActorHandle_MoveY"), colorY);
			nstatic_mesh_component::ptr moveZ = load_axis_mesh(_T("ActorHandle_MoveZ"), colorZ);

			m_handleMeshs.push_back(moveX);
			m_handleMeshs.push_back(moveY);
			m_handleMeshs.push_back(moveZ);
		}

		//-- load rotate axis
		{
			nstatic_mesh_component::ptr rotX = load_rotate_mesh(_T("ActorHandle_RotX"), colorX);
			nstatic_mesh_component::ptr rotY = load_rotate_mesh(_T("ActorHandle_RotY"), colorY);
			nstatic_mesh_component::ptr rotZ = load_rotate_mesh(_T("ActorHandle_RotZ"), colorZ);

			m_handleMeshs.push_back(rotX);
			m_handleMeshs.push_back(rotY);
			m_handleMeshs.push_back(rotZ);
		}

		set_actor_space(object_space());
	}

	void nactor_handles::set_actor_space(const object_space& sp)
	{
		if(m_handleMeshs.empty())
			return;

		//-- move
		nstatic_mesh_component::ptr meshX = m_handleMeshs[0];
		nstatic_mesh_component::ptr meshY = m_handleMeshs[1];
		nstatic_mesh_component::ptr meshZ = m_handleMeshs[2];

		meshX->move(sp.location, vector3::zero, vector3(1,1,1));
		meshY->move(sp.location, vector3::zero, vector3(1,1,1));
		meshZ->move(sp.location, vector3::zero, vector3(1,1,1));

		{
			object_space meshSpace = meshX->get_space();
			meshSpace.rotation.z -= nHalfPI;
			meshX->move(meshSpace.location, meshSpace.rotation, meshSpace.scale);
		}

		{
			object_space meshSpace = meshZ->get_space();
			meshSpace.rotation.x += nHalfPI;
			meshZ->move(meshSpace.location, meshSpace.rotation, meshSpace.scale);
		}

		//-- rotate
		nstatic_mesh_component::ptr rotX = m_handleMeshs[3];
		nstatic_mesh_component::ptr rotY = m_handleMeshs[4];
		nstatic_mesh_component::ptr rotZ = m_handleMeshs[5];

		rotX->move(sp.location, vector3::zero, vector3(1,1,1));
		rotY->move(sp.location, vector3::zero, vector3(1,1,1));
		rotZ->move(sp.location, vector3::zero, vector3(1,1,1));

		{
			object_space meshSpace = rotX->get_space();
			meshSpace.rotation.z -= nHalfPI;
			rotX->move(meshSpace.location, meshSpace.rotation, meshSpace.scale);
		}

		{
			object_space meshSpace = rotZ->get_space();
			meshSpace.rotation.x += nHalfPI;
			rotZ->move(meshSpace.location, meshSpace.rotation, meshSpace.scale);
		}
	}

	nstatic_mesh_component::ptr nactor_handles::load_axis_mesh(const nstring& name, const vector4& color)
	{		
		nresource_static_mesh::ptr mesh_res = nresource_manager::instance()->new_static_mesh( name+_T("_RES") );
		mesh_res->import_lod(-1, _T("../nexus_game/engine_data/editor_res/axis.obj"));

		//-- 箭头的杆子和头部使用两个材质，这样在设置handle为hot时箭头的颜色保留
		resource_location shader_loc(_T("engine_data:material/ENG_ConstColor.hlsl"));
		nmtl_static::ptr mtl_a ( new nmtl_static(_T("axis_mesh_color")) );
		mtl_a->default_create(shader_loc);
		mtl_a->set_value_parameter("MTL_DiffuseColor", color);

		mesh_res->import_set_material(0, 0, mtl_a);
		mesh_res->post_edit_change(true);


		nmtl_static::ptr mtl_b ( new nmtl_static(_T("axis_mesh_color")) );
		mtl_b->default_create(shader_loc);
		mtl_b->set_value_parameter("MTL_DiffuseColor", color);
		mesh_res->import_set_material(0, 1, mtl_b);


		nstatic_mesh_component::ptr axis_mesh(
			new nstatic_mesh_component(name)
			);
		axis_mesh->reset_resource(mesh_res);
		
		return axis_mesh;
	}

	nstatic_mesh_component::ptr nactor_handles::load_rotate_mesh(const nstring& name, const vector4& color)
	{
		nresource_static_mesh::ptr mesh_res = nresource_manager::instance()->new_static_mesh( name+_T("_RES") );
		mesh_res->import_lod(-1, _T("../nexus_game/engine_data/editor_res/axis_rot.obj"));

		resource_location shader_loc(_T("engine_data:material/ENG_ConstColor.hlsl"));
		nmtl_static::ptr mtl_a ( new nmtl_static(_T("axis_mesh_color")) );
		mtl_a->default_create(shader_loc);
		mtl_a->set_value_parameter("MTL_DiffuseColor", color);
		
		mesh_res->import_set_material(0, 0, mtl_a);
		mesh_res->post_edit_change(true);

		nstatic_mesh_component::ptr axis_mesh(
			new nstatic_mesh_component(name)
			);

		axis_mesh->reset_resource( mesh_res );
		return axis_mesh;
	}

	void nactor_handles::draw_move(nrender_primitive_draw_interface* PDI, bool hit_proxy)
	{
		draw(0, PDI, hit_proxy);
	}

	void nactor_handles::draw_rotate(nrender_primitive_draw_interface* PDI, bool hit_proxy)
	{
		draw(3, PDI, hit_proxy);
	}
	
	void nactor_handles::draw(size_t base_index, nrender_primitive_draw_interface* PDI, bool hit_proxy)
	{
		for (size_t i=base_index; 
			i-base_index<3 && i<m_handleMeshs.size(); 
			i++)
		{
			PDI->draw_mesh_component(m_handleMeshs[i].get(), hit_proxy, true);
		}
	}

	int nactor_handles::find_handle(hit_id hid)
	{
		for (size_t i=0; i<m_handleMeshs.size(); i++)
		{
			if(m_handleMeshs[i]->get_name().name_crc == hid)
				return (int)i;
		}
		return -1;
	}

	void nactor_handles::set_hot_handle(int h)
	{	
		vector4 gray_tab[3];
		float c = 195/256.0f;
		gray_tab[0] = vector4(c, 0, 0, 1);
		gray_tab[1] = vector4(0, c, 0, 1);
		gray_tab[2] = vector4(0, 0, c, 1);

		vector4 light_color(250/256.0f, 240/256.0f, 0, 1);		
		
		for (int i=0; i<(int)m_handleMeshs.size(); i++)
		{
			nstring name = m_handleMeshs[i]->get_name().name_str;
			int axis = name[name.length()-1]-_T('X');
			nASSERT(axis >=0 && axis < 3);

			nmtl_base* pmtl = m_handleMeshs[i]->get_material(0, 0);
			nmtl_static* mtl = ndynamic_cast<nmtl_static>(pmtl);
			if(mtl)
			{
				if(i == h)
					mtl->set_value_parameter("MTL_DiffuseColor", light_color);				
				else				
					mtl->set_value_parameter("MTL_DiffuseColor", gray_tab[axis%3]);				
			}// end of if
		}// end of for
	}

	void nactor_handles::fit_camera(const ncamera* cam)
	{		
		const int len_in_pixel = 128;	// 想要把数轴渲染成多少像素长度
		const float len = 100.0f;		// 坐标轴模型未缩放时的长度

		vector3 t0 = cam->screen2world(0,0);
		vector3 t1 = cam->screen2world(0,len_in_pixel);
		float target_len = vec_distance(t0,t1);
		
		for (size_t i=0; i<m_handleMeshs.size(); i++)
		{
			nstatic_mesh_component::ptr m = m_handleMeshs[i];
			object_space os = m->get_space();
			
			float depth = vec_distance(os.location, cam->get_eye_pos());
			float len2 = target_len/cam->get_znear()*depth;
			
			float s = len2/len;

			os.scale = vector3(s,s,s);
			m->move(os.location, os.rotation, os.scale);
		}
	}
}//namespace NexusEngine