#ifndef ObjGroupDAL_h__
#define ObjGroupDAL_h__

#pragma once
#include "basedal.h"
class CObjGroupDAL :
	public CBaseDAL
{
public:
	CObjGroupDAL(CDBConnection* pCon);
	~CObjGroupDAL(void);
};

typedef	std::shared_ptr<CObjGroupDAL>	CObjGroupDALSPtr;
typedef	std::weak_ptr<CObjGroupDAL>	CObjGroupDALWPtr;
#endif // ObjGroupDAL_h__
