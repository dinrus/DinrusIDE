struct LngEntry__ {
	цел         lang;
	кткст0 text;
};

проц    AddModule(const LngEntry__ *entry, кткст0 modulename = NULL);


кткст0 t_GetLngString(кткст0 ид);  // Only works with string literals because of caching !

#define     LNG_enUS LNG_('E','N','U','S')

кткст0 GetENUS(кткст0 ид);

// t_ and tt_ would cause translation files sync to report Ошибка, therefore it
// has to be included from file that is not part of package...
#include "t_.h"

/* t_.h:
#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

#define t_(x)          t_GetLngString(x)
#define tt_(x)         x
*/

/* lt_.h:
#ifdef t_
#undef t_
#endif

#ifdef tt_
#undef tt_
#endif

#define t_(x)          x
#define tt_(x)         x
*/

Ткст     GetLngString(кткст0 ид); // Only works with string literals because of caching !

Ткст     GetLngString(цел lang, кткст0 ид);

Индекс<цел> GetLngSet();
Индекс<цел> GetLngSet(const Ткст& module);
проц       SaveLngFile(ФайлВывод& out, цел lang, цел lang2 = 0);
бул       LoadLngFile(кткст0 file);

проц       SetCurrentLanguage(цел lang);
цел        дайТекЯз();

проц       LngSetAdd(кткст0 ид, цел lang, кткст0 txt, бул addid);
