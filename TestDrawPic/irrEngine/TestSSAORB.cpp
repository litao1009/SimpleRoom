#include "stdafx.h"

#include "TestSSAORB.h"

#include "XEffect/EffectHandler.h"
#include "irrlicht.h"

#include "resource.h"
#include "DlgShader.h"

void TestSSAORB::OnPreRender( EffectHandler* effect )
{
	SSAORenderCallback::OnPreRender(effect);

	if ( !Dlg_ )
	{
		return;
	}

	auto driver = effect->getIrrlichtDevice()->getVideoDriver();

	auto SSAO_REACH = Dlg_->GetSSAOReach();
	effect->setPostProcessingEffectConstant(materialType, "SSAO_REACH", &SSAO_REACH, 1);

	auto MAX_DEPTH_DECLINE = Dlg_->GetMaxDepthDecline();
	effect->setPostProcessingEffectConstant(materialType, "MAX_DEPTH_DECLINE", &MAX_DEPTH_DECLINE, 1);

	auto DEPTH_ALIASING_EPISILON = Dlg_->GetDepthAliasingEpsilon();
	effect->setPostProcessingEffectConstant(materialType, "DEPTH_ALIASING_EPISILON", &DEPTH_ALIASING_EPISILON, 1);

	auto RAND_TEXTURE_TILES = Dlg_->GetRandTextureTiles();
	effect->setPostProcessingEffectConstant(materialType, "RAND_TEXTURE_TILES", &RAND_TEXTURE_TILES, 1);

	auto SSAO_OUTPUT_MULTIPLIER = Dlg_->GetSSAOOutputMultiplier();
	effect->setPostProcessingEffectConstant(materialType, "SSAO_OUTPUT_MULTIPLIER", &SSAO_OUTPUT_MULTIPLIER, 1);
}
