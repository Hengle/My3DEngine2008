#ifndef _NEXUS_AVATAR_ACTOR_H_
#define _NEXUS_AVATAR_ACTOR_H_
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../animation/nskel_anim_play.h"
namespace nexus
{
	class nAPI navatar_actor :
		public nactor
	{
	public:
		typedef shared_ptr<navatar_actor> ptr;

		navatar_actor(const nstring& name_str);
		virtual ~navatar_actor(void);

		void create(const resource_location& anim_set_loc);

		virtual void update(float delta_time, const nviewport& view);

		/**
		 *	���һ��skeletal mesh�����������ʾ��Ϸ��ɫ�Ŀ��׵��涯�����ε�װ��
		*/
		void bind_skeletal_mesh(const nstring& comp_name, const resource_location& res_loc);
		
		/**
		 *	���һ���Ҽ������󶨵�ָ���Ĺ�����������ʾ��Ϸ��ɫ��������Ӳ�ʵĶ���
		*/
		void bind_attachment(const nstring& comp_name, const nstring& attach_bone, const resource_location& res_loc);
		

		/**
		 *	ʹ�û����remove_component()�ӿڿ���ȥ��ĳ�����
		*/

		nskel_anim_controller::ptr get_anim_ctrl() const	{	return m_anim_ctrl;}


		/**
		* ��Ӷ������Žӿڷ���CLI���װ
		*/
		virtual void set_anim(const nstring& sequence);
		virtual void play_anim(bool loop, float rate, float start_time);
		virtual void stop_anim();
		virtual void set_position(float new_time, bool fire_notifies);
		// ��ȡ��ǰanim sequenceʣ��ʱ��
		virtual float get_time_left() const;
		// ��ȡ��ǰanim sequence�ܳ���
		virtual float get_anim_length() const;
		// ��ȡ��ǰanim sequence��֡��
		virtual size_t get_anim_frame() const;
		// ��ȡ��ǰanim sequence
		virtual const TCHAR* get_anim_playing() const;

	protected:
		shared_ptr<nskel_anim_play>	m_anim_ctrl;

		nDECLARE_NAMED_CLASS(navatar_actor)
	};
	
}//namespace nexus

#endif //_NEXUS_AVATAR_ACTOR_H_