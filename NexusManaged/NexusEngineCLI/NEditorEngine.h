/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NEngine.h"
#include "NViewport.h"
#include "NEditorLog.h"
#include "NTerrainEditor.h"
#include "NActorEditorBase.h"
#include "NLevel.h"
#include "EditorCommand.h"
#include "NRenderElement.h"

namespace NexusEngine
{
	ref class NCamera;
	ref class ThumbnailCreatorManager;

	public value struct NEditorConfig
	{
		System::String^ actorEdClassAssembly;
		System::String^	actorEditorClass;
	};	
	
	/**
	 *	�༭��ʹ�õ��������
	*/
	public ref class NEditorEngine : public NEngine
	{
	public:
		System::EventHandler^ SelectionLockTag;

		//--
		NEditorEngine(void);

		//!	����"����ͼ����"����
		virtual void InitModules(NEngineConfig^ engineCfg, NEditorConfig editofCfg);

		//!	����"����ͼ����"����
		virtual void Close() override;

		//!	��Ⱦָ����Level
		virtual void RenderLevel(System::String^ lvName, NViewport^ vp, bool present);		

		//!	��Ⱦָ����Level,������HitProxyMap
		virtual void RenderMainLevel(System::String^ lvName, NViewport^ vp);
		

		//!	�������йؿ��еĶ���
		virtual void UpdateLevel(System::String^ lvName, float delta_time);

		System::IntPtr CreateThumbnail(NResourceLoc loc, int w, int h);		
		
		void RedirectLog(NEditorLog^ logOutput);

		//! ����viewport�Ĵ�����Ϣ
		//!	@return 0-û�д��������Ϣ��1�Ѿ������������Ϣ��2Ӧ��CameraCtrl���������Ϣ
		virtual int HandleViewportMessage(System::Windows::Forms::Message% msg, NViewport^ view);		

		property EditorCommand Command
		{
			EditorCommand get()	{	return m_command; }
			void set(EditorCommand val)	{	m_command = val; }
		}

		property NTerrainEditor^ TerrainEd
		{
			NTerrainEditor^ get()	{	return m_terrainEd;}
		}

		property NActorEditorBase^ ActorEd
		{
			NActorEditorBase^ get()	{	return m_actorEd;}
		}

		property NLevel^ MainLevel
		{
			NLevel^ get() { return m_mainLevel; }            
		}

		property System::String^ MainLevelName
		{
			System::String^ get() { return MainLevel->Name; }
		}        	

		property bool SelectionLocked
		{
			bool get()	{	return m_selectionLock;}
		}

		virtual void CreateMainLevel(System::String^ levelName);

		NResourceLoc GetCurrentResourceLoc()
		{
			return m_curResrouce;
		}
	protected:
		void CreateSubEditors();		
		void ResetSubEditors();		

		EActorEditHandle PickActorHandle(NViewport^ vp, int mouseX, int mouseY);
		
		//!	���ʰȡ���ؿ��е�Actor
		NActor^ PickMainLevelActor(NViewport^ vp, int mouseX, int mouseY);

	protected:
		bool						m_selectionLock;
		EditorCommand				m_command;
		NEditorConfig				m_editofCfg;
		
		NLevel^						m_mainLevel;	// ���ڱ༭���Ĺؿ�
		NTerrainEditor^				m_terrainEd;
		NActorEditorBase^			m_actorEd;
		ThumbnailCreatorManager^	m_thumbCreator;

		NResourceLoc				m_curFolder;   // ��ǰ����·��(�ļ���)
		NResourceLoc				m_curResrouce; // ��ǰѡ�����Դ(�ļ�)
		
		//-- Native Wrapper
	protected:
		property neditor_engine* NativePtr
		{
			neditor_engine* get();
		}
	};
}//namespace NexusEngine