#ifndef _NEXUS_GAMEFRAMEWORK_NLOCAL_CC_H_
#define _NEXUS_GAMEFRAMEWORK_NLOCAL_CC_H_
#include "ncore.h"
#include "ncharacter_controller.h"

namespace gameframework
{
	/**
	* ���ؽ�ɫ��������ֻҪְ��ͨ���������AI���ƽ�ɫ�˶���������һ���Ĳ��Է����ƶ���Ϣ
	*/
	class nGameFrameworkAPI nlocal_cc 
		: public ncharacter_controller
	{
	public:
		typedef boost::shared_ptr<nlocal_cc> ptr;

		nlocal_cc();
		virtual ~nlocal_cc();
	};

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NLOCAL_CC_H_