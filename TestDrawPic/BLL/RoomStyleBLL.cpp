#include "stdafx.h"
#include "RoomStyleBLL.h"


CRoomStyleBLL::CRoomStyleBLL(void)
{
	CRoomStyleLineBLLSPtr sp0=std::make_shared<CRoomStyleLineBLL>();
	CRoomStyleLineBLLSPtr sp1=std::make_shared<CRoomStyleLineBLL>();
	CRoomStyleLineBLLSPtr sp2=std::make_shared<CRoomStyleLineBLL>();
	CRoomStyleLineBLLSPtr sp3=std::make_shared<CRoomStyleLineBLL>();
	CRoomStyleLineBLLSPtr sp4=std::make_shared<CRoomStyleLineBLL>();
	CRoomStyleLineBLLSPtr sp5=std::make_shared<CRoomStyleLineBLL>();
	m_arrLine.push_back(sp0);
	m_arrLine.push_back(sp1);
	m_arrLine.push_back(sp2);
	m_arrLine.push_back(sp3);
	m_arrLine.push_back(sp4);
	m_arrLine.push_back(sp5);
}


CRoomStyleBLL::~CRoomStyleBLL(void)
{
}
