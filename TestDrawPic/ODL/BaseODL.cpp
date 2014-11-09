#include "stdafx.h"

#include "BaseODL.h"

#include "gp_Quaternion.hxx"

CBaseODL::CBaseODL( const SRenderContextWPtr& rc ):TSceneNode(rc)
{
	Dirty_ = false;
	Sweeping_ = false;
	Picking_ = false;
}

CBaseODL::~CBaseODL( void )
{

}

EObjectDisplayLayerType CBaseODL::GetType() const
{
	return EODLT_BASE;
}

void CBaseODL::UpdateShapeAndDataSceneNode()
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

void CBaseODL::UpdateAbsoluteTransform()
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
