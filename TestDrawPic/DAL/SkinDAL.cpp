#include "stdafx.h"
#include "SkinDAL.h"


CSkinDAL::CSkinDAL(CDBConnection* pCon):CBaseDAL(pCon)
{
}


CSkinDAL::~CSkinDAL(void)
{
}

HRESULT CSkinDAL::GetSkinList( INT nFactoryID, INT nShopID, INT nGroupID, INT nGTypeID, std::vector<StSkinInfo>& arrSkin )
{
	CString strSQL=_T("");
	strSQL.Format(_T("select sk.SkinID, sk.GTypeID, sk.GroupID, sk.FactoryID, sk.ShopID, sk.SkinPicID, sk.IsActive, sk.VisitCount, sk.XLen, sk.YLen, pic.FilePath as SkinPicPath, pic.FileHashCode as SkinHashCode, sk.SkinCode, sk.SkinName, sk.SkinMemo from JKD_SkinInfo sk inner join JKD_PicInfo pic on sk.FactoryID=pic.FactoryID and sk.ShopID=pic.ShopID and sk.SkinPicID=pic.PicID where sk.FactoryID=%d and sk.ShopID=%d"), nFactoryID, nShopID);
	CString strTmpSQL=_T("");
	if (nGroupID>=0)
	{
		strTmpSQL.Format(_T(" and sk.GroupID=%d"), nGroupID);
		strSQL.Append(strTmpSQL);
	}
	if (nGTypeID>0)
	{
		strTmpSQL.Format(_T(" and sk.GTypeID=%d"), nGTypeID);
		strSQL.Append(strTmpSQL);
	}
	ReturnInfo rtInfo;
	
	m_pCon->GetTable(strSQL, rtInfo);
	INT nCount = (INT)rtInfo.RowList.size();
	for (int i=0; i<nCount; i++)
	{
		StSkinInfo stInfo;
		CString strTmp;

		strTmp = m_pCon->GetColValue(_T("SkinID"), rtInfo, i);
		stInfo.SkinID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("GTypeID"), rtInfo, i);
		stInfo.GTypeID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("GroupID"), rtInfo, i);
		stInfo.GroupID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("FactoryID"), rtInfo, i);
		stInfo.FactoryID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("ShopID"), rtInfo, i);
		stInfo.ShopID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("SkinPicID"), rtInfo, i);
		stInfo.SkinPicID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("IsActive"), rtInfo, i);
		stInfo.IsActive = _ttoi(strTmp)!=0;

		strTmp = m_pCon->GetColValue(_T("VisitCount"), rtInfo, i);
		stInfo.VisitCount = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("XLen"), rtInfo, i);
		stInfo.XLen = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("YLen"), rtInfo, i);
		stInfo.YLen = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("SkinPicPath"), rtInfo, i);
		_tcscpy_s(stInfo.SkinPicPath, strTmp);

		strTmp = m_pCon->GetColValue(_T("SkinHashCode"), rtInfo, i);
		_tcscpy_s(stInfo.SkinHashCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("SkinCode"), rtInfo, i);
		_tcscpy_s(stInfo.SkinCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("SkinName"), rtInfo, i);
		_tcscpy_s(stInfo.SkinName, strTmp);

		strTmp = m_pCon->GetColValue(_T("SkinMemo"), rtInfo, i);
		_tcscpy_s(stInfo.SkinMemo, strTmp);
		
		arrSkin.push_back(stInfo);
	}
	return S_OK;
}
