#include "stdafx.h"
#include "DataManager.h"

CDataManager::CDataManager()
{

}

void CDataManager::Init(CDBConnection* pCon)
{
	m_pCon =pCon;
	m_spDecorMgr = std::make_shared<CDecorDAL>(pCon);
	//设计方案数据库管理(Design/ObjGroup)
	 m_spDesignMgr = std::make_shared<CDesignDAL>(pCon);
	 m_spObjGroupMgr = std::make_shared<CObjGroupDAL>(pCon);
	//订单数据管理
	 m_spOrderMgr = std::make_shared<COrderDAL>(pCon);

	 m_spSkinUnitMgr = std::make_shared<CSkinUnitDAL>(pCon);
	//墙数据管理(Wall/Face/Zone)
	m_spWallMgr = std::make_shared<CWallDAL>(pCon);
	//地板数据管理(Floor/Zone)
	m_spFloorMgr = std::make_shared<CFloorDAL>(pCon);
	//纹理数据库管理
	m_spSkinMgr = std::make_shared<CSkinDAL>(pCon);
}


CDataManager::~CDataManager(void)
{
}
