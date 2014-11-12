#include "stdafx.h"

#include "BaseODL.h"

#include "gp_Quaternion.hxx"

BaseODL::BaseODL( const SRenderContextWPtr& rc ):TSceneNode(rc)
{
	Dirty_ = false;
	Sweeping_ = false;
	Picking_ = false;
}

BaseODL::~BaseODL( void )
{

}

EObjectDisplayLayerType BaseODL::GetType() const
{
	return EODLT_BASE;
}

void BaseODL::UpdateShapeAndDataSceneNode()
{
	UpdateShape();

	if ( GetDataSceneNode() )
	{
		auto& trans = GetTranslation();
		auto& rotate = GetRotation();
		auto& scale = GetScale();
		
		Standard_Real rx,ry,rz;
		rotate.GetEulerAngles(gp_Extrinsic_XYZ, rx, ry, rz);

		GetDataSceneNode()->setPosition(irr::core::vector3df(static_cast<float>(trans.X()), static_cast<float>(trans.Y()), static_cast<float>(trans.Z())));
		GetDataSceneNode()->setRotation(irr::core::vector3df(static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rz)));
		GetDataSceneNode()->setScale(irr::core::vector3df(static_cast<float>(scale.X()), static_cast<float>(scale.Y()), static_cast<float>(scale.Z())));
	}
}

void BaseODL::UpdateAbsoluteTransform()
{
	if ( GetParent().expired() )
	{
		AbsoluteTransform_ =  GetTransform();
	}
	else
	{
		AbsoluteTransform_ = GetParent().lock()->GetAbsoluteTransform() * GetTransform();
	}
}
