#pragma once
#include<iostream>
#include<unordered_map>

// 分包任务的信息
// 目前基类里面不需要存任何信息，但后续有可能扩展
class PackageInfo {
public:
	virtual ~PackageInfo() {

	}
};

// 分包式的消息管理器
class PackageMsgManager {
public:
	// 下一个需要分配的分包id
	uint32_t nextPackageId = 10000;

	// 目前已经注册过的所有分包信息
	std::unordered_map<uint32_t, PackageInfo*> infoMap;
	// 远端的package map
	std::unordered_map<uint32_t, PackageInfo*> remotePackage;

	// 注册分包任务
	uint32_t registerPackageTask(PackageInfo* pkgInfo) {
		infoMap[nextPackageId] = pkgInfo;
		++nextPackageId;
		return nextPackageId - 1;
	}

	// 注册远端的package
	void registerRemotePackage(uint32_t idPackage, PackageInfo* pkgInfo) {
		if (remotePackage.count(idPackage)) {
			throw std::runtime_error(std::to_string(idPackage) + "remote exist");
		}
		remotePackage[idPackage] = pkgInfo;
	}

	// 删除分包任务
	void deletePackagInfo(uint32_t infoId) {
		// 对package做析构
		delete infoMap.at(infoId);
		infoMap.erase(infoId);
	}

	//根据id获取本地信息
	PackageInfo* getLocalInfo(uint32_t infoId) {
		// 如果infoMap里面没有这个信息，就直接结束了
		if (infoMap.count(infoId) == 0) {
			throw std::runtime_error("Cannot find infoId: " + std::to_string(infoId));
		}
		return infoMap.at(infoId);
	}

	// 获取远端的数据包
	PackageInfo* getRemotePackage(uint32_t idPackage) {
		if (remotePackage.count(idPackage) == 0) {
			throw std::runtime_error("Cannot find remote package: " + std::to_string(idPackage));
		}
		return remotePackage.at(idPackage);
	}
};