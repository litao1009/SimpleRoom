#ifndef SimpleOrthoAnimator_h__
#define SimpleOrthoAnimator_h__

#pragma once

#include "ISceneNodeAnimatorCameraFPS.h"
#include "vector2d.h"
#include "position2d.h"
#include "SKeyMap.h"
#include "irrArray.h"

namespace irr
{
namespace gui
{
	class ICursorControl;
}
}

//! Special scene node animator for FPS cameras
class CSimpleOrthoAnimator : public irr::scene::ISceneNodeAnimatorCameraFPS
{
public:

	//! Constructor
	CSimpleOrthoAnimator(irr::gui::ICursorControl* cursorControl,
		irr::f32 rotateSpeed = 100.0f, irr::f32 moveSpeed = 2.f);

	//! Destructor
	virtual ~CSimpleOrthoAnimator();

	//! Animates the scene node, currently only works on cameras
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

	//! Event receiver
	virtual bool OnEvent(const irr::SEvent& event);

	//! Returns the speed of movement in units per second
	virtual irr::f32 getMoveSpeed() const;

	//! Sets the speed of movement in units per second
	virtual void setMoveSpeed(irr::f32 moveSpeed);

	//! Returns the rotation speed
	virtual irr::f32 getRotateSpeed() const;

	//! Set the rotation speed
	virtual void setRotateSpeed(irr::f32 rotateSpeed);

	//! Sets the keyboard mapping for this animator (old style)
	//! \param keymap: an array of keyboard mappings, see SKeyMap
	//! \param count: the size of the keyboard map array
	virtual void setKeyMap(irr::SKeyMap *map, irr::u32 count);

	//! Sets the keyboard mapping for this animator
	//!	\param keymap The new keymap array 
	virtual void setKeyMap(const irr::core::array<irr::SKeyMap>& keymap);

	//! Gets the keyboard mapping for this animator
	virtual const irr::core::array<irr::SKeyMap>& getKeyMap() const;

	//! Sets whether vertical movement should be allowed.
	virtual void setVerticalMovement(bool allow);

	//! Sets whether the Y axis of the mouse should be inverted.
	/** If enabled then moving the mouse down will cause
	the camera to look up. It is disabled by default. */
	virtual void setInvertMouse(bool invert);

	//! This animator will receive events when attached to the active camera
	virtual bool isEventReceiverEnabled() const
	{
		return true;
	}

	//! Returns the type of this animator
	virtual irr::scene::ESCENE_NODE_ANIMATOR_TYPE getType() const
	{
		return irr::scene::ESNAT_CAMERA_FPS;
	}

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer once you're
	done with it. */
	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager=0);

private:
	void allKeysUp();

	irr::gui::ICursorControl *CursorControl;

	irr::f32 MaxVerticalAngle;

	irr::f32 MoveSpeed;
	irr::f32 RotateSpeed;
	// -1.0f for inverted mouse, defaults to 1.0f
	irr::f32 MouseYDirection;

	irr::s32 LastAnimationTime;

	irr::core::array<irr::SKeyMap> KeyMap;
	irr::core::position2df CenterCursor, CursorPos;
	irr::core::position2di CursorIPos;

	bool CursorKeys[irr::EKA_COUNT];
	bool MouseKeys[3];

	bool firstUpdate;
	bool ResetMouse;
	bool firstInput;
};

#endif // SimpleOrthoAnimator_h__