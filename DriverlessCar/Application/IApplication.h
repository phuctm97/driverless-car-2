#ifndef __SB_APPLICATION_H__
#define __SB_APPLICATION_H__
#include "Keyboard/IKeyboard.h"

namespace sb
{
class IApplication
{
public:
	virtual ~IApplication() = default;
	
	virtual IKeyboard* getKeyboard() = 0;

	virtual void run() = 0;

	virtual void exit() = 0;
};
}

#endif //!__SB_APPLICATION_H__
