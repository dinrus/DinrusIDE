//#BLITZ_APPROVE

#ifndef KEYGROUPNAME
#ifdef  KEYNAMESPACE
#define KEYGROUPNAME #KEYNAMESPACE
#else
#define KEYGROUPNAME ""
#endif
#endif

#define KEY(id, имя, def)        ИнфОКлюче& КОМБИНИРУЙ(AK_, id)();
#define KEY2(id, имя, def, def2) ИнфОКлюче& КОМБИНИРУЙ(AK_, id)();

#ifdef KEYNAMESPACE
namespace KEYNAMESPACE {
#endif

#include KEYFILE

#ifdef KEYNAMESPACE
};
#endif

#ifdef KEYNAMESPACE
#undef KEYNAMESPACE
#endif

#undef KEYGROUPNAME
#undef KEY
#undef KEY2
#undef KEYFILE
