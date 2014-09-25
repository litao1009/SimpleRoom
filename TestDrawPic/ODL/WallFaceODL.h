#pragma once
//////////////////////////////////////////////////////////////////////////
//���������ǽ��ǽ��
//1��ǽ������2���棬���汳�棬������6����
//ǽ�������Shape����ǽCreatPointʱ��ͬʱ����
//////////////////////////////////////////////////////////////////////////

#include "baseodl.h"
#include "WallFaceZoneODL.h"
#include "WithHoleODL.h"

#include <vector>
#include <map>

class CWallFaceODL : public IWithHoleODL
{
public:
	CWallFaceODL(const TopoDS_Face& curFace);
	~CWallFaceODL(void);
public:

	bool		UpdateMesh();

	//////////////////////////////////////////////////////////////////////////
	//����ͼ��ʾ
	virtual void OnDrawTop(Gdiplus::Graphics& gcDrawer, float fScale);

	//�ƶ�����
	virtual void OnTopMoving(const gp_Pnt& stInfo);
	//�ƶ����
	virtual void OnTopMovingFinish(const gp_Pnt& stInfo, bool bCommitChange);
	//ȡ���ƶ�
	virtual void CancelTopMoving();
	//////////////////////////////////////////////////////////////////////////
	//ǽ��ͼ��ʾ
	virtual void	OnDrawFace(Gdiplus::Graphics& gcDrawer, float fScale);

public:

	virtual	bool	UpdateHoleShape();

	virtual void	UpdateHoleRect(RelationRect rect);
};


typedef	std::shared_ptr<CWallFaceODL>	CWallFaceODLSPtr;
typedef	std::weak_ptr<CWallFaceODL>		CWallFaceODLWPtr;
