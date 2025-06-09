#pragma once
#include<iostream>
#include<unordered_map>

// �ְ��������Ϣ
// Ŀǰ�������治��Ҫ���κ���Ϣ���������п�����չ
class PackageInfo {
public:
	virtual ~PackageInfo() {

	}
};

// �ְ�ʽ����Ϣ������
class PackageMsgManager {
public:
	// ��һ����Ҫ����ķְ�id
	uint32_t nextPackageId = 10000;

	// Ŀǰ�Ѿ�ע��������зְ���Ϣ
	std::unordered_map<uint32_t, PackageInfo*> infoMap;

	// ע��ְ�����
	uint32_t registerPackageTask(PackageInfo* pkgInfo) {
		infoMap[nextPackageId] = pkgInfo;
		++nextPackageId;
		return nextPackageId - 1;
	}

	// ɾ���ְ�����
	void deletePackagInfo(uint32_t infoId) {
		// ��package������
		delete infoMap.at(infoId);
		infoMap.erase(infoId);
	}

	//����id��ȡ������Ϣ
	PackageInfo* getLocalInfo(uint32_t infoId) {
		// ���infoMap����û�������Ϣ����ֱ�ӽ�����
		if (infoMap.count(infoId) == 0) {
			throw std::runtime_error("Cannot find infoId: " + std::to_string(infoId));
		}
		return infoMap.at(infoId);
	}
};