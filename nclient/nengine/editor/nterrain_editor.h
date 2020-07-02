/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_TERRAIN_EDITOR_H_
#define _NEXUS_TERRAIN_EDITOR_H_
#include "neditor_cmd_mgr.h"
#include "../terrain/nterrain_actor.h"
#include "mouse_helper.h"

namespace nexus
{
	class nrender_primitive_draw_interface;

	/** ��ˢ��ֵ���� */
	enum ETrnLerpMode
	{
		ELM_None = 0,
		ELM_Linear = 1,
		ELM_Cosine = 2,
		ELM_Quadratic = 3,

		ELM_End
	};

	/** ��ˢ����������״���� */
	enum ETrnBrushShape
	{
		EBS_Circle = 0,
		EBS_Square = 1,

		EBS_End
	};

	/** ��ˢ������������*/
	enum ETrnBurshTarget
	{
		EBT_Heightmap = 0,
		EBT_Alphamap,
		EBT_Decomap
	};

	/** ��ˢ���� */
	enum ETrnBrushOP
	{
		EOP_Paint = 0,		
		EOP_Flatten,
		EOP_Smooth,
		EOP_Apply,
		EOP_Noise,

		EHT_End
	};

	/** ��ˢ���� */
	struct nterrain_brush
	{
		ETrnBrushOP		op;
		ETrnBrushShape	shape;
		ETrnLerpMode	lerp_mode;
		int				inner_radius;
		int				outer_radius;
		int				strength;
		int				noise_freq;
		ETrnBurshTarget	target;
		int				target_index;
		
		nterrain_brush()
		{	
			memset(this, 0, sizeof(*this)); 
			inner_radius = 4;
			outer_radius = 8;
			strength = 1;
			noise_freq = 8;
		}
	};

	/**
	 *	���α༭��
	 *	@remarks �ṩ�༭���ܽӿڣ��ṩ����Python�༭�����õİ�װ�ӿ�
	*/
	class nEDAPI nterrain_editor
	{
	public:
		nterrain_editor(void);
		~nterrain_editor(void);

		void bind_terrain(const nterrain_actor::ptr& trn_ptr);

		void close()
		{
			m_trn_actor.reset();
		}

		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);
		void import_heightmap(const nstring& img_file_name);
		
		// material
		void create_material_basic(const resource_location& texture_loc);
		void create_texture_splatting(size_t alpha_w, size_t alpha_h);
		void splat_set_texture(size_t layer_index, const resource_location& texture_loc);
		void splat_layer_noise(size_t layer_index, nrect rc, int numOctaves, float amplitude, float frequency);

		void set_brush(const nterrain_brush& brush)		{	m_brush = brush; }
		void draw_widgets(nrender_primitive_draw_interface* PDI);

		void on_mouse_left_down(const npoint& pt);
		void on_mouse_left_up();
		void on_mouse_right_down();
		void on_mouse_right_up();
		void on_mouse_wheel(int delta);
		void on_mouse_move(const npoint& pt, bool ctrl_down, const ncamera* cam);

	protected:
		void draw_brush_cursor(nrender_primitive_draw_interface* PDI);
		void draw_terrain_circle(int centre_x, int centre_z, int radius, nrender_primitive_draw_interface* PDI);
		void draw_terrain_quad(int left,int right,int top,int bottom, nrender_primitive_draw_interface* PDI);

		void brush_paint(bool neg);

	protected:
		nterrain_brush					m_brush;

		// todo : ΪʲôPython������shared_ptr���ܱ���Ϊweak_ptr
		boost::shared_ptr<nterrain_actor>	m_trn_actor;

		//-- mouse input
		mouse_drag	m_left_drag;
		mouse_drag	m_right_drag;
		
		npoint	m_cursor_pos;
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_EDITOR_H_