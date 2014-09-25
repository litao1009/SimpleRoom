#include "stdafx.h"

#include "SimpleFPSAnimator.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "Keycodes.h"
#include "ICursorControl.h"
#include "ICameraSceneNode.h"
#include "ISceneNodeAnimatorCollisionResponse.h"

//! constructor
CSimpleFPSAnimator::CSimpleFPSAnimator(irr::gui::ICursorControl* cursorControl,irr::f32 rotateSpeed, irr::f32 moveSpeed)
: CursorControl(cursorControl), MaxVerticalAngle(88.0f),
	MoveSpeed(moveSpeed), RotateSpeed(rotateSpeed), JumpSpeed(0),
	MouseYDirection(1.0f),
	LastAnimationTime(0), firstUpdate(true), firstInput(true), ResetMouse(false)
{
	#ifdef _DEBUG
	setDebugName("CSimpleFPSAnimator");
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
CSimpleFPSAnimator::~CSimpleFPSAnimator()
{
	if (CursorControl)
		CursorControl->drop();
}


//! It is possible to send mouse and key events to the camera. Most cameras
//! may ignore this input, but camera scene nodes which are created for
//! example with scene::ISceneManager::addMayaCameraSceneNode or
//! scene::ISceneManager::addFPSCameraSceneNode, may want to get this input
//! for changing their position, look at target or whatever.
bool CSimpleFPSAnimator::OnEvent(const irr::SEvent& evt)
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
		case irr::EMIE_RMOUSE_PRESSED_DOWN:
			MouseKeys[2] = true;
			break;
		case irr::EMIE_MMOUSE_PRESSED_DOWN:
			CursorControl->setVisible(false);
			MouseKeys[1] = true;
			ResetMouse = true;
			break;
		case irr::EMIE_LMOUSE_LEFT_UP:
			MouseKeys[0] = false;
			break;
		case irr::EMIE_RMOUSE_LEFT_UP:
			MouseKeys[2] = false;
			break;
		case irr::EMIE_MMOUSE_LEFT_UP:
			CursorControl->setVisible(true);
			MouseKeys[1] = false;
			break;
		case irr::EMIE_MOUSE_MOVED:
			CursorPos = CursorControl->getRelativePosition();
			break;
		case irr::EMIE_MOUSE_WHEEL:
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


void CSimpleFPSAnimator::animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs)
{
	if (!node || node->getType() != irr::scene::ESNT_CAMERA)
	{
		return;
	}

	auto camera = static_cast<irr::scene::ICameraSceneNode*>(node);

	// update position
	auto pos = camera->getPosition();

	// Update rotation
	auto target = (camera->getTarget() - camera->getAbsolutePosition());
	auto relativeRotation = target.getHorizontalAngle();

	if ( firstUpdate )
	{
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
				relativeRotation.Y -= -(0.5f - CursorPos.X) * RotateSpeed;
				relativeRotation.X -= (0.5f - CursorPos.Y) * RotateSpeed * MouseYDirection;

				// X < MaxVerticalAngle or X > 360-MaxVerticalAngle

				if (relativeRotation.X > MaxVerticalAngle*2 &&
					relativeRotation.X < 360.0f-MaxVerticalAngle)
				{
					relativeRotation.X = 360.0f-MaxVerticalAngle;
				}
				else
					if (relativeRotation.X > MaxVerticalAngle &&
						relativeRotation.X < 360.0f-MaxVerticalAngle)
					{
						relativeRotation.X = MaxVerticalAngle;
					}

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

	// set target

	target.set(0,0, target.getLength());
	auto movedir = target;

	irr::core::matrix4 mat;
	mat.setRotationDegrees(irr::core::vector3df(relativeRotation.X, relativeRotation.Y, 0));
	mat.transformVect(target);

	mat.setRotationDegrees(irr::core::vector3df(0, relativeRotation.Y, 0));
	mat.transformVect(movedir);

	movedir.normalize();

	if (CursorKeys[irr::EKA_MOVE_FORWARD])
		pos += movedir * timeDiff * MoveSpeed;

	if (CursorKeys[irr::EKA_MOVE_BACKWARD])
		pos -= movedir * timeDiff * MoveSpeed;

	// strafing

	auto strafevect = target;
	strafevect = strafevect.crossProduct(camera->getUpVector());

	strafevect.Y = 0.0f;

	strafevect.normalize();

	if (CursorKeys[irr::EKA_STRAFE_LEFT])
		pos -= strafevect * timeDiff * MoveSpeed;

	if (CursorKeys[irr::EKA_STRAFE_RIGHT])
		pos += strafevect * timeDiff * MoveSpeed;

	// write translation
	camera->setPosition(pos);

	// write right target
	target += pos;
	camera->setTarget(target);
}


void CSimpleFPSAnimator::allKeysUp()
{
	for (irr::u32 i=0; i<irr::EKA_COUNT; ++i)
		CursorKeys[i] = false;

	for (irr::u32 i=0; i<3; ++i)
		MouseKeys[i] = false;
}


//! Sets the rotation speed
void CSimpleFPSAnimator::setRotateSpeed(irr::f32 speed)
{
	RotateSpeed = speed;
}


//! Sets the movement speed
void CSimpleFPSAnimator::setMoveSpeed(irr::f32 speed)
{
	MoveSpeed = speed;
}


//! Gets the rotation speed
irr::f32 CSimpleFPSAnimator::getRotateSpeed() const
{
	return RotateSpeed;
}


// Gets the movement speed
irr::f32 CSimpleFPSAnimator::getMoveSpeed() const
{
	return MoveSpeed;
}


//! Sets the keyboard mapping for this animator
void CSimpleFPSAnimator::setKeyMap(irr::SKeyMap *map, irr::u32 count)
{
	
}

void CSimpleFPSAnimator::setKeyMap(const irr::core::array<irr::SKeyMap>& keymap)
{
	
}

const irr::core::array<irr::SKeyMap>& CSimpleFPSAnimator::getKeyMap() const
{
	return KeyMap;
}


//! Sets whether vertical movement should be allowed.
void CSimpleFPSAnimator::setVerticalMovement(bool allow)
{
	
}


//! Sets whether the Y axis of the mouse should be inverted.
void CSimpleFPSAnimator::setInvertMouse(bool invert)
{
	
}


irr::scene::ISceneNodeAnimator* CSimpleFPSAnimator::createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager)
{
	auto newAnimator = new CSimpleFPSAnimator(CursorControl,	RotateSpeed, MoveSpeed);
	newAnimator->setKeyMap(KeyMap);
	return newAnimator;
}