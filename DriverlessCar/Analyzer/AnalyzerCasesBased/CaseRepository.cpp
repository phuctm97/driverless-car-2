#include "CaseRepository.h"

const std::list<sb::ICase*>& sb::CaseRepository::getAll() const
{
	return _cases;
}

void sb::CaseRepository::push( ICase* newCase )
{
	if( _cases.size() == _capacity ) {
		_cases.pop_back();
	}

	_cases.push_front( newCase );
}

void sb::CaseRepository::release() {}
