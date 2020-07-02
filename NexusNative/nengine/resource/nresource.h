/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_RESOURCE_H_
#define _NEXUS_RESOURCE_H_
#include <boost/thread/mutex.hpp>
#include "ncore.h"

namespace nexus
{
	struct resource_location
	{
		nstring	pkg_name;
		nstring	file_name;

		bool is_valid() const
		{	
			return !pkg_name.empty()
				&& !file_name.empty();
		}
		resource_location()	{}
		// pkg:file
		explicit resource_location(const nstring& str)
		{
			nstring::size_type f = str.find(_T(':'));
			if( f != nstring::npos )
			{
				pkg_name = str.substr(0, f);
				file_name = str.substr(f+1);
			}
		}

		resource_location(const nstring& pkg, const nstring& file):pkg_name(pkg),file_name(file)
		{}

		nstring to_string() const
		{
			nstring ret = pkg_name;
			ret += _T(':');
			ret += file_name;
			return ret;
		}

		nstring get_file_ext() const
		{
			nstring ext;
			nstring::size_type f = file_name.find_last_of(_T('.'));
			if( f != nstring::npos )
			{
				ext = file_name.substr(f+1);
			}
			return ext;
		}
	};

	template<>
	inline narchive& nserialize(narchive& ar, resource_location& obj, const TCHAR* obj_name)
	{
		nstring class_name(_T("resource_location"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, obj.pkg_name, _T("pkg_name"));
		nserialize(ar, obj.file_name, _T("file_name"));
		ar.object_end();

		return ar;
	}

	/**
	 *	������Դ����
	 *	@remark һ����Դͨ����Ӧһ�������ļ�,������ͼ, mesh��
	 *	@remark ���̰߳�ȫ��
	*/
	class nAPI nresource :
		public nobject
	{
	public:
		typedef boost::intrusive_ptr<nresource> ptr;

		explicit nresource(const nstring& name_str) : m_ref_count(0), m_name(name_str), m_ready(0)
		{}
		virtual ~nresource(void)
		{}

		const resource_location& get_location() const;
		const nname& get_name() const					{	return m_name;}
		const nstring& get_name_str() const				{	return m_name.name_str;}
		void add_ref();
		virtual void release();

		virtual void load_from_file(const resource_location& loc);
		virtual void save_to_file(const resource_location& loc, bool xml);		
		
		//!	������Դ������ɣ��������С�����Դ��Ҳ������ɲŷ���true
		virtual bool ready() const	{	return m_ready!=0; }

		//! �ڱ༭���龰�£�newһ����Դ��û��ready�ģ���Ҫ�ֶ��л���ready�������ϲ��޷���ʾ
		virtual void post_edit_change(bool ready);

		virtual size_t get_system_memory_size() const	{	return 0; }
		virtual size_t get_render_memory_size() const	{	return 0; }

		virtual void serialize(narchive& ar);

		virtual void _on_device_lost(int param)		{	(void)param; }
		virtual bool _on_device_reset(int param)	{	(void)param; return true; }

	protected:
		void do_serialize_io(const resource_location& loc, enum EFileMode fmode, bool xml);

		volatile long		m_ref_count;
		volatile long		m_ready;	// �Ƿ������Ѿ�������ϣ���Ҫ�������첽IO�ĳ���

		mutable boost::mutex		m_mutex;
		nname						m_name;
		resource_location			m_location;

		nDECLARE_VIRTUAL_CLASS(nresource)
	};
}//namespace nexus

namespace boost
{
	inline void intrusive_ptr_add_ref( nexus::nresource* res )
	{
		res->add_ref();
	}

	inline void intrusive_ptr_release( nexus::nresource* res )
	{
		res->release();
	}
}

#endif //_NEXUS_RESOURCE_H_