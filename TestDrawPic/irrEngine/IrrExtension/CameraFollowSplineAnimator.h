#ifndef CameraFollowSplineAnimator_h__
#define CameraFollowSplineAnimator_h__

#include "ISceneNode.h"
#include "irrArray.h"
#include "Irrlicht/ISceneNodeAnimatorFinishing.h"

#include <tuple>
#include <boost/optional.hpp>

//! Scene node animator based free code Matthias Gall wrote and sent in. (Most of
//! this code is written by him, I only modified bits.)
class CCameraFollowSplineAnimator : public irr::scene::ISceneNodeAnimatorFinishing
{
public:

	typedef	irr::core::array<irr::core::vector3df>	PointsList;

public:

	//! constructor
	CCameraFollowSplineAnimator(irr::u32 startTime,
		const irr::core::array< irr::core::vector3df >& posPoints,
		const irr::core::array< irr::core::vector3df >& targetPoints,
		const irr::core::array< irr::core::vector3df >& UpVectors,
		irr::f32 posSpeed = 1.0f, irr::f32 targetSpeed = 1.0f, irr::f32 upSpeed = 1.0f,
		irr::f32 tightness = 0.5f, bool pingpong=false);

	//! animates a scene node
	virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs);

	//! Writes attributes of the scene node animator.
	virtual void serializeAttributes(irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options=0) const;

	//! Reads attributes of the scene node animator.
	virtual void deserializeAttributes(irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options=0);

	//! Returns type of the scene node animator
	virtual irr::scene::ESCENE_NODE_ANIMATOR_TYPE getType() const { return irr::scene::ESNAT_FOLLOW_SPLINE; }

	//! Creates a clone of this animator.
	/** Please note that you will have to drop
	(IReferenceCounted::drop()) the returned pointer after calling
	this. */
	virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager=0);

private:

	std::tuple<bool,boost::optional<irr::core::vector3df>>	_AnimateVec(irr::u32 timeMs, const irr::core::array<irr::core::vector3df>& points, irr::f32 spped);

protected:

	//! clamps a the value idx to fit into range 0..size-1
	irr::s32 clamp(irr::s32 idx, irr::s32 size);

	irr::core::array<irr::core::vector3df> PositionPoints;
	irr::core::array<irr::core::vector3df> TargetPoints;
	irr::core::array<irr::core::vector3df> UpVectors;

	irr::f32 PositionSpeed;
	irr::f32 TargetSpeed;
	irr::f32 UpSpeed;

	irr::f32 Tightness;
	irr::u32 StartTime;
	bool PingPong;
};

#endif // CameraFollowSplineAnimator_h__