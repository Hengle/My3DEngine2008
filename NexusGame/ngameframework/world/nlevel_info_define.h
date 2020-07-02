#ifndef _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_DEFINE_H_
#define _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_DEFINE_H_
#include "ncore.h"

using namespace nexus;

namespace gameframework
{
	/**
	* client/server���õĵ�ͼ������Դ�ṹ�嶨��
	*/
	struct nGameFrameworkAPI info_data_base
	{
		// ���ַ�����ΪID
		nname		name;
		// �Ѻ����֣��������
		nstring		friend_name;
		// ����
		nstring		description;

		info_data_base()
		{}

		explicit info_data_base(const nstring& id) 
			: name(id)
		{
		}

		virtual void serialize(narchive& ar)
		{
			nSERIALIZE(ar, name);
			nSERIALIZE(ar, friend_name);
			nSERIALIZE(ar, description);
		}
	};

	/**
	* ����������
	*/
	struct nGameFrameworkAPI start_point_data
		: public info_data_base
	{
		// ������λ��
		vector3		location;

		// ����
		vector3		rotation;

		// ������һ����Χ�����
		float		radius;

		start_point_data() : radius(0)
		{}
		explicit start_point_data(const nstring& id) 
			: info_data_base(id)
		{
		}

		virtual void serialize(narchive& ar)
		{
			info_data_base::serialize(ar);
			nSERIALIZE(ar, location);
			nSERIALIZE(ar, radius);
		}
	};

	/** 
	* �������ṹ�嶨��
	*/
	struct nGameFrameworkAPI trigger_data 
		: public info_data_base
	{
		// λ��
		vector3		location;

		// ��Բ�η�Χ�ڴ���
		float		radius;

		// �л����µĵ�ͼ����������Ҫ�л���ͼ
		nname		transfer_map;

		// �л����µĵ�ͼ��Ӧ�ĳ�����
		nname		start_point;

		// ���ýű�
		nname		invoke_script;

		// �ӳٶ೤ʱ�䴥��
		float		delay_time;

		// �Ƿ��Ѿ�������
		bool		locked;

		trigger_data() : radius(0),delay_time(0),locked(false)
		{}
		explicit trigger_data(const nstring& id) 
			: info_data_base(id)
		{
		}

		virtual void serialize(narchive& ar)
		{
			info_data_base::serialize(ar);
			nSERIALIZE(ar, location);
			nSERIALIZE(ar, radius);
			nSERIALIZE(ar, transfer_map);
			nSERIALIZE(ar, start_point);
			nSERIALIZE(ar, invoke_script);
			nSERIALIZE(ar, delay_time);
			nSERIALIZE(ar, locked);
		}

	};

} // end of namespace gameframework

namespace nexus
{
	template<>
	inline narchive& nserialize(narchive& ar, gameframework::info_data_base& info, const TCHAR* obj_name)
	{
		nstring class_name(_T("info_data_base"));
		ar.object_begin(obj_name, class_name);
		info.serialize(ar);
		ar.object_end();
		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, gameframework::start_point_data& info, const TCHAR* obj_name)
	{
		nstring class_name(_T("start_point_data"));
		ar.object_begin(obj_name, class_name);
		info.serialize(ar);
		ar.object_end();
		return ar;
	}

	template<>
	inline narchive& nserialize(narchive& ar, gameframework::trigger_data& info, const TCHAR* obj_name)
	{
		nstring class_name(_T("trigger_data"));
		ar.object_begin(obj_name, class_name);
		info.serialize(ar);
		ar.object_end();
		return ar;
	}
}

#endif // end of _NEXUS_GAMEFRAMEWORK_NLEVEL_INFO_DEFINE_H_