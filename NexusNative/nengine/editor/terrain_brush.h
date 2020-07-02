/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_TERRAIN_BRUSH_H_
#define _NEXUS_TERRAIN_BRUSH_H_
#include "../terrain/nheight_map.h"

namespace nexus
{
	/**	�����ˢ��ֵ
	 *	@return ���x==0�򷵻�a
	*/
	template<typename T>
	T trn_brush_lerp(T a,T b,float x, ETrnLerpMode type)
	{
		switch(type)
		{
		case ELM_None:
			return x<0.5f ? a : b;
		case ELM_Cosine:
			x=(1-cos(x*3.1415927f))*0.5f;
			break;
		case ELM_Quadratic:
			x=x*x;
			break;
		case ELM_Linear:
		default:
			break;
		}

		return (T)(a*(1-x)+b*x);
	}

	//-----------------------------------------------------------------------------------------------
	/** �����ˢ�����򣬴����ֵ������������operator�������*/
	template<typename HT, typename OP>
	nrect do_terrain_brush(nheight_map<HT>* hmap, npoint cursor_pt, const nterrain_brush& brush, OP& op)
	{
		nrect region(0,0,0,0);
		if( hmap == NULL )
			return region;

		switch( brush.shape )
		{
		case EBS_Circle:
			{
				nrect outer_rect;

				outer_rect.left		= cursor_pt.x-brush.outer_radius;
				outer_rect.right	= cursor_pt.x+brush.outer_radius;
				outer_rect.top		= cursor_pt.y-brush.outer_radius;
				outer_rect.bottom	= cursor_pt.y+brush.outer_radius;
				_clip_rect(outer_rect, hmap->get_width(), hmap->get_height());
				
				region = outer_rect;


				int x, z;
				for(z=outer_rect.top; z<outer_rect.bottom; z++)
				{
					for(x=outer_rect.left; x<outer_rect.right; x++)
					{
						int dx = x-cursor_pt.x;
						int dz = z-cursor_pt.y;
						float dist = sqrtf(float(dx*dx + dz*dz));

						if(dist > brush.outer_radius)
							continue;
						if(dist < brush.inner_radius)
						{
							op.do_inner(hmap, x, z, brush.strength);
						}
						else
						{
							// �뿪�ھ�ԽԶrֵԽ��,���ھ��Ϸ���0
							float r = (dist-brush.inner_radius)/(brush.outer_radius-brush.inner_radius);
							op.do_outer(hmap, x, z, brush.strength, r, brush.lerp_mode);

						}
					}//endof for(x)
				}//endof for(z);
			}
			break;
		case EBS_Square:
			{
				nrect inner_rect, outer_rect;

				inner_rect.left		= cursor_pt.x-brush.inner_radius;
				inner_rect.right	= cursor_pt.x+brush.inner_radius;
				inner_rect.top		= cursor_pt.y-brush.inner_radius;
				inner_rect.bottom	= cursor_pt.y+brush.inner_radius;
				_clip_rect(inner_rect, hmap->get_width(), hmap->get_height());

				outer_rect.left		= cursor_pt.x-brush.outer_radius;
				outer_rect.right	= cursor_pt.x+brush.outer_radius;
				outer_rect.top		= cursor_pt.y-brush.outer_radius;
				outer_rect.bottom	= cursor_pt.y+brush.outer_radius;
				_clip_rect(outer_rect, hmap->get_width(), hmap->get_height());				
				region = outer_rect;

				float strip = float(brush.outer_radius-brush.inner_radius);
				int x, z;
				for(z=outer_rect.top; z<outer_rect.bottom; z++)
				{
					for(x=outer_rect.left; x<outer_rect.right; x++)
					{
						if(z >= inner_rect.top
							&& z < inner_rect.bottom
							&& x >= inner_rect.left
							&& x < inner_rect.right)
						{
							op.do_inner(hmap, x, z, brush.strength);
						}
						else
						{
							if(strip > 0)
							{
								//����Ȧ�ľ���
								int zdist = abs(z-cursor_pt.y) - brush.inner_radius;
								int xdist = abs(x-cursor_pt.x) - brush.inner_radius;
								float r = max((zdist/strip),(xdist/strip));


								op.do_outer(hmap, x, z, brush.strength, r, brush.lerp_mode);
							}
						}//endof else
					}
				}//endof for
			}
			break;
		}// end of switch

		return region;
	}

	//-----------------------------------------------------------------------------------------------
	//!	��brush��ֵ�ӵ�height map��
	template<typename T>
	class trn_brush_add
	{
	public:
		void do_inner(nheight_map<T>* hmap, int x, int z, int brush_val)
		{
			hmap->checked_add(x, z, brush_val);
		}

		void do_outer(nheight_map<T>* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			int lerped_val = (int)trn_brush_lerp((float)brush_val, 0.0f, ratio, lerp_mode);
			hmap->checked_add(x, z, lerped_val);
		}
	};

	//-----------------------------------------------------------------------------------------------
	//!	����Bursh�ھ���Χ�ڸ߶ȵ�ƽ��ֵ
	template<typename T>
	class trn_brush_average
	{
	public:
		trn_brush_average(void)
		{	
			count=0;
			sum=0;		
		}
		void do_inner(nheight_map<T>* hmap, int x, int z, int brush_val)
		{
			(void)brush_val;
			count++;
			sum += hmap->get_value(x,z);
		}
		void do_outer(nheight_map<T>* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			(void)hmap;
			(void)x;
			(void)z;
			(void)brush_val;
			(void)ratio;
			(void)lerp_mode;
		}

		int get_result() const
		{
			return sum/count;
		}
		int get_count()	const
		{
			return count;
		}

	private:
		int count;
		int sum;
	};

	//-----------------------------------------------------------------------------------------------
	//!	���ھ���Χ�ڵ�height map����Ϊָ��ֵ,���ھ����⾶֮��ʹ��ָ��ֵ��ԭ��ֵ��ֵ
	template<typename T>
	class trn_brush_apply_val
	{
	public:
		trn_brush_apply_val(int val)
		{
			m_val = val;
		}
		void do_inner(nheight_map<T>* hmap, int x, int z, int brush_val)
		{
			(void)brush_val;
			hmap->set_value(x, z, m_val);
		}
		void do_outer(nheight_map<T>* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			(void)brush_val;

			int old_val = hmap->get_value(x,z);
			int new_val = trn_brush_lerp(m_val, old_val, ratio, lerp_mode);
			hmap->set_value(x, z, new_val);
		}
	private:
		int m_val;
	};

	//-----------------------------------------------------------------------------------------------
	//!	���ھ���Χ�ڵ�height mapʹ���Ǿ�����ƽ��,�����ھ��⾶֮���heightmapʹ��ƽ��ֵ��ԭֵ���в�ֵ
	template<typename T>
	class trn_brush_smooth
	{
	private:
		int		size;
		float	pFilter[256];
		float	filterSum;

		struct sm_val
		{
			int x,z;
			int val;
		};
		vector<sm_val>	valArray;	// smooth����Ҫ������Χ�ڵ�ԭ����hmapֵ�������ȴ����������Ÿı�hmap
	private:
		int cal_smooth_val(nheight_map<T>* hmap, int x, int z)
		{
			float sum = 0;
			int hSize = size/2;
			for(int fy=0; fy<size; fy++)
			{
				for(int fx=0; fx<size; fx++)
				{
					int xp = x+fx-hSize;
					int yp = z+fy-hSize;

					if(hmap->is_valid_index(xp, yp))
					{
						float f = pFilter[fy*size+fx];
						sum += hmap->get_value(xp, yp)*f;
					}
				}
			}//endof for

			return int(sum/filterSum+0.5f);//��������
		}
	public:
		trn_brush_smooth(int filterSize)
		{
			nASSERT(filterSize > 0 && filterSize<=16);
			size = filterSize;			

			float hSize = filterSize/2.0f;
			float r = sqrtf(hSize*hSize);
			filterSum=0;

			for(int y=0; y<filterSize; y++)
			{
				int cy = y-filterSize/2;
				for(int x=0; x<filterSize; x++)
				{
					int cx = x-filterSize/2;

					float myR = sqrtf(float(cx*cx+cy*cy));//��ǰ���뿪���ĵľ���

					float val = myR/r;
					pFilter[y*filterSize+x] = val;
					filterSum += val;
				}
			}//endof for			
		}

		void do_inner(nheight_map<T>* hmap, int x, int z, int brush_val)
		{
			(void)brush_val;

			int s = cal_smooth_val(hmap, x, z);
			sm_val saveVal = {x, z, s};
			valArray.push_back(saveVal);

		}

		void do_outer(nheight_map<T>* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			(void)brush_val;

			int s = cal_smooth_val(hmap, x, z);
			int oldValue = hmap->get_value(x, z);
			int newValue = trn_brush_lerp(s, oldValue, ratio, lerp_mode);

			sm_val saveVal = {x, z, newValue};
			valArray.push_back(saveVal);
		}

		void apply(nheight_map<T>* hmap)
		{
			int valArraySize=(int)valArray.size();
			for(int i=0;i<valArraySize;i++)
			{
				const sm_val& tv=valArray[i];
				hmap->checked_set(tv.x,tv.z,tv.val);
			}
		}
	};

	extern boost::shared_ptr<Perlin> g_brush_noise;
	//-----------------------------------------------------------------------------------------------
	//! ��brush�ھ���Χ��Ӧ��noiseֵ, ���ھ����⾶֮��ʹ��noise��ԭֵ���в�ֵ
	template<typename T>
	class trn_brush_noise
	{
	private:		
		float	m_xStep,
				m_yStep;
		npoint	m_startPt;

	private:
		float get_noise(int mapX, int mapY)
		{			
			int ix = mapX - m_startPt.x;
			int iy = mapY - m_startPt.y;

			return g_brush_noise->Get(ix*m_xStep, iy*m_yStep);
		}
	public:
		trn_brush_noise(int brushW, int brushH, npoint startPt)
		{			
			m_xStep = 1.0f/brushW;
			m_yStep = 1.0f/brushH;
			m_startPt = startPt;
		}

		void do_inner(nheight_map<T>* hmap, int x, int z, int brush_val)
		{
			(void)brush_val;

			float n = get_noise(x, z);
			hmap->checked_add(x, z, int(n));
		}

		void do_outer(nheight_map<T>* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			(void)brush_val;

			float n = get_noise(x, z);

			int lerpedVal = (int)trn_brush_lerp(n, 0.0f, ratio, lerp_mode);
			hmap->checked_add(x, z, lerpedVal);			
		}
	};

}//namespace nexus

#endif// endof _NEXUS_TERRAIN_BRUSH_H_