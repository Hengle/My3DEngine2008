#ifndef _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_RESOURCE_H_
#define _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_RESOURCE_H_
#include "ncore.h"
#include "nlevel_info_define.h"

namespace gameframework
{
	// ���л�ʱ�Ĺ�����
	enum ELevelInfoType
	{
		ELIRT_StartPoint	= 1<<1,
		ELIRT_Trigger		= 1<<2,

		ELIRT_ALL			= 0xFFFFFFFF
	};

	/**
	* ������������Դ
	*/ 
	class nGameFrameworkAPI nlevel_info_resource
	{
	public:
		typedef boost::shared_ptr<nlevel_info_resource> ptr;

		typedef std::vector<start_point_data>		start_point_data_vec;
		typedef std::vector<trigger_data>			trigger_data_vec;

		nlevel_info_resource();
		virtual ~nlevel_info_resource();

		//--  ���л�
		void serialize( nfile_system* fs,const nstring& pkg_name, const nstring& file_name, enum EFileMode mode, ELevelInfoType filter);

		// create data (?todo:���ȥ����ID�ظ�������,Ŀǰ���������ⲿȥ��֤IDΨһ�����������ͬID�򷵻�false)
		bool add_info(const start_point_data& data);
		bool add_info(const trigger_data& data);

		// remove
		void remove_start_point_data(const nname& id);
		void remove_trigger_data(const nname& id);

		// find
		const start_point_data* find_start_point_data(const nname& id);
		const trigger_data* find_trigger_data(const nname& id);

		// get vector
		const start_point_data_vec& get_start_point_data() const { return m_vec_start_points; }
		const trigger_data_vec& get_trigger_data() const { return m_vec_triggers; }

		// clear
		void clear()
		{
			m_vec_start_points.clear();
			m_vec_triggers.clear();
		}

		// �ṩһЩ���ֺ��������ⲿ����Ψһ��ʶ
		nstring make_unique_id(ELevelInfoType info_type);

	private:
		start_point_data_vec			m_vec_start_points;
		trigger_data_vec				m_vec_triggers;
	};
} // end of namespace gameframework

#endif // end of _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_RESOURCE_H_