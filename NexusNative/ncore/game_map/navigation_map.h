/**
*	Core of Nexus Engine
*
*	Author: zewoo
*	Date:	2010.3 @raycolor.net
*/

#ifndef _NEXUS_CORE_NAVIGATION_MAP_H_
#define _NEXUS_CORE_NAVIGATION_MAP_H_
#include "../ncore.h"
#include "../height_map/nheight_map.h"
#include "../height_map/nbit_map.h"
#include "../math/ncheck_result.h"

namespace nexus
{
	/**	\class navigation_map
		\brief Level������Ϣ,ÿ����Ϸ��LevelӦ�ñ���һ��
	*/
	class nCoreAPI navigation_map
	{
	public:
		typedef boost::shared_ptr<navigation_map> ptr;

		navigation_map(){};
		virtual ~navigation_map(){};

		// ��¡����ͼ���ú�����newһ������ͼ����
		static navigation_map::ptr clone(navigation_map* src_nav_map)
		{
			navigation_map::ptr new_nav_map(new navigation_map);
			new_nav_map->create(src_nav_map->get_raw_width(),src_nav_map->get_raw_height(), src_nav_map->get_location(), src_nav_map->get_scale());
			new_nav_map->blit(src_nav_map, 0, 0);

			return new_nav_map;
		}

		/** ��������ͼ
			\param : wָ������ͼ��� h�߶� loc����ͼλ�ã�scale����
		*/
		virtual void create(size_t w, size_t h, const vector3& loc, const vector3& scale);

		//-- ���߼���㷨
		// param start: ���߼�����ʼ�㣨�������꣩
		// param start: ���߼�����ֹ�㣨�������꣩
		// param walkable_check: �ɷ����������⣬true������Ƿ������ͼ��false��������Ƿ������ͼ��ֻ���߶�ͼ
		// param out_hit: �����⵽��ײ��Ϣ
		// return false:����֮����˳��ͨ��û���赲��true: ����֮�����赲
		virtual bool line_of_sight(const vector3& start, const vector3& end, bool walkable_check, ncheck_result& out_hit);
		/**	����һ�������ڵ������ϻ�������
		@return 1 == �ڵ���ͼ�Ϸ�, -1 == �ڵ���ͼ�·� */
		int classify_side(const vector3& wpt);
		int classify_side(const npoint& tile_pt, float test_height);

		/** ʹ�û����㷨�ж��Ƿ��ֱ�ߵ���,ֻ����Ƿ������λͼ
		\ param start_pt:	��ʼ��������
		\ param end_pt:		��ֹ��������
		\ param near_pos:	���һ����������ĵ�
		\ return ����һ���������յ�᷵��true
		*/
		bool can_walkable_to(const npoint& start_pt,const npoint& end_pt, npoint& out_near_pos);

		//-- ��src_map��copy�߶���Ϣ������ָ����ʼcopy��ƫ��
		virtual void blit(navigation_map* src_map, size_t x_offest, size_t y_offest);

		//-- �ı䵼��ͼ�Ĵ�С�����Ǳ���ԭ���ĸ߶�ͼ��Ϣ
		virtual void resize(size_t w, size_t h);

		//-- �������굽��������װ��
		npoint world2tile(float wx, float wz) const;
		
		npoint safe_world2tile(float x,float z) const;

		//-- �������굽��������ת��
		vector3 tile2world(int tx, int tz) const;

		//-- ͨ�����������ȡ�߶�
		float get_world_height(float wx, float wz) const;

		//-- ͨ�����������ȡ�߶�
		float get_tile_height(int tx, int tz) const;

		//-- ͨ�����������ȡ��Ӧ��ԭʼ�߶�ͼ�߶�
		int get_raw_height(int tx, int tz) const
		{
			if( m_height_map.is_valid() 
				&& m_height_map.is_valid_index(tx,tz) ) 
				return m_height_map.get_value(tx,tz);
			else
				return 0;
		}

		bool is_valid_index(int tx, int tz) const
		{
			return m_height_map.is_valid_index(tx,tz);
		}

		//-- ��ȡ�߶�ͼ�Ŀ�Ⱥ͸߶�
		int get_raw_width() const { return m_height_map.get_width(); }
		int get_raw_height() const { return m_height_map.get_height(); }

		// ���ø߶�ͼ���Ӹ߶�
		void set_raw_height(int x, int y, short value) { m_height_map.set_value(x,y,value); }
		void set_height(int x, int y, float value);

		nheight_map16* get_height_map() { return &m_height_map; }
		nbit_map* get_walkable_map() { return &m_walkable_map; }

		//-- ��Ӧ�ĸ����Ƿ������
		bool get_tile_walkable(int tx, int tz) const;
		void set_tile_walkable(int tx, int tz, bool value) { m_walkable_map.set_value(tx,tz,value); };
		//-- ͨ�����������ȡ�Ƿ������
		bool get_world_walkable(float wx, float wz) const;

		// �õ�ĳ�����ӵķ���
		vector3 get_tile_normal(int x, int z) const;

		//-- ���úͻ�ȡ����ֵ
		const vector3& set_scale(const vector3& value)	{ m_scale = value; return m_scale; }
		const vector3& get_scale() const { return m_scale; }

		//-- ���ú���ʼλ������ֵ
		const vector3& set_location(const vector3& value)	{ m_location = value; return m_location; }
		const vector3& get_location() const { return m_location; }

		//--  ���л�
		virtual void serialize( nfile_system* fs,const nstring& pkg_name,const nstring& file_name, enum EFileMode mode );

	private:
		//-- ����ͼ��λ�ã�λ�þ�������ͼ��ƫ��
		vector3 m_location;
		//-- ����ͼ�����ţ�����x��z�����˵���ͼ���Ӵ�С��y�����߶�����
		vector3 m_scale;
		//-- �߶�ͼ��Ϣ
		nheight_map16	m_height_map;
		//-- �ɷ�����,true:��������ߣ�false:����������
		nbit_map	m_walkable_map;
	};

}//namespace nexus

#endif // end of _NEXUS_CORE_NAVIGATION_MAP_H_