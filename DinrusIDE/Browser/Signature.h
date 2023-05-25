#ifndef _DinrusIDE_Browser_Signature_h_
#define _DinrusIDE_Browser_Signature_h_
//Предназначено для использования пакетом DinrusIDE/clang
//#include <DinrusIDE/Browser/Signature.h>

bool IsCppKeyword(const String& id);

bool IsCppType(const String& id);

int InScListIndext(const char *s, const char *list);

#endif
