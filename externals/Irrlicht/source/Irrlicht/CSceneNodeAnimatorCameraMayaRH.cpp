// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CSceneNodeAnimatorCameraMayaRH.h"
#include "ICursorControl.h"
#include "ICameraSceneNode.h"
#include "SViewFrustum.h"
#include "ISceneManager.h"
#include "os.h"
#include <string>

static float wheelZoom = 0;
namespace irr
{
namespace scene
{

//! constructor
CSceneNodeAnimatorCameraMayaRH::CSceneNodeAnimatorCameraMayaRH(gui::ICursorControl* cursor,
	f32 rotateSpeed, f32 zoomSpeed, f32 translateSpeed, f32 distance)
	: CursorControl(cursor), OldCamera(0), MousePos(0.5f, 0.5f),
	ZoomSpeed(zoomSpeed), RotateSpeed(rotateSpeed), TranslateSpeed(translateSpeed),
	CurrentZoom(distance), RotY(0.0f), RotX(0.0f),
	Zooming(false), Rotating(false), Moving(false), Translating(false)
{
	#ifdef _DEBUG
	setDebugName("CSceneNodeAnimatorCameraMayaRH");
	#endif

	if (CursorControl)
	{
		CursorControl->grab();
		MousePos = CursorControl->getRelativePosition();
	}

	allKeysUp();
}


//! destructor
CSceneNodeAnimatorCameraMayaRH::~CSceneNodeAnimatorCameraMayaRH()
{
	if (CursorControl)
		CursorControl->drop();
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addMeshViewerCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool CSceneNodeAnimatorCameraMayaRH::OnEvent(const SEvent& event)
{
	if (EET_MOUSE_INPUT_EVENT == event.EventType)
	{
		switch(event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			MouseKeys[0] = true;
			break;
		case EMIE_RMOUSE_PRESSED_DOWN:
			MouseKeys[2] = true;
			break;
		case EMIE_MMOUSE_PRESSED_DOWN:
			MouseKeys[1] = true;
			break;
		case EMIE_LMOUSE_LEFT_UP:
			MouseKeys[0] = false;
			break;
		case EMIE_RMOUSE_LEFT_UP:
			MouseKeys[2] = false;
			break;
		case EMIE_MMOUSE_LEFT_UP:
			MouseKeys[1] = false;
			break;
		case EMIE_MOUSE_MOVED:
			MousePos = CursorControl->getRelativePosition();
			break;
		case EMIE_MOUSE_WHEEL:
			wheelZoom = event.MouseInput.Wheel;
			break;
		case EMIE_LMOUSE_DOUBLE_CLICK:
		case EMIE_RMOUSE_DOUBLE_CLICK:
		case EMIE_MMOUSE_DOUBLE_CLICK:
		case EMIE_LMOUSE_TRIPLE_CLICK:
		case EMIE_RMOUSE_TRIPLE_CLICK:
		case EMIE_MMOUSE_TRIPLE_CLICK:
		case EMIE_COUNT:
			return false;
		}
	}
	else if ( EET_USER_EVENT == event.EventType )
	{
		switch (event.UserEvent.UserData1)
		{
		case EVT_FRONTVIEW:
			{
				RotX = 0;
				RotY = 0;
			}break;
		case EVT_HALFVIEW:
			{
				RotX = 45;
				RotY = 0;
			}break;
		case EVT_TOPVIEW:
			{
				RotX = 90;
				RotY = 0;
			}break;
		default:return false;
		}
	}
	else if ( EET_KEY_INPUT_EVENT == event.EventType )
	{
		switch (event.KeyInput.Key)
		{
		case KEY_UP:
			{
				KeyboardKeys[0] = event.KeyInput.PressedDown;
			}break;
		case KEY_DOWN:
			{
				KeyboardKeys[1] = event.KeyInput.PressedDown;
			}break;
		case KEY_LEFT:
			{
				KeyboardKeys[2] = event.KeyInput.PressedDown;
			}break;
		case KEY_RIGHT:
			{
				KeyboardKeys[3] = event.KeyInput.PressedDown;
			}break;
		}
	}

	
	return false;
}


//! OnAnimate() is called just before rendering the whole scene.
void CSceneNodeAnimatorCameraMayaRH::animateNode(ISceneNode *node, u32 timeMs)
{
	//Alt + LM = Rotate around camera pivot
	//Alt + LM + MM = Dolly forth/back in view direction (speed % distance camera pivot - max distance to pivot)
	//Alt + MM = Move on camera plane (Screen center is about the mouse pointer, depending on move speed)

	if (!node || node->getType() != ESNT_CAMERA)
		return;

	ICameraSceneNode* camera = static_cast<ICameraSceneNode*>(node);

	// If the camera isn't the active camera, and receiving input, then don't process it.
	if (!camera->isInputReceiverEnabled())
		return;

	scene::ISceneManager * smgr = camera->getSceneManager();
	if (smgr && smgr->getActiveCamera() != camera)
		return;

	if (OldCamera != camera)
	{
		LastCameraTarget = OldTarget = camera->getTarget();
		OldCamera = camera;
	}
	else
	{
		OldTarget += camera->getTarget() - LastCameraTarget;
	}

	f32 nRotY = RotY;
	f32 nRotX = RotX;
	f32 nZoom = CurrentZoom;

	if ( wheelZoom > 0 )
	{
		if ( CurrentZoom < 10000 )
		{
			if ( CurrentZoom < 5000 )
			{
				CurrentZoom += 100;
				nZoom = CurrentZoom;
			}
			else
			{
				core::vector3df translate(OldTarget);

				core::vector3df dir = OldTarget - camera->getPosition();
				dir.normalize();
				core::vector3df xdir = dir.crossProduct(camera->getUpVector());

				translate -= 100.f*dir;
				OldTarget = translate;
			}
			
		}
	}
	else if ( wheelZoom < 0 )
	{
		if ( CurrentZoom > 50 )
		{
			if ( CurrentZoom > 200 )
			{
				CurrentZoom -= 100;
			}
			else
			{
				core::vector3df translate(OldTarget);

				core::vector3df dir = OldTarget - camera->getPosition();
				dir.normalize();
				core::vector3df xdir = dir.crossProduct(camera->getUpVector());

				translate += 100.f*dir;
				OldTarget = translate;
			}
		}
		nZoom = CurrentZoom;
	}
	wheelZoom = 0;

	// Translation ---------------------------------

	core::vector3df translate(OldTarget);
	const core::vector3df upVector(camera->getUpVector());
	const core::vector3df target = camera->getTarget();

	core::vector3df pos = camera->getPosition();
	core::vector3df tvectX = pos - target;
	tvectX = tvectX.crossProduct(upVector);
	tvectX.normalize();

	const SViewFrustum* const va = camera->getViewFrustum();
	core::vector3df tvectY = (va->getFarLeftDown() - va->getFarRightDown());
	tvectY = tvectY.crossProduct(upVector.Y > 0 ? pos - target : target - pos);
	tvectY.normalize();

	if (isMouseKeyDown(1) && !Zooming)
	{
		if (!Translating)
		{
			TranslateStart = MousePos;
			Translating = true;
		}
		else
		{
			translate +=  tvectX * (TranslateStart.X - MousePos.X)*TranslateSpeed +
			              tvectY * (TranslateStart.Y - MousePos.Y)*TranslateSpeed;
		}
	}
	else if (Translating)
	{
		translate += tvectX * (TranslateStart.X - MousePos.X)*TranslateSpeed +
		             tvectY * (TranslateStart.Y - MousePos.Y)*TranslateSpeed;
		OldTarget = translate;
		Translating = false;
	}

	if ( KeyboardKeys[0] || KeyboardKeys[1] || KeyboardKeys[2] || KeyboardKeys[3] )
	{
		core::vector3df pos = camera->getPosition();
		core::vector3df dir = translate - pos;
		dir.normalize();
		core::vector3df xdir = dir.crossProduct(camera->getUpVector());

		if ( KeyboardKeys[0] )
		{
			translate += 15.f*dir;
		}

		if ( KeyboardKeys[1] )
		{
			translate -= 15.f*dir;
		}

		if ( KeyboardKeys[2] )
		{
			translate -= 15.f*xdir;
		}

		if ( KeyboardKeys[3] )
		{
			translate += 15.f*xdir;
		}

		OldTarget = translate;
	}

	// Rotation ------------------------------------

	if (isMouseKeyDown(0) && !Zooming)
	{
		if (!Rotating)
		{
			RotateStart = MousePos;
			Rotating = true;
			if ( RotX > 90 )
			{
				RotX = 90;
			}
			else if ( RotX < -90 )
			{
				RotX = -90;
			}
			nRotY = RotY;
			nRotX = RotX;
		}
		else
		{
			nRotY += (RotateStart.X - MousePos.X) * RotateSpeed;
			nRotX += (RotateStart.Y - MousePos.Y) * RotateSpeed;
			if ( nRotX > 90 )
			{
				nRotX = 90;
			}
			else if ( nRotX < -90 )
			{
				nRotX = -90;
			}
		}
	}
	else if (Rotating)
	{
		RotY += (RotateStart.X - MousePos.X) * RotateSpeed;
		RotX += (RotateStart.Y - MousePos.Y) * RotateSpeed;
		if ( RotX > 90 )
		{
			RotX = 90;
		}
		else if ( RotX < -90 )
		{
			RotX = -90;
		}
		nRotY = RotY;
		nRotX = RotX;
		Rotating = false;
	}

	// Set pos ------------------------------------

	pos = translate;
	pos.Z += nZoom;

	pos.rotateYZBy(-nRotX, translate);
	pos.rotateXZBy(nRotY, translate);

	camera->setPosition(pos);
	camera->setTarget(translate);

	// Rotation Error ----------------------------

	// jox: fixed bug: jitter when rotating to the top and bottom of y
	pos.set(0,1,0);
	pos.rotateYZBy(nRotX);
	pos.rotateXZBy(nRotY+180.f);
	camera->setUpVector(pos);
	LastCameraTarget = camera->getTarget();

// 	std::string str = "Pos:" + std::to_string((long double)pos.X) + " " + std::to_string((long double)pos.Y) + " " + std::to_string((long double)pos.Z);
// 	irr::os::Printer::print(str.c_str());
}


bool CSceneNodeAnimatorCameraMayaRH::isMouseKeyDown(s32 key) const
{
	return MouseKeys[key];
}


void CSceneNodeAnimatorCameraMayaRH::allKeysUp()
{
	for (s32 i=0; i<3; ++i)
		MouseKeys[i] = false;

	for (s32 i=0; i<4; ++i)
	{
		KeyboardKeys[i] = false;
	}
}


//! Sets the rotation speed
void CSceneNodeAnimatorCameraMayaRH::setRotateSpeed(f32 speed)
{
	RotateSpeed = speed;
}


//! Sets the movement speed
void CSceneNodeAnimatorCameraMayaRH::setMoveSpeed(f32 speed)
{
	TranslateSpeed = speed;
}


//! Sets the zoom speed
void CSceneNodeAnimatorCameraMayaRH::setZoomSpeed(f32 speed)
{
	ZoomSpeed = speed;
}


//! Set the distance
void CSceneNodeAnimatorCameraMayaRH::setDistance(f32 distance)
{
	CurrentZoom=distance;
}

		
//! Gets the rotation speed
f32 CSceneNodeAnimatorCameraMayaRH::getRotateSpeed() const
{
	return RotateSpeed;
}


// Gets the movement speed
f32 CSceneNodeAnimatorCameraMayaRH::getMoveSpeed() const
{
	return TranslateSpeed;
}


//! Gets the zoom speed
f32 CSceneNodeAnimatorCameraMayaRH::getZoomSpeed() const
{
	return ZoomSpeed;
}


//! Returns the current distance, i.e. orbit radius
f32 CSceneNodeAnimatorCameraMayaRH::getDistance() const
{
	return CurrentZoom;
}


ISceneNodeAnimator* CSceneNodeAnimatorCameraMayaRH::createClone(ISceneNode* node, ISceneManager* newManager)
{
	CSceneNodeAnimatorCameraMayaRH * newAnimator =
		new CSceneNodeAnimatorCameraMayaRH(CursorControl, RotateSpeed, ZoomSpeed, TranslateSpeed);
	return newAnimator;
}

} // end namespace
} // end namespace

