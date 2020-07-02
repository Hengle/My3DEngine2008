/**
*	nexus core - math
*
*	Author: Neil
*	Date:	Jan, 2008
*/

#ifndef _NEXUS_MATRIX_H_
#define _NEXUS_MATRIX_H_
#include "../base_define.h"
#include "vector.h"

#if _MSC_VER >= 1200
#pragma warning(push)
#endif
#pragma warning(disable:4201) // anonymous unions warning

namespace nexus
{
	/** matrix 3x3 for rotation */
	struct nCoreAPI matrix33
	{
		union 
		{
			struct 
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;		
			};
			float m[3][3];
		};

		matrix33(void)	
		{
			memset(this, 0, sizeof(*this)); 
		}
		matrix33(float f11, float f12, float f13,
			float f21, float f22, float f23,
			float f31, float f32, float f33):
			_11(f11), _12(f12), _13(f13),
			_21(f21), _22(f22), _23(f23),
			_31(f31), _32(f32), _33(f33)
		{}
	};

	/** matrix 4x4 */
	#pragma pack( push, 16)
	struct nCoreAPI matrix44
	{	
		union 
		{
			struct 
			{
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;

			};
			float m[4][4];
		};

		static const matrix44 identity;

		matrix44(void)	
		{
			memset(this, 0, sizeof(*this)); 
		}
		matrix44(float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24,
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44):
			_11(f11), _12(f12), _13(f13), _14(f14),
			_21(f21), _22(f22), _23(f23), _24(f24),
			_31(f31), _32(f32), _33(f33), _34(f34),
			_41(f41), _42(f42), _43(f43), _44(f44)
		{}

		explicit matrix44(const matrix33& mat)
		{
			int SIZE1 = 4;
			int SIZE2 = 3;
			for (int r = 0; r < SIZE1; ++r) 
			{
				for (int c = 0; c < SIZE1; ++c) 
				{
					if (r < SIZE2 && c < SIZE2)
						m[r][c] = mat.m[r][c]; 
					else 
						m[r][c] = r == c ? 1.f : 0.f; 
				}
			}
		}

		vector3 get_axis_x() const
		{
			return vector3(_11, _12, _13);
		}
		vector3 get_axis_y() const
		{
			return vector3(_21, _22, _23);
		}
		vector3 get_axis_z() const
		{
			return vector3(_31, _32, _33);
		}
		vector3 get_origin() const
		{
			return vector3(_41, _42, _43);
		}

		void set_axis_x( const vector3& x )
		{
			_11 = x.x;
			_12 = x.y;
			_13 = x.z;
		}

		void set_axis_y( const vector3& y )
		{
			_21 = y.x;
			_22 = y.y;
			_23 = y.z;
		}

		void set_axis_z( const vector3& z )
		{
			_31 = z.x;
			_32 = z.y;
			_33 = z.z;
		}

		inline void set_origin( const vector3& origin )
		{
			_41 = origin.x;
			_42 = origin.y;
			_43 = origin.z;
		}

	};
	
	struct _matrix44
	{
		float m[4][4];
	};

	#pragma pack( pop )

	/** ��һ��������Ϊ��λ���� */
	nCoreAPI void mat_set_identity(matrix44& mat);

	/** ������� */
	nCoreAPI matrix44 operator + (const matrix44& lv, const matrix44& rv);

	/** ������� */
	nCoreAPI matrix44 operator - (const matrix44& lv, const matrix44& rv);

	/** �������ϵ�� */
	nCoreAPI matrix44 operator * (const matrix44& lv, float s);

	/** ����������Ϊһ������ϵLook At���� */
	nCoreAPI void mat_set_lookat_LH(matrix44& mat, const vector3& eye, const vector3& at, const vector3& up);

	/** ����������Ϊһ������ϵ͸�Ӿ���*/
	nCoreAPI void mat_set_perspective_LH(matrix44& mat, float FOV, float aspect, float zNear, float zFar);

	nCoreAPI void mat_set_ortho_LH(matrix44& mat, float w, float h, float zNear, float zFar);

	/** ����������Ϊһ��ƽ�ƾ���*/	
	nCoreAPI void mat_set_translation(matrix44& mat, float tx, float ty, float tz);
	inline void mat_set_translation(matrix44& mat, const vector3& offset)
	{
		mat_set_translation(mat, offset.x, offset.y, offset.z);
	}

	/** ����������Ϊһ�����ž���*/
	nCoreAPI void mat_set_scaling(matrix44& mat, float sx, float sy, float sz);

	/** ��һ����������Ϊ��X����ת�ľ���*/
	nCoreAPI void mat_set_rotation_X(matrix44& mat, float angle);

	/** ��һ����������Ϊ��Y����ת�ľ���*/
	nCoreAPI void mat_set_rotation_Y(matrix44& mat, float angle);

	/** ��һ����������Ϊ��Z����ת�ľ���*/
	nCoreAPI void mat_set_rotation_Z(matrix44& mat, float angle);

	/** ��һ����������Ϊ��ת����*/
	nCoreAPI void mat_set_rotation_yaw_pitch_roll(matrix44& mat, float yaw, float pitch, float roll);

	/** ��ָ���ġ���λ������Ϊ�ᣬ��תһ���Ƕ�*/
	nCoreAPI void mat_set_rotation_axis(matrix44& mat, float angle, const vector3& axis);

	nCoreAPI void mat_set_rotation_axis(matrix44& mat, const vector3& u, const vector3& v);

	/** �����������*/
	nCoreAPI matrix44 operator * (const matrix44& lv, const matrix44& rv);

	/** Ϊһ���������ƽ�Ʋ���*/	
	nCoreAPI void mat_translate(matrix44& mat, float tx, float ty, float tz);
	inline void mat_translate(matrix44& mat, const vector3& t)
	{
		mat_translate(mat, t.x, t.y, t.z);
	}

	/** Ϊһ������������Ų���*/
	nCoreAPI void mat_scale(matrix44& mat, float sx, float sy, float sz);

	/** Ϊһ���������X����ת����*/
	nCoreAPI void mat_rotate_X(matrix44& mat, float angle);

	/** Ϊһ���������Y����ת����*/
	nCoreAPI void mat_rotate_Y(matrix44& mat, float angle);

	/** Ϊһ���������Z����ת����*/
	nCoreAPI void mat_rotate_Z(matrix44& mat, float angle);

	/** ����ת�� */
	nCoreAPI matrix44 mat_transpose(const matrix44& mat);
	/** �������� */
	nCoreAPI matrix44 mat_inverse(const matrix44& mat);

	//-- transform vector
	nCoreAPI vector4 mat_transform(const vector4& v, const matrix44& mat);

	// ������
	inline vector3 operator * (const vector3& v, const matrix44& mat)
	{
		return vector3(v.x*mat._11+v.y*mat._21+v.z*mat._31+mat._41,
			v.x*mat._12+v.y*mat._22+v.z*mat._32+mat._42,
			v.x*mat._13+v.y*mat._23+v.z*mat._33+mat._43);
	}

	inline vector4 operator * (const vector4& v, const matrix44& mat)
	{
		return vector4(v.x*mat._11+v.y*mat._21+v.z*mat._31+mat._41,
			v.x*mat._12+v.y*mat._22+v.z*mat._32+mat._42,
			v.x*mat._13+v.y*mat._23+v.z*mat._33+mat._43,
			v.x*mat._14+v.y*mat._24+v.z*mat._34+mat._44);
	}

	//-- transform normal
	/*inline vector3 mat_transform_normal(const vector3& normal, const matrix44& mat)
	{
	matrix44 matIT = mat_inverse(mat);
	matIT = mat_transpose(matIT);

	return normal*matIT;
	}*/


	template<> struct IS_POD<matrix33>
	{ 
		static const bool value = true; 
	};

	template<> struct IS_POD<matrix44>
	{ 
		static const bool value = true; 
	};

}//namespace nexus

#if _MSC_VER >= 1200
#pragma warning(pop)
#else
#pragma warning(default:4201)
#endif

#endif //_NEXUS_MATRIX_H_