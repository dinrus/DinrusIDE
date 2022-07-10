//#BLITZ_APPROVE

#ifndef KEYGROUPNAME
#ifdef  KEYNAMESPACE
#define KEYGROUPNAME #KEYNAMESPACE
#else
#define KEYGROUPNAME ""
#endif
#endif

#include <CtrlLib/akt_.h>

#ifdef KEYNAMESPACE
namespace KEYNAMESPACE {
#endif

#define KEY(id, имя, def)         ИнфОКлюче& КОМБИНИРУЙ(AK_, id)() { static ИнфОКлюче x = { имя, { def } }; return x; }
#define KEY2(id, имя, def1, def2) ИнфОКлюче& КОМБИНИРУЙ(AK_, id)() { static ИнфОКлюче x = { имя, { def1, def2 } }; return x; }
#include KEYFILE
#undef  KEY
#undef  KEY2

#define KEY(id, имя, def)         регистрируйПривязкуКлюча(KEYGROUPNAME, #id, КОМБИНИРУЙ(AK_, id));
#define KEY2(id, имя, def1, def2) регистрируйПривязкуКлюча(KEYGROUPNAME, #id, КОМБИНИРУЙ(AK_, id));
ИНИЦБЛОК {
#include KEYFILE
}
#undef KEY
#undef KEY2

#ifdef KEYNAMESPACE
}
#endif

#include <Core/t_.h>

#ifdef KEYNAMESPACE
#undef KEYNAMESPACE
#endif

#undef KEYGROUPNAME
#undef KEYFILE
