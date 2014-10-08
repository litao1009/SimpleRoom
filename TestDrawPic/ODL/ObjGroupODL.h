#ifndef ObjGroupODL_h__
#define ObjGroupODL_h__

#pragma once
#include "baseodl.h"

class CObjGroupODL :
	public CBaseODL
{
public:
	CObjGroupODL(void);
	~CObjGroupODL(void);

	//Rect°üº¬Object
	virtual bool	IsFaceInShape(BRepExtrema_DistShapeShape& dss);

	//RectÏà½»Object
	virtual bool	HasTopCollisionWith(const TopoDS_Shape& theShape);

	virtual void SetIsTopSelected( bool isSelected);
};

typedef	std::shared_ptr<CObjGroupODL>	CObjGroupODLSPtr;

#endif // ObjGroupODL_h__
