#include "StdAfx.h"
#include "ThumbnailCreator.h"
#include "ImageThumbnailCreator.h"
#include "MeshThumbnailCreator.h"
#include "NWrapperException.h"

using namespace System;
using namespace System::Collections;
using namespace System::Collections::Generic;

namespace NexusEngine
{
	void ThumbnailCreatorManager::Init()
	{
		//-- create containers
		m_creatorList = gcnew List<ThumbnailCreator^>();
		m_fileExtDict = gcnew Dictionary<String^, ThumbnailCreator^>();

		//--
		ImageThumbnailCreator^ imgThumb = gcnew ImageThumbnailCreator();
		MeshThumbnailCreator^ meshThumb = gcnew MeshThumbnailCreator();
		AddThumbnailCreator(imgThumb);
		AddThumbnailCreator(meshThumb);

		m_fallback = imgThumb->CreateThumbnail(
			NResourceLoc(_T("engine_data"),_T("editor_res/nlogo.bmp")),
			256, 256
			);
		m_error = imgThumb->CreateThumbnail(
			NResourceLoc(_T("engine_data"),_T("editor_res/FileError.png")),
			256, 256
			);
		m_fileTypeXML = imgThumb->CreateThumbnail(
			NResourceLoc(_T("engine_data"),_T("editor_res/FileType_XML.png")),
			256, 256
			);

		m_fileTypeAnimSet = imgThumb->CreateThumbnail(
			NResourceLoc(_T("engine_data"),_T("editor_res/animset.bmp")),
			256, 256
			);

		m_imgCreator = imgThumb;
	}

	void ThumbnailCreatorManager::AddThumbnailCreator(ThumbnailCreator^ thumb)
	{
		thumb->Init();
		m_creatorList->Add(thumb);
		
		ArrayList^ fileExtList = thumb->AssociatedFileExtension;
		for (int i=0; i<fileExtList->Count; i++)
		{
			String^ fileExt = (String^)fileExtList[i];
			m_fileExtDict->Add(fileExt, thumb);
		}
	}

	void ThumbnailCreatorManager::Close()
	{
		if( m_fallback )
		{
			::DeleteObject(m_fallback);
			m_fallback = NULL;
		}

		for (int i=0; i<m_creatorList->Count; i++)
		{
			ThumbnailCreator^ thumb = m_creatorList[i];
			thumb->Close();
		}
		m_creatorList->Clear();
		m_fileExtDict->Clear();
	}

	HBITMAP ThumbnailCreatorManager::LoadTextureAsBitmap(NResourceLoc loc)
	{
		return m_imgCreator->LoadTextureAsBitmap(loc);
	}

	HBITMAP ThumbnailCreatorManager::CreateThumbnail(NResourceLoc loc, int w, int h)
	{
		HBITMAP ret = NULL;

		String^ ext = loc.FileExtension;

		//-- �̶���һЩ�ļ����ͣ�ֱ�ӷ����Ѽ��ص�ͼƬ
		if( ext == _T("xml") )
		{
			ret = m_fileTypeXML;
		}
		else if( ext == _T("animset") )
		{
			ret =  m_fileTypeAnimSet;
		}
		else
		{
			//-- ������Ӧ���ഴ������ͼ
			if( m_fileExtDict->ContainsKey(loc.FileExtension) )
			{
				try
				{
					ThumbnailCreator^ thumb = m_fileExtDict[loc.FileExtension];
					ret = thumb->CreateThumbnail(loc, w, h);
				}
				catch(Exception ^ex)
				{
					(void)ex;
					resource_location nloc;
					loc.ToNative(nloc);

					nLog_Error(_T("Create Thumbnail Failed, file=%s\r\n"), nloc.to_string().c_str());

					ret = m_error;
				}
			}
			else
			{
				// û���ҵ�ָ�����ͺ�׺�ģ���ʹ��Ĭ��ͼ��
				ret = m_fallback;
			}
		}
		
		if( ret == NULL )
		{			
			// return error image
			return m_error;
		}
		else
		{			
			return ret;
		}
	}
}//namespace NexusEngine