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
	//装饰系统数据库管理
	CDecorDALSPtr m_spDecorMgr;
	//设计方案数据库管理(Design/ObjGroup)
	CDesignDALSPtr m_spDesignMgr;
	CObjGroupDALSPtr m_spObjGroupMgr;
	//订单数据管理
	COrderDALSPtr m_spOrderMgr;
	
	CSkinUnitDALSPtr m_spSkinUnitMgr;
	//墙数据管理(Wall/Face/Zone)
	CWallDALSPtr m_spWallMgr;
	//地板数据管理(Floor/Zone)
	CFloorDALSPtr m_spFloorMgr;
	//纹理数据库管理
	CSkinDALSPtr m_spSkinMgr;
private:
	CDBConnection* m_pCon;
};

#endif // DataManager_h__
