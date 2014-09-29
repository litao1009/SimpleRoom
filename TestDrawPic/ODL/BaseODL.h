#ifndef BaseODL_h__
#define BaseODL_h__

#pragma once

#include "PublicDefine.h"
#include "ODLTools.h"
#include "BaseODLFwd.h"

#include "BLL/BaseBLL.h"

#include "TSceneNode.h"

#include "TopoDS_Shape.hxx"
#include "BRepExtrema_DistShapeShape.hxx"

class CBaseODL : public TSceneNode<CBaseODL>
{
public:

	CBaseODL(void);
	
	virtual ~CBaseODL(void);

public:

	virtual	ENUM_OBOJECT_TYPE	GetType() const { return EOBT_Base; }

	virtual	void				Init() {}

public:

	TopoDS_Shape&		GetBaseShape() { return BaseShape_; }

	const TopoDS_Shape&	GetBaseShape() const { return BaseShape_; }

	void				SetBaseShape(const TopoDS_Shape& shape) { BaseShape_ = shape; }

	BRepExtrema_DistShapeShape&	GetSelector() { return BaseSelector_; }

	const BRepExtrema_DistShapeShape&	GetSelector() const { return BaseSelector_; }

private:

	TopoDS_Shape				BaseShape_;
	BRepExtrema_DistShapeShape	BaseSelector_;
};

#endif // BaseODL_h__
