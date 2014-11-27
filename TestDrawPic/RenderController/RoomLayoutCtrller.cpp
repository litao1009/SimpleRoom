#include "stdafx.h"

#include "RoomLayoutCtrller.h"
#include "UserEvent.h"

#include "RenderController/RoomLayoutDrawingCtrller.h"
#include "RenderController/RoomLayoutPictureCtrller.h"
#include "RenderController/RoomLayoutBrowserCtrller.h"

RoomLayoutCtrller::RoomLayoutCtrller( const GraphODLWPtr& odl, const SRenderContextWPtr& rc ):StatesController(rc), GraphODL_(odl)
{

}


bool RoomLayoutCtrller::OnPostEvent( const irr::SEvent& evt )
{
	if ( evt.EventType == irr::EET_USER_EVENT )
	{
		if ( evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_DRAWLINE_FINISH )
		{
			SetCurrentState(ERoomLayoutSatate::ERS_BROWSE);
			return true;
		}

		if ( evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_FINISH )
		{
			SetCurrentState(ERoomLayoutSatate::ERS_BROWSE);
			return true;
		}

		if ( evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_DRAWLINE )
		{
			SetCurrentState(ERoomLayoutSatate::ERS_DRAWING);
		}

		if ( evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_ALPHA ||
			evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_VISIBLE ||
			evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_SETTING ||
			evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTUREREFLINE_SIZE ||
			evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTUREREFLINE_DRAW ||
			evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTURE_POSITION)
		{
			SetCurrentState(ERoomLayoutSatate::ERS_PICTURE);
		}
	}

	return StatesController::OnPostEvent(evt);
}

void RoomLayoutCtrller::Init()
{
	//ÁÙÄ¡Í¼
	AddController(ERoomLayoutSatate::ERS_PICTURE, std::make_shared<RoomLayoutPictureCtrller>(GraphODL_, GetRenderContextSPtr()));

	//»æÖÆ
	AddController(ERoomLayoutSatate::ERS_DRAWING, std::make_shared<RoomLayoutDrawingCtrller>(GraphODL_, GetRenderContextSPtr()));

	//ä¯ÀÀ
	AddController(ERoomLayoutSatate::ERS_BROWSE, std::make_shared<RoomLayoutBrowserCtrller>(GraphODL_, GetRenderContextSPtr()));

	//Ä¬ÈÏ×´Ì¬
	SetCurrentState(ERoomLayoutSatate::ERS_BROWSE);
}
