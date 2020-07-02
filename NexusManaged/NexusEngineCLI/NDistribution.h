#pragma once
#include "NMath.h"

namespace NexusEngine
{
	enum DistributionType
	{
		DistributionConst=0,	//����
		DistributionUniform,	//���ֵ
		DistributionCurve,		//����
	};
	/*
	 * �ṩNDisribution����ת��Ϊstring��ʹ��PropertyGrid�ؼ��ܹ�չ������
	*/
	public ref class NDistributionConverter : public ExpandableObjectConverter
	{
	public:
		virtual bool CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType) override;		
		virtual Object^ ConvertTo(ITypeDescriptorContext^ context, Globalization::CultureInfo^ culture, Object^ value, Type^ destinationType) override;
	};

	/**
	 *	һ��Distribution���࣬��Ϊһ����־��
	*/
	//[TypeConverter(NDistributionConverter::typeid)]
	public ref class NDistribution abstract
	{
	};

}//namespace NexusEngine