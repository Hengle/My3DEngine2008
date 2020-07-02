/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_MOUSE_HELPER_H_
#define _NEXUS_MOUSE_HELPER_H_
#include <boost/timer.hpp>

namespace nexus
{
	/**
	 * ����϶�����
	 * @brief ��Ҫ�������������϶��еĲ����ظ����
	*/
	struct mouse_drag
	{
		bool			m_dragging;
		boost::timer	m_timer;
		double			m_repeat_time;

		mouse_drag():m_dragging(false),m_repeat_time(0.1)
		{}

		/** �Ƿ����϶�״̬ */
		bool draging() const	{	return m_dragging; }

		/** ��������ʱ���� */
		void begin_drag()
		{
			m_timer.restart();
			m_dragging = true;
		}

		/** ����̧��ʱ���� */
		void end_drag()
		{
			m_dragging = false;
		}

		/** �Ƿ�Ӧ�ò����ظ����� */
		bool repeat()
		{
			if( !m_dragging )
				return false;

			if( m_timer.elapsed() > m_repeat_time )
			{
				m_timer.restart();
				return true;
			}
			else
				return false;
		}
	};

}//namespace nexus
#endif