#pragma once
#include"MeshSolver.hpp"
#include<functional>

class MeshFinishFunctor {
public:
	virtual void meshFinishProcess(MeshSolver* mesh, uint32_t idPackage) = 0;
};

class MeshFinishLambdaFunctor : public MeshFinishFunctor {
public:
	using FuncType = std::function<void(MeshSolver*, uint32_t)>;
	FuncType func;

	MeshFinishLambdaFunctor(FuncType function):func(function) {
	}

	virtual void meshFinishProcess(MeshSolver* mesh, uint32_t idPackage) override {
		this->func(mesh, idPackage);
	}
};