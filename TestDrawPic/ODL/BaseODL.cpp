#include "stdafx.h"

#include "BaseODL.h"

CBaseODL::CBaseODL( void )
{
	Selected_ = false;
}

CBaseODL::~CBaseODL( void )
{

}

EObjectDisplayLayerType CBaseODL::GetType() const
{
	return EODLT_BASE;
}
