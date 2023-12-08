topic "Утилиты FileSel";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Вспомогательные Функции на Основе 
FileSel]]}}&]
[s9; Эти функции упрощают открытие файлов, 
используя единый глобальный селектор 
файлов (файлвыборку). Конфигурация 
файлвыборки сохраняется в [^topic`:`/`/Core`/src`/SerializationUtils`_ru`-ru^ г
лобальной конфигурации].&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Функций]]}}&]
[s3;%- &]
[s5;:SelectFileOpen`(const char`*`):%- [_^String^ String]_[* SelectFileOpen]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; Выбирает файл для открытия, [%-*@3 types] 
следуют правилам метода [^topic`:`/`/CtrlLib`/src`/FileSel`_ru`-ru`#FileSel`:`:Types`(const char`*`)^ T
ypes] из FileSel. Если выделенное отменено, 
возвращается пустая String.&]
[s3; &]
[s4; &]
[s5;:SelectFileSaveAs`(const char`*`):%- [_^String^ String]_[* SelectFileSaveAs]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 types])&]
[s2;~~~3200; Выбирает файл для сохранения, 
[%-*@3 types] следуют правилам метода [^topic`:`/`/CtrlLib`/src`/FileSel`_ru`-ru`#FileSel`:`:Types`(const char`*`)^ T
ypes] из FileSel. Если выделенное отменено, 
возвращается пустая String.&]
[s3; &]
[s4;%- &]
[s5;:SelectDirectory`(`):%- [_^String^ String]_[* SelectDirectory]()&]
[s2; Выбирает директорию.&]
[s3;%- &]
[s4; &]
[s5;:SelectLoadFile`(const char`*`):%- [_^String^ String]_[* SelectLoadFile]([@(0.0.255) co
nst]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; Выбирает файл и загружает егоt,[%-*@3 types] 
следуют правилам метода [^topic`:`/`/CtrlLib`/src`/FileSel`_ru`-ru`#FileSel`:`:Types`(const char`*`)^ T
ypes] из FileSel. Если выделенное отменено 
или файл не может быть открыт, возвращает 
пустую String`::GetVoid().&]
[s3; &]
[s4; &]
[s5;:SelectSaveFile`(const char`*`,const String`&`):%- [@(0.0.255) bool]_[* SelectSaveFil
e]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 types], [@(0.0.255) const]_[_^String^ String
][@(0.0.255) `&]_[*@3 data])&]
[s2; Выбирает файл и сохраняет в него 
[%-*@3 data], [%-*@3 types] следует правилам метода 
[^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ Types
] из FileSel. Возвращает true при успехе.&]
[s3;%- &]
[s0;%- &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@2 Класс SelectFileIn]]}}&]
[s9;%- TЭтот класс наследует от FileIn. Его 
конструктор вызывает глобальный 
FileSel и открывает выбранный файл для 
чтения:&]
[s0;%- &]
[s5;:SelectFileIn`:`:SelectFileIn`(const char`*`):%- [* SelectFileIn]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 types])&]
[s2; [%-*@3 types] следуют правилам метода [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] из FileSel. В случае любого провала 
(выбор отменён, открытие не удалось), 
итоговый объект оказывается в неоткрытом 
состоянии (можно проверить оператором 
bool).&]
[s3;%- &]
[s0;%- &]
[s0;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@2 Класс SelectFileOut]]}}&]
[s9;%- Этот класс наследует отs FileOut.Его 
конструктор вызывает глобальный 
FileSel и открывает выбранный файл для 
записи:&]
[s0;%- &]
[s5;:SelectFileOut`:`:SelectFileOut`(const char`*`):%- [* SelectFileOut]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 types])&]
[s2; [%-*@3 types] следуют правилам метода [^topic`:`/`/CtrlLib`/src`/FileSel`$en`-us`#FileSel`:`:Types`(const char`*`)^ T
ypes] из FileSel. В случае любого провала 
(выбор отменён, открытие не удалось), 
итоговый объект оказывается в неоткрытом 
состоянии (можно проверить оператором 
bool).&]
[s3;%- &]
[s0;%- ]]