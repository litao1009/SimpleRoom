#ifndef SkyODL_h__
#define SkyODL_h__

#pragma once
//////////////////////////////////////////////////////////////////////////
//这里保存天花板
//天花板也是一个和地板类似的形状
//天花板的显示face，从下往上看。即，Face的点信息是：地面点信息的Z=>-Z,Y=>-Y
//////////////////////////////////////////////////////////////////////////
#include "baseodl.h"
#include "TopoDS_Face.hxx"

class CSkyODL :
	public CBaseODL
{
public:
	CSkyODL(const TopoDS_Face& theFace);
	~CSkyODL(void);
public:	
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);
	virtual void OnDrawFace( Gdiplus::Graphics& gcDrawer, float fScale );
	//移动过程
	virtual void OnTopMoving(const gp_Pnt& stInfo);
	virtual void OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	virtual void CancelTopMoving();};

typedef	std::shared_ptr<CSkyODL>		CSkyODLSPtr;
typedef	std::weak_ptr<CSkyODL>		CSkyODLWPtr;
#endif // SkyODL_h__
