#ifndef RoomStyleBLL_h__
#define RoomStyleBLL_h__

#pragma once
//////////////////////////////////////////////////////////////////////////
//���仧�������У�һ��������ɵĶ���
//////////////////////////////////////////////////////////////////////////

#include "basebll.h"
#include "RoomStyleLineBLL.h"

class CRoomStyleBLL :
	public CBaseBLL
{
public:
	CRoomStyleBLL(void);
	~CRoomStyleBLL(void);
public:
	void GetLineList(std::vector<CRoomStyleLineBLLSPtr>& arrLine){arrLine=m_arrLine;}
private:
	std::vector<CRoomStyleLineBLLSPtr> m_arrLine;
};

typedef	std::shared_ptr<CRoomStyleBLL>		CRoomStyleBLLSPtr;
typedef	std::weak_ptr<CRoomStyleBLL>		CRoomStyleBLLWPtr;
#endif // RoomStyleBLL_h__
