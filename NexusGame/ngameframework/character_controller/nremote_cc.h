#ifndef _NEXUS_GAMEFRAMEWORK_NREMOTE_CC_H_
#define _NEXUS_GAMEFRAMEWORK_NREMOTE_CC_H_
#include "ncore.h"
#include "ncharacter_controller.h"

namespace gameframework
{
	/**
	* Զ�̽�ɫ��������ֻҪְ���ǽ����ƶ���Ϣ�������ƶ���Ϣ���ƵĽ�ɫ�˶�
	*/
	class nGameFrameworkAPI nremote_cc
		: public ncharacter_controller
	{
	public:
		typedef boost::shared_ptr<nremote_cc> ptr;

		nremote_cc();
		virtual ~nremote_cc();
	};

} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NREMOTE_CC_H_