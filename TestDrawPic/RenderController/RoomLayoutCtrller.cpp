#include "stdafx.h"

#include "RoomLayoutCtrller.h"
#include "UserEvent.h"

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
			evt.UserEvent.UserData1 == EUT_ROOMLAYOUT_PICTUREREFLINE_DRAW)
		{
			SetCurrentState(ERoomLayoutSatate::ERS_PICTURE);
		}
	}

	return StatesController::OnPostEvent(evt);
}
