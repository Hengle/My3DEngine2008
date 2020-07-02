#ifndef _NEXUS_GAMEFRAMEWORK_DEBUG_DRAW_INTERFACE_H_
#define _NEXUS_GAMEFRAMEWORK_DEBUG_DRAW_INTERFACE_H_
#include "ncore.h"

namespace gameframework
{
	/**
	* ��ͼ�ӿڣ����ⲿʵ�ֻ��Ƶ�������Ϣ
	*/
	class nGameFrameworkAPI ndebug_draw_interface
	{
	public:
		ndebug_draw_interface() {};
		~ndebug_draw_interface() {};

		virtual void draw_line(const vector3& start, const vector3& end, const color4f& color) = 0;
		virtual void draw_line_strip(const std::vector<vector3>& pos_array, const color4f& color) = 0;
		virtual void draw_wired_box(const AABBox& box, const color4f& color) = 0;
		virtual void draw_debug_string(size_t x, size_t y, const nstring& text, const color4ub& color) = 0;
	};

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_DEBUG_DRAW_INTERFACE_H_