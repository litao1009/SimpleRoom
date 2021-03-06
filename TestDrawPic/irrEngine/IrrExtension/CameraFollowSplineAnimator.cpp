#include "stdafx.h"

#include "CameraFollowSplineAnimator.h"
#include "ICameraSceneNode.h"

using namespace irr;
using namespace scene;

//! constructor
CCameraFollowSplineAnimator::CCameraFollowSplineAnimator(u32 time,
const irr::core::array< irr::core::vector3df >& posPoints,
const irr::core::array< irr::core::vector3df >& targetPoints,
const irr::core::array< irr::core::vector3df >& UpPoints,
irr::f32 posSpeed, irr::f32 targetSpeed, irr::f32 upSpeed,
f32 tightness, bool pingpong)
: ISceneNodeAnimatorFinishing(0),
PositionPoints(posPoints),
PositionSpeed(posSpeed),
TargetSpeed(targetSpeed),
TargetPoints(targetPoints),
UpVectors(UpPoints),
UpSpeed(upSpeed),
Tightness(tightness),
StartTime(time)
,PingPong(pingpong)
{
	#ifdef _DEBUG
	setDebugName("CCameraFollowSplineAnimator");
	#endif
}


inline s32 CCameraFollowSplineAnimator::clamp(s32 idx, s32 size)
{
	return ( idx<0 ? size+idx : ( idx>=size ? idx-size : idx ) );
}


//! animates a scene node
void CCameraFollowSplineAnimator::animateNode(ISceneNode* node, u32 timeMs)
{
	if(!node)
	{
		return;
	}

	if ( HasFinished )
	{
		return;
	}

	assert(node->getType() == ESNT_CAMERA);

	auto cameraNode = static_cast<ICameraSceneNode*>(node);

	auto posInfo = _AnimateVec(timeMs, PositionPoints, PositionSpeed);
	auto targetInfo = _AnimateVec(timeMs, TargetPoints, TargetSpeed);
	auto upInfo = _AnimateVec(timeMs, UpVectors, UpSpeed);

	HasFinished = std::get<0>(posInfo) && std::get<0>(targetInfo) && std::get<0>(upInfo);

	auto rawPos = cameraNode->getPosition();
	auto rawTarget = cameraNode->getTarget();

	auto pos = std::get<1>(posInfo);
	if ( pos )
	{
		cameraNode->setPosition(*pos);
	}

	auto target = std::get<1>(targetInfo);
	if ( target )
	{
		cameraNode->setTarget(*target);
	}

	auto up = std::get<1>(upInfo);
	if ( up )
	{
		cameraNode->setUpVector(*up);
	}
}


//! Writes attributes of the scene node animator.
void CCameraFollowSplineAnimator::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
{
	
}


//! Reads attributes of the scene node animator.
void CCameraFollowSplineAnimator::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
{
	
}


ISceneNodeAnimator* CCameraFollowSplineAnimator::createClone(ISceneNode* node, ISceneManager* newManager)
{
	auto newAnimator =	new CCameraFollowSplineAnimator(StartTime, PositionPoints, TargetPoints, UpVectors, PositionSpeed, TargetSpeed, UpSpeed, Tightness);

	return newAnimator;
}

std::tuple<bool,boost::optional<irr::core::vector3df>> CCameraFollowSplineAnimator::_AnimateVec( irr::u32 timeMs, const irr::core::array<irr::core::vector3df>& points, irr::f32 speed )
{
	const u32 pSize = points.size();
	if (pSize==0)
	{
		return std::make_tuple(true, boost::none);
	}
	if (pSize==1)
	{
		if ( timeMs > StartTime )
		{
			return std::make_tuple(true, points[0]);
		}
		else
		{
			return std::make_tuple(false, boost::none);
		}
	}

	const f32 dt = ( (timeMs-StartTime) * speed * 0.001f );
	const s32 unwrappedIdx = core::floor32( dt );
	if ( unwrappedIdx >= (s32)pSize-1 )
	{
		return std::make_tuple(true, points[pSize-1]);
	}

	const bool pong = PingPong && (unwrappedIdx/(pSize-1))%2;
	const f32 u =  pong ? 1.f-core::fract ( dt ) : core::fract ( dt );
	const s32 idx = pong ?	(pSize-2) - (unwrappedIdx % (pSize-1))
		: (PingPong ? unwrappedIdx % (pSize-1)
		: unwrappedIdx % pSize);
	//const f32 u = 0.001f * fmodf( dt, 1000.0f );

	const core::vector3df& p0 = points[ clamp( idx - 1, pSize ) ];
	const core::vector3df& p1 = points[ clamp( idx + 0, pSize ) ]; // starting point
	const core::vector3df& p2 = points[ clamp( idx + 1, pSize ) ]; // end point
	const core::vector3df& p3 = points[ clamp( idx + 2, pSize ) ];

	// hermite polynomials
	const f32 h1 = 2.0f * u * u * u - 3.0f * u * u + 1.0f;
	const f32 h2 = -2.0f * u * u * u + 3.0f * u * u;
	const f32 h3 = u * u * u - 2.0f * u * u + u;
	const f32 h4 = u * u * u - u * u;

	// tangents
	const core::vector3df t1 = ( p2 - p0 ) * Tightness;
	const core::vector3df t2 = ( p3 - p1 ) * Tightness;

	return std::make_tuple(false, p1 * h1 + p2 * h2 + t1 * h3 + t2 * h4);
}
