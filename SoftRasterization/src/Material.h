#ifndef Material_h
#define Material_h

namespace SoftRender 
{
	struct Material
	{
		Color ambient;	//�ڻ���������������巴�����ʲô��ɫ��ͨ�����Ǻ�������ɫ��ͬ����ɫ
		Color diffuse;	//��������������������ɫ�����ͻ�������һ����
		Color specular;	//������ն��������ɫӰ��

		float shininess;//����ȣ�����߹��ɢ��/�뾶

		Material()
		{
			shininess = 0;
		}

		Material(Color& _ambient, Color& _diffuse, Color& _specular, float _shininess):
			ambient(_ambient), diffuse(_diffuse), specular(_specular), shininess(_shininess)
			{}
	};
}

#endif