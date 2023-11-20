topic "Класс RasterFormat";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс RasterFormat]]}}&]
[s3; &]
[s1;:RasterFormat`:`:struct: [@(0.0.255)3 class][3 _][*3 RasterFormat]&]
[s9;%RU-RU Этот класс служит как описание 
и как конвертер для различных пиксельных 
форматов. В U`+`+, как правило, используется 
фиксированный 32`-битный формат RGBA 
для всякой обработки, но требуются 
и специальные форматы, напр., для работы 
с файлами изображений или для сбережения 
памяти.&]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Растровые Типы]]}}&]
[s3; &]
[ {{3523:6477^ [s0; RASTER`_1]
:: [s0; 1 бит на пиксель (2 цвета, с палитрой).]
:: [s0; RASTER`_2]
:: [s0; 2 бита на пиксель (4 цвета, с палитрой).]
:: [s0; RASTER`_4]
:: [s0; 4 бита на пиксель (16 цветов, с палитрой).]
:: [s0; RASTER`_8]
:: [s0; 8 битов на пиксель (256 цветов, с палитрой).]
:: [s0; RASTER`_8ALPHA]
:: [s0; 16 битов на пиксель; 8 бит для цвета 
(256 цветов, с палитрой) и 8 бит для альфы.]
:: [s0; RASTER`_16]
:: [s0; 16 битов на пиксель (65536 цветов без 
палитры), число битов и параметры 
для отдельных каналов зависят от 
других переменных.]
:: [s0; RASTER`_24]
:: [s0; 24 бита на пиксель RGB (truecolor), число 
битов и параметры для отдельных каналов 
зависят от других переменных.]
:: [s0; RASTER`_32]
:: [s0; 32 бита на пиксель RGB (truecolor), число 
битов и параметры для отдельных каналов 
зависят от других переменных.]
:: [s0; RASTER`_32ALPHA]
:: [s0; 32 бита на пиксель RGBA (truecolor), альфа 
не премультиплицируется, число битов 
и параметры для отдельных каналов 
зависят от других переменных.]
:: [s0; RASTER`_32PREMULTIPLIED]
:: [s0; 32 бита на пиксель RGBA (truecolor), альфа 
не премультиплицируется, число битов 
и параметры для отдельных каналов 
зависят от других переменных.]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;i448;a25;kKO9; Все эти значения могут комбинироват
ься (оператором`|) с&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s0;i448;a25;kKO9; RASTER`_MSBFIRST-|-|Указывает, что байты 
или nibble`'ы в реверсированном порядке 
(в big endian).&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:RasterFormat`:`:Set1lf`(`): [@(0.0.255) void]_[* Set1lf]()&]
[s2; Устанавливает формат 1 бит на пиксель 
(RASTER`_1).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set1mf`(`): [@(0.0.255) void]_[* Set1mf]()&]
[s2;%RU-RU Устанавливает формат 1 бит на 
пиксель в реверсированном режиме 
MSB (RASTER`_1`| RASTER`_MSBFIRST).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set2lf`(`): [@(0.0.255) void]_[* Set2lf]()&]
[s2;%RU-RU Устанавливает формат 2 бита на 
пиксель (RASTER`_2).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set2mf`(`): [@(0.0.255) void]_[* Set2mf]()&]
[s2;%RU-RU Устанавливает формат 2 бита на 
пиксель в реверсированном режиме 
MSB (RASTER`_2`| RASTER`_MSBFIRST).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set4lf`(`): [@(0.0.255) void]_[* Set4lf]()&]
[s2;%RU-RU Устанавливает формат 4 бита на 
пиксель (RASTER`_4).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set4mf`(`): [@(0.0.255) void]_[* Set4mf]()&]
[s2;%RU-RU Устанавливает формат 4 бита на 
пиксель в реверсированном режиме 
MSB (RASTER`_4`|RASTER`_MSBFIRST).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set8`(`): [@(0.0.255) void]_[* Set8]()&]
[s2;%RU-RU Устанавливает формат 8 битов на 
пиксель (RASTER`_8).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set8A`(`): [@(0.0.255) void]_[* Set8A]()&]
[s2;%RU-RU Устанавливает формат 8`+8 битов 
на пиксель, где 8 битов `- для цвета 
с палитрой и 8 битов `- для альфы (RASTER`_8ALPHA).&]
[s3; &]
[s4; &]
[s5;:RasterFormat`:`:Set16le`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set16le]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%RU-RU Устанавливает формат 16 битов 
на пиксель, параметры каналов определяются 
по маске канала в режиме little`-endian (RASTER`_16).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set16be`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set16be]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%RU-RU Устанавливает формат 16 битов 
на пиксель, параметры каналов определяются 
по маске канала в режиме big`-endian (RASTER`_16 
`| RASTER`_MSBFIRST).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set24le`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set24le]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%RU-RU Устанавливает формат 24 бита на 
пиксель, параметры каналов определяются 
по маске канала в режиме little`-endian (RASTER`_24).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set24be`(dword`,dword`,dword`): [@(0.0.255) void]_[* Set24be]([_^dword^ d
word]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask])&]
[s2;%RU-RU Устанавливает формат 24 бита на 
пиксель, параметры каналов определяются 
по маске канала в режиме big`-endian (RASTER`_24 
`| RASTER`_MSBFIRST).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set32le`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* Set32le](
[_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask], 
[_^dword^ dword]_[*@3 amask]_`=_[@3 0])&]
[s2;%RU-RU Устанавливает формат 32 бита на 
пиксель, параметры каналов определяются 
по маске канала в режиме little`-endian . 
Если [%-*@3 amask ] !`= ноль, то формат с премультипли
цированной альфой (RASTER`_32PREMULTIPLIED), 
иначе он без альфы (RASTER`_32).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set32be`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* Set32be](
[_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], [_^dword^ dword]_[*@3 bmask], 
[_^dword^ dword]_[*@3 amask]_`=_[@3 0])&]
[s2;%RU-RU Устанавливает формат 32 бита на 
пиксель, параметры каналов определяются 
по маске канала в режиме big`-endian mode. 
 Если [%-*@3 amask ] !`= ноль, то формат с премультипли
цированной альфой  (RASTER`_32PREMULTIPLIED `| 
RASTER`_MSBFIRST),  иначе он без альфы (RASTER`_32`|RASTER`_MSBFIR
ST).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set32leStraight`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* S
et32leStraight]([_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], 
[_^dword^ dword]_[*@3 bmask], [_^dword^ dword]_[*@3 amask])&]
[s2;%RU-RU Устанавливает формат 32 бита на 
пиксель,параметры каналов определяются 
по маске канала в режиме little`-endian с 
непремультиплицированной альфой 
(RASTER`_32ALPHA).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Set32beStraight`(dword`,dword`,dword`,dword`): [@(0.0.255) void]_[* S
et32beStraight]([_^dword^ dword]_[*@3 rmask], [_^dword^ dword]_[*@3 gmask], 
[_^dword^ dword]_[*@3 bmask], [_^dword^ dword]_[*@3 amask])&]
[s2;%RU-RU Устанавливает формат 32 бита на 
пиксель,параметры каналов определяются 
по маске канала в режиме little`-endian с 
непремультиплицированной альфой 
(RASTER`_32ALPHA `| RASTER`_MSBFIRST).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:SetRGBA`(`): [@(0.0.255) void]_[* SetRGBA]()&]
[s2;%RU-RU Устанавливает режим, который 
побитно (bitwise) эквивалентен стандартным 
RGBA quads, используемым в изо Image.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:SetRGBAStraight`(`): [@(0.0.255) void]_[* SetRGBAStraight]()&]
[s2;%RU-RU Подобен SetRGBA, но с непремультиплицирован
ной альфой.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:GetType`(`)const: [@(0.0.255) int]_[* GetType]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущий формат.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:IsRGBA`(`)const: [@(0.0.255) int]_[* IsRGBA]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если формат побитно эквивалентен 
стандартным RGBA quads, используемым в 
изо Image.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:GetByteCount`(int`)const: [@(0.0.255) int]_[* GetByteCount]([@(0.0.255) i
nt]_[*@3 cx])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число байтов, требуемое 
для пикселей [%-*@3 cx] в текущем формате.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:GetBpp`(`)const: [@(0.0.255) int]_[* GetBpp]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает число битов на пиксель 
в текущем формате.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:HasAlpha`(`)const: [@(0.0.255) bool]_[* HasAlpha]()_[@(0.0.255) const]&]
[s2;%RU-RU True, если у текущего формата есть 
альфа.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:GetColorCount`(`)const: [@(0.0.255) int]_[* GetColorCount]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает число цветов, поддерживаемых
 текущим форматом.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:GetPaletteCount`(`)const: [@(0.0.255) int]_[* GetPaletteCount]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает размер палитры текущего 
формата. Если у формата нет палитры, 
то возвращается 0.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Read`(RGBA`*`,const byte`*`,int`,const RGBA`*`)const: [@(0.0.255) v
oid]_[* Read]([_^RGBA^ RGBA]_`*[*@3 t], [@(0.0.255) const]_[_^byte^ byte]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 palette])_[@(0.0.255) con
st]&]
[s2;%RU-RU Преобразует сканлинию [%-*@3 s], составленную
 из [%-*@3 cx] пикселей в текущем формате, 
в обычную сканлинию RGBA [%-*@3 t], используя 
палитру [%-*@3 palette] (игнорируемую для 
16`-битных и truecolor форматов). Если у 
формата отсуствует альфа, то всем 
её целевым пикселям присваивается 
255 (непрозрачный, opaque).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:RasterFormat`:`:Write`(byte`*`,const RGBA`*`,int`,const PaletteCv`*`)const: [@(0.0.255) v
oid]_[* Write]([_^byte^ byte]_`*[*@3 t], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 s], 
[@(0.0.255) int]_[*@3 cx], [@(0.0.255) const]_[_^PaletteCv^ PaletteCv]_`*[*@3 palcv])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Преобразует обычную сканлиниюr 
RGBA [%-*@3 s] из [%-*@3 cx] пикселей в сканлинию 
[%-*@3 t] в текущем формате, используя 
[%-*@3 palcv] для преобразования truecolor в 
палитровые форматы (игнорируется 
для 16`-битных и truecolor форматов).&]
[s3; &]
[s0; ]]