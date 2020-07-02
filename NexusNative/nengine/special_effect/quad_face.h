#ifndef _NEXUS_QUAD_FACE_H_
#define _NEXUS_QUAD_FACE_H_
#include "ncore.h"
#include "neffect_vertex_data.h"

namespace nexus
{

	enum ESFX_FaceType
	{
		ESFX_NormalFace,		// ��ͨ��Ƭ
		ESFX_SphereBillboard,	// ���ι������
		ESFX_CylinderBillboard,	// Ͳ�ι������
	};


	struct quad_face
	{
		vector3		pos;		// λ��
		vector3		rot;		// ���ڹ���壬ֻ��x��������
		vector3		revolution;	// ��ת
		vector2		half_size;	// ��ǰ"�뾶"
		color4f		color;		//��ǰɫ��
		
		vector2		uv_start;	//��������
		vector2		uv_end;

		quad_face(void)
		{
			memset(this, 0, sizeof(*this));
		}

		static void build_mesh_vertices(neffect_vertex_data& vb, const std::vector<quad_face>& faces, const ncamera& cam, const matrix44& world_mat, ESFX_FaceType type,int face_count=-1);
	};

	typedef std::vector<quad_face> st_quad_array;
}//namespace nexus

#endif //_NEXUS_QUAD_FACE_H_