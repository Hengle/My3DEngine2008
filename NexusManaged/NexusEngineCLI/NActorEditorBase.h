#pragma once
#include "NLevel.h"
#include "NActor.h"
#include "NRenderElement.h"
#include "math/BoundingBox.h"
#include "EditorCommand.h"
#include "../NexusNative/nengine/editor/nactor_handles.h"

namespace NexusEngine
{
	public enum class EActorEditHandle
	{		
		MoveX = 0,
		MoveY,
		MoveZ,
		RotateX,
		RotateY,
		RotateZ,

		Invalid = 255,
	};

	typedef System::Collections::Generic::List<NActor^> ActorList;
	public ref class NActorEditorBase
	{		
	public:
		NActorEditorBase(void);
		virtual ~NActorEditorBase(void);

		void Init();
		void Close();

		void				SetMainLevel(NLevel^ lv)			{	m_level = lv;	}

		// ��ָ����actor��ӵ���ѡ��actor�б�
		virtual void		AddSelectedActor(NActor^ actor);

		// ��ָ��actor����ΪΨһ��ѡ�е�actor�����actorΪnull�����������ѡ��
		virtual void		BindActor(NActor^ actor);

		// ��ȡ��ǰӵ�в����ֱ��ı�ѡ��actor
		virtual NActor^		GetActor()							{	return m_actor;	}

		// ��ȡ��ѡ�е�actor�б�
		virtual ActorList^	GetSelectedActors()					{	return m_actor_list;	}

		// ɾ�����б�ѡ�е�actor
		virtual void		DeleteSelectedActors();

		// ɾ����ӵ�в����ֱ���actor��ͬ���͵�actor���ù���Ŀǰ��δ���ƣ���Ϊ��û����Ч���ж�����actor�Ƿ�����ͬ����
		virtual void		SelectSameTypeActors();

		// ѡ�е�ǰ������ڵ�����actor
		virtual void		SelectAllActorsInLayer();

		// ��ѡ��ǰ���ڵ�actor
		virtual void		SelectOtherActors();

		// ��ʾ�����������е�actor
		virtual void		VisibleAllActors(bool v);

		// ����û�б�ѡ�е�����actor
		virtual void		VisibleSelectedActorsOnly();

		// ����ѡ�е�actor���������Ƿ����
		virtual void		CopySelectedActors(bool cut);

		// ճ�������Ƶ�����actor����Ļ��ָ�����괦
		virtual void		PasteCopyedActors(NCamera^ cam,int x,int y);

		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam, EditorCommand cmd);
		
		virtual void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);
		void DrawActorHandlesHitProxy(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy);

		void SetHotActorHandle(EActorEditHandle h);
		void UpdateView(NCamera^ cam);

		int FindActorHandle(hit_id hid);
		bool IsLeftDraging() {	return m_leftDrag;}

		void ActorMoved();

		property float MouseMoveFactor
		{
			float get()	{	return m_moveFactor;}
			void set(float val)	{	m_moveFactor = val;}
		}

		property float MouseRotateFactor
		{
			float get()	{	return m_rotateFactor;}
			void set(float val)	{	m_rotateFactor = val;}
		}

		property float MoveAlign
		{
			float get()	{	return m_move_align;}
			void set(float val)	{	m_move_align = val>0?val:1.0f;}
		}

		property float RotateAlign
		{
			float get()	{	return m_rotate_align;}
			void set(float val)	{	m_rotate_align = val>0?val:0.001f;}
		}

		property float ScaleAlign
		{
			float get()	{	return m_scale_align;}
			void set(float val)	{	m_scale_align = val>0?val:0.001f;}
		}
	protected:
		virtual void OnMouseMove(int x, int y, bool ctrlDown, NCamera^ cam);
		virtual void OnMouseLButtonDown(int x, int y, NCamera^ cam, EditorCommand cmd);
		virtual void OnMouseLButtonUp();

		void PlaceActorToGround();		
		void DrawActorHandles(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy);
	
	protected:
		NLevel^			m_level;	//���ڱ༭��Level
		NActor^			m_actor;		
		ActorList^		m_actor_list;

		//����-����-ճ����ر���
		System::Collections::Generic::List<String^>^		m_copy_list;	//Ҫ���ƻ��߼��е�actor�б�
		Vector3			m_copy_center;	//actors������
		bool			m_cut_mode;		//�Ƿ��Ǽ���ģʽ������ǣ�����ճ�����Զ�ɾ��ԭ��������

		//-- mouse
		bool	m_leftDrag;
		System::Drawing::Point	m_cursorPos;
		float	m_moveFactor;
		float	m_rotateFactor;
		//-----------------------------------------------------------------------------------
		//λ�ö���
		float	m_move_align;
		float	m_rotate_align;
		float	m_scale_align;

		Vector3	m_move_states;
		Vector3 m_rotate_states;

		bool	m_shift_down;
		bool	m_enable_shift_copy;

	private:
		EActorEditHandle	m_hotHandle;
		nexus::nactor_handles*		m_actorHandles;
	};

	//��Ȼ�������ʹ��ģ�壬���ǿ��ǵ���float��vector3����Ҫ�ػ�������Ŀǰֻ��Ҫ�õ�float��vector3������ֱ��ʹ�ú������ؽ�Ϊ�׵���
	/*template<typename T>
	void inline align(T& value,T align_size)
	{}*/

	void inline align(float& value,float align_size)
	{
		float mod=fmodf(value,align_size);
		value=(mod>=align_size*0.5f)?(value+align_size-mod):(value-mod);	//������������ʵ�֡�������Ч��
	}

	void inline align(Vector3& vec,float align_size)
	{
		align(vec.x,align_size);
		align(vec.y,align_size);
		align(vec.z,align_size);
	}

}//namespace NexusEngine