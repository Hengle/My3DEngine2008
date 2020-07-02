#pragma once

namespace NexusEngine
{
	public value class NSubUV
	{
	public:
		property int HorizontalImageCount
		{
			int get() 
			{ 
				return num_images_h; 
			}
			void  set(int val) 
			{ 
				num_images_h=(val==0?1:val);
			}
		}
		property int VerticalImageCount
		{
			int get() 
			{ 
				return num_images_v; 
			}
			void  set(int val) 
			{ 
				num_images_v=(val==0?1:val);
			}
		}

		property int ImageIndex
		{
			int get() 
			{ 
				return image_index; 
			}
			void  set(int val) 
			{ 
				image_index=val;
			}
		}

		static NSubUV FromNative(const nexus::nsub_uv& uv)
		{
			return NSubUV(uv);
		}

		nexus::nsub_uv ToNative()
		{
			return nexus::nsub_uv(num_images_h,num_images_v,image_index);
		}

	protected:
		int		num_images_h;	// ����texture�к����м���ͼƬ
		int		num_images_v;	// ����texture�������м���ͼƬ
		int		image_index;	// ��ǰ����ʹ���ĸ�ͼƬ		
	public:
		NSubUV(int h,int v,int i)
		{
			num_images_h=h;
			num_images_v=v;
			image_index=i;
		}
		NSubUV(const nexus::nsub_uv& uv)
		{
			num_images_h=uv.num_images_h;
			num_images_v=uv.num_images_v;
			image_index=uv.image_index;
		}
	};
}