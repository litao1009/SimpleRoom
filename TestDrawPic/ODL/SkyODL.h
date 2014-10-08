#ifndef SkyODL_h__
#define SkyODL_h__

#pragma once
//////////////////////////////////////////////////////////////////////////
//���ﱣ���컨��
//�컨��Ҳ��һ���͵ذ����Ƶ���״
//�컨�����ʾface���������Ͽ�������Face�ĵ���Ϣ�ǣ��������Ϣ��Z=>-Z,Y=>-Y
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
	//�ƶ�����
	virtual void OnTopMoving(const gp_Pnt& stInfo);
	virtual void OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	virtual void CancelTopMoving();};

typedef	std::shared_ptr<CSkyODL>		CSkyODLSPtr;
typedef	std::weak_ptr<CSkyODL>		CSkyODLWPtr;
#endif // SkyODL_h__
