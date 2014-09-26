#include "stdafx.h"

#include "GUIImageButton.h"
#include "IGUIEnvironment.h"

using namespace irr;
using namespace gui;

CGUIImageButton::CGUIImageButton( irr::gui::IGUIEnvironment* environment, irr::gui::IGUIElement* parent, irr::s32 id, irr::core::rect<irr::s32> rectangle )
	:CGUIImage(environment, parent, id, rectangle)
{
	State_ = EIS_NORMAL;
	HoldPressed_ = false;
}

bool CGUIImageButton::OnEvent( const SEvent& event )
{
	SEvent newEvent;
	newEvent.EventType = EET_GUI_EVENT;
	newEvent.GUIEvent.Caller = this;
	newEvent.GUIEvent.Element = 0;

	switch (State_)
	{
	case EIS_NORMAL:
		{
			if ( EET_GUI_EVENT == event.EventType && event.GUIEvent.Caller == this && EGET_ELEMENT_HOVERED == event.GUIEvent.EventType )
			{
				State_ = EIS_HOVERD;

				if ( Parent )
				{
					newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_HOVERD;
					return Parent->OnEvent(newEvent);
				}
			}
		}
		break;
	case EIS_HOVERD:
		{
			if ( EET_GUI_EVENT == event.EventType && event.GUIEvent.Caller == this && EGET_ELEMENT_LEFT == event.GUIEvent.EventType )
			{
				State_ = EIS_NORMAL;

				if ( Parent )
				{
					newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_NORMAL;
					return Parent->OnEvent(newEvent);
				}
			}

			if ( EET_MOUSE_INPUT_EVENT == event.EventType && EMIE_LMOUSE_PRESSED_DOWN == event.MouseInput.Event )
			{
				State_ = EIS_PRESSED;

				StartPoint_.X = event.MouseInput.X;
				StartPoint_.Y = event.MouseInput.Y;

				if ( Parent )
				{
					newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_PRESSED;
					return Parent->OnEvent(newEvent);
				}
			}
		}
		break;
	case EIS_PRESSED:
		{
			if ( EET_GUI_EVENT == event.EventType && event.GUIEvent.Caller == this && EGET_ELEMENT_LEFT == event.GUIEvent.EventType )
			{
				if ( !HoldPressed_ )
				{
					State_ = EIS_NORMAL;

					if ( Parent )
					{
						newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_NORMAL;
						return Parent->OnEvent(newEvent);
					}
				}
				
			}

			if ( EET_MOUSE_INPUT_EVENT == event.EventType && EMIE_LMOUSE_LEFT_UP == event.MouseInput.Event && Environment->getHovered() == this )
			{
				State_ = EIS_HOVERD;

				if ( Parent )
				{
					newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_LEFTUP;
					Parent->OnEvent(newEvent);
					newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_HOVERD;
					Parent->OnEvent(newEvent);
				}

				return true;
			}

			if ( EET_MOUSE_INPUT_EVENT == event.EventType && EMIE_LMOUSE_LEFT_UP == event.MouseInput.Event && HoldPressed_ && Environment->getHovered() != this )
			{
				State_ = EIS_NORMAL;

				if ( Parent )
				{
					newEvent.GUIEvent.EventType = (irr::gui::EGUI_EVENT_TYPE)EIS_NORMAL;
					return Parent->OnEvent(newEvent);
				}
			}
		}
	default:
		break;
	}

	return false;
}
