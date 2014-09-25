#ifndef SSAORenderCallback_h__
#define SSAORenderCallback_h__

#pragma once

#include "IPostProcessingRenderCallback.h"
#include "irrTypes.h"
#include "matrix4.h"

// We need to pass the view projection matrix to the SSAO shader, so we create a class that inherits from
// IPostProcessingRenderCallback and pass the shader constant in OnPreRender using setPostProcessingEffectConstant.
class SSAORenderCallback : public IPostProcessingRenderCallback
{
public:
	SSAORenderCallback(irr::s32 materialTypeIn) : materialType(materialTypeIn) {}

	void OnPreRender(EffectHandler* effect);

	void OnPostRender(EffectHandler* effect) {}

	irr::core::matrix4 viewProj;
	irr::s32 materialType;
};

#endif // SSAORenderCallback_h__
