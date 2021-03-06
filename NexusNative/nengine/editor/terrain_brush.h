/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_TERRAIN_BRUSH_H_
#define _NEXUS_TERRAIN_BRUSH_H_
#include "../../ncore/height_map/nheight_map.h"
#include "nbrush_noise.h"

namespace nexus
{
	const float TRN_BRUSH_CURSOR_ZBIAS = 0.04f;

	/**	处理笔刷插值
	 *	@return 如果x==0则返回a
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
	/** 对高度图或者位图笔刷进行计算笔刷的区域，处理插值操作，并调用operator对象操作 */
	template<typename map_type, typename operator_type>
	nrect do_brush(map_type* map, npoint cursor_pt, const nterrain_brush& brush, operator_type& op)
	{
		nrect region(0,0,0,0);
		if( map == NULL )
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
				clip_rect(outer_rect, (long)map->get_width(), (long)map->get_height());
				if(outer_rect.left>outer_rect.right)
				{
					long temp=outer_rect.left;	outer_rect.left=outer_rect.right;	outer_rect.right=temp;
				}

				if(outer_rect.top>outer_rect.bottom)
				{
					long temp=outer_rect.top;	outer_rect.top=outer_rect.bottom;	outer_rect.bottom=temp;
				}
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
							op.do_inner(map, x, z, brush.strength);
						}
						else
						{
							// 离开内径越远r值越大,在内径上返回0
							float r = 1;
							if(brush.outer_radius != brush.inner_radius)
								r = (dist-brush.inner_radius)/(brush.outer_radius-brush.inner_radius);

							op.do_outer(map, x, z, brush.strength, r, brush.lerp_mode);

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
				clip_rect(inner_rect, (long)map->get_width(), (long)map->get_height());

				outer_rect.left		= cursor_pt.x-brush.outer_radius;
				outer_rect.right	= cursor_pt.x+brush.outer_radius;
				outer_rect.top		= cursor_pt.y-brush.outer_radius;
				outer_rect.bottom	= cursor_pt.y+brush.outer_radius;
				clip_rect(outer_rect, (long)map->get_width(), (long)map->get_height());				
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
							op.do_inner(map, x, z, brush.strength);
						}
						else
						{
							if(strip > 0)
							{
								//离内圈的距离
								int zdist = abs(z-cursor_pt.y) - brush.inner_radius;
								int xdist = abs(x-cursor_pt.x) - brush.inner_radius;
								float r = max((zdist/strip),(xdist/strip));


								op.do_outer(map, x, z, brush.strength, r, brush.lerp_mode);
							}
						}//endof else
					}
				}//endof for
			}
			break;
		}// end of switch

		return region;
	}

	/** 计算笔刷的区域，处理插值操作，并调用operator对象操作*/
	template<typename HT, typename OP>
	nrect do_terrain_brush(nheight_map<HT>* hmap, npoint cursor_pt, const nterrain_brush& brush, OP& op)
	{
		return do_brush(hmap, cursor_pt, brush, op);
	}

	//-----------------------------------------------------------------------------------------------
	//!	将brush的值加到height map上
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
	//!	计算Bursh内径范围内高度的平均值
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
	//!	在内径范围内的height map设置为指定值,在内径和外径之间使用指定值和原来值插值
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
	//!	对内径范围内的height map使用虑镜进行平滑,对于内径外径之间的heightmap使用平滑值与原值进行插值
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
		vector<sm_val>	valArray;	// smooth计算要用来范围内的原来的hmap值，所以先存起来，最后才改变hmap
	private:
		float cal_smooth_val(nheight_map<T>* hmap, int x, int z)
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
			}//end of for

			return sum/filterSum;
		}
	public:
		trn_brush_smooth(int filterSize)
		{
			nASSERT(filterSize > 0 && filterSize<=16);
			size = filterSize;			

			float hSize = filterSize/2.0f;
			float r = sqrtf(hSize*hSize*2);
			filterSum=0;

			for(int y=0; y<filterSize; y++)
			{
				int cy = y-filterSize/2;
				for(int x=0; x<filterSize; x++)
				{
					int cx = x-filterSize/2;

					float myR = sqrtf(float(cx*cx+cy*cy));//当前点离开中心的距离

					float val = trn_brush_lerp(1.0f, 0.0f, myR/r, ELM_Cosine);										
					pFilter[y*filterSize+x] = val;
					filterSum += val;
				}				
			}//end of for			
		}

		void do_inner(nheight_map<T>* hmap, int x, int z, int brush_val)
		{
			(void)brush_val;

			int s = (int)cal_smooth_val(hmap, x, z);
			sm_val saveVal = {x, z, s};
			valArray.push_back(saveVal);

		}

		void do_outer(nheight_map<T>* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			(void)ratio;
			(void)lerp_mode;
			do_inner(hmap, x, z, brush_val);
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

	//-----------------------------------------------------------------------------------------------
	//! 在brush内径范围内应用noise值, 在内径和外径之间使用noise和原值进行插值
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

			return nbrush_noise::instance()->get_perlin_brush_noise()->Get(ix*m_xStep, iy*m_yStep);
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

	//-----------------------------------------------------------------------------------------------
	//!	在内外径范围内的bitmap设置为指定值
	class bitmap_brush_apply_val
	{
	public:
		void do_inner(nbit_map* bitmap, int x, int z, int brush_val)
		{
			bitmap->set_value(x, z, (brush_val>0));
		}
		void do_outer(nbit_map* bitmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			UNREFERENCED_PARAMETER(ratio);
			UNREFERENCED_PARAMETER(lerp_mode);
			bitmap->set_value(x, z, (brush_val>0));
		}
	};

	//! 在外径范围内的bitmap设置为随机值
	class trn_brush_deco_rand
	{
	private:
		size_t	m_terrain_width;
		size_t	m_terrain_height;
	public:
		trn_brush_deco_rand(size_t terrain_width,size_t terrain_height):m_terrain_width(terrain_width),m_terrain_height(terrain_height)
		{
		}
		int	 get_rand(int max_val,int min_val)
		{
			return (int)(((float)rand()/RAND_MAX)*(max_val-min_val)+min_val);
		}

		void do_inner(nalpha_map* hmap, int x, int z, int brush_val)
		{
			if(brush_val==0)
			{
				hmap->set_value(x,z,0);
			}
			else
			{
				hmap->checked_add(x, z, brush_val);
			}
		}

		void do_outer(nalpha_map* hmap, int x, int z, int brush_val,float ratio, ETrnLerpMode lerp_mode)
		{
			UNREFERENCED_PARAMETER(ratio);
			UNREFERENCED_PARAMETER(lerp_mode);
			if(brush_val==0)
			{
				hmap->set_value(x,z,0);
			}
			else
			{
				int lerped_val = (int)trn_brush_lerp((float)brush_val, 0.0f, ratio, lerp_mode);
				hmap->checked_add(x, z, lerped_val);
			}
		}
	};
}//namespace nexus

#endif// endof _NEXUS_TERRAIN_BRUSH_H_