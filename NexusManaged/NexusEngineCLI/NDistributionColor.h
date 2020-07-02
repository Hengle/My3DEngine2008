#pragma once
#include "NMath.h"
#include "NDistribution.h"

namespace NexusEngine
{
	/**
	*	Color4f Distribution����
	*/
	public ref class NDistributionColor : public NDistribution
	{
	public:
	
	public:
		NDistributionColor(nexus::ndistribution<color4f>::ptr nativeObj);		
		NDistributionColor(void)
		{
			m_native = NULL;
		}
		~NDistributionColor(void);		

		Color4f GetValueByTime(float time);

		nexus::ndistribution<color4f>::ptr GetNativeSmartPtr()
		{
			return *m_native;
		}
	protected:
		property nexus::ndistribution<color4f>* NativePtr
		{
			nexus::ndistribution<color4f>* get()
			{
				return m_native->get();
			}
		}

		nexus::ndistribution<color4f>::ptr* m_native;
		DistributionType	m_Type;
	};

	/*
	*	Color4f����
	*/
	public ref class NDistributionColorConst : public NDistributionColor
	{
	public:
		[Category("Distribution")]
		property Color4f Value
		{
			Color4f get()
			{
				return Color4f::FromNative(
						((nexus::ndistribution_const<color4f>*)NativePtr)->m_value
					);
			}
			void set(Color4f val)
			{
				((nexus::ndistribution_const<color4f>*)NativePtr)->m_value= val.ToNative();
			}
		}

	public:
		NDistributionColorConst(nexus::ndistribution_const<color4f>::ptr nativeObj)
			: NDistributionColor(nativeObj)
		{}
		NDistributionColorConst(void);
	};

	/**
	*	Color4f�����Χ
	*/
	public ref class NDistributionColorUniform: public NDistributionColor
	{
	public:
		[Category("Distribution")]
		property Color4f MaxValue
		{
			Color4f get()
			{
				return Color4f::FromNative( NativePtr->m_max );
			}
			void set(Color4f val)
			{
				NativePtr->m_max = val.ToNative();
			}
		}

		[Category("Distribution")]
		property Color4f MinValue
		{
			Color4f get()
			{
				return Color4f::FromNative( NativePtr->m_min );
			}
			void set(Color4f val)
			{
				NativePtr->m_min = val.ToNative();
			}
		}

	public:
		NDistributionColorUniform(nexus::ndistribution_random<color4f>::ptr nativeObj)
			: NDistributionColor(nativeObj)
		{}
		NDistributionColorUniform(void);

	private:
		property nexus::ndistribution_random<color4f>* NativePtr
		{
			nexus::ndistribution_random<color4f>* get()
			{
				return (nexus::ndistribution_random<color4f>*)(m_native->get());
			}
		}
	};

	/**
	*	Color4f��ʱ��������߱仯
	*/
	public ref class NDistributionColorCurve : public NDistributionColor
	{
	public:
		NDistributionColorCurve(nexus::ndistribution_curve<color4f>::ptr nativeObj);
		NDistributionColorCurve(void);

		[Category("Distribution")]
		property int StreamCount
		{
			int get()
			{
				return 4;
			}
		}

		property int	KeyCount
		{
			int get()
			{
				return NativePtr->get_num_keys();;
			}
		}

		float	GetTime(int index)
		{
			return NativePtr->get_key(index).second;
		}

		Color4f	GetValueByTime(float time)
		{
			return Color4f::FromNative(NativePtr->get_value(time));
		}

		Color4f	GetValue(int index)
		{
			return Color4f::FromNative(NativePtr->get_key(index).first);
		}

		void SetValue(int index,float time,Color4f value)
		{
			return NativePtr->set_key_value(index,value.ToNative(),time);
		}

		void AddKey(Color4f val, float time)
		{
			NativePtr->add_key(val.ToNative(), time);
		}

		void RemoveKey(int index)
		{
			NativePtr->remove_key((size_t)index);
		}

		void Clear()
		{
			NativePtr->clear();
		}

		property nexus::ndistribution_curve<color4f>* NativePtr
		{
			nexus::ndistribution_curve<color4f>* get()
			{
				return (nexus::ndistribution_curve<color4f>*)(m_native->get());
			}
		}
	};

	public ref class NDistributionUniformColor:public NDistribution
	{
	public:
		[Category("Distribution"), Description("������������"),Browsable(false)]
		property int StreamCount
		{
			int get()
			{
				return 4;
			}
		}

		[Category("Distribution"), Description("���߲�ֵ�Ľڵ�����"),Browsable(false)]
		property int	KeyCount
		{
			int get()
			{
				return NativePtr->get_num_keys();;
			}
		}

		[Category("Distribution"), Description("�Ƿ������ģʽ")]
		property bool	IsRandom
		{
			bool get()
			{
				return NativePtr->is_random();
			}
			void set(bool value)
			{
				NativePtr->set_random(value);
			}
		}

		[Category("Distribution"), Description("���ģʽ�µ����ֵ")]
		property Color4f MaxValue
		{
			Color4f get()
			{
				return Color4f::FromNative( NativePtr->get_random_max() );
			}
			void set(Color4f val)
			{
				NativePtr->set_random_max(val.ToNative());
			}
		}

		[Category("Distribution"), Description("���ģʽ�µ���Сֵ")]
		property Color4f MinValue
		{
			Color4f get()
			{
				return Color4f::FromNative( NativePtr->get_random_min() );
			}
			void set(Color4f val)
			{
				NativePtr->set_random_min(val.ToNative());
			}
		}

		[Category("Distribution"), Description("�Ƿ�ѭ��,ֻ�����߲�ֵģʽ��������")]
		property bool	IsLoop
		{
			bool get()
			{
				return NativePtr->is_loop();
			}
			void set(bool value)
			{
				NativePtr->set_loop(value);
			}
		}

		float	GetTime(int index)
		{
			return NativePtr->get_key(index).second;
		}

		Color4f	GetValueByTime(float time)
		{
			return Color4f::FromNative(NativePtr->get_value(time));
		}

		Color4f	GetValue(int index)
		{
			return Color4f::FromNative(NativePtr->get_key(index).first);
		}

		void SetValue(int index,float time,Color4f value)
		{
			return NativePtr->set_key_value(index,value.ToNative(),time);
		}

		void AddKey(Color4f val, float time)
		{
			NativePtr->add_key(val.ToNative(), time);
		}

		void RemoveKey(int index)
		{
			NativePtr->remove_key((size_t)index);
		}

		void Clear()
		{
			NativePtr->clear();
		}

		[Browsable(false)]
		property nexus::ndistribution_uniform<color4f>* NativePtr
		{
			nexus::ndistribution_uniform<color4f>* get()
			{
				return m_NativePtr;
			}
		}
	private:
		ndistribution_uniform<color4f>*	m_NativePtr;
	public:
		NDistributionUniformColor()
		{
			m_NativePtr=new ndistribution_uniform<color4f>();
		}
		~NDistributionUniformColor()
		{
			delete m_NativePtr;
		}
	};

}//namespace NexusEngine