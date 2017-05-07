#include "ApplicationTestCollector.h"

void sb::ApplicationTestCollector::run()
{
	// init
	_exiting = false;

	sb::CollectData* collectData = new CollectData;

	if ( _collector == nullptr || _collector->init() < 0 ) _exiting = true;

	// run
	while ( !_exiting ) {
		if ( _collector->collect( collectData ) < 0 ) break;

		cv::imshow( "TestCollector", collectData->colorImage );

		// keyboard event
		if ( _keyboard != nullptr ) {
			_keyboard->checkBufferedKey();
		}
	}

	// release
	if ( collectData != nullptr ) {
		sb::release( collectData );
		delete collectData;
	}
}

void sb::ApplicationTestCollector::exit()
{
	_exiting = true;
}

void sb::ApplicationTestCollector::release()
{
	if ( _keyboard != nullptr ) {
		_keyboard->release();
		delete _keyboard;
	}

	if ( _collector != nullptr ) {
		_collector->release();
		delete _collector;
	}
}

void sb::ApplicationTestCollector::addKeyboardCallback( const std::function<void( int )>& callback )
{
	if ( _keyboard != nullptr ) _keyboard->addKeyboardCallback( callback );

}
