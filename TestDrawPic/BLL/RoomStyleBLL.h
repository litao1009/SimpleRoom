#ifndef RoomStyleBLL_h__
#define RoomStyleBLL_h__

#pragma once
//////////////////////////////////////////////////////////////////////////
//房间户型线序列：一组折线组成的对象
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
