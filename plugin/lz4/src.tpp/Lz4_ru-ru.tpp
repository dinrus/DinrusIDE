topic "Сжатие LZ4";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс LZ4CompressStream ]]}}&]
[s3; &]
[s1;:Upp`:`:LZ4CompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 LZ4CompressStream][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Stream]&]
[s2;%RU-RU Предоставляет сжатие [^https`:`/`/github`.com`/Cyan4973`/lz4^ L
Z4]. LZ4 крайне быстрый (400 МБ/сек на ядро 
ЦПБ) алгоритм сжатия, изобретённый 
Yann Collet, со средней степенью сжатия 
около 60%. Сжатый поток следует [^https`:`/`/docs`.google`.com`/document`/d`/1cl8N1bmkTdIpPLtnlzbBSFAdUeyNo5fwfHbHU7VRNWY`/edit^ L
Z4 формату фрейма] (и может быть расжат 
средствами командной строки, предоставленны
е автором LZ4, Yann Collet).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Upp`:`:LZ4CompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует многопоточный режим. 
Параллелизация полностью инкапсулирована, 
ничего особого от вызывающего потока 
не требуется.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:Upp`:`:LZ4CompressStream`:`:Open`(Upp`:`:Stream`&`): [@(0.0.255) void]_[* Open]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 out`_])&]
[s5;:Upp`:`:LZ4CompressStream`:`:LZ4CompressStream`(Upp`:`:Stream`&`): [* LZ4CompressSt
ream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out])&]
[s2;%RU-RU Устанавливает поток вывода, в 
котором сохраняются сжатые данные.&]
[s3;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс LZ4DecompressStream ]]}}&]
[s3; &]
[s1;:Upp`:`:LZ4DecompressStream`:`:class: [@(0.0.255)3 class][3 _][*3 LZ4DecompressStream][3 _
:_][@(0.0.255)3 public][3 _][*@3;3 Stream]&]
[s2;%RU-RU Предоставляет расжатие LZ4. Гарантирует 
расжатие потока, сжатого с помощью 
plugin/lz4 и [/ большинства] (но не всех) потоков, 
совместимых с форматом lz4`-frame.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3;%RU-RU &]
[s5;:Upp`:`:LZ4DecompressStream`:`:Co`(bool`): [@(0.0.255) void]_[* Co]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует многопоточный режим. 
Параллелизация полностью инкапсулирована, 
ничего особого от вызывающего потока 
не требуется.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:LZ4DecompressStream`:`:Open`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* Open]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 in])&]
[s5;:Upp`:`:LZ4DecompressStream`:`:LZ4DecompressStream`(Upp`:`:Stream`&`): [* LZ4Decomp
ressStream]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in])&]
[s2;%RU-RU Устанавливает поток ввода, из 
которого читаются сжатые данные.&]
[s3;%RU-RU &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Фукнции LZ4 Compress/Decompress]]}}&]
[s4; &]
[s5;:Upp`:`:LZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* LZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* LZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:LZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:LZ4Compress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Compress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Compress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s],
 [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=
_Null)&]
[s5;:Upp`:`:LZ4Decompress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Decompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:LZ4Decompress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* LZ4Decompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s5;:Upp`:`:CoLZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoLZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:int64^ i
nt64]_[* CoLZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Compress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Compress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:CoLZ4Decompress`(Upp`:`:Stream`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Decompress]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s5;:Upp`:`:CoLZ4Compress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Compress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Compress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s5;:Upp`:`:CoLZ4Decompress`(const void`*`,Upp`:`:int64`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Decompress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], 
[_^Upp`:`:int64^ int64]_[*@3 len], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:CoLZ4Decompress`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`): [_^Upp`:`:String^ S
tring]_[* CoLZ4Decompress]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s2;%RU-RU Простые функции сжатия/расжатия 
LZ4. Парметры [%-*@3 out] [%-*@3 in] `- это потоки 
ввода / вывода. Функция возвращает 
число сохранённых в [%-*@3 out ]байтов, 
иначе ввод предоставляется как блок 
памяти [%-*@3 data] длиной [%-*@3 len] байтов или 
как вводная String [%-*@3 data], а вывод является 
возвратным значением типа String. [%-*@3 progress] 
можно использовать для отслеживания 
прогресса операции и прерывать его, 
возвращая true. Функции с префиксом 
Co выполняются многопоточно.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IsLZ4`(Upp`:`:Stream`&`): [@(0.0.255) bool]_[* IsLZ4]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Проверяет поток Stream на магическое 
число, идентифицирующее поток LZ4. 
Перемещается обратно после проверки. 
Возвращает true, если магическое число 
обнаружено.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]