topic "Introduction";
[#l13;b21;a17;t300; $$1,1#3900D79871E0B7385BE54A8E49FCAF50:ДинрусДок]
[#H4;b83;*4 $$2,0#07864147445237544204411237157677:title]
[h(128.0.255)t300; $$0,0#00000000000000000000000000000000:Дефолт]
[{_}%RU-RU 
[s2; [@3 Содержание]&]
[s0;2 &]
[s2; [^topic`:`/`/DinrusIDE`/app`/IconDes`_ru`-ru`#1^ 1. Введение]&]
[s2; [^topic`:`/`/DinrusIDE`/app`/IconDes`_ru`-ru`#2^ 2. Фундаментальные
 элементы управления]&]
[s2; [^topic`:`/`/DinrusIDE`/app`/IconDes`_ru`-ru`#3^ 3. Суперсэмплинг]&]
[s2; [^topic`:`/`/DinrusIDE`/app`/IconDes`_ru`-ru`#4^ 4. Интерполяция]&]
[s0; &]
[s2; 1. Введение&]
[s0;~~~1184~3072; &]
[s0; The Windows version of Scintilla is a Windows Control. As such, 
its primary programming interface is through Windows messages. 
Early versions of Scintilla emulated much of the API defined 
by the standard Windows Edit and RichEdit controls but those 
APIs are now deprecated in favour of Scintilla`'s own, more consistent 
API. In addition to messages performing the actions of a normal 
Edit control, Scintilla allows control of syntax styling, folding, 
markers, autocompletion and call tips.&]
[s0; &]
[s0; The GTK version also uses messages in a similar way to the Windows 
version. This is different to normal GTK practice but made it 
easier to implement rapidly.&]
[s0; &]
[s0; Scintilla also builds with Cocoa on macOS and with Qt, and follows 
the conventions of those platforms.&]
[s0; &]
[s0; Scintilla provides only limited experimental support on Windows 
for right`-to`-left languages like Arabic and Hebrew. While text 
in these languages may appear correct, interaction with this 
text may not work correctly as occurs with other editors.&]
[s0; &]
[s0; This documentation describes the individual messages and notifications 
used by Scintilla. It does not describe how to link them together 
to form a useful editor. For now, the best way to work out how 
to develop using Scintilla is to see how SciTE uses it. SciTE 
exercises most of Scintilla`'s facilities.&]
[s0; &]
[s0; There is a more type`-safe binding of this API that can be used 
from C`+`+. It is implemented in the ScintillaTypes.h, ScintillaMessages.h, 
ScintillaStructures.h, and ScintillaCall.h headers and call/ScintillaCall.cxx. 
The ScintillaTypes.h, ScintillaMessages.h, and ScintillaStructures.h 
headers can be used without ScintillaCall but ScintillaCall makes 
it easier to use the other headers by wrapping the messages in 
methods which also avoids much type casting. ScintillaCall throws 
Scintilla`::Failure exceptions when a call fails.&]
[s0; &]
[s0; In the descriptions that follow, the messages are described 
as function calls with zero, one or two arguments. These two 
arguments are the standard wParam and lParam familiar to Windows 
programmers. These parameters are integers that are large enough 
to hold pointers, and the return value is also an integer large 
enough to contain a pointer. Although the commands only use the 
arguments described, because all messages have two arguments 
whether Scintilla uses them or not, it is strongly recommended 
that any unused arguments are set to 0. This allows future enhancement 
of messages without the risk of breaking existing code. Common 
argument types are:&]
[s0; &]
[s0; bool-|Arguments expect the values 0 for false and 1 for true.&]
[s0; int-|Arguments are 32`-bit or 64`-bit signed integers depending 
on the platform. Эквивалентно intptr`_t.&]
[s0; position-|Positions and lengths in document. Эквивалентно intptr`_t.&]
[s0; line-|A line number in the document. Эквивалентно intptr`_t.&]
[s0; const char `*-|Arguments point at text that is being passed to 
Scintilla but not modified. The text may be zero terminated or 
another argument may specify the character count, the description 
will make this clear.&]
[s0; char `*-|Arguments point at text buffers that Scintilla will 
fill with text. In some cases, another argument will tell Scintilla 
the buffer size. In others, you must make sure that the buffer 
is big enough to hold the requested text. If a NULL pointer (0) 
is passed then, for SCI`_`* calls, the length that should be 
allocated, not including any terminating NUL, is returned. Some 
calls (marked `"NUL`-terminated`") add a NUL character to the 
result but other calls do not: to generically handle both types, 
allocate one more byte than indicated and set it to NUL.&]
[s0; pointer-|A memory address. In some cases this is a pointer to 
a sequence of char inside Scintilla that will only be available 
for a limited period. Эквивалентно void `*.&]
[s0; colour-|Colours are set using the RGB format (Red, Green, Blue). 
The intensity of each colour is set in the range 0 to 255. If 
you have three such intensities, they are combined as: red `| 
(green << 8) `| (blue << 16). If you set all intensities to 255, 
the colour is white. If you set all intensities to 0, the colour 
is black. When you set a colour, you are making a request. What 
you will get depends on the capabilities of the system and the 
current screen mode.&]
[s0; colouralpha-|Colours are set using the RGBA format (Red, Green, 
Blue, Alpha). This is similar to colour but with a byte of alpha 
added. They are combined as: red `| (green << 8) `| (blue << 16) 
`| (alpha << 24). Fully opaque uses an alpha of 255.&]
[s0; alpha-|Translucency is set using an alpha value. Alpha ranges 
from 0 (SC`_ALPHA`_TRANSPARENT) which is completely transparent 
to 255 (SC`_ALPHA`_OPAQUE) which is opaque. Previous versions 
used the value 256 (SC`_ALPHA`_NOALPHA) to indicate that drawing 
was to be performed opaquely on the base layer. This is now discouraged 
and code should use the …LAYER… methods to choose the layer.&]
[s0; <unused>-|This is an unused argument. Setting it to 0 will ensure 
compatibility with future enhancements.]]