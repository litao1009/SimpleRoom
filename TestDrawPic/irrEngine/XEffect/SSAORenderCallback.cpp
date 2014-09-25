#include "stdafx.h"

#include "SSAORenderCallback.h"
#include "irrlicht.h"
#include "EffectHandler.h"

void SSAORenderCallback::OnPreRender( EffectHandler* effect )
{
	auto driver = effect->getIrrlichtDevice()->getVideoDriver();
	viewProj = driver->getTransform(irr::video::ETS_PROJECTION) * driver->getTransform(irr::video::ETS_VIEW);
	effect->setPostProcessingEffectConstant(materialType, "mViewProj", viewProj.pointer(), 16);
}