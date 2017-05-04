#include "ApplicationTestCalculator.h"

sb::IKeyboard* sb::ApplicationTestCalculator::getKeyboard() { return _keyboard; }

void sb::ApplicationTestCalculator::run()
{
	// init
	_exiting = false;

	sb::CollectData* collectData = new CollectData;
	sb::CalculateData* calculateData = new CalculateData;

	if ( _collector == nullptr || _collector->init() < 0 ) _exiting = true;
	if ( _calculator == nullptr || _calculator->init() < 0 ) _exiting = true;

	// run
	while ( !_exiting ) {
		if ( _collector->collect( collectData ) < 0 ) break;

		if ( _calculator->calculate( collectData, calculateData ) ) break;

		// act

		// keyboard event
		if ( _keyboard != nullptr ) {
			_keyboard->checkBufferedKey();
		}
	}

	// release
	delete collectData;
	delete calculateData;

	release();
}

void sb::ApplicationTestCalculator::exit()
{
	_exiting = true;
}

void sb::ApplicationTestCalculator::release()
{
	_collector->release();
	delete _collector;
	_collector = nullptr;

	_calculator->release();
	delete _calculator;
	_calculator = nullptr;

	delete _keyboard;
	_keyboard = nullptr;
}
