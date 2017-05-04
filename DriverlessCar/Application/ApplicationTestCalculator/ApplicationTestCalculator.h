#ifndef __SB_APPLICATION_TEST_COLLECTOR_H__
#define __SB_APPLICATION_TEST_COLLECTOR_H__
#include "../IApplication.h"
#include "../../Collector/ICollector.h"
#include "../../Calculator/ICalculator.h"

namespace sb
{
class ApplicationTestCalculator : public IApplication
{
private:
	IKeyboard* _keyboard;

	ICollector* _collector;

	ICalculator* _calculator;

	bool _exiting;

public:
	ApplicationTestCalculator( ICollector* collector, ICalculator* calculator, IKeyboard* keyboard )
		: _exiting( false ), _collector( collector ), _calculator( calculator ), _keyboard( keyboard ) {}

	IKeyboard* getKeyboard() override;

	void run() override;

	void exit() override;

private:
	void release();
};
}

#endif //!__SB_APPLICATION_TEST_COLLECTOR_H__
