#ifndef __SB_CASE_REPOSITORY_H__
#define __SB_CASE_REPOSITORY_H__
#include <queue>
#include "Cases/ICase.h"

namespace sb
{
class CaseRepository
{
private:
	int _capacity;

	std::list<ICase*> _cases;

public:
	CaseRepository( int capacity )
		: _capacity( capacity ) {}

	const std::list<ICase*>& getAll() const;

	void push( ICase* newCase );

	void release();
};
}

#endif //!__SB_CASE_REPOSITORY_H__
