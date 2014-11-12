#include "stdafx.h"

#include "UpdateTransformingCtrller.h"

#include "ODL/BaseODL.h"

void	Update(BaseODLSPtr& odl)
{
	for ( auto& curChild : odl->GetChildrenList() )
	{
		curChild->UpdateAbsoluteTransform();
		Update(curChild);
	}
}


UpdateTransformingCtrller::UpdateTransformingCtrller()
{

}

UpdateTransformingCtrller::~UpdateTransformingCtrller()
{

}

bool UpdateTransformingCtrller::PreRender3D()
{
	assert(!RootODL_.expired());

	auto rootODL = RootODL_.lock();
	Update(rootODL);

	return false;
}

