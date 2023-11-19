topic "Класс RasterEncoder";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс RasterEncoder]]}}&]
[s3;%- &]
[s1;:RasterEncoder`:`:class:%- [@(0.0.255)3 class][3 _][*3 RasterEncoder]&]
[s9; RasterEncoder служит потребителем данных 
сканлиний RGBA. Обычно используется 
как кодировщик в один из общих файловых 
форматов изображений, наподобие PNG. 
RasterEncoder предоставляет буфер вывода 
для кода`-клиента (производителя) 
для сохранения выводной сканлинии 
RGBA, когда сканлиния окончена, производитель 
вызывает WriteLine и RasterEncoder преобразует 
сканлинию RGBA в специфичный формат, 
в соответствии с защищённой переменной`-член
ом [* format], а затем вызывает виртуальный 
метод WriteLineRaw, который выполняет окончательн
ую обработку (напр., PNGEncoder сохраняет 
её в выводной поток PNG).&]
[s9; Оптимизация RGBA: Требуется особо избегать 
повторных преобразований и копирований, 
когда целью является обычный Image (с 
ImageEncoder). Метод SetLine может заменить 
дефолтный буфер сканлинии, предоставленный 
RasterEncoder`'ом; в таком случае он может 
использовать указатель на данные 
ImageBuffer`'а, и далее сохранять эти данные 
прямо в ImageBuffer (ImageEncoder вызывает SetLine 
в методах Start и WriteLineRaw). WriteLineRaw затем 
может проверять, указывает ли [%-@3 data] 
на рекомендуемый буфер (это негарантировано
, т.к. производитель может предоставлять 
ещё один буфер сканлайн в WriteLine), и 
в этом случае копирование в целевую 
сканлинию можно полностью отменить.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0;%- [* Список Защищённых Членов]]}}&]
[s3;%- &]
[s5;:RasterEncoder`:`:format:%- [_^RasterFormat^ RasterFormat]_[* format]&]
[s2; Производный кодер должен сохранять 
здесь требуемый формат преобразования 
в методе Start.&]
[s3;%- &]
[s4;%- &]
[s5;:RasterEncoder`:`:SetLine`(RGBA`*`):%- [@(0.0.255) void]_[* SetLine]([_^RGBA^ RGBA]_`*[*@3 `_
line])&]
[s2; Заменяет дефолтный буфер сканлинии, 
чтобы оптимизировать хранение данных 
RGBA в прямых целях.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:RasterEncoder`:`:GetPaletteCount`(`):%- [@(0.0.255) virtual] [@(0.0.255) int]_[* GetPa
letteCount]()&]
[s2; Возвращает число цветов в палитре 
(или ноль).&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:Start`(Size`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Start]([_^Size^ S
ize]_[*@3 sz])_`=_[@3 0]&]
[s2; Вызывается в начале обработки [%-*@3 sz].&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:WriteLineRaw`(const byte`*`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* WriteLineRaw]([@(0.0.255) const]_[_^byte^ byte]_`*[*@3 data])_`=_[@3 0]&]
[s2; Вызывается для записи данных единичной 
сканлинии [%-*@3 data] в вывод.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:WriteLine`(`):%- [@(0.0.255) void]_[* WriteLine]()&]
[s2; Кодирует вутренний буфер сканлинии 
RasterEncoder`'а `- преобразует его в требуемый 
формат и вызывает WriteLineRaw.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:WriteLine`(const RGBA`*`):%- [@(0.0.255) void]_[* WriteLine]([@(0.0.255) c
onst]_[_^RGBA^ RGBA]_`*[*@3 s])&]
[s2; Кодирует сканлинию [%-*@3 s] `- преобразует 
его в требуемый формат и вызывает 
WriteLineRaw.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetFormat`(`)const:%- [@(0.0.255) const]_[_^RasterFormat^ RasterFor
mat][@(0.0.255) `&]_[* GetFormat]()_[@(0.0.255) const]&]
[s2; Возвращает текущий формат [* format].&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetPalette`(`):%- [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[* GetPalette](
)&]
[s2; Возвращает текущую палитру.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetPaletteCv`(`):%- [@(0.0.255) const]_[_^PaletteCv^ PaletteCv]_`*[* G
etPaletteCv]()&]
[s2; Возвращает конвертор палитр.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:operator RGBA`*`(`):%- [@(0.0.255) operator]_[_^RGBA^ RGBA]_`*()&]
[s5;:RasterEncoder`:`:operator`~`(`):%- [_^RGBA^ RGBA]_`*[* operator`~]()&]
[s2; Возвращает указатель на буфер сканлинии, 
используемый в беспараметрном WriteLine.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetSize`(`)const:%- [_^Size^ Size]_[* GetSize]()_[@(0.0.255) const]&]
[s2; Возвращает размер.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetWidth`(`)const:%- [@(0.0.255) int]_[* GetWidth]()_[@(0.0.255) cons
t]&]
[s2; То же, что и GetSize().cx.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) co
nst]&]
[s2; То же, что и GetSize().cy.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetHotSpot`(Point`):%- [@(0.0.255) void]_[* SetHotSpot]([_^Point^ Poi
nt]_[*@3 p])&]
[s2; Устанавливает горячую точку. Используется 
для определения курсоров экрана (файлов 
.ico или .cur в Win32). Производному кодеру 
может потребоваться информация о 
горячей точке (он считывает её, вызывая 
GetHotSpot, и сохраняет в файле вывода 
в Start).&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetHotSpot`(`)const:%- [_^Point^ Point]_[* GetHotSpot]()_[@(0.0.255) c
onst]&]
[s2; Считывает горячую точку, установленную 
SetHotSpot.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetDots`(Size`):%- [@(0.0.255) void]_[* SetDots]([_^Size^ Size]_[*@3 `_
dots])&]
[s2; Устанавливает физический размер 
выводимого изображения, в 1/600 дюйма. 
Производный кодер должен считать 
эту информацию в Start и сохранить в 
файл вывода, если заданный формат 
поддерживает соответствующую информацию.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:GetDots`(`)const:%- [_^Size^ Size]_[* GetDots]()_[@(0.0.255) const]&]
[s2; Возвращает размер, установленный 
SetDots.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetPalette`(const RGBA`*`):%- [@(0.0.255) void]_[* SetPalette]([@(0.0.255) c
onst]_[_^RGBA^ RGBA]_`*[*@3 palette])&]
[s2; Устанавливает палитру [%-*@3 palette], которая 
будет использована с растром.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:SetPalette`(Raster`&`):%- [@(0.0.255) void]_[* SetPalette]([_^Raster^ R
aster][@(0.0.255) `&]_[*@3 pal`_raster])&]
[s2; Генерирует оптимальную палитру на 
основе цветов из [%-*@3 pal`_raster] и устанавливает 
её для дальнейшей обработки.&]
[s3; &]
[s4; &]
[s5;:RasterEncoder`:`:Create`(Size`):%- [@(0.0.255) void]_[* Create]([_^Size^ Size]_[*@3 sz])
&]
[s5;:RasterEncoder`:`:Create`(int`,int`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) int]_[*@3 c
x], [@(0.0.255) int]_[*@3 cy])&]
[s5;:RasterEncoder`:`:Create`(Size`,const RGBA`*`):%- [@(0.0.255) void]_[* Create]([_^Size^ S
ize]_[*@3 sz], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 palette])&]
[s5;:RasterEncoder`:`:Create`(int`,int`,const RGBA`*`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) i
nt]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 palette])&]
[s5;:RasterEncoder`:`:Create`(Size`,Raster`&`):%- [@(0.0.255) void]_[* Create]([_^Size^ Siz
e]_[*@3 sz], [_^Raster^ Raster][@(0.0.255) `&]_[*@3 pal`_raster])&]
[s5;:RasterEncoder`:`:Create`(int`,int`,Raster`&`):%- [@(0.0.255) void]_[* Create]([@(0.0.255) i
nt]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [_^Raster^ Raster][@(0.0.255) `&]_[*@3 pal`_raster])
&]
[s2; Инициализирует кодировщик на вывод 
изображения указанного размера, опционно 
устанавливая палитру.&]
[s3; &]
[s0; ]]