#pragma once
#include"GaussianDescriptor.h"

class GaussianSplatSolver {
public:
	// ���3D gaussian������
	virtual void fillGsScene(UGaussianDescriptor* gsDesc) = 0;
};