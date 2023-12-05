topic "Сжатие Zstd";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс ZstdCompressStream]]}}&]
[s3; &]
[s1;:Upp`:`:ZstdCompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 ZstdCompressStream][3 _:_
][@(0.0.255)3 public][3 _][*@3;3 Stream]&]
[s2;%RU-RU [^https`:`/`/github`.com`/Cyan4973`/zstd^ Zstandard] `- 
это очень быстрый (200 МБ/с на ядро ЦПБ) 
алгоритм сжатия с отличным соотношением 
сжатия (в среднем 50%), автор которого 
Янн Коле (Yann Collet). Файл, сжатый ZstdCompressStream, 
можно разжать утилитами командной 
строки Zstd.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Upp`:`:ZstdCompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует многопоточный режим. 
Параллелизация полностью инкапсулирована, 
при вызове потока не требуется ничего 
особенного.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Upp`:`:ZstdCompressStream`:`:Open`(Upp`:`:Stream`&`,int`): [@(0.0.255) void]_[* Open
]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) int]_[*@3 level]_`=_[@3 1])
&]
[s2;%RU-RU  [%-*@3 out] [%-*@3 level] .&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Upp`:`:ZstdCompressStream`:`:ZstdCompressStream`(Upp`:`:Stream`&`,int`): [* ZstdCo
mpressStream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) int]_[*@3 lev
el]_`=_[@3 1])&]
[s2;%RU-RU Устанавливает выводящий поток, 
в который сохраняются сжатые данные. 
[%-*@3 level] `- уровень сжатия `-более высокие 
уровни предоставляют лучшее сжатие, 
но протекают с медленной скоростью. 
Для быстрого сжатия рекомендуется 
уровень 1.&]
[s0;*@7;4%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс ZstdDecompressStream]]}}&]
[s3; &]
[s1;:Upp`:`:ZstdDecompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 ZstdDecompressStream
][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Stream]&]
[s2;%RU-RU Предоставляет расжатие Zstd.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3;%RU-RU &]
[s5;:Upp`:`:ZstdDecompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_
[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует многопоточный режим. 
 Параллелизация полностью инкапсулирована, 
при вызове потока не требуется ничего 
особенного.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:ZstdDecompressStream`:`:Open`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Open]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 in])&]
[s5;:Upp`:`:ZstdDecompressStream`:`:ZstdDecompressStream`(Upp`:`:Stream`&`): [* ZstdDec
ompressStream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in])&]
[s2;%RU-RU Устанавливает вводный поток, 
из которого считываются сжатые данные.&]
[s3;%RU-RU &]
[s0;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции Zstd Compress/Decompress]]}}&]
[s3; &]
[s5;:Upp`:`:ZstdCompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* ZstdCompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdDecompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* ZstdDecompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdCompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdCompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdCompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdCompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s],
 [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdDecompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdDecompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:ZstdDecompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* ZstdDecompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdCompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoZstdCompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdDecompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoZstdDecompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdCompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdCompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdCompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdCompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdDecompress`(const void`*`,Upp`:`:int64`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdDecompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64
], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:Upp`:`:CoZstdDecompress`(const Upp`:`:String`&`,Upp`:`:EventGate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoZstdDecompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:EventGate^ EventGate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Функции простого сжатия/расжатия 
Zstd. Параметры [%-*@3 out] [%-*@3 in] `- вводный/выводящий 
потоки, функция возвращает число 
сохранённых байтов в [%-*@3 out], иначе 
ввод предоставлен как блок памяти 
[%-*@3 data] в [%-*@3 len] байтов размером или 
как вводный String [%-*@3 data] и выводом является 
возвратное значение типа String. [%-*@3 progress] 
может использоваться для отслеживания 
прогресса операции и её прерывании 
при возврате true. Функции с префиксом 
Co выполняются многопоточно.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IsZstd`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* IsZstd]([_^Upp`:`:Stream^ Strea
m][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Проверяет Stream на магическое число, 
идентифицирующее поток Zstd. Возвращается 
обратно после проверки. Возвращает 
true, если магическое число обнаружено.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]