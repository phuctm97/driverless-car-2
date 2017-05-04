#ifndef __SB_APPLICATION_TEST_ANALYZER_H__
#define __SB_APPLICATION_TEST_ANALYZER_H__
#include "../IApplication.h"
#include "../../Collector/ICollector.h"
#include "../../Calculator/ICalculator.h"
#include "../../Analyzer/IAnalyzer.h"

namespace sb
{
class ApplicationTestAnalyzer : public IApplication
{
private:
	IKeyboard* _keyboard;

	ICollector* _collector;

	ICalculator* _calculator;

	IAnalyzer* _analyzer;

	bool _exiting;

public:
	ApplicationTestAnalyzer( ICollector* collector, ICalculator* calculator, IAnalyzer* analyzer, IKeyboard* keybaord )
		: _exiting( false ), _collector( collector ), _calculator( calculator ), _analyzer( analyzer ), _keyboard( keybaord ) {}

	IKeyboard* getKeyboard() override;
	
	void run() override;
	
	void exit() override;

private:
	void release();
};
}

#endif //!__SB_APPLICATION_TEST_ANALYZER_H__
