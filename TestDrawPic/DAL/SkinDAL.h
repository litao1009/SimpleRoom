#ifndef SkinDAL_h__
#define SkinDAL_h__

#pragma once
#include "basedal.h"
class CSkinDAL :
	public CBaseDAL
{

	
public:
	CSkinDAL(CDBConnection* pCon);
	~CSkinDAL(void);

	struct StSkinInfo
	{
		INT SkinID;
		INT GTypeID;
		INT GroupID;
		INT FactoryID;
		INT ShopID;
		INT SkinPicID;
		BOOL IsActive;
		INT VisitCount;
		FLOAT XLen;
		FLOAT YLen;
		TCHAR SkinPicPath[256];
		TCHAR SkinHashCode[64];
		TCHAR SkinCode[51];
		TCHAR SkinName[51];
		TCHAR SkinMemo[501];
		StSkinInfo()
		{
			ZeroMemory(this, sizeof(StSkinInfo));
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//GTypeID:5201ǽֽ��5202ǽש��5101�ذ�ש��5102ľ�ذ塢5104����
	HRESULT GetSkinList(INT nFactoryID, INT nShopID, INT nGroupID, INT nGTypeID, std::vector<StSkinInfo>& arrSkin);
};

typedef	std::shared_ptr<CSkinDAL>	CSkinDALSPtr;
typedef	std::weak_ptr<CSkinDAL>	CSkinDALWPtr;
#endif // SkinDAL_h__
