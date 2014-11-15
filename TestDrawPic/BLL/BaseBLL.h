#ifndef BaseBLL_h__
#define BaseBLL_h__

#pragma once

#include <memory>

class BaseBLL
{
public:
	BaseBLL(void);
	virtual ~BaseBLL(void);
};

typedef	std::shared_ptr<BaseBLL>	BaseBLLSPtr;
typedef	std::weak_ptr<BaseBLL>		BaseBLLWPtr;

#endif // BaseBLL_h__
