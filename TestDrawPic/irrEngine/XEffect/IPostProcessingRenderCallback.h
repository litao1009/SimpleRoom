#ifndef IPostProcessingRenderCallback_h__
#define IPostProcessingRenderCallback_h__

#pragma once

// This is a general interface that can be overidden if you want to perform operations before or after
// a specific post-processing effect. You will be passed an instance of the EffectHandler.
// The function names themselves should be self-explanatory ;)
class EffectHandler;
class IPostProcessingRenderCallback
{
public:
	virtual void OnPreRender(EffectHandler* effect) = 0;
	virtual void OnPostRender(EffectHandler* effect) = 0;
};

#endif // IPostProcessingRenderCallback_h__
