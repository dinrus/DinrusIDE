topic "Установка тарбола U++ на POSIX/X11";
[a83;*R6 $$1,2#31310162474203024125188417583966:caption]
[b42;a42;ph2 $$2,2#45413000475342174754091244180557:text]
[H4;b83;*4 $$3,2#07864147445237544204411237157677:title]
[2 $$4,4#589063B430329DC9D1C058DDB04EA569:code]
[b83;*+117 $$5,5#2AC2FD302E8CD436942CC99374D81489:SubTitle]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s1; Установка U`+`+ на POSIX&]
[s2; U`+`+ для POSIX (OpenBSD, FreeBSD)  идёт как single 
sandboxed archive. После загрузки этого архива, 
его распаковывают в удобное место 
командой&]
[ {{10000G1@7 [s4; tar xf upp`-posix`-[/ 14411].tar.xz]}}&]
[s2; (замените[/ 14411] на загруженную вами 
ревизию). Конечно же, можно использовать 
для распаковки не только командную 
строку, но и десктопную ГИП`-утилиту.&]
[s2; В результате появляется папка `"upp`", 
заполненная файлами.&]
[ {{10000G1@7 [s4; cd upp]}}&]
[s2; нужно ввести в терминале, чтобы попасть 
в неё. Теперь у нас есть две опции:&]
[s2;l128;i150;O0; Использовать сценарий [* ./install], 
который разберётся со всем, от чего 
зависит быстрое построение DinrusIDE 
(обычно этот процесс занимает пару 
минут).&]
[s2;l128;i150;O0; Самостоятельно разрешите 
зависимости и примените [* make], чтобы 
построить [*/ DinrusIDE ](отстутствуют [/ ./configure] 
или [/ make install], так как не требуются). 
Впрочем, заглянуть в [* ./install] не помешает 
`- это поможет разобраться в требуемых 
зависимостях. Также следует построить 
[*/ umk] в командной строке с помощью [* make 
`-f uMakefile], но и это не столь уж необходимо 
для первого теста U`+`+.&]
[s2; В завершение процесса у вас появляются 
два бинарника в папке, [*/ DinrusIDE] и [*/ umk]. 
Запустите [*/ DinrusIDE], чтобы насладиться 
U`+`+.&]
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