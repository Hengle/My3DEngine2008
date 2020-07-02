#ifndef _NEXUS_RESOURCE_ASYNC_IO_H_
#define _NEXUS_RESOURCE_ASYNC_IO_H_
#include <queue>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include "nresource.h"
#include "nresource_io.h"

namespace nexus
{
	enum EIORequestType
	{
		EIOReq_None,
		EIOReq_LoadResource,	// ����һ����Դ			
	};

	/** һ����ԴIO������� */
	struct io_request
	{
		enum EIORequestType			req_type;
		enum EResourceIOPriority	priority;
		nresource::ptr				res_ptr;		
		
		nstring				pkg_name;
		nstring				file_name;

		io_request():req_type(EIOReq_None), res_ptr(NULL), priority(EIOP_Normal)
		{}
	};

	struct _comp_req
	{
		bool operator()(const io_request& r1, const io_request& r2)
		{
			return r1.priority < r2.priority;
		}
	};

	/*
	 *	�̰߳�ȫ����ԴIO������У������ȼ��Ŷӣ�
	*/
	class io_request_queue
	{
	public:
		io_request_queue(void)	{}
		~io_request_queue(void)	{}

		void push_request(const io_request& req);
		void pop_request(io_request& out_req);		
		bool empty() const;
		void wait_for_request();
		void cancel_wait();

	private:
		typedef std::priority_queue<io_request, vector<io_request>, _comp_req> st_queue;

		mutable boost::mutex		m_mutex;
		boost::condition	m_con;
		st_queue			m_queue;
	};
	
	/**
	 *	�첽��ԴIO���ԣ�ʹ��һ���߳�������IO���ȼ�ȥ������Դ
	*/
	class nresource_async_io
		: public nresource_io
	{			
	public:
		nresource_async_io(void);
		virtual ~nresource_async_io(void);

		virtual void start();
		virtual void shutdown();

		virtual void load_resource(nresource::ptr res_ptr,
			const nstring& pkg_name, const nstring& file_name, enum EResourceIOMode mode, enum EResourceIOPriority pri);

		void block_till_all_requests_finish();

	private:
		void io_thread_func();
	
	private:
		boost::thread_group	m_threads;

		io_request_queue	m_requests;	// �������
		volatile long		m_shutdown;	// �رձ�־

		nDECLARE_CLASS(nresource_async_io)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_ASYNC_IO_H_