topic "Установка Тарболла U++ на POSIX/X11";
[a83;*R6 $$1,2#31310162474203024125188417583966:caption]
[b42;a42;ph2 $$2,2#45413000475342174754091244180557:text]
[H4;b83;*4 $$3,2#07864147445237544204411237157677:title]
[2 $$4,4#589063B430329DC9D1C058DDB04EA569:code]
[b83;*+117 $$5,5#2AC2FD302E8CD436942CC99374D81489:SubTitle]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s1; Установка U`+`+ на POSIX&]
[s2; U`+`+ для POSIX (OpenBSD, FreeBSD) находится в 
одном `"запечатанном`" `- sandboxed `- архиве. 
После загрузки, этот архив распаковывается 
в удобном месте командой&]
[ {{10000G1@7 [s4; tar xf upp`-posix`-[/ 14411].tar.xz]}}&]
[s2; (замените [/ 14411] номерацией загруженной 
версии). Конечно же, можно использовать 
и десктопные утилиты для распаковки.&]
[s2; Это породит папку `"upp`", заполненную 
файлами. Делаем команду,&]
[ {{10000G1@7 [s4; cd upp]}}&]
[s2; чтобы попасть в неё. Теперь у нас 
два варианта:&]
[s2;l128;i150;O0; Использовать сценарий [* ./install], 
который разрешит все зависимости 
постройки программы в короткие сроки 
(обычно около пары минут).&]
[s2;l128;i150;O0; Resolve dependecies yourself and use [* make] to 
build [*/ DinrusIDE ](there is no [/ ./configure] or [/ make install], 
it is not necessarry). Even if you do this, peeking into [* ./install] 
is a good idea to get a clue about dependecies required. You 
should also build [*/ umk] command line too with [* make `-f uMakefile], 
also this is not strictly necessarry for the first taste of U`+`+.&]
[s2; At the end of this process, you should have two binaries in 
the folder, [*/ DinrusIDE] and [*/ umk]. Start [*/ DinrusIDE] to have 
some fun with U`+`+.&]
[s2; As long as you keep [*/ DinrusIDE] (or [*/ umk]) just in the original 
directory, nothing will be written outside of it `- its `"sandboxed`" 
(with one exception described bellow). If you get bored with 
U`+`+ and need to clean the space, simply delete the `"upp`" 
folder.&]
[s2; If you move DinrusIDE e.g. to [/ `~/bin], it will start to write 
things to [/ `~/.config] and [/ `~/.cache].&]
[s2; The exception to the sanboxing rule is single command invoked 
from DinrusIDE menu, [*/ Setup ][* / ][*/ Install DinrusIDE.desktop]. 
This will write proper .desktop file to [/ `~/.local/share/applications] 
`- the effect of this on most desktop environments is that DinrusIDE 
will appear in the desktop menu somewhere, usually in the Start 
menu under [/ Programming] category. Sometimes desktop environment 
needs restert to this to take effect.&]
[s2; ]]