#pragma once
#include"GaussianDescriptor.h"

class GaussianSplatSolver {
public:
	// Ìî³ä3D gaussianµÄÊý¾Ý
	virtual void fillGsScene(UGaussianDescriptor* gsDesc) = 0;
};