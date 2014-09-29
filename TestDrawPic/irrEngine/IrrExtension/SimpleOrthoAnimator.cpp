#include "stdafx.h"

#include "SimpleOrthoAnimator.h"

#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "Keycodes.h"
#include "ICursorControl.h"
#include "ICameraSceneNode.h"
#include "ISceneNodeAnimatorCollisionResponse.h"
#include "ISceneCollisionManager.h"

static auto MouseWheel = 0.f;

//! constructor
CSimpleOrthoAnimator::CSimpleOrthoAnimator(irr::gui::ICursorControl* cursorControl,irr::f32 rotateSpeed, irr::f32 moveSpeed)
	: CursorControl(cursorControl), MaxVerticalAngle(88.0f),
	MoveSpeed(moveSpeed), RotateSpeed(rotateSpeed),
	MouseYDirection(-1.0f),
	LastAnimationTime(0), firstUpdate(true), firstInput(true), ResetMouse(false)
{
#ifdef _DEBUG
	setDebugName("CSimpleMayaAnimator");
#endif

	if (CursorControl)
		CursorControl->grab();

	allKeysUp();

	// create default key map
	KeyMap.push_back(irr::SKeyMap(irr::EKA_MOVE_FORWARD, irr::KEY_KEY_W));
	KeyMap.push_back(irr::SKeyMap(irr::EKA_MOVE_BACKWARD, irr::KEY_KEY_S));
	KeyMap.push_back(irr::SKeyMap(irr::EKA_STRAFE_LEFT, irr::KEY_KEY_A));
	KeyMap.push_back(irr::SKeyMap(irr::EKA_STRAFE_RIGHT, irr::KEY_KEY_D));
}


//! destructor
CSimpleOrthoAnimator::~CSimpleOrthoAnimator()
{
	if (CursorControl)
		CursorControl->drop();
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addFPSCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool CSimpleOrthoAnimator::OnEvent(const irr::SEvent& evt)
{
	switch(evt.EventType)
	{
	case irr::EET_KEY_INPUT_EVENT:
		for (irr::u32 i=0; i<KeyMap.size(); ++i)
		{
			if (KeyMap[i].KeyCode == evt.KeyInput.Key)
			{
				CursorKeys[KeyMap[i].Action] = evt.KeyInput.PressedDown;
				return true;
			}
		}
		break;

	case irr::EET_MOUSE_INPUT_EVENT:
		switch (evt.MouseInput.Event)
		{
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			MouseKeys[0] = true;
			break;
		case irr::EMIE_MMOUSE_PRESSED_DOWN:
			CursorControl->setVisible(false);
			MouseKeys[1] = true;
			ResetMouse = true;
			break;
		case irr::EMIE_RMOUSE_PRESSED_DOWN:
			MouseKeys[2] = true;
			break;
		case irr::EMIE_LMOUSE_LEFT_UP:
			MouseKeys[0] = false;
			break;
		case irr::EMIE_MMOUSE_LEFT_UP:
			CursorControl->setVisible(true);
			MouseKeys[1] = false;
			break;
		case irr::EMIE_RMOUSE_LEFT_UP:
			MouseKeys[2] = false;
			break;
		case irr::EMIE_MOUSE_MOVED:
			CursorPos = CursorControl->getRelativePosition();
			CursorIPos.X = CursorControl->getPosition().X;
			CursorIPos.Y = CursorControl->getPosition().Y;
			break;
		case irr::EMIE_MOUSE_WHEEL:
			MouseWheel = evt.MouseInput.Wheel;
		case irr::EMIE_LMOUSE_DOUBLE_CLICK:
		case irr::EMIE_RMOUSE_DOUBLE_CLICK:
		case irr::EMIE_MMOUSE_DOUBLE_CLICK:
		case irr::EMIE_LMOUSE_TRIPLE_CLICK:
		case irr::EMIE_RMOUSE_TRIPLE_CLICK:
		case irr::EMIE_MMOUSE_TRIPLE_CLICK:
		case irr::EMIE_COUNT:
			return false;
		}

	default:
		break;
	}

	return false;
}


void CSimpleOrthoAnimator::animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs)
{
	if (!node || node->getType() != irr::scene::ESNT_CAMERA)
	{
		return;
	}

	auto camera = static_cast<irr::scene::ICameraSceneNode*>(node);

	auto pos = camera->getPosition();
	auto target = camera->getTarget();
	auto tarToPos = (camera->getAbsolutePosition() - camera->getTarget());

	if ( firstUpdate )
	{
		target.Y = 0;

		camera->updateAbsolutePosition();
		LastAnimationTime = timeMs;
		firstUpdate = false;
	}

	// get time
	irr::f32 timeDiff = (irr::f32) ( timeMs - LastAnimationTime );
	if ( timeDiff > 100 || timeDiff < 0 )
	{
		timeDiff = 16;
	}
	LastAnimationTime = timeMs;

	if ( ResetMouse )
	{
		if (CursorControl )
		{
			CursorControl->setPosition(0.5f, 0.5f);
			CursorPos = CenterCursor = CursorControl->getRelativePosition();
		}
		ResetMouse = false;
	}

	if ( MouseKeys[1] )
	{
		auto smgr = camera->getSceneManager();
		if(smgr && smgr->getActiveCamera() != camera)
		{
			return;
		}

		if (CursorControl)
		{
			if (CursorPos != CenterCursor)
			{
				auto center = (smgr->getVideoDriver()->getViewPort().UpperLeftCorner + smgr->getVideoDriver()->getViewPort().LowerRightCorner)/2;
				irr::core::plane3df zeroPlane(irr::core::vector3df(0,0,0), irr::core::vector3df(0,1,0));

				irr::core::vector3df mousePos;
				{
					auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(CursorIPos, camera);
					auto ret = zeroPlane.getIntersectionWithLine(line.start, line.getVector(), mousePos);
					assert(ret);
				}
				irr::core::vector3df centerPos;
				{
					auto line = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(center, camera);
					auto ret = zeroPlane.getIntersectionWithLine(line.start, line.getVector(), centerPos);
					assert(ret);
				}			

				auto ralationVec = centerPos - mousePos;
				pos += ralationVec;
				target += ralationVec;

				// Do the fix as normal, special case below
				// reset cursor position to the centre of the window.
				CursorControl->setPosition(0.5f, 0.5f);
				CenterCursor = CursorControl->getRelativePosition();

				// needed to avoid problems when the event receiver is disabled
				CursorPos = CenterCursor;
			}

			// Special case, mouse is whipped outside of window before it can update.
			auto driver = smgr->getVideoDriver();
			irr::core::vector2d<irr::u32> mousepos(irr::u32(CursorControl->getPosition().X), irr::u32(CursorControl->getPosition().Y));
			irr::core::rect<irr::u32> screenRect(0, 0, driver->getScreenSize().Width, driver->getScreenSize().Height);

			// Only if we are moving outside quickly.
			bool reset = !screenRect.isPointInside(mousepos);

			if(reset)
			{
				// Force a reset.
				CursorControl->setPosition(0.5f, 0.5f);
				CenterCursor = CursorControl->getRelativePosition();
				CursorPos = CenterCursor;
			}
		}
	}

	auto movedir = camera->getUpVector();
	movedir.normalize();

	if (CursorKeys[irr::EKA_MOVE_FORWARD])
	{
		target += movedir * timeDiff * MoveSpeed;
		pos += movedir * timeDiff * MoveSpeed;
	}

	if (CursorKeys[irr::EKA_MOVE_BACKWARD])
	{
		target -= movedir * timeDiff * MoveSpeed;
		pos -= movedir * timeDiff * MoveSpeed;
	}

	pos += movedir * timeDiff * MouseWheel;
	MouseWheel = 0.f;

	// strafing

	auto strafevect = -tarToPos;
	strafevect = strafevect.crossProduct(camera->getUpVector());

	strafevect.normalize();

	if (CursorKeys[irr::EKA_STRAFE_LEFT])
	{
		pos -= strafevect * timeDiff * MoveSpeed;
		target -= strafevect * timeDiff * MoveSpeed;
	}

	if (CursorKeys[irr::EKA_STRAFE_RIGHT])
	{
		pos += strafevect * timeDiff * MoveSpeed;
		target += strafevect * timeDiff * MoveSpeed;
	}

	camera->setTarget(target);
	camera->setPosition(pos);
}


void CSimpleOrthoAnimator::allKeysUp()
{
	for (irr::u32 i=0; i<irr::EKA_COUNT; ++i)
		CursorKeys[i] = false;

	for (irr::u32 i=0; i<3; ++i)
		MouseKeys[i] = false;
}


//! Sets the rotation speed
void CSimpleOrthoAnimator::setRotateSpeed(irr::f32 speed)
{
	RotateSpeed = speed;
}


//! Sets the movement speed
void CSimpleOrthoAnimator::setMoveSpeed(irr::f32 speed)
{
	MoveSpeed = speed;
}


//! Gets the rotation speed
irr::f32 CSimpleOrthoAnimator::getRotateSpeed() const
{
	return RotateSpeed;
}


// Gets the movement speed
irr::f32 CSimpleOrthoAnimator::getMoveSpeed() const
{
	return MoveSpeed;
}


//! Sets the keyboard mapping for this animator
void CSimpleOrthoAnimator::setKeyMap(irr::SKeyMap *map, irr::u32 count)
{

}

void CSimpleOrthoAnimator::setKeyMap(const irr::core::array<irr::SKeyMap>& keymap)
{

}

const irr::core::array<irr::SKeyMap>& CSimpleOrthoAnimator::getKeyMap() const
{
	return KeyMap;
}


//! Sets whether vertical movement should be allowed.
void CSimpleOrthoAnimator::setVerticalMovement(bool allow)
{

}


//! Sets whether the Y axis of the mouse should be inverted.
void CSimpleOrthoAnimator::setInvertMouse(bool invert)
{

}


irr::scene::ISceneNodeAnimator* CSimpleOrthoAnimator::createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager)
{
	auto newAnimator = new CSimpleOrthoAnimator(CursorControl, RotateSpeed, MoveSpeed);
	newAnimator->setKeyMap(KeyMap);
	return newAnimator;
}