// Lexilla lexer library use example
/** @file CheckLexilla.c
 ** Check that Lexilla.h works.
 **/
// Copyright 2021 by Neil Hodgson <neilh@scintilla.org>
// This file is in the public domain.
// If the public domain is not possible in your location then it can also be used under the same
// license as Scintilla. https://www.scintilla.org/License.txt

/* Build and run

    Win32
gcc CheckLexilla.c -I ../../include -o CheckLexilla
CheckLexilla
CheckLexilla ../SimpleLexer/SimpleLexer.dll

   Win32 Visual C++
cl CheckLexilla.c -I ../../include -Fe: CheckLexilla
CheckLexilla
CheckLexilla ../SimpleLexer/SimpleLexer.dll

    macOS
clang CheckLexilla.c -I ../../include -o CheckLexilla
./CheckLexilla
./CheckLexilla ../SimpleLexer/SimpleLexer.dylib

    Linux
gcc CheckLexilla.c -I ../../include -ldl -o CheckLexilla
./CheckLexilla
./CheckLexilla ../SimpleLexer/SimpleLexer.so

While principally meant for compilation as C to act as an example of using Lexilla
from C it can also be built as C++.

Warnings are intentionally shown for the deprecated typedef LexerNameFromIDFn when compiled with
GCC or Clang or as C++.

*/

#include <stdio.h>

#if _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef __cplusplus
#include <Scintilla/ILexer.h>
#endif

#include <Lexilla/Lexilla.h>

#ifdef __cplusplus
using namespace Lexilla;
#endif

#if _WIN32
typedef FARPROC Function;
typedef HMODULE Module;
#else
typedef void *Function;
typedef void *Module;
#endif

static Function FindSymbol(Module m, const char *symbol) {
#if _WIN32
    return GetProcAddress(m, symbol);
#else
    return dlsym(m, symbol);
#endif
}

int main(int argc, char *argv[]) {

            int nLexers = GetLexerCount();
            printf("There are %d lexers.\n", nLexers);
            for (int i = 0; i < nLexers; i++) {
                char name[100] = "";
                GetLexerName(i, name, sizeof(name));
                printf("%s ", name);
            }
            printf("\n");

            LexerFactoryFunction lexerFactory4 = GetLexerFactory(4);    // 4th entry is "as" which is an object lexer so works
            printf("Lexer factory 4 -> %p.\n", lexerFactory4);

            ILexer5 *lexerCpp = CreateLexer("cpp");
            printf("Created cpp lexer -> %p.\n", lexerCpp);


                const char *lexerNameCpp = LexerNameFromID(3);  // SCLEX_CPP=3
                if (lexerNameCpp)
                    printf("Lexer name 3 -> %s.\n", lexerNameCpp);


                const char *names = GetLibraryPropertyNames();
                printf("Property names '%s'.\n", names);

                SetLibraryProperty("key", "value");

                const char *nameSpace = GetNameSpace();
                printf("Name space '%s'.\n", nameSpace);
        
    
}
