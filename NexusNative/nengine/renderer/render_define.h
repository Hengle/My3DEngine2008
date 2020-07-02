/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _RENDER_DEFINE_H_
#define _RENDER_DEFINE_H_

namespace nexus
{
	typedef unsigned int hit_id;

	enum ERenderMode
	{
		ERM_Wireframe,
		ERM_Unlit,
		ERM_Lit,
		ERM_LightingOnly,
	};


	// ���ײ�Ļ��Ʋ���ö�ٳ����������ʰ�Tech Usageʹ��
	enum EDrawingPolicy
	{
		EDP_PreDepth	,	// ���Ƴ�Z����
		EDP_BaseColor	,	// ���Ƴ��Է��⣬LightMap��̬״̬���Լ��ǹ���ģʽ����
		EDP_Unlit			,	// �ǹ���ģʽ��Ⱦ
		EDP_Wireframe	,	// �����߿�ģʽ
		EDP_Point_Light				,	//
		EDP_Spot_Light					,	//
		EDP_Directional_Light		,	//
		EDP_Hit_Proxy,
		EDP_Num	
	};

	enum EDepthGroup
	{
		EDG_Background = 0,	// ������
		EDG_WorldGeometry,	// ���糡��Mesh�飬ʹ�ü򵥵�͸��������Ⱦ����
		EDG_WorldActor,		// ��������飬ʹ�ø��ӵ�͸��������Ⱦ����
		EDG_Foreground,		// ǰ����
		EDG_Max,
	};

	
	enum EBlendType
	{
		EOpaque = 0,	// ��ȫ��͸��	
		ETranslucent,	//��ȫ͸��,alpha���
		EAdditive,			//��ӻ��
		EMax
	};

	struct render_config
	{
		void*	window_handle;
		size_t	width,
				height;
		size_t	color_bits;		
		bool	bWindowed;
		bool	bEnableHDR;
		bool bEnableSSAO;
	};

	enum EPrimitiveType
	{
		EDraw_None = 0,
		EDraw_PointList,
		EDraw_LineList,
		EDraw_LineStrip,
		EDraw_TriangleList,
		EDraw_TriangleStrip,
		EDraw_TriangleFan,
	};

	struct shader_define
	{
		std::string	name;
		std::string	value;

		shader_define(void)	{}
		shader_define(const std::string& _name, const std::string& _value):name(_name),value(_value)
		{}
	};
	typedef std::vector<shader_define> shader_macro_array;

	/**
	*	just a interface holder
	*/
	struct nshader_modifier
	{
	public:
		nshader_modifier()
		{
			m_macro_array.reserve(8);
			m_name.reserve(32);
		}

		void clear()
		{
			m_macro_array.clear();
			m_name.clear();
		}

		void add_macro(const shader_define& marco)
		{
			m_macro_array.push_back(marco);
			m_name.append(conv_string(marco.name));
		}

		const std::vector<shader_define>& get_macro_array() const
		{
			return m_macro_array;
		}

		const nstring& get_name() const
		{
			return m_name;
		}

		const nshader_modifier& operator += (const nshader_modifier& v)
		{
			if (!v.get_name().empty())
			{
				m_macro_array.insert(m_macro_array.end(),v.get_macro_array().begin(),v.get_macro_array().end());
				m_name += v.get_name();
			}
			
			return *this;
		}

	protected:
		std::vector<shader_define> m_macro_array;
		nstring m_name;
	};

	template<>
	inline narchive& nserialize(narchive& ar, shader_define& d, const TCHAR* obj_name)
	{
		nstring class_name(_T("shader_define"));
		ar.object_begin(obj_name, class_name);
		nserialize(ar, d.name, _T("name"));
		nserialize(ar, d.value, _T("value"));		
		ar.object_end();
		return ar;
	}

}//namespace nexus

#endif //_RENDER_DEFINE_H_