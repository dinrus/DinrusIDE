topic "Функции поддержки основных форматов PasteClip";
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
[{_} 
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции поддержки основных 
форматов PasteClip.]]}}&]
[s3; &]
[s5;:ClipFmtsText`(`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* ClipFmtsText]()&]
[s2;%RU-RU Возвращает список идентификаторов 
формата, ассоциированных с простым 
текстом (реально возвращает `"text;wtext`").&]
[s3; &]
[s4; &]
[s5;:AcceptText`(PasteClip`&`): [@(0.0.255) bool]_[* AcceptText]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%RU-RU Принимает [%-*@3 clip], если это текст 
(возвращает true для броса (drop)).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetString`(PasteClip`&`): [_^String^ String]_[* GetString]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s5;:GetWString`(PasteClip`&`): [_^WString^ WString]_[* GetWString]([_^PasteClip^ PasteClip
][@(0.0.255) `&]_[*@3 clip])&]
[s2;%RU-RU Получает текст из [%-*@3 clip].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTextClip`(const String`&`,const String`&`): [_^String^ String]_[* GetTextClip]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 text], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])&]
[s5;:GetTextClip`(const WString`&`,const String`&`): [_^String^ String]_[* GetTextClip]([@(0.0.255) c
onst]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])&]
[s2;%RU-RU Созданные ClipData двоичные данные 
для текста [%-*@3 text], основанные на формате 
[%-*@3 fmt].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Append`(VectorMap`<String`,ClipData`>`&`,const String`&`): [@(0.0.255) void]_[* Appe
nd]([_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>`&_[*@3 data], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s5;:Append`(VectorMap`<String`,ClipData`>`&`,const WString`&`): [@(0.0.255) void]_[* App
end]([_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>`&_[*@3 data], 
[@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%RU-RU Приставляет текст [%-*@3 text], как форматы 
`"text`" и `"wtext`", к данным [%-*@3 data].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ClipFmtsImage`(`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* ClipFmtsImage]()&]
[s2;%RU-RU Возвращает специфичный для хостовой 
платформы, разделённый точкой с запятой, 
список идентификаторов форматов, 
ассоциированных с растровыми рисунками.&]
[s3; &]
[s4; &]
[s5;:AcceptImage`(PasteClip`&`): [@(0.0.255) bool]_[* AcceptImage]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%RU-RU Принимает [%-*@3 clip], если он содержит 
растровый рисунок (возвращает true 
для броса).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetImage`(PasteClip`&`): [_^Image^ Image]_[* GetImage]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%RU-RU Извлекает рисунок Image из клипа 
[%-*@3 clip].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetImageClip`(const Image`&`,const String`&`): [_^String^ String]_[* GetImageClip]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])&]
[s2;%RU-RU Конструирует двоичные данные 
клипа из рисунка [%-*@3 m] формата [%-*@3 fmt].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Append`(VectorMap`<String`,ClipData`>`&`,const Image`&`): [@(0.0.255) void]_[* Appen
d]([_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>`&_[*@3 data], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Приставляет платформо`-специфичные 
форматы растровых рисунков из [%-*@3 img] 
в [%-*@3 data].&]
[s3;%RU-RU &]
[s4; &]
[s5;:IsAvailableFiles`(PasteClip`&`): [@(0.0.255) bool]_[* IsAvailableFiles]([_^PasteClip^ P
asteClip][@(0.0.255) `&]_[*@3 clip])&]
[s2;%RU-RU Возвращает true, если [%-*@3 clip].содержит 
список файлов оболочки ОС (операция 
тяга из списка файлов оболочки ОС 
в приложение U`+`+).&]
[s3;%RU-RU &]
[s4; &]
[s5;:AcceptFiles`(PasteClip`&`): [@(0.0.255) bool]_[* AcceptFiles]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%RU-RU Принимает [%-*@3 clip], если он содержит 
список файлов оболочки ОС.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetClipFiles`(const String`&`): [_^Vector^ Vector]<[_^String^ String]>_[* GetClipFiles
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%RU-RU Извлекает список файлов оболочки 
ОС из сырых данных буфера обмена [%-*@3 data] 
(результат вызова ClipData`::Get).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetFiles`(PasteClip`&`): [_^Vector^ Vector]<[_^String^ String]>_[* GetFiles]([_^PasteClip^ P
asteClip][@(0.0.255) `&]_[*@3 clip])&]
[s2;%RU-RU Извлекает список фалов оболочки 
ОС из [%-*@3 clip].&]
[s3;%RU-RU &]
[s4; &]
[s5;:ClipFmt`(`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^String^ String]_[* Clip
Fmt]()&]
[s2;%RU-RU Конструирует специальный ид формата 
клипа, основываясь на типе. Применим 
при переносе данных между приложениями 
U`+`+&]
[s3; &]
[s4; &]
[s5;:Accept`(PasteClip`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) bo
ol]_[* Accept]([_^PasteClip^ PasteClip][@(0.0.255) `&]_[*@3 clip])&]
[s2;%RU-RU Принимает [%-*@3 clip], если он содержит 
ид формата, основанный на ClipFmt.&]
[s3;%RU-RU &]
[s4; &]
[s5;:InternalClip`(const T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>_[* InternalClip](
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 i
d]_`=_`"`")&]
[s2;%RU-RU Создаёт `'внутренний`' клип для 
переменной [%-*@3 x]. Внутренние клипы 
работают только в рамках одного процесса, 
так как используют простые указатели 
на объекты. [%-*@3 x] должно существовать 
до тех пор, пока клип доступен.&]
[s3;%RU-RU &]
[s4; &]
[s5;:IsAvailableInternal`(PasteClip`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) bool]_[* IsAvailableInternal]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"`")&]
[s2;%RU-RU Возвращает true, если имеется внутренний 
клип требуемого типа и ид [%-*@3 id ]находится 
в данных [%-*@3 d].&]
[s3;%RU-RU &]
[s4; &]
[s5;:AcceptInternal`(PasteClip`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 T]>_[@(0.0.255) bool]_[* AcceptInternal]([_^PasteClip^ PasteClip][@(0.0.255) `&]_[*@3 d
], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"`")&]
[s2;%RU-RU Принимает внутренний клип требуемого 
типа и [%-*@3 id].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetInternal`(PasteClip`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[* GetInternal]([_^PasteClip^ PasteClip][@(0.0.255) `&]_[*@3 d])
&]
[s2;%RU-RU Возвращает ссылку на внутренний 
клип.&]
[s4; &]
[s5;:Upp`:`:GetInternalPtr`(Upp`:`:PasteClip`&`,const char`*`): [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) const]_[*@4 T]_`*[* GetInternalPtr]([_^Upp`:`:PasteClip^ P
asteClip][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"`")&]
[s2;%RU-RU Если доступны внутренний клип 
требуемого типа и [%-*@3 id], возвращает 
указатель на него, NULL `- в противном 
случае.&]
[s3;%RU-RU ]]