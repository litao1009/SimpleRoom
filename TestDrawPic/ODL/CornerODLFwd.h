#ifndef CornerODLFwd_h__
#define CornerODLFwd_h__

#pragma once

#include <memory>
#include <vector>

class	CornerODL;

typedef	std::shared_ptr<CornerODL>	CornerODLSPtr;
typedef	std::weak_ptr<CornerODL>	CornerODLWPtr;

typedef	std::vector<CornerODLSPtr>	CornerODLList;
typedef	std::vector<CornerODLWPtr>	CornerODLWList;


#endif // CornerODLFwd_h__