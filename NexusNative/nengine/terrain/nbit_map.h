/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_BIT_MAP_H_
#define _NEXUS_BIT_MAP_H_

namespace nexus
{
	/**
	 *	������Height map, ����ÿ��"����"ֻռ��һ��bit
	*/
	class nbit_map
	{
	public:
		nbit_map(void);
		~nbit_map(void);

		void create(size_t w, size_t h, bool init_val);
		void destroy();

		bool get_value(size_t x, size_t y) const;
		void set_value(size_t x, size_t y, bool val);

		/**	�ڲ�����ȫ����0	*/
		void clear();

	private:
		struct	impl;
		impl*	m_impl;
	};
}//namespace nexus

#endif //_NEXUS_BIT_MAP_H_