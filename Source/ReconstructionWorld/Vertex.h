#pragma once

//һ���ڵ������
class Vertex
{
public:
	//λ��
	float position[3];
	//������
	float normal[3];
	//��г�����Ĳ���
	float shs[48];
	//͸����
	float opacity;
	//���������scale
	float scale[3];
	//����Ԫ�����ɵ���ת
	float rotation[4];
};

