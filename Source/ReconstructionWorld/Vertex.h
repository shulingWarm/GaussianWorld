#pragma once

//一个节点的数据
class Vertex
{
public:
	//位置
	float position[3];
	//法向量
	float normal[3];
	//球谐函数的参数
	float shs[48];
	//透明度
	float opacity;
	//三个方向的scale
	float scale[3];
	//由四元数构成的旋转
	float rotation[4];
};

