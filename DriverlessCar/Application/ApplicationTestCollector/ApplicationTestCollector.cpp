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
	delete collectData;

	release();
}

void sb::ApplicationTestCollector::exit()
{
	_exiting = true;
}

sb::IKeyboard* sb::ApplicationTestCollector::getKeyboard() { return _keyboard; }

void sb::ApplicationTestCollector::release()
{
	_collector->release();
	delete _collector;
	_collector = nullptr;

	delete _keyboard;
	_keyboard = nullptr;
}
