#include "stdafx.h"
#include "DecorDAL.h"


CDecorDAL::CDecorDAL(CDBConnection* pCon):CBaseDAL(pCon)
{
}


CDecorDAL::~CDecorDAL(void)
{
}

HRESULT CDecorDAL::GetDecorList( INT nFactoryID, INT nShopID, INT nGroupID, INT nGTypeID, std::vector<StDecorInfo>& arrDecor )
{
	CString strSQL=_T("");
	strSQL.Format(_T("select dc.DecorID, dc.GTypeID, dc.GroupID, dc.FactoryID, dc.ShopID, dc.IsActive, dc.ModelID, sp.PicID as ShowPicID, tp.PicID as TopPicID, fp.PicID as FrontPicID, dc.VisitCount, dc.XLen, dc.YLen, dc.ZLen, md.ArcX as ModelArcX, md.ArcY as ModelArcY, md.ArcZ as ModelArcZ, md.ModelName,md.FilePath as ModelFilePath, md.FileHashCode as ModelHashCode, sp.FilePath as ShowPicPath, sp.FileHashCode as ShowHashCode, tp.FilePath as TopPicPath, tp.FileHashCode as TopHashCode, fp.FilePath as FrontPicPath, fp.FileHashCode as FrontHashCode, dc.DecorCode, dc.DecorName, dc.DecorMemo from JKD_DecorInfo dc inner join JKD_ModelInfo md on dc.FactoryID=md.FactoryID and dc.ShopID=md.ShopID and dc.ModelID=md.ModelID left outer join JKD_PicInfo sp on md.FactoryID=sp.FactoryID and md.ShopID=sp.ShopID and md.ShowPicID=sp.PicID left outer join JKD_PicInfo tp on md.FactoryID=tp.FactoryID and md.ShopID=tp.ShopID and md.TopPicID=tp.PicID left outer join JKD_PicInfo fp on md.FactoryID=fp.FactoryID and md.ShopID=fp.ShopID and md.FrontPicID=fp.PicID where dc.FactoryID=%d and dc.ShopID=%d"), nFactoryID, nShopID);
	CString strTmpSQL=_T("");
	if (nGroupID>=0)
	{
		strTmpSQL.Format(_T(" and dc.GroupID=%d"), nGroupID);
		strSQL.Append(strTmpSQL);
	}
	if (nGTypeID>0)
	{
		strTmpSQL.Format(_T(" and dc.GTypeID=%d"), nGTypeID);
		strSQL.Append(strTmpSQL);
	}
	ReturnInfo rtInfo;

	m_pCon->GetTable(strSQL, rtInfo);
	INT nCount = (INT)rtInfo.RowList.size();
	for (int i=0; i<nCount; i++)
	{
		StDecorInfo stInfo;
		CString strTmp;

		strTmp = m_pCon->GetColValue(_T("DecorID"), rtInfo, i);
		stInfo.DecorID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("GTypeID"), rtInfo, i);
		stInfo.GTypeID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("GroupID"), rtInfo, i);
		stInfo.GroupID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("FactoryID"), rtInfo, i);
		stInfo.FactoryID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("ShopID"), rtInfo, i);
		stInfo.ShopID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("IsActive"), rtInfo, i);
		stInfo.IsActive = _ttoi(strTmp)!=0;

		strTmp = m_pCon->GetColValue(_T("ModelID"), rtInfo, i);
		stInfo.ModelID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("ShowPicID"), rtInfo, i);
		stInfo.ShowPicID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("TopPicID"), rtInfo, i);
		stInfo.TopPicID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("FrontPicID"), rtInfo, i);
		stInfo.FrontPicID = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("VisitCount"), rtInfo, i);
		stInfo.VisitCount = _ttoi(strTmp);

		strTmp = m_pCon->GetColValue(_T("XLen"), rtInfo, i);
		stInfo.XLen = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("YLen"), rtInfo, i);
		stInfo.YLen = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("ZLen"), rtInfo, i);
		stInfo.ZLen = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("ModelArcX"), rtInfo, i);
		stInfo.ModelArcX = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("ModelArcY"), rtInfo, i);
		stInfo.ModelArcY = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("ModelArcZ"), rtInfo, i);
		stInfo.ModelArcZ = StrToFloat(strTmp);

		strTmp = m_pCon->GetColValue(_T("ModelName"), rtInfo, i);
		_tcscpy_s(stInfo.ModelName, strTmp);

		strTmp = m_pCon->GetColValue(_T("ModelFilePath"), rtInfo, i);
		_tcscpy_s(stInfo.ModelFilePath, strTmp);

		strTmp = m_pCon->GetColValue(_T("ModelHashCode"), rtInfo, i);
		_tcscpy_s(stInfo.ModelHashCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("ShowPicPath"), rtInfo, i);
		_tcscpy_s(stInfo.ShowPicPath, strTmp);

		strTmp = m_pCon->GetColValue(_T("ShowHashCode"), rtInfo, i);
		_tcscpy_s(stInfo.ShowHashCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("TopPicPath"), rtInfo, i);
		_tcscpy_s(stInfo.TopPicPath, strTmp);

		strTmp = m_pCon->GetColValue(_T("TopHashCode"), rtInfo, i);
		_tcscpy_s(stInfo.TopHashCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("FrontPicPath"), rtInfo, i);
		_tcscpy_s(stInfo.FrontPicPath, strTmp);

		strTmp = m_pCon->GetColValue(_T("FrontHashCode"), rtInfo, i);
		_tcscpy_s(stInfo.FrontHashCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("DecorCode"), rtInfo, i);
		_tcscpy_s(stInfo.DecorCode, strTmp);

		strTmp = m_pCon->GetColValue(_T("DecorName"), rtInfo, i);
		_tcscpy_s(stInfo.DecorName, strTmp);

		strTmp = m_pCon->GetColValue(_T("DecorMemo"), rtInfo, i);
		_tcscpy_s(stInfo.DecorMemo, strTmp);

		arrDecor.push_back(stInfo);
	}
	return S_OK;
}
