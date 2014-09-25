#include "stdafx.h"
#include "DataManager.h"

CDataManager::CDataManager()
{

}

void CDataManager::Init(CDBConnection* pCon)
{
	m_pCon =pCon;
	m_spDecorMgr = std::make_shared<CDecorDAL>(pCon);
	//��Ʒ������ݿ����(Design/ObjGroup)
	 m_spDesignMgr = std::make_shared<CDesignDAL>(pCon);
	 m_spObjGroupMgr = std::make_shared<CObjGroupDAL>(pCon);
	//�������ݹ���
	 m_spOrderMgr = std::make_shared<COrderDAL>(pCon);

	 m_spSkinUnitMgr = std::make_shared<CSkinUnitDAL>(pCon);
	//ǽ���ݹ���(Wall/Face/Zone)
	m_spWallMgr = std::make_shared<CWallDAL>(pCon);
	//�ذ����ݹ���(Floor/Zone)
	m_spFloorMgr = std::make_shared<CFloorDAL>(pCon);
	//�������ݿ����
	m_spSkinMgr = std::make_shared<CSkinDAL>(pCon);
}


CDataManager::~CDataManager(void)
{
}
