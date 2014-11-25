#pragma once

#include <memory>
#include <vector>

class	BaseODL;

typedef	std::shared_ptr<BaseODL>	BaseODLSPtr;
typedef	std::weak_ptr<BaseODL>		BaseODLWPtr;

typedef	std::vector<BaseODLSPtr>	BaseODLList;
typedef	std::vector<BaseODLWPtr>	BaseODLWList;