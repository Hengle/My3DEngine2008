#ifndef _NEXUS_CORE_GRAPH_BIT_MAP_H_
#define _NEXUS_CORE_GRAPH_BIT_MAP_H_
#include "../ncore.h"
#include "../height_map/nbit_map.h"
#include "micropather.h"
using namespace micropather;

namespace nexus
{
	/**
	* �������micropather��λͼѰ·ͼ
	*/
	class nCoreAPI ngraph_bit_map 
		: public Graph
	{
	private:
		ngraph_bit_map(){}

		DISALLOW_COPY_AND_ASSIGN(ngraph_bit_map);
	public:
		typedef std::vector<npoint> path_result_type;

		enum EFindPatherResult
		{
			EFPR_SOLVED,
			EFPR_NO_SOLUTION,
			EFPR_START_END_SAME,
		};

		explicit ngraph_bit_map(nbit_map* bit_map);
		virtual ~ngraph_bit_map();

		void destroy();

		// Ѱ·
		// param results: �����ҵ���·����
		EFindPatherResult find_pather(const npoint& start, const npoint& end, path_result_type& results);

		unsigned int check_sum() { return m_pather->Checksum(); }

		void reset_pather() { m_pather->Reset(); }

		bool passable(size_t x, size_t y)
		{
			return m_bit_map->get_value(x, y);
		}

		// micropather node ������֮���ת��
		void node2tile(void* node, int* x, int* y)
		{
			int index = (int)node;
			*y = index / m_bit_map->get_width();
			*x = index - *y * m_bit_map->get_width();
		}

		void* tile2node(int x, int y)
		{
			return (void*)( y * m_bit_map->get_width() + x);
		}

		// ����Graph�ӿ�,ʹ������ʽ���㹫ʽ
		virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd );

		virtual void AdjacentCost( void* node, std::vector< StateCost > *neighbors );

		virtual void PrintStateInfo( void* node );

	private:
		// micropather
		MicroPather* m_pather;
		nbit_map* m_bit_map;
	};

} // end of namespace nexus

#endif // end of _NEXUS_CORE_GRAPH_BIT_MAP_H_