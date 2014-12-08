#ifndef ExtShaders_h__
#define ExtShaders_h__

#pragma once

#include "IShaderConstantSetCallBack.h"
#include "SMaterial.h"

#include "irrEngine/irrEngineFwd.h"

enum EShaderType
{
	EST_LUMINANCE,

	EST_LINE,

	EST_VERTEX_ALPHA,

	EST_FONT,

	EST_ADS_LIGHT,

	EST_PICKING,

	EST_COUNT
};


class	ExtShaders
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	static	void	InitInstance(const IrrEngineUPtr& engine);

	static	ExtShaders&	GetInstance();

public:

	ExtShaders();

	~ExtShaders();

public:

	irr::video::E_MATERIAL_TYPE	GetShaderType(EShaderType enm);
};


#endif // ExtShaders_h__
