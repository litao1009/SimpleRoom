#include "stdafx.h"

#include "ResizeController.h"
#include "irrEngine/Implemention/EventUserType.h"


ResizeController::ResizeController(HWND h)
{
	Hwnd_ = h;
}


bool ResizeController::OnEvent( const irr::SEvent& event )
{
	if ( event.EventType == irr::EET_USER_EVENT && event.UserEvent.UserData1 == EUT_RESIZE_MINIMUM )
	{
		::SendMessage(Hwnd_, WM_IRR_DLG_MSG, WM_RESIZE_MINIMUM, 0);
		return true;
	}

	if ( event.EventType == irr::EET_USER_EVENT && event.UserEvent.UserData1 == EUT_RESIZE_RESTORE )
	{
		::SendMessage(Hwnd_, WM_IRR_DLG_MSG, WM_RESIZE_RESTORE, 0);
		return true;
	}

	if ( event.EventType == irr::EET_USER_EVENT && event.UserEvent.UserData1 == EUT_RESIZE_CHANGE_BEGIN )
	{
		::SendMessage(Hwnd_, WM_IRR_DLG_MSG, WM_RESIZE_CHANGE_BEGIN, 0);
		return true;
	}

	if ( event.EventType == irr::EET_USER_EVENT && event.UserEvent.UserData1 == EUT_RESIZE_CHANGE_DURING )
	{
		::SendMessage(Hwnd_, WM_IRR_DLG_MSG, WM_RESIZE_CHANGE_DURING, 0);
		return true;
	}

	if ( event.EventType == irr::EET_USER_EVENT && event.UserEvent.UserData1 == EUT_RESIZE_CHANGE_END )
	{
		::SendMessage(Hwnd_, WM_IRR_DLG_MSG, WM_RESIZE_CHANGE_END, 0);
		return true;
	}

	return false;
}