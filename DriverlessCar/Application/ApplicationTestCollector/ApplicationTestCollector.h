#ifndef __SB_APPLICATION_TEST_COLLECTOR_H__
#define __SB_APPLICATION_TEST_COLLECTOR_H__
#include "../IApplication.h"
#include "../../Collector/ICollector.h"

namespace sb
{
class ApplicationTestCollector : public IApplication
{
private:
	IKeyboard* _keyboard;

	ICollector* _collector;

	bool _exiting;
public:
	ApplicationTestCollector( ICollector* collector, IKeyboard* keyboard )
		: _exiting( false ), _collector( collector ), _keyboard( keyboard ) {}

	void run() override;

	void exit() override;

	void release() override;

	void addKeyboardCallback( const std::function<void( int )>& callback ) override;
};
}

#endif //!__SB_APPLICATION_TEST_COLLECTOR_H__
