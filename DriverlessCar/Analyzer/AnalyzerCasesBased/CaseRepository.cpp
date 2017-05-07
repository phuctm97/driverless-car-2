#include "CaseRepository.h"

bool sb::CaseRepository::empty() const { return _cases.empty(); }

const std::list<sb::ICase*>& sb::CaseRepository::getAll() const
{
	return _cases;
}

sb::ICase* sb::CaseRepository::last() const
{
	if( !empty() ) return _cases.front();
	else return nullptr;
}

void sb::CaseRepository::push( ICase* newCase )
{
	if ( _cases.size() == _capacity ) {
		_cases.pop_back();
	}

	_cases.push_front( newCase );
}

void sb::CaseRepository::release()
{
	for ( ICase* caseToRelease : _cases ) {
		delete caseToRelease;
	}
}
