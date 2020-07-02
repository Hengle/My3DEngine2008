/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RENDER_GROUP_FULL_H_
#define _NEXUS_RENDER_GROUP_FULL_H_
#include "render_group.h"
#include "drawing_atom.h"

namespace nexus
{
	// ��mesh��ɰ���ʹ��instanceһ��draw call����
	struct mesh_packet
	{};

	class render_group_world :
		public render_group
	{
	public:
		render_group_world(void);
		virtual ~render_group_world(void);

		virtual bool empty() const;
		virtual void clear();
		virtual void add_obj(const nrender_proxy* obj);
		
		virtual void draw_all(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter=NULL);
		
		void sort_draw_order(const d3d_view_info* view);
		// draw all opaque objects and alpha masked opaque part of mixed transparent objects
		void draw_opaque(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter=NULL);
		// draw unlit transparent objects
		void draw_unlit_trans(const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter=NULL);
	private:		
		void draw_atom_array(st_drawing_atom_array& pv, const d3d_view_info* view, drawing_policy* dp, drawing_filter* filter=NULL);		

		st_drawing_atom_array	m_atoms[ETrans_Max];
	};
}//namespace nexus

#endif //_NEXUS_RENDER_GROUP_FULL_H_