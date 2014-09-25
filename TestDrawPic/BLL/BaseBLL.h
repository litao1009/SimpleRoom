#ifndef BaseBLL_h__
#define BaseBLL_h__

#pragma once

#include <memory>
#include <vector>

class CBaseBLL
{
public:
	CBaseBLL(void);
	virtual ~CBaseBLL(void);
};

typedef	std::shared_ptr<CBaseBLL>	CBaseBLLSPtr;
typedef	std::weak_ptr<CBaseBLL>		CBaseBLLWPtr;

#endif // BaseBLL_h__
