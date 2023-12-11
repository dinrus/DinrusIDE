topic "Общие Классы Растров";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс ImageRaster]]}}&]
[s3; &]
[s1;:ImageRaster`:`:class: [@(0.0.255)3 class][3 _][*3 ImageRaster][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Raster]&]
[s9;%RU-RU Этот класс предоставляет интерфейс 
Raster к существующему Image.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:ImageRaster`:`:ImageRaster`(const Image`&`): [* ImageRaster]([@(0.0.255) const]_[_^Image^ I
mage][@(0.0.255) `&]_[*@3 img])&]
[s2;%RU-RU Создаёт растр Raster из изо [%-*@3 img].&]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс MemoryRaster]]}}&]
[s3;%RU-RU &]
[s1;:MemoryRaster`:`:class: [@(0.0.255)3 class][3 _][*3 MemoryRaster][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Raster]&]
[s9;%RU-RU Этот класс представляет рисунок 
в специфичном формате, сохранённый 
в памяти. У него косвенный пик`-трансфер.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:MemoryRaster`:`:Load`(Raster`&`): [@(0.0.255) void]_[* Load]([_^Raster^ Raster][@(0.0.255) `&
]_[*@3 raster])&]
[s2;%RU-RU Загружает раст [%-*@3 raster] в память 
MemoryRaster. Сохраняет формат источника.&]
[s3;%RU-RU &]
[s4; &]
[s5;:MemoryRaster`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает число байтов, использованных
 для сохранения скан`-линий MemoryRaster.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Конструктор  detail]]}}&]
[s3; &]
[s5;:MemoryRaster`:`:MemoryRaster`(`): [* MemoryRaster]()&]
[s2;%RU-RU Конструирует пустой растр в памяти 
MemoryRaster.&]
[s3; &]
[s4; &]
[s5;:MemoryRaster`:`:MemoryRaster`(Raster`&`): [* MemoryRaster]([_^Raster^ Raster][@(0.0.255) `&
]_[*@3 raster])&]
[s2;%RU-RU Загружает [%-*@3 raster] в память MemoryRaster. 
Сохраняет формат источника.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[s0;%RU-RU &]
[s0;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс StreamRaster]]}}&]
[s3;%RU-RU &]
[s1;:StreamRaster`:`:class: [@(0.0.255)3 class][3 _][*3 StreamRaster][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Raster]&]
[s9;%RU-RU Представляет собой изображение, 
сохранённое в поток Stream; обычно используется
 в качестве класса`-основы для декодеров 
изображений, напр., PNGRaster. Могут регистрирова
ться производные от StreamRaster классы, 
в таком случае `"фабрика`" такого класса 
сохраняется в массиве и пытается 
загружать файл неизвестного формата.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:StreamRaster`:`:GetStream`(`): [_^Stream^ Stream][@(0.0.255) `&]_[* GetStream]()&]
[s2;%RU-RU Возвращает ассоциированный поток.&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:Open`(Stream`&`): [@(0.0.255) bool]_[* Open]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Ассоциирует поток [%-*@3 s].с растром 
в памяти StreamRaster`'ом (сохраняется ссылка) 
и вызывает виртуальный метод Create, 
после чего производный класс может 
прочесть заглавие потока и подготовиться к чтению сканлиний. Если Create сигнализирует
 о неудаче, возвращая false, тогда StreamRaster устанавливается в состояние ошибки 
и возвращается false.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StreamRaster`:`:IsError`(`): [@(0.0.255) bool]_[* IsError]()&]
[s2;%RU-RU Возвращает true, если возникла какая`-либо ошибка. Переустанавливается (сбрасывается) 
в методе Open.&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:SetError`(`): [@(0.0.255) void]_[* SetError]()&]
[s2;%RU-RU Вводит растр в памяти StreamRaster в состояние ошибки.&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:Load`(Stream`&`,const Gate2`<int`,int`>`): [_^Image^ Image]_[* Load](
[_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) in
t], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:StreamRaster`:`:LoadFile`(const char`*`,const Gate2`<int`,int`>`): [_^Image^ Image
]_[* LoadFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], [@(0.0.255) const]_[_^Gate2^ G
ate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:StreamRaster`:`:LoadString`(const String`&`,const Gate2`<int`,int`>`): [_^Image^ I
mage]_[* LoadString]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Используя виртуальные методы Raster, загружает изо Image из потока Stream, 
из файла или из строки. [%-*@3 progress] может использоваться для отслеживания прогресса и отмены загрузки (возвратом 
true).&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Registered Decoders and multi`-format support]]}}&]
[s4;H0; &]
[s5;:StreamRaster`:`:Register`(`): [@(0.0.255) static] [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) void]_[* Register]()&]
[s2;%RU-RU Регистрирует производный от StreamRaster класс [%-*@4 T].&]
[s3; &]
[s4; &]
[s5;:StreamRaster`:`:OpenAny`(Stream`&`): [@(0.0.255) static] [_^One^ One]<[_^StreamRaster^ S
treamRaster]>_[* OpenAny]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Пытается открыть растр изо, сохранённый 
в [%-*@3 s], используя все производные 
от StreamRaster классы (декодеры), зарегистрирован
ные посредством Register (обычно, установленные 
форматы файлов изображений, типа 
png, gif, bmp регистрируются).Если любой 
из декодером способен открыть этот 
поток, возвращает контенер One с открытым 
декодером, иначе возвращаемый One пуст.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StreamRaster`:`:LoadAny`(Stream`&`,const Gate2`<int`,int`>`): [@(0.0.255) static] 
[_^Image^ Image]_[* LoadAny]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s5;:StreamRaster`:`:LoadFileAny`(const char`*`,const Gate2`<int`,int`>`): [@(0.0.255) s
tatic] [_^Image^ Image]_[* LoadFileAny]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 fn], 
[@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s5;:StreamRaster`:`:LoadStringAny`(const String`&`,const Gate2`<int`,int`>`): [@(0.0.255) s
tatic] [_^Image^ Image]_[* LoadStringAny]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 s], [@(0.0.255) const]_[_^Gate2^ Gate2]<[@(0.0.255) int], [@(0.0.255) int]>__`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Пытается прочесть изо Image из потока, 
из файла или из строки, используя 
все зарегистрированные декодеры. 
[%-*@3 progress] может использоваться для 
отслеживания прогресса операции 
или её отмены, возвратом true.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Конструктор  detail]]}}&]
[s4;H0; &]
[s5;:StreamRaster`:`:StreamRaster`(`): [* StreamRaster]()&]
[s2;%RU-RU Ставит StreamRaster в состояние ошибки 
(состояние ошибки сбрасывается посредством 
Open).&]
[s3; &]
[s0; ]]