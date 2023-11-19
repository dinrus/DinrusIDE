topic "Структура ImageMaker - система кэша рисунка";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура ImageMaker `- система кэша 
рисунка]]}}&]
[s1;@(0.0.255)3 &]
[s0;%RU-RU Система кэша Image предоставляет 
средства для создания и кэширования 
Image`'й. Например, в ГИП могут потребоваться 
монохроматические версии иконок, 
для выражения отключенных состояний. 
В таком случае, может быть определён 
производный от ImageMaker класс, чтобы 
делать такие Image из цветной версии. 
MakeImage вводит использование механизма 
кэширования. Кэш изображений использует 
глобальный кжш значений, как `"глубинный`" 
механизм.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Структура ImageMaker]]}}&]
[s0; &]
[s5; [@(0.0.255) struct]_[* ImageMaker]&]
[s2;%RU-RU Этот класс служит в качестве класса`-осно
вы для всех `"имиджмейкеров`". Класс`-клиент 
обычно добавляет параметры в классе, 
производном от ImageMaker. Например, для 
кэширования ремасштабированных изображений
, потребовались бы параметры исходного 
Image и конечного размера.&]
[s3; &]
[s4; &]
[s5;:ImageMaker`:`:Key`(`)const: [@(0.0.255) virtual] [_^String^ String]_[* Key]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2;%RU-RU Возвращает ключ для предоставленного 
набора параметров. Заметьте, что идентификац
ия типа ImageMaker автоматически добавляется 
посредством MakeImage.&]
[s3; &]
[s4; &]
[s5;:ImageMaker`:`:Make`(`)const: [@(0.0.255) virtual] [_^Image^ Image]_[* Make]()_[@(0.0.255) c
onst]_`=_[@3 0]&]
[s2;%RU-RU Создаёт Image на основе параметров.&]
[s3; &]
[s4; &]
[s5;:ImageMaker`:`:`~ImageMaker`(`): [@(0.0.255) `~][* ImageMaker]()&]
[s2;%RU-RU Виртуальный деструктор.&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s0; &]
[s5;:MakeImage`(const ImageMaker`&`): [_^Image^ Image]_[* MakeImage]([@(0.0.255) const]_[_^ImageMaker^ I
mageMaker][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Возвращает Image на основе ImageMaker 
(либо `"достаёт`" из кэша, либо создаёт 
и сохраняет в кэш). Количество сохранённых 
изображений контролируется SetImageCacheSize 
и SetImageCacheMax.&]
[s3;%RU-RU &]
[s4; &]
[s5;:MakeImage`(const Image`&`,Image`(`*`)`(const Image`&`)`): [_^Image^ Image]_[* MakeIm
age]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 image], 
[_^Image^ Image]_(`*[*@3 make])([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 image
]))&]
[s2;%RU-RU Упрощённая версия процесса Image 
making`'а, не требующая других параметров, 
кроме единственного вводного изображения.&]
[s3;%RU-RU &]
[s4; &]
[s5;:MakeImagePaintOnly`(const ImageMaker`&`): [_^Image^ Image]_[* MakeImagePaintOnly]([@(0.0.255) c
onst]_[_^ImageMaker^ ImageMaker][@(0.0.255) `&]_[*@3 m])&]
[s2;%RU-RU Подобно MakeImage, но создаёт Image, который 
можно использовать только в Draw`::DrawImage 
(это оптимизационный хинт (hint), который 
в некоторых ситуациях может сберечь 
память).&]
[s3;%RU-RU &]
[s4; &]
[s5;:CachedRescale`(const Image`&`,Size`,const Rect`&`,int`): [_^Image^ Image]_[* CachedR
escale]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [_^Size^ Size]_[*@3 sz], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], [@(0.0.255) int]_[*@3 filter]_`=_
Null)&]
[s5;:CachedRescale`(const Image`&`,Size`,int`): [_^Image^ Image]_[* CachedRescale]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [_^Size^ Size]_[*@3 sz], 
[@(0.0.255) int]_[*@3 filter]_`=_Null)&]
[s2;%RU-RU Возвращает ремасштабированный 
Image, с кэшированием итога. [%-*@3 filter] может 
быть одним из предопределённых фильтров 
для функции RescaleFilter (напр., FILTER`_BILINEAR), 
если Null, то используется кастомный 
алгоритм стандартной оптимизации 
на скорость.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CachedRescalePaintOnly`(const Image`&`,Size`,const Rect`&`,int`): [_^Image^ Image]_
[* CachedRescalePaintOnly]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) int]_[*@3 filter]_`=_Null)&]
[s5;:CachedRescalePaintOnly`(const Image`&`,Size`,int`): [_^Image^ Image]_[* CachedRescal
ePaintOnly]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], 
[_^Size^ Size]_[*@3 sz], [@(0.0.255) int]_[*@3 filter]_`=_Null)&]
[s2;%RU-RU Возвращает ремасштабированный 
Image, с кэшированием итога, который 
можно использовать только с Draw`::DrawImage(это 
оптимизационный хинт (hint), который 
в некоторых ситуациях поможет сберечь 
память). [%-*@3 filter] может быть одним из 
предопределённых фильтров для функции 
RescaleFilter (напр., FILTER`_BILINEAR), если Null, то 
используется кастомный алгоритм 
стандартной оптимизации на скорость.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CachedSetColorKeepAlpha`(const Image`&`,Color`): [_^Image^ Image]_[* CachedSetColorK
eepAlpha]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img], 
[_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU Кэшированный вариант SetColorKeepAlpha. 
Заменяет цвета всех пикселей у Image 
на цвет [%-*@3 color], при этом не меняя значение 
альфа.&]
[s3;%RU-RU &]
[s4; &]
[s5;:CachedSetColorKeepAlphaPaintOnly`(const Image`&`,Color`): [_^Image^ Image]_[* Cached
SetColorKeepAlphaPaintOnly]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img],
 [_^Color^ Color]_[*@3 color])&]
[s2;%RU-RU Кэшированный вариант SetColorKeepAlpha. 
Заменяет цвета всех пикселей у Image 
на цвет [%-*@3 color], при этом не меняя значение 
альфа, с кэшированием итога, который 
может использоваться только с Draw`::DrawImage(это 
оптимизационный хинт (hint), который 
в некоторых ситуациях поможет сберечь 
память).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Magnify`(const Upp`:`:Image`&`,int`,int`): [_^Upp`:`:Image^ Image]_[* Magnify
]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 nx], [@(0.0.255) int]_[*@3 ny])&]
[s2;%RU-RU Умножает разрешение [%-*@3 img`'а] на 
факторы [%-*@3 nx] и [%-*@3 ny], просто повторяя 
пиксели (каждый пиксель становится 
[%-*@3 nx] x [%-*@3 ny] одного и того же цветового 
блока).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Minify`(const Upp`:`:Image`&`,int`,int`,bool`): [_^Upp`:`:Image^ Image]_[* Mi
nify]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 nx], [@(0.0.255) int]_[*@3 ny], [@(0.0.255) bool]_[*@3 co]_`=_[@(0.0.255) f
alse])&]
[s2;%RU-RU Уменьшает разрешение [%-*@3 img] на 
факторы [%-*@3 nx] и [%-*@3 ny], усредняя значение 
пикселя (каждый [%-*@3 nx] x [%-*@3 ny] блок пикселей 
усредняется и производит единичный 
пиксель в итоговом изображении). Если 
[%-*@3 co] `= true, включается параллельная 
обработка.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:MinifyCached`(const Upp`:`:Image`&`,int`,int`,bool`): [_^Upp`:`:Image^ Imag
e]_[* MinifyCached]([@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 img], 
[@(0.0.255) int]_[*@3 nx], [@(0.0.255) int]_[*@3 ny], [@(0.0.255) bool]_[*@3 co])&]
[s2;%RU-RU  [%-*@3 img] [%-*@3 nx] [%-*@3 ny] [%-*@3 co] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Upscale2x`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Upscale2x]([@(0.0.255) c
onst]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 src])&]
[s2;%RU-RU Удваивает разрешение изображения, 
используя умный эыристический алгоритм, 
разработанный для увеличения (upscaling) 
синтетических изображений (вроде 
иконок).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:Downscale2x`(const Upp`:`:Image`&`): [_^Upp`:`:Image^ Image]_[* Downscale2x](
[@(0.0.255) const]_[_^Upp`:`:Image^ Image][@(0.0.255) `&]_[*@3 src])&]
[s2;%RU-RU Уполовинивает разрешение изображения.&]
[s3;%RU-RU ]]