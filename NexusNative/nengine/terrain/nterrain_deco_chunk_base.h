#ifndef _NEXUS_TERRAIN_DECO_CHUNK_BASE_H_
#define _NEXUS_TERRAIN_DECO_CHUNK_BASE_H_
#include "ncore.h"
#include "../resource/nresource.h"
#include "../../ncore/height_map/nheight_map.h"
#include "../Actor/nmesh_component.h"
#include <set>

class Perlin;
namespace nexus
{
	class nterrain_actor;
	class nterrain_chunk;
	class nterrain_deco_layer;
	class nterrain_deco_chunk_base;
	typedef std::set<nterrain_chunk*> chunk_list;

	enum nAPI deco_layer_type
	{
		deco_layer_static_mesh=0,	//��̬ģ�Ͳ�
		deco_layer_grass,			//��Ҷ��
	};

	//-----------------------------------------------------------------------------------
	struct nAPI terrain_deco_object
	{
		typedef boost::shared_ptr<terrain_deco_object>	ptr;

		terrain_deco_object():m_pos(0,0,0),m_rotation(0,0,0),m_scaling(1.0f,1.0f,1.0f)
		{}

		void	to_matrix(matrix44& matrix)
		{
			mat_set_scaling(matrix,m_scaling.x,m_scaling.y,m_scaling.z);
			mat_rotate_Z(matrix, m_rotation.z);
			mat_rotate_X(matrix, m_rotation.x);
			mat_rotate_Y(matrix, m_rotation.y);
			mat_translate(matrix,m_pos);
		}

		//ʵ�������ķֲ�ͼ����
		int		m_X,m_Y;
		//ʵ����λ��
		vector3	m_pos;
		//ʵ������ת
		vector3 m_rotation;
		//ʵ��������
		vector3 m_scaling;	
	};

	//-----------------------------------------------------------------------------------
	class nAPI nterrain_deco_chunk_base
	{
		friend class nterrain_deco_layer;
	public:
		typedef boost::shared_ptr<nterrain_deco_chunk_base>	ptr;

		//����chunk
		void create(float x, float y, float width,float height);

		//�ֲ�ͼ�����ı�ʱ�Ĵ���
		virtual bool	on_density_map_changed()=0;
		virtual bool	on_density_map_changed(nterrain_deco_layer* layer,const nrect& region)=0;

		//��ָ�����괦���һ���µ�ʵ�����������ֲ�ͼ���꣬����ռ�����
		virtual terrain_deco_object::ptr	add_instance(int x,int z,float wx,float wz)=0;

		//����Ҫ��Ⱦ��mesh-element�����б�
		virtual void get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp)=0;

		//�豸��ʧʱ�Ĵ���
		virtual void on_device_lost()	{}
		//�豸����ʱ�Ĵ���
		virtual void on_device_reset()	{}
		//-----------------------------------------------------------------------------------
		nterrain_deco_layer*		get_owner()	{return m_owner;}
	protected:
		nterrain_deco_layer*		m_owner;
		nterrain_chunk*				m_terrain_chunk;
		//chunk��λ�úʹ�С����Ϊ�ֲ�ͼ�ǿɱ��С�ģ����������õ�����Դ�С
		float		m_x,m_y;
		float		m_width,m_height;
		nrect		m_region;
		//-----------------------------------------------------------------------------------
		vector<terrain_deco_object::ptr>	m_object_list;
		typedef vector<terrain_deco_object::ptr>::iterator OI;
	public:
		nterrain_deco_chunk_base(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk);
		virtual ~nterrain_deco_chunk_base();
	};

	//-----------------------------------------------------------------------------------
	typedef nheight_map<terrain_deco_object*>	ndeco_map;

	class nAPI nterrain_deco_layer:public nobject
	{
	public:
		typedef boost::shared_ptr<nterrain_deco_layer> ptr;

		//��ʼ��
		virtual void initialize(nterrain_actor* terrain);

		//����һ��deco chunk
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height);

		//��ȡ�����ò������
		const	nstring&	get_name()			{return m_name;}
		virtual void	set_name(const nstring& value)	{m_name=value;}

		//��ȡ�����õ�ǰ���ģ����Դ����������˲�ͬ����Դ�����������еĶ���ģ��
		const	resource_location&	get_resource_loc()	{return m_resource;}
		virtual void	set_resource_loc(const resource_location& loc)=0;

		//��ȡ������Ĭ�ϵ������תֵ����ֵ������󴴽�ʱ��Y��������ת
		float	get_random_offset()	{return m_random_offset;}
		virtual void	set_random_offset(float value)	{m_random_offset=value;rebuild();}

		//��ȡ�����ö��󴴽�ʱ�ڸ߶��ϵ�λ��
		float	get_height_offset()	{return m_height_offset;}
		virtual void	set_height_offset(float value)	{m_height_offset=value;rebuild();}

		//��ȡ������Ĭ�ϵ������תֵ����ֵ������󴴽�ʱ��Y��������ת
		float	get_random_rotation()	{return m_random_rotation;}
		virtual void	set_random_rotation(float value)	{m_random_rotation=value;rebuild();}

		//��ȡ������������ŵ����ֵ����Сֵ
		float	get_max_random_scale()	{return m_max_random_scale;}
		virtual void	set_max_random_scale(float value)	{m_max_random_scale=value;rebuild();}

		float	get_min_random_scale()	{return m_min_random_scale;}
		virtual void	set_min_random_scale(float value)	{m_min_random_scale=value;rebuild();}

		//��ȡ�����ñ�ˢ���ܶ�
		float	get_density()	{return m_density;}
		virtual void	set_density(float value);

		//��ȡ�ֲ�ͼ
		nalpha_map::ptr	get_density_map()	{return m_density_map;}

		//��ȡ�����С
		size_t	get_grid_size()	{return m_grid_size;}
		void	set_grid_size(size_t value);

		//��ȡ���������������
		int		get_random_seed()	{return m_random_seed;}
		void	set_random_seed(int value);

		//��ȡ���ζ���
		nterrain_actor*	get_terrain_actor()	{return m_terrain;}

		//-----------------------------------------------------------------------------------
		//Ũ��ͼ��������
		virtual void	on_update_density_map(const nrect& region,chunk_list& chunks);

		//�ؽ�������
		virtual void	rebuild();

		//����
		virtual void	do_test();

		//���л�
		virtual void	serialize(narchive& ar);

	protected:
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_deco_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height)=0;
	protected:
		//�������
		nstring		m_name;

		//�ò�ʹ�õ���Դ,�Ծ�̬ģ�Ͳ���˵��ģ���ļ�·�������ڲ�Ҷ��˵������·��
		resource_location	m_resource;

		//Ĭ�ϵ������תֵ����,���ȡֵΪ30��������-30~30��֮����ת��ע�⣬����ʹ�õ��Ƕȣ����ǻ��ȡ�
		float	m_random_rotation;

		//Ĭ�ϵ����ƫ��ֵ,���ȡ100������xyz���궼��-100~100�ķ�Χ�����ƫ��
		float	m_random_offset;

		//�߶��ϵ�λ��
		float	m_height_offset;

		//Ĭ�ϵ��������ֵ
		float	m_min_random_scale;
		float	m_max_random_scale;

		//��ˢ���ܶ�
		float	m_density;

		//�ֲ�ͼ�ĳߴ�
		size_t	m_grid_size;

		//���������
		int		m_random_seed;

		//�ֲ�ͼ
		nalpha_map::ptr		m_density_map;

		//��������
		nterrain_actor*		m_terrain;

		//��ǰ��ӵ�е�chunk�б�
		vector<boost::shared_ptr<nterrain_deco_chunk_base> > m_chunks;

	public:
		nDECLARE_VIRTUAL_CLASS(nterrain_deco_layer)
		nterrain_deco_layer();
		virtual ~nterrain_deco_layer();
	};

	
};

#endif