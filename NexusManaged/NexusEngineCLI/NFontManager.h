#pragma once
#include "NRenderFont.h"
#include "FontDescription.h"

namespace NexusEngine
{

	public ref class NFontManager sealed
	{
	//-- ���嵥�����ʽӿ�
	private:
		static NFontManager^ s_instance;
		NFontManager(void);

	public:
		static property NFontManager^ Instance
		{
			NFontManager^ get()
			{
				return s_instance;
			}
		}

		static NFontManager(void)
		{
			s_instance = gcnew NFontManager;
		}

	public:
		virtual ~NFontManager(void);

		/// <summary> ����ָ�������壬����Ѿ������򷵻ش��������� </summary>
		NRenderFont^ ConstructFont(FontDescription description);

		/// <summary> ����������û�д��� </summary>
		bool ContainsFont(FontDescription description);

		/// <summary> �Ƴ��ƶ������� </summary>
		bool RemoveFont(FontDescription description);

		///
		void ClearFont();
		
	private:
		//-- �����Ѿ�������Font
		System::Collections::Generic::Dictionary<FontDescription,NRenderFont^>		m_fontCache;
	};

} // end of namespace NexusEngine

