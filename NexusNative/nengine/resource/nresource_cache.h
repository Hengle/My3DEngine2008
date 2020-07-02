/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RESOURCE_CACHE_H_
#define _NEXUS_RESOURCE_CACHE_H_
#include "ncore.h"
#include "nresource.h"

namespace nexus
{
	/**
	 *	resource cache���Ի���
	*/
	class nresource_cache
		: public nobject
	{
	public:
		nresource_cache(void)	{	}
		virtual ~nresource_cache(void)	{	}

		virtual void cache_resource(nresource* res_ptr) = 0;

		/**
		 * @return �Ƿ���Ҫdelete resource����
		*/
		virtual bool release_resource(nresource* res_ptr) = 0;
		virtual nresource* active_resource(const nname& res_name) = 0;
		virtual nresource* find_resource(const nname& res_name) = 0;
		virtual size_t get_num_resource() = 0;

		nDECLARE_VIRTUAL_CLASS(nresource_cache)
	};
}//namespace nexus
#endif //_NEXUS_RESOURCE_CACHE_H_