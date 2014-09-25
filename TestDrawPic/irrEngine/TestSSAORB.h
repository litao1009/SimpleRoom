#pragma once

#include "XEffect/SSAORenderCallback.h"
#include <memory>

class	CDlgShader;
class	TestSSAORB : public SSAORenderCallback
{
public:

	TestSSAORB(irr::s32 materialTypeIn):SSAORenderCallback(materialTypeIn) {}

	void OnPreRender(EffectHandler* effect);

	std::shared_ptr<CDlgShader>	Dlg_;
};