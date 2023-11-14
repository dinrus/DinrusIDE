topic "RichText и вывод HTML";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс HtmlObjectSaver]]}}&]
[s3;%- &]
[s1;:Upp`:`:HtmlObjectSaver`:`:class:%- [@(0.0.255)3 class][3 _][*3 HtmlObjectSaver]&]
[s2; Это вспомогательный абстрактный 
класс, используемый для сохранения 
экземпляров RichObject при экспорте RichText 
в HTML.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:Upp`:`:HtmlObjectSaver`:`:GetHtml`(const Upp`:`:RichObject`&`):%- [@(0.0.255) virt
ual] [_^Upp`:`:String^ String]_[* GetHtml]([@(0.0.255) const]_[_^Upp`:`:RichObject^ RichO
bject][@(0.0.255) `&]_[*@3 object])&]
[s2; Экспортирует объект [%-*@3 object]. Дефолтная 
реализация пустая.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:HtmlObjectSaver`:`:GetHtml`(const Upp`:`:RichObject`&`,const Upp`:`:String`&`):%- [@(0.0.255) v
irtual] [_^Upp`:`:String^ String]_[* GetHtml]([@(0.0.255) const]_[_^Upp`:`:RichObject^ Ri
chObject][@(0.0.255) `&]_[*@3 object], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 link])&]
[s2; Экспортирует объект [%-*@3 object], имеющтй 
на данный момент якорь [%-*@3 link]. Дефолтная 
реализация вызывает GetHtml с одним параметром.
&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Функции экспорта]]}}&]
[s3;%- &]
[s5;:Upp`:`:EncodeHtml`(const Upp`:`:RichText`&`,Upp`:`:Index`<Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,const Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Zoom`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,int`):%- [_^Upp`:`:String^ S
tring]_[* EncodeHtml]([@(0.0.255) const]_[_^Upp`:`:RichText^ RichText][@(0.0.255) `&]_[*@3 t
ext], [_^Upp`:`:Index^ Index]<[_^Upp`:`:String^ String]>`&_[*@3 css], 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 links], [@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorM
ap]<[_^Upp`:`:String^ String], [_^Upp`:`:String^ String]>`&_[*@3 labels], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 base]_`=_Null, 
[_^Upp`:`:Zoom^ Zoom]_[*@3 z]_`=_[_^Upp`:`:Zoom^ Zoom]([@3 8], [@3 40]), 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 escape]_`=_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ S
tring], [_^Upp`:`:String^ String]>(), [@(0.0.255) int]_[*@3 imtolerance]_`=_[@3 0])&]
[s5;:Upp`:`:EncodeHtml`(const Upp`:`:RichText`&`,Upp`:`:Index`<Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,Upp`:`:HtmlObjectSaver`&`,Upp`:`:Zoom`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`):%- [_^Upp`:`:String^ S
tring]_[* EncodeHtml]([@(0.0.255) const]_[_^Upp`:`:RichText^ RichText][@(0.0.255) `&]_[*@3 t
ext], [_^Upp`:`:Index^ Index]<[_^Upp`:`:String^ String]>`&_[*@3 css], 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 links], [@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorM
ap]<[_^Upp`:`:String^ String], [_^Upp`:`:String^ String]>`&_[*@3 labels], 
[_^Upp`:`:HtmlObjectSaver^ HtmlObjectSaver][@(0.0.255) `&]_[*@3 object`_saver], 
[_^Upp`:`:Zoom^ Zoom]_[*@3 z]_`=_[_^Upp`:`:Zoom^ Zoom]([@3 8], [@3 40]), 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 escape]_`=_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ S
tring], [_^Upp`:`:String^ String]>())&]
[s2; Экспортирует текст RichText [%-*@3 text] в формат 
html. &]
[s2; [%-*@3 css] устанавливается в определениях 
CSS. Имя класса CSS `- это индекс определения, 
отформатированный посредством FormatIntAlpha.&]
[s2; [%-*@3 links] `- опциональный набор линк`-трансляций
. Вначале проводится попытка выполнить 
всю линк`-трансляцию, затем попытка 
транслировать только часть, расположенную 
перед `'#`'.&]
[s2; [%-*@3 labels] `- опциональный набор трансляций 
ярлыков.&]
[s2; [%-*@3 object`_saver] `- экземпляр HtmlObjectSaver, используемы
й для сохранения экземпляров RichObject.&]
[s2; [%-*@3 z] `- фактор зума (масштабирования).&]
[s2; [%-*@3 escape] `- опциональный мап, транслирующий 
индексные записи RichText в сырой код 
html, который внедряется в тело документа 
(body). &]
[s2; [%-*@3 path] `- перегрузка с этим параметром 
использует дефолтный HtmlObjectSaver, чтобы 
экспортировать экземпляры RichObject 
как .png в директорию [%-*@3 path], как с масштабирова
нным размером, так и с оригинальным, 
если при этом отсутствует ссылка, 
ассоциированная с этим изображением.&]
[s2; [%-*@3 base] `- префикс, добавляемый к имени 
.png, потенциально используемый для 
того, чтобы сделать его уникальным, 
если есть вероятность конфликта, 
в слкчае если директория вывода общая 
для ряда страниц .html.&]
[s2; [%-*@3 imtolerance] `- максимальная толерантность 
ошибки размера изображения до того, 
как он ремасштабируется в процентном 
соотношении.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AsCss`(Upp`:`:Index`<Upp`:`:String`>`&`):%- [_^Upp`:`:String^ String]_[* AsCs
s]([_^Upp`:`:Index^ Index]<[_^Upp`:`:String^ String]>`&_[*@3 ss])&]
[s2; Преобразует набор определений класса 
CSS, произведённый EncodeHtml, в блок css HTML. 
Цель отделения этого процесса от 
произведения тела EncodeHtml в том, чтобы 
добиться возможности комбинировать 
несколько тел (bodies) (несколькими вызовами 
EncodeHtml) в одну страницу HTML.&]
[s2; &]
[s3; &]
[s0; ]]