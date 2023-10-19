topic "Работа с форматом LZMA";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Работа с форматом LZMA]]}}&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Функций]]}}&]
[s3; &]
[s5;:LZMACompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`,int`): [_^int64^ i
nt64]_[* LZMACompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])
&]
[s5;:LZMACompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`,int`): [_^int64^ int64]_[* L
ZMACompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_
[*@3 in], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) f
alse], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s5;:LZMACompress`(const void`*`,int64`,Gate2`<int64`,int64`>`,int`): [_^String^ String
]_[* LZMACompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^int64^ int64]_[*@3 len], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progres
s]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s5;:LZMACompress`(const String`&`,Gate2`<int64`,int64`>`,int`): [_^String^ String]_[* LZ
MACompress]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], 
[@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s2;%RU-RU Сжимает данные, используя lzma из 
входного потока [%-*@3 in], памяти [%-*@3 data][%- , 
][%-*@3 len] или вводной строки [%-*@3 s] в поток 
вывода [%-*@3 out] (в этом случае возвращает 
число записанных байтов или отрицательное 
целое, сигнализирующее об ошибке), 
либо в возвратное значение типа String 
(возвращает при ошибке IsVoid String). [%-*@3 progress] 
может использоваться для отслеживания 
прогресса операции, возврат true отменяет 
операцию. [%-*@3 lvl] может использоваться 
для настройки коэффициента сжатия, 
допускается значение от 1 до 9, `- значения, 
большие 6, однако, могут потребовать 
больше времени и памяти (сотни МБ).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LZMADecompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* LZM
ADecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_
[*@3 in], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) f
alse])&]
[s5;:LZMADecompress`(const void`*`,int64`,Gate2`<int64`,int64`>`): [_^String^ String]_[* L
ZMADecompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len
], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false
])&]
[s5;:LZMADecompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* LZMAD
ecompress]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Расжимает данные в формате lzma 
из воодного потока [%-*@3 in], памяти [%-*@3 data][%- , 
][%-*@3 len] или вводной строки [%-*@3 s] в выводной 
поток [%-*@3 out] (в этом случае возвращает 
число записанных ьайтов или отрицательное 
целое, сигнализирующее от ошибке) 
, либо в возвратное значение типа 
String (при ошибке возвращает IsVoid String). 
[%-*@3 progress] может использоваться для 
отслеживания прогресса операции, 
возврат true отменяет операцию.&]
[s3; &]
[s4; &]
[s5;:LZMACompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`,int`): [@(0.0.255) b
ool]_[* LZMACompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])
&]
[s5;:LZMACompressFile`(const char`*`,Gate2`<int64`,int64`>`,int`): [@(0.0.255) bool]_[* L
ZMACompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress], 
[@(0.0.255) int]_[*@3 lvl]_`=_[@3 6])&]
[s2;%RU-RU Сжимает файл в формат lzma. Если 
[%-*@3 dstfile] отсутствует, имя создаётся 
путём приставления расширения .lzma 
к [%-*@3 srcfile]. Возвращает true при успехе. 
[%-*@3 lvl] может использоваться для настройки 
коэффициента сжатия, допускается 
значение от 1 до 9, `- значения, большие 
6, однако, могут потребовать больше 
времени и памяти (сотни МБ).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LZMADecompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) b
ool]_[* LZMADecompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:LZMADecompressFile`(const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) bool]_[* LZMA
DecompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress])&]
[s2;%RU-RU Расжимает файл из формата lzma. 
Если [%-*@3 dstfile] отсутствует, имя создаётся 
путём удаления расширения .lzma у [%-*@3 srcfile]. 
Если [%-*@3 srcfile] не имеет расширения .lzma, 
функция возвращает false, сигнализируя 
об ошибке, и ничего не делает. Возвращает 
true при успехе.&]
[s3;%RU-RU ]]