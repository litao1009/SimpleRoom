#ifndef DataManager_h__
#define DataManager_h__

#pragma once

#include "DecorDAL.h"
#include "DesignDAL.h"
#include "ObjGroupDAL.h"
#include "OrderDAL.h"
#include "SkinUnitDAL.h"
#include "WallDAL.h"
#include "FloorDAL.h"
#include "SkinDAL.h"

class CDataManager
{
public:
	CDataManager();
	~CDataManager();
public:
	static CDataManager& GetInstance()
	{
		static CDataManager Instance;
		return Instance;
	}

public:
	void Init(CDBConnection* pCon);
	//װ��ϵͳ���ݿ����
	CDecorDALSPtr m_spDecorMgr;
	//��Ʒ������ݿ����(Design/ObjGroup)
	CDesignDALSPtr m_spDesignMgr;
	CObjGroupDALSPtr m_spObjGroupMgr;
	//�������ݹ���
	COrderDALSPtr m_spOrderMgr;
	
	CSkinUnitDALSPtr m_spSkinUnitMgr;
	//ǽ���ݹ���(Wall/Face/Zone)
	CWallDALSPtr m_spWallMgr;
	//�ذ����ݹ���(Floor/Zone)
	CFloorDALSPtr m_spFloorMgr;
	//�������ݿ����
	CSkinDALSPtr m_spSkinMgr;
private:
	CDBConnection* m_pCon;
};

#endif // DataManager_h__
