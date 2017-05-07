#ifndef __SB_CASE_REPOSITORY_H__
#define __SB_CASE_REPOSITORY_H__

#include <list>
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

	bool empty() const;

	const std::list<ICase*>& getAll() const;
	
	ICase* last() const;

	void push( ICase* newCase );

	void release();
};
}

#endif //!__SB_CASE_REPOSITORY_H__
