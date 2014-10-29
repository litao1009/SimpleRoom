#ifndef ExtShaders_h__
#define ExtShaders_h__

#pragma once

#include "IShaderConstantSetCallBack.h"
#include "SMaterial.h"

class	LuminanceCB : public irr::video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	virtual void OnSetMaterial(const irr::video::SMaterial& material);

	static	const char*	GetVertexShader();
	static	const char*	GetPixelShader();

private:

	irr::video::SMaterial	CurrentMaterial_;
};

class	LineColorCB : public irr::video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	virtual void OnSetMaterial(const irr::video::SMaterial& material);

	static	const char*	GetVertexShader();
	static	const char*	GetPixelShader();

private:

	irr::video::SMaterial	CurrentMaterial_;
};

class	VertexAlphaCB : public irr::video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData);

	virtual void OnSetMaterial(const irr::video::SMaterial& material);

	static	const char*	GetVertexShader();
	static	const char*	GetPixelShader();

private:

	irr::video::SMaterial	CurrentMaterial_;
};

#endif // ExtShaders_h__
