#pragma once
//////////////////////////////////////////////////////////////////////////
//������ǽ�ĵ�����
//////////////////////////////////////////////////////////////////////////
#include "basebll.h"
class CRoomStyleLineBLL :
	public CBaseBLL
{
public:
	CRoomStyleLineBLL(void);
	~CRoomStyleLineBLL(void);
	void GetPointList(INT nIndex, std::vector<Gdiplus::PointF>& arrPoint);

};

typedef	std::shared_ptr<CRoomStyleLineBLL>		CRoomStyleLineBLLSPtr;
typedef	std::weak_ptr<CRoomStyleLineBLL>		CRoomStyleLineBLLWPtr;
