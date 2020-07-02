/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	April, 2010
*/
#ifndef _NEXUS_TERRAIN_MTL_SETUP_H_
#define _NEXUS_TERRAIN_MTL_SETUP_H_
#include "ncore.h"
#include "../resource/nresource.h"
#include "nterrain_mtl.h"

namespace nexus
{
	class nterrain_actor;

	const int TERRAIN_MTL_MAX_LAYER = 16;

	/**
	 *	���β������á��༭���ݹ���
	 *	@remarks ����һ����������ֲ��material�ֿ������
	*/
	struct layer_data_editor
	{
		// ��ͨ���༭���༭������
		vector2					uv_scale;
		float					uv_rotate;		
		float					spec;
		float					spec_power;
		nresource_texture::ptr	diffuse_tex;
		nresource_texture::ptr	normal_tex;

		layer_data_editor()
			: uv_scale(vector2(1,1)),
			uv_rotate(0),
			spec(0),
			spec_power(32)
		{
		}


		nstring get_name() const	// ʹ��diffuse��ͼ��location��Ϊ����
		{
			if( diffuse_tex )
				return diffuse_tex->get_location().to_string();
			else
				return nstring(_T("EmptyLayer"));
		}
	};

	struct layer_data : public layer_data_editor
	{
		nalpha_map::ptr		alpha_map;			

		layer_data()
			:layer_data_editor()
		{}

		void copy_from(const layer_data_editor& other)
		{
			this->uv_scale		= other.uv_scale;
			this->uv_rotate		= other.uv_rotate;
			this->spec			= other.spec;
			this->spec_power	= other.spec_power;
			this->diffuse_tex	= other.diffuse_tex;
			this->normal_tex	= other.normal_tex;
		}
	};

	class nAPI nterrain_mtl_setup :	public nobject
	{
	public:
		nterrain_mtl_setup(void);
		virtual ~nterrain_mtl_setup(void);

		//!	����ڲ����ݣ����󶨸�����
		void reset(nterrain_actor* owner);		

		//!	���������û�����ͼ��������
		void create(const resource_location& basic_texture_loc, int alpha_map_w, int aplha_map_h);
				
		//! �ܹ��ж��ٸ�ͼ��
		size_t get_num_layers() const	{	return m_layers.size();}

		//! ȡ��ͼ�����alpha map�����������
		const layer_data& get_layer_data(size_t i);
		void set_layer_data(size_t i,const layer_data_editor& ld );

		//! ��������ȡ��ͼ�������
		nstring get_layer_name(size_t i) const;

		//! ����ͼ�������ȡ��ͼ���index
		int get_layer_index(const nstring& layer_name) const;

		//! ����ͼ������ȡ�ö�Ӧ��alpha map
		nalpha_map::ptr get_layer_alpha(const nstring& layer_name) const;

		//! ����ͼ��indexȡ�ö�Ӧ��alpha map
		nalpha_map::ptr get_layer_alpha(size_t i) const;

		/** ����һ��ͼ��
		 *	@return ��������ͼ���index��-1��ʾ���ʧ��
		*/
		int add_layer(const layer_data_editor& ld);

		//! ����һ��ͼ��Ĳ�������
		void update_layer();

		//! ɾ��һ��ͼ��
		void remove_layer(size_t i);

		//! ��һ��ͼ�������ƶ�
		void shift_up_layer(size_t i);

		//! ��һ��ͼ�������ƶ�
		void shift_down_layer(size_t i);

		//! ����ͼ���alpha map
		void import_layer_alpha(const nstring& layer_name, const nstring& img_file_name);

		//! ��ͼ���ָ��������������alphaֵ
		void generate_noise(const nstring& layer_name, nrect rc, int numOctaves, float amplitude, float frequency);

		//! �༭��֪ͨ����ͼ���alpha map
		void post_layer_alpha_change(const nstring& layer_name, nrect rc);

		//!	����chunk��Ӧ��rect�Ƿ���ĳ��ͼ����أ��������������ص�alpha���ڷ�ֵ��
		bool calulate_alpha_relative(size_t layer_index, const nrect& rc);
		
		//! �����ڲ����ݴ����������β��ʶ���
		nterrain_mtl_basic::ptr setup_render_mtl_low();

		//! �����ڲ����ݴ����ֲ���β��ʶ���
		ntexture_splatting::ptr setup_render_mtl_high(int chunk_size);

		//! ���л�������ֵ���ݴ����ĵ���ÿ��alpha map�������һ��raw�ļ�
		virtual void serialize(narchive& ar);
	private:
		//! ����ͼ�����Ʋ���
		int find_layer_by_name(const nstring& layer_name) const;		

		//! ����alpha map������ȡʱ��Ӧ���ļ���
		nstring get_serialize_alpha_map_name(narchive& ar, int layer_index) const;

	private:
		typedef std::vector<layer_data> st_layer_array;
		
		int				m_alpha_w,	// alpha map�ĳߴ�
						m_alpha_h;
		st_layer_array	m_layers;	// ͼ������

		resource_location	m_basic_texture;	// ����ģʽ����ʹ�õ���ͼ
		
		nterrain_actor*	m_owner;
	};
}//namespace nexus
#endif //_NEXUS_TERRAIN_MTL_SETUP_H_