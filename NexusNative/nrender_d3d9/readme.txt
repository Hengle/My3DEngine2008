ʹ��D3D9, Shader Model 3ʵ�ֵ���Ⱦϵͳ<���>

.����Scene Depth
	����Depth of Field������ɢ���Ч������Ҫ���ʳ�����zֵ��������ҪDepth Texture������D3D9����depth texture���ص�����
	��ǰ��zָ�ȽϵĽ������Ȼnvidia�ṩ��FOURCC format--INTZ��geforce 8x��,RAWZ(geforce 7x)��ATI�ṩ��DF16/DF24����ʹ��
	���������кܶ಻����ĵط�������������ʹ��һ��R32F�ĸ�ʽ������linear depth--��Ҳ��CryEngine2�ķ�����
	float3 m=float3(0.996093809371817670572857294849, 0.0038909914428586627756752238080039, 1.5199185323666651467481343000015e-5);
	float d = dot(tex2D(depthSampler, depthUV).arg, m);