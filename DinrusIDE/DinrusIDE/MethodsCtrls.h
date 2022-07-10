#ifndef _ide_MethodsCtrls_h_
#define _ide_MethodsCtrls_h_

#include <CtrlLib/CtrlLib.h>

namespace РНЦП {

class TextOption : public Опция {
public:
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;
};

class TextSwitch : public Switch {
public:
	virtual void   устДанные(const Значение& data);
	virtual Значение  дайДанные() const;
};

}

#endif
