#include "Collector/ICollector.h"
#include "Collector/CollectorWithVideo/CollectorWithVideo.h"
#include "Application/IApplication.h"
#include "Application/ApplicationTestCollector/ApplicationTestCollector.h"
#include "Application/Keyboard/WindowsKeyboard.h"

sb::IApplication* application = nullptr;

void onKeyPressed( int key );

int main( const int argc, const char** argv )
{
	sb::IKeyboard* keyboard = new sb::WindowsKeyboard( 33 );

	sb::ICollector* collector = new sb::CollectorWithVideo( "..\\Debug\\sample-3.avi" );

	application = new sb::ApplicationTestCollector( collector, keyboard );

	application->getKeyboard()->addKeyboardCallback( &onKeyPressed );

	application->run();

	delete application;

	return 0;
}

void onKeyPressed( int key )
{
	if ( key == 27 ) application->exit();
}
