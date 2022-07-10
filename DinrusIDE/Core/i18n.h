struct LngEntry__ {
	int         lang;
	const char *text;
};

void    AddModule(const LngEntry__ *entry, const char *modulename = NULL);


const char *t_GetLngString(const char *ид);  // Only works with string literals because of caching !

#define     LNG_enUS LNG_('E','N','U','S')

const char *GetENUS(const char *ид);

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

Ткст     GetLngString(const char *ид); // Only works with string literals because of caching !

Ткст     GetLngString(int lang, const char *ид);

Индекс<int> GetLngSet();
Индекс<int> GetLngSet(const Ткст& module);
void       SaveLngFile(ФайлВывод& out, int lang, int lang2 = 0);
bool       LoadLngFile(const char *file);

void       SetCurrentLanguage(int lang);
int        GetCurrentLanguage();

void       LngSetAdd(const char *ид, int lang, const char *txt, bool addid);
