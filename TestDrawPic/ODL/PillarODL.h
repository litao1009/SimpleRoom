#pragma once

#include "BaseODL.h"

class	PillarODL : public BaseODL
{
	class	Imp;
	std::unique_ptr<Imp>	ImpUPtr_;

public:

	PillarODL(const SRenderContextWPtr& rc);
	~PillarODL();

public:

	virtual	EObjectDisplayLayerType	GetType() const { return EODLT_PILLAR; }

	virtual	void	Init();

public:

	float	GetOffsetHeight() const;

	void	SetOffsetHeight(float val);

	void	SetSize(float xLength, float yLength, float zLength);

	void	SetSize(const gp_XYZ& size);

	const gp_XYZ&	GetSize() const;

	void	UpdateMesh();

	void	SetDefaultTexture();
};