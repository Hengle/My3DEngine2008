#pragma once

namespace NexusEngine
{
	/*
	 *	Editor Command Manager��Ƹ�Ҫ
	 *	.�ؼ�����native�����managed���붼Ҫ����command stack������command stack������CLI�����������
	 *	.native ������Ҫ����CLI��Ĵ��룬���漰��native�������managed����Ĺ��̣���ͨ��native��editor engine��������ʵ�֣�
	*/

	/**
	 *	��Ϊ��Ҫ����managed��native����command������������Ҫһ�������
	*/
	public interface class NEditorCommandBase
	{
	public:
		bool Execute();
		bool Undo();
		bool Redo();
		property System::String^ Name// ����һ����̵�����
		{
			System::String^ get();
		}
		property System::String^ Desc	// ����һ���Գ�������
		{			
			System::String^ get();			
		}
	};


	/**
	 *	��װnative��command�����
	*/
	public ref class NNativeEditorCommand
		: public NEditorCommandBase
	{
		neditor_cmd::ptr*	m_nativeCmd;
	public:
		NNativeEditorCommand(neditor_cmd::ptr nativeCmd);
		~NNativeEditorCommand();

		virtual bool Execute();
		virtual bool Undo();
		virtual bool Redo();
		property System::String^ Name
		{
			virtual System::String^ get()
			{
				if(m_nativeCmd)
				{
					nstring nstr = (*m_nativeCmd)->get_name();
					return gcnew System::String(nstr.c_str());
				}

				return gcnew System::String("Unknown Command");
			}
		}

		property System::String^ Desc
		{
			virtual System::String^ get()
			{
				if(m_nativeCmd)
				{
					nstring nstr = (*m_nativeCmd)->get_desc();
					return gcnew System::String(nstr.c_str());
				}

				return gcnew System::String("Unknown Command");
			}
		}
	};

	/**
	 *	command������������undo��redo����command stack
	*/
	public ref class NEditorCommandManager
	{
	public:
		System::EventHandler^ UndoStackChanged;
		System::EventHandler^ RedoStackChanged;

	public:
		NEditorCommandManager(void);
		virtual ~NEditorCommandManager(void);

		property int StackSize
		{
			int get()	{	return m_stackSize;}
			void set(int val)	{	m_stackSize = val;}
		}

		void PushCommand(NEditorCommandBase^ cmd)
		{
			AppendCommand(cmd, m_undoStack);
			if(UndoStackChanged)
				UndoStackChanged(this, nullptr);
		}

		bool Undo()
		{
			bool ret = false;
			if (m_undoStack->Count > 0)
			{
				int lastIndex = m_undoStack->Count-1;
				NEditorCommandBase^ lastCmd =  safe_cast<NEditorCommandBase^>(m_undoStack[lastIndex]);
				ret = lastCmd->Undo();

				//-- �����command��undoջ�ƶ���redoջ				
				m_undoStack->RemoveAt(lastIndex);
				if(UndoStackChanged)
					UndoStackChanged(this, nullptr);
				if( ret)
				{
					AppendCommand(lastCmd, m_redoStack);
					if(RedoStackChanged)
						RedoStackChanged(this, nullptr);
				}
			}
			return ret;
		}

		bool Redo()
		{
			bool ret = false;
			if (m_redoStack->Count > 0)
			{
				int lastIndex = m_redoStack->Count-1;
				NEditorCommandBase^ lastCmd = safe_cast<NEditorCommandBase^>(m_redoStack[lastIndex]);
				ret = lastCmd->Redo();

				//-- �����command��undoջ�ƶ���redoջ
				m_redoStack->RemoveAt(lastIndex);
				if(RedoStackChanged)
					RedoStackChanged(this, nullptr);
				if( ret)
				{
					AppendCommand(lastCmd, m_undoStack);
					if(UndoStackChanged)
						UndoStackChanged(this, nullptr);
				}
			}
			return ret;
		}

		property NEditorCommandBase^ UndoStackTop
		{
			NEditorCommandBase^ get()	
			{
				if(m_undoStack->Count > 0)
					return  safe_cast<NEditorCommandBase^>(m_undoStack[m_undoStack->Count-1]);
				return nullptr;
			}
		}

		property NEditorCommandBase^ RedoStackTop
		{
			NEditorCommandBase^ get()	
			{
				if(m_redoStack->Count > 0)
					return  safe_cast<NEditorCommandBase^>(m_redoStack[m_redoStack->Count-1]);
				return nullptr;
			}
		}

	protected:
		void AppendCommand(NEditorCommandBase^ cmd, System::Collections::ArrayList^ cmdStack)
		{
			// ���µ�command��ӵ�����
			cmdStack->Add(cmd);
			if(cmdStack->Count > m_stackSize)
			{
				// �Ƴ���ǰ���һ��
				cmdStack->RemoveAt(0);
			}
		}

	protected:
		int	m_stackSize;	// ��ౣ����ٸ�command�ڵ���stack��
		System::Collections::ArrayList^ 	m_undoStack;
		System::Collections::ArrayList^ 	m_redoStack;
	};
}//namespace NexusEngine