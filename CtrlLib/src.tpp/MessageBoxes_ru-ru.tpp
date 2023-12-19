topic "Боксы Сообщений (Сооббоксы)";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;ph2 $$9,9#13035079074754324216151401829390:normal]
[b42;a42;ph2 $$10,10#E71D914572A169C91BE52E3FB27F810F:text]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Боксы Сообщений (Сооббоксы)]]}}&]
[s10;%RU-RU Сооббоксы в U`+`+ предоставляют 
до трёх кнопок. Для сообщения используется 
насыщенный текстовый формат [*^topic`:`/`/RichText`/srcdoc`/QTF`_ru`-ru^ Q
TF], это означает, что можно, например, 
внедрить гиперссылку, или даже таблицу 
или рисунок. Однако, это может порой 
причинять неудобства, если присутствует 
какой`-то `"нелитеральный`" текст, в 
котором случайно попадаются символы 
форматирования QTF. Чтобы решить эту 
проблему, можно воспользоваться функцией 
[*^topic`:`/`/RichText`/src`/Utilities`_ru`-ru`#Upp`:`:DeQtf`(const char`*`)^ DeQtf
][*  ]с[*  ]`'`\1`' переключателем (подавляет 
разбор QTF до следующей `'`\1`'), например:&]
[s10;l160;%RU-RU [/@(28.42.150) PromptOK(DeQtf(`"C:`\`\Program Files`\`\Upp`\`\DinrusIDE`"
));]&]
[s10;l160;%RU-RU [/@(28.42.150) PromptOK(`"`\1C:`\`\Program Files`\`\Upp`\`\DinrusIDE`");
]&]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Функции]]}}&]
[s3; &]
[s0; [@(0.0.255) void]_[* PromptOK]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptOKCancel`(const char`*`): [@(0.0.255) int]_[* PromptOKCancel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptOKCancelAll`(const char`*`): [@(0.0.255) int]_[* PromptOKCancelAll]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptYesNo`(const char`*`): [@(0.0.255) int]_[* PromptYesNo]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptYesNoCancel`(const char`*`): [@(0.0.255) int]_[* PromptYesNoCancel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptYesNoAll`(const char`*`): [@(0.0.255) int]_[* PromptYesNoAll]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptRetryCancel`(const char`*`): [@(0.0.255) int]_[* PromptRetryCancel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptAbortRetry`(const char`*`): [@(0.0.255) int]_[* PromptAbortRetry]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptAbortRetryIgnore`(const char`*`): [@(0.0.255) int]_[* PromptAbortRetryI
gnore]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptSaveDontSaveCancel`(const char`*`): [@(0.0.255) int]_[* PromptSaveDontS
aveCancel]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ShowExc`(const Upp`:`:Exc`&`): [@(0.0.255) void]_[* ShowExc]([@(0.0.255) const]_
[_^Upp`:`:Exc^ Exc][@(0.0.255) `&]_[*@3 exc])&]
[s5;:Upp`:`:Exclamation`(const char`*`): [@(0.0.255) void]_[* Exclamation]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorOK`(const char`*`): [@(0.0.255) void]_[* ErrorOK]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorOKCancel`(const char`*`): [@(0.0.255) int]_[* ErrorOKCancel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorYesNo`(const char`*`): [@(0.0.255) int]_[* ErrorYesNo]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorYesNoCancel`(const char`*`): [@(0.0.255) int]_[* ErrorYesNoCancel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorYesNoAll`(const char`*`): [@(0.0.255) int]_[* ErrorYesNoAll]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorRetryCancel`(const char`*`): [@(0.0.255) int]_[* ErrorRetryCancel]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorAbortRetry`(const char`*`): [@(0.0.255) int]_[* ErrorAbortRetry]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:ErrorAbortRetryIgnore`(const char`*`): [@(0.0.255) int]_[* ErrorAbortRetryIgn
ore]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf])&]
[s5;:Upp`:`:PromptOKOpt`(const char`*`,const char`*`): [@(0.0.255) void]_[* PromptOKOpt](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 o
pt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptOKCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* PromptOK
CancelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptOKCancelAllOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* Promp
tOKCancelAllOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptYesNoOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* PromptYesNo
Opt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptYesNoCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* Promp
tYesNoCancelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptYesNoAllOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* PromptYe
sNoAllOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptRetryCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* Promp
tRetryCancelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptAbortRetryOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* Prompt
AbortRetryOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptAbortRetryIgnoreOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* P
romptAbortRetryIgnoreOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:PromptSaveDontSaveCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_
[* PromptSaveDontSaveCancelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ExclamationOpt`(const char`*`,const char`*`): [@(0.0.255) void]_[* Exclamatio
nOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char
]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ErrorOKOpt`(const char`*`,const char`*`): [@(0.0.255) void]_[* ErrorOKOpt]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id]_`=
_NULL)&]
[s5;:Upp`:`:ErrorOKCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorOKCa
ncelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ErrorYesNoOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorYesNoOp
t]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ErrorYesNoCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorY
esNoCancelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ErrorYesNoAllOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorYesN
oAllOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ErrorRetryCancelOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorR
etryCancelOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id]_`=_NULL)&]
[s5;:Upp`:`:ErrorAbortRetryOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorAb
ortRetryOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorAbortRetryIgnoreOpt`(const char`*`,const char`*`): [@(0.0.255) int]_[* E
rrorAbortRetryIgnoreOpt]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ShowExcOpt`(const Upp`:`:Exc`&`,const char`*`): [@(0.0.255) void]_[* ShowExcO
pt]([@(0.0.255) const]_[_^Upp`:`:Exc^ Exc][@(0.0.255) `&]_[*@3 exc], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptOKOpt1`(const char`*`,const char`*`): [@(0.0.255) void]_[* PromptOKOpt1
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 opt`_id])&]
[s5;:Upp`:`:PromptOKCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* PromptO
KCancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptOKCancelAllOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* Prom
ptOKCancelAllOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptYesNoOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* PromptYesN
oOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptYesNoCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* Prom
ptYesNoCancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptYesNoAllOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* PromptY
esNoAllOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptRetryCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* Prom
ptRetryCancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptAbortRetryOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* Promp
tAbortRetryOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptAbortRetryIgnoreOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_
[* PromptAbortRetryIgnoreOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:PromptSaveDontSaveCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int
]_[* PromptSaveDontSaveCancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ExclamationOpt1`(const char`*`,const char`*`): [@(0.0.255) void]_[* Exclamati
onOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) ch
ar]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorOKOpt1`(const char`*`,const char`*`): [@(0.0.255) void]_[* ErrorOKOpt1](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 o
pt`_id])&]
[s5;:Upp`:`:ErrorOKCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorOKC
ancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorYesNoOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorYesNoO
pt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) char]_
`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorYesNoCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* Error
YesNoCancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorYesNoAllOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorYes
NoAllOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorRetryCancelOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* Error
RetryCancelOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorAbortRetryOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* ErrorA
bortRetryOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ErrorAbortRetryIgnoreOpt1`(const char`*`,const char`*`): [@(0.0.255) int]_[* E
rrorAbortRetryIgnoreOpt1]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 qtf], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s5;:Upp`:`:ShowExcOpt1`(const Upp`:`:Exc`&`,const char`*`): [@(0.0.255) void]_[* ShowExc
Opt1]([@(0.0.255) const]_[_^Upp`:`:Exc^ Exc][@(0.0.255) `&]_[*@3 exc], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 opt`_id])&]
[s2; Существует много различных функций, 
выводящих `"боксы`" сообщений. Первое 
слово в имени функции in camel case определяет 
иконку, используемую в этом боксе, 
и возможный ассоциированный звук. 
За ним следуют названия кнопок и необязатель
ная часть [* Opt] или [* Opt1]. [* Opt] и [* Opt1] обозначают, 
что у этого диалогового окна есть 
опция `"Больше не показывать`" (в дальнейшем).&]
[s2; Если имеется 2 или 3 кнопки, тогда 
функция возвращает int, который репортирует 
о том, какая кнопка нажата. Первая 
кнопка, упомянутая в имени , возвращает 
1, вторая 0 а третья `-1 (напр., в PromptYesNo, 
Yes `= 1, а No `= 0; в PromptYesNoCancel, Yes `= 1, No `= 0, 
а Cancel `= `-1).&]
[s2; Если имя содержит [* Opt] или [* Opt1],процесс 
игнорирования этого диалогового 
окна в другой раз автоматизирован 
через сохранение специфичного ключа 
типа String `- он передаётся функции в 
качестве второго параметра const char 
`* `- если это NULL (дефолт), то он генерируется 
хэшированием сообщения.&]
[s2; Разница между [* Opt] и [* Opt1] состоит в 
том, что [* Opt] запоминает нажатую кнопку 
и возвращает в другой раз то же значение 
(напр., если в PromptOKCancel выбрано `"[%RU-RU Больше 
не показывать]`" и пользователь нажал 
Cancel, в другой раз вызывается этот 
же PromptOKCancel, и он возвращает ноль, не 
показывая диалог). Так как такое поведение 
в некотором контексте может смутить, 
вариант [* Opt1] only grants `"[%RU-RU Больше не показывать]`"
 request if the user chooses button that returns 1 напр., 
OK или Yes).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ClearPromptOptHistory`(`): [@(0.0.255) void]_[* ClearPromptOptHistory]()&]
[s2;%RU-RU Очищает список боксов сообщений 
`"Больше не показывать`".&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ClearPromptOptHistory`(Upp`:`:Gate`<Upp`:`:String`>`): [@(0.0.255) void]_[* C
learPromptOptHistory]([_^Upp`:`:Gate^ Gate]<[_^Upp`:`:String^ String]>_[*@3 filter])&]
[s2;%RU-RU Очищает список боксов сообщений 
`"Больше не показывать`" `- удаляет 
только те, где [%-*@3 filter] возвращает true 
для [%-*@3 opt`_id].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:SerializePromptOptHistory`(Upp`:`:Stream`&`): [@(0.0.255) void]_[* SerializeP
romptOptHistory]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Сериализует список боксов сообщений 
`"Больше не показывать`".&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:RedirectPrompts`(Upp`:`:RedirectPromptFn`): [@(0.0.255) void]_[* RedirectProm
pts]([_^Upp`:`:RedirectPromptFn^ RedirectPromptFn]_[*@3 r])&]
[s2;%RU-RU Полностью перенаправляет все 
боксы сообщений в другую функцию.&]
[s3;%RU-RU &]
[s0; ]]