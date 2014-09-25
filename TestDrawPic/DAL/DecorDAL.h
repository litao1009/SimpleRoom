#ifndef DecorDAL_h__
#define DecorDAL_h__

#pragma once
#include "basedal.h"
class CDecorDAL :
	public CBaseDAL
{
public:
	CDecorDAL(CDBConnection* pCon);
	~CDecorDAL(void);

	struct StDecorInfo
	{
		LONG DecorID;
		INT GTypeID;
		INT GroupID;
		INT FactoryID;
		INT ShopID;
		BOOL IsActive;
		INT ModelID;
		INT ShowPicID;       //Model
		INT TopPicID;        //Model
		INT FrontPicID;      //Model
		INT VisitCount;      //
		FLOAT XLen;
		FLOAT YLen;
		FLOAT ZLen;
		FLOAT ModelArcX;           //Model
		FLOAT ModelArcY;           //Model
		FLOAT ModelArcZ;           //Model
		TCHAR ModelName[51];      //Model
		TCHAR ModelFilePath[256];  //Model
		TCHAR ModelHashCode[64];  //Model
		TCHAR ShowPicPath[256];
		TCHAR ShowHashCode[51];
		TCHAR TopPicPath[256];
		TCHAR TopHashCode[64];
		TCHAR FrontPicPath[256];
		TCHAR FrontHashCode[64];
		TCHAR DecorCode[51];
		TCHAR DecorName[51];
		TCHAR DecorMemo[501];
		StDecorInfo()
		{
			ZeroMemory(this, sizeof(StDecorInfo));
		}
	};
public:
	HRESULT GetDecorList(INT nFactoryID, INT nShopID, INT nGroupID, INT nGTypeID, std::vector<StDecorInfo>& arrDecor);
};

typedef	std::shared_ptr<CDecorDAL>	CDecorDALSPtr;
typedef	std::weak_ptr<CDecorDAL>	CDecorDALWPtr;
#endif // DecorDAL_h__
