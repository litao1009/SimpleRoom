#include "stdafx.h"

#include "FlyCameraController.h"
#include "ISceneManager.h"

FlyCameraController::FlyCameraController()
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

bool FlyCameraController::PreRender3D( const SRenderContext& rc )
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

	return true;
}
