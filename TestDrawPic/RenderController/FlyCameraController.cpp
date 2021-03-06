#include "stdafx.h"

#include "FlyCameraController.h"
#include "ISceneManager.h"

FlyCameraController::FlyCameraController(const SRenderContextWPtr& rc):IRenderController(rc)
{
	ActiveCamera_ = nullptr;
	ToSwitchCamera_ = nullptr;
	Animator_ = nullptr;
}

bool FlyCameraController::IsFlying() const
{
	return ActiveCamera_ && ToSwitchCamera_ && Animator_;
}

void FlyCameraController::SetFlying( irr::scene::ICameraSceneNode* toSwitchCamera, irr::scene::ICameraSceneNode* activeCamera, irr::scene::ISceneNodeAnimatorFinishing* animator )
{
	assert(toSwitchCamera && activeCamera && animator);

	ToSwitchCamera_ = toSwitchCamera;
	ActiveCamera_ = activeCamera;
	Animator_ = animator;
}

bool FlyCameraController::PreRender3D()
{
	if ( IsFlying() )
	{
		if ( Animator_->hasFinished() )
		{
			ToSwitchCamera_->getSceneManager()->setActiveCamera(ToSwitchCamera_);
			ActiveCamera_->remove();

			ActiveCamera_ = nullptr;
			ToSwitchCamera_ = nullptr;
			Animator_ = nullptr;
		}
	}

	return false;
}

bool FlyCameraController::OnPreEvent( const irr::SEvent& event )
{
	if ( IsFlying() )
	{
		return true;
	}

	return false;
}

void FlyCameraController::PostRender3D()
{

}
