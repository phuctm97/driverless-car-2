#include "ApplicationFinal.h"

void sb::ApplicationFinal::run()
{
	// init
	_exiting = false;

	sb::CollectData* collectData = new CollectData;
	sb::CalculateData* calculateData = new CalculateData;
	sb::AnalyzeData* analyzeData = new AnalyzeData;

	if ( _collector == nullptr || _collector->init() < 0 ) _exiting = true;
	if ( _calculator == nullptr || _calculator->init() < 0 ) _exiting = true;
	if ( _analyzer == nullptr || _analyzer->init() < 0 ) _exiting = true;
	if ( _driver == nullptr || _driver->init() < 0 ) _exiting = false;

	// run
	while ( !_exiting ) {
		if ( _collector->collect( collectData ) < 0 ) break;
		if ( _calculator->calculate( collectData, calculateData ) < 0 ) break;
		if ( _analyzer->analyze( collectData, calculateData, analyzeData ) < 0 ) break;
		if ( _driver->drive( analyzeData ) < 0 ) break;
	}

	// release
	if ( collectData != nullptr ) {
		sb::release( collectData );
		delete collectData;
	}

	if ( calculateData != nullptr ) {
		sb::release( calculateData );
		delete calculateData;
	}

	if ( analyzeData != nullptr ) {
		sb::release( analyzeData );
		delete analyzeData;
	}
}

void sb::ApplicationFinal::exit()
{
	_exiting = true;
}

void sb::ApplicationFinal::release()
{
	if ( _keyboard != nullptr ) {
		_keyboard->release();
		delete _keyboard;
	}

	if ( _collector != nullptr ) {
		_collector->release();
		delete _collector;
	}

	if ( _calculator != nullptr ) {
		_calculator->release();
		delete _calculator;
	}

	if ( _analyzer != nullptr ) {
		_analyzer->release();
		delete _analyzer;
	}

	if ( _driver != nullptr ) {
		_driver->release();
		delete _driver;
	}
}

void sb::ApplicationFinal::addKeyboardCallback( const std::function<void( int )>& callback )
{
	if ( _keyboard != nullptr ) _keyboard->addKeyboardCallback( callback );
}
