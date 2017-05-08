#ifndef __SB_CASE_REPOSITORY_H__
#define __SB_CASE_REPOSITORY_H__

#include <list>
#include <map>
#include "Cases/ICase.h"

namespace sb
{
class CaseRepository
{
private:
	int _capacity;

	std::list<ICase*> _cases;

	std::map<int, int> _rowToRoadWidthMap;

public:
	CaseRepository( int capacity )
		: _capacity( capacity ) {}

	bool empty() const;

	const std::list<ICase*>& getAll() const;
	
	ICase* last() const;

	void push( ICase* newCase );

	void mapRoadWidth( int row, int width );

	void release();
};
}

#endif //!__SB_CASE_REPOSITORY_H__
