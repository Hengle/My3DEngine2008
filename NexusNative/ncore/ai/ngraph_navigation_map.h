#ifndef _NEXUS_CORE_GRAPH_NAVIGATION_MAP_H_
#define _NEXUS_CORE_GRAPH_NAVIGATION_MAP_H_
#include "../ncore.h"
#include "../game_map/navigation_map.h"
#include "micropather.h"
using namespace micropather;

namespace nexus
{
	/**
	* �������micropather��λͼѰ·ͼ
	*/
	class nCoreAPI ngraph_navigation_map 
		: public Graph
	{
	private:
		ngraph_navigation_map(){}

		DISALLOW_COPY_AND_ASSIGN(ngraph_navigation_map);
	public:
		typedef std::vector<npoint> path_result_type;

		enum EFindPatherResult
		{
			EFPR_SOLVED,
			EFPR_NO_SOLUTION,
			EFPR_START_END_SAME,
		};

		explicit ngraph_navigation_map(navigation_map::ptr bit_map);
		virtual ~ngraph_navigation_map();

		void destroy();

		// Ѱ·
		// param: start Ѱ·��ʼ��ĸ�������
		// param: end Ѱ·��ֹ��ĸ�������
		// param: step_height �߶Ȳ����stepoffest��Ϊ�ǲ���ͨ��
		// results: �����ҵ���·����
		EFindPatherResult find_pather(const npoint& start, const npoint& end, float step_height, path_result_type& results);

		MP_UPTR check_sum() { return m_pather->Checksum(); }

		void reset_pather() { m_pather->Reset(); }

		bool passable(size_t x, size_t y)
		{
			return m_nav_map->get_tile_walkable(x, y);
		}

		// micropather node ������֮���ת��
		void node2tile(void* node, int* x, int* y)
		{
			int index = (int)node;
			*y = index / m_nav_map->get_raw_width();
			*x = index - *y * m_nav_map->get_raw_width();
		}

		void* tile2node(int x, int y)
		{
			return (void*)( y * m_nav_map->get_raw_width() + x);
		}

		// ����Graph�ӿ�,ʹ������ʽ���㹫ʽ
		virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd );

		virtual void AdjacentCost( void* node, std::vector< StateCost > *neighbors );

		virtual void PrintStateInfo( void* node );

	private:
		// micropather
		MicroPather* m_pather;
		navigation_map::ptr m_nav_map;

		// ������Ӹ߶Ȳ����һ��ֵ�Ժ�Ͳ���ͨ��
		float m_step_height;
	};

}// end of namespace nexus

#endif // end of _NEXUS_CORE_GRAPH_NAVIGATION_MAP_H_