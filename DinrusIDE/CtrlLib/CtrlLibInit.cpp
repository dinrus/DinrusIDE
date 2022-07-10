#include "CtrlLib.h"

namespace РНЦП {

#define  TFILE <CtrlLib/CtrlLib.t>
#include <Core/t.h>

void CtrlSetDefaultSkin(void (*_skin)());

ИНИЦИАЛИЗАТОР(CtrlLib) {
	CtrlSetDefaultSkin(ChHostSkin);
};

}
