topic "Инкапсуляция Zlib";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс Zlib]]}}&]
[s3; &]
[s1;:Zlib`:`:class: [@(0.0.255)3 class][3 _][*3 Zlib]&]
[s2;%RU-RU Этот класс инкапсулирует бибилиотеку 
zlib, предоставляя утилиту сжатия/расжатия.&]
[s2;%RU-RU Инкапсуляция имеет особый дизайн, 
позволяющий использовать поточный 
режим при операциях. Вводные данные 
поставляются методом Put, и могут быть 
либо получены на выходе, как неполная 
String, или через обратный вызов вывода. 
При использовании String, также всегда 
можно очищать это неполную выводную 
String, преобразовывая память.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Zlib`:`:WhenOut: [_^Callback2^ Callback2]<[@(0.0.255) const]_[@(0.0.255) void]_`*, 
[_^dword^ int]>_[* WhenOut]&]
[s2;%RU-RU Обрвызов вывода. Представляет 
собой `"функцию`-потребитель`": Когда 
доступны выводные данные, они передаются 
вовне посредством этого обратного 
вызова. Дефолтно данные сохраняются 
в строке вывода внутри Zlib.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Compress`(`): [@(0.0.255) void]_[* Compress]()&]
[s2;%RU-RU Устанавливает Zlib в режим сжатия 
(`"deflate`" в терминологии zlib).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Decompress`(`): [@(0.0.255) void]_[* Decompress]()&]
[s2;%RU-RU Устанавливает Zlib в режим расжатия 
(`"inflate`" в терминологии zlib).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Put`(const void`*`,int`): [@(0.0.255) void]_[* Put]([@(0.0.255) const]_[@(0.0.255) v
oid]_`*[*@3 ptr], [@(0.0.255) int]_[*@3 size])&]
[s5;:Zlib`:`:Put`(const String`&`): [@(0.0.255) void]_[* Put]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2;%RU-RU Помещает блок данных под обработку 
(сжатых или расжатых, в зависимости 
от режима) посредством Zlib.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Zlib`:`:End`(`): [@(0.0.255) void]_[* End]()&]
[s2;%RU-RU Сигнализирует о конце вводного 
потока.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Сбрасывает Zlib в начальное состояние 
(отбрасывая данные).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:Get`(`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[* Get]()_[@(0.0.255) c
onst]&]
[s5;:Zlib`:`:operator const String`&`(`)const: [@(0.0.255) operator][* _const_String`&]()
_[@(0.0.255) const]&]
[s5;:Zlib`:`:operator`~`(`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[* ope
rator`~]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текущую строку вывода 
(создаётся только, если WhenOut не перенаправляе
тся куда`-либо ещё).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:ClearOut`(`): [@(0.0.255) void]_[* ClearOut]()&]
[s2;%RU-RU Очищает строку вывода. Может вызываться 
в любое время.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GetCRC`(`)const: [_^dword^ dword]_[* GetCRC]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает CRC несжатого потока, 
если активен режим CRC.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:IsError`(`)const: [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если при обработке 
ввода имелись ошибки.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GetGZipName`(`)const: [_^String^ String]_[* GetGZipName]()_[@(0.0.255) const]&]
[s2;%RU-RU Если режим GZip активен и заглавие 
Gzip обработано, возвращает имя файла.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GetGZipComment`(`)const: [_^String^ String]_[* GetGZipComment]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Если режим GZip активен и заглавие
Gzip обработано, возвращает комментарий 
gzip.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:GZip`(bool`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* GZip]([@(0.0.255) bool]_[*@3 gzip`_
]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует режим GZip. В этом режиме 
Zlib может напрямую юобрабатывать формат 
фала .gzip.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Zlib`:`:Header`(bool`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* Header]([@(0.0.255) bool]_[*@3 hd
r`_]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Активирует заглавие zlib (смотрите 
документацию к zlib для уточнения).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Zlib`:`:NoHeader`(`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* NoHeader]()&]
[s2;%RU-RU То же, что и Header(false).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:CRC`(bool`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* CRC]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) t
rue])&]
[s2;%RU-RU Активирует режим CRC. В этом режиме, 
Zlib собирает CRC расжатых данных, который 
может находиться в конце, и может 
быть получен методом GetCRC.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Zlib`:`:NoCRC`(`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* NoCRC]()&]
[s2;%RU-RU То же, что и CRC(false).&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:ChunkSize`(int`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* ChunkSize]([@(0.0.255) int]_[*@3 n
])&]
[s2;%RU-RU Устанавливает максимальное количество 
вводных или выводных данных, обрабатываемых
 за один вызов zlib.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Zlib`:`:Level`(int`): [_^Zlib^ Zlib][@(0.0.255) `&]_[* Level]([@(0.0.255) int]_[*@3 compre
ssion`_lvl])&]
[s2;%RU-RU Устанавливает уровень сжатия. 
Должен быть 1..9. Дефолтный уровень 
равен 6.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Zlib`:`:Zlib`(`): [* Zlib]()&]
[s2;%RU-RU Конструктор.&]
[s3; &]
[s4; &]
[s5;:Zlib`:`:`~Zlib`(`): [@(0.0.255) `~][* Zlib]()&]
[s2;%RU-RU Деструктор.&]
[s3;%RU-RU &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции сжатия/расжатия]]}}&]
[s3; &]
[s5;:ZCompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`,bool`): [_^int64^ int
64]_[* ZCompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 hdr]_`=_[@(0.0.255) t
rue])&]
[s5;:ZCompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* ZCompres
s]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZCompress`(const void`*`,int64`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZComp
ress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZCompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZCompress](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^Gate2^ Gate2]<[_^int64^ int6
4], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Сжимает сырые данные, используя 
zlib, из вводного потока [%-*@3 in], памяти 
[%-*@3 data][%- , ][%-*@3 len] или вводной строки [%-*@3 s] 
в выводной поток [%-*@3 out] (в этом случае 
возвращает число записанных байтов 
или отрицательное число, сигнализирующее 
об ошибке) или в возвратное значение 
типа String (при ошибке возвращает IsVoid 
String). [%-*@3 progress] может использоваться 
для отслеживания прогресса операции, 
возврат true отменяет операцию. Параметр 
[%-*@3 hdr] может использоваться для отключения 
заголовочника zlib (подробности ищите 
в документации к zlib).&]
[s3;%RU-RU &]
[s4; &]
[s5;:ZDecompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`,bool`): [_^int64^ i
nt64]_[* ZDecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 hdr]_`=_[@(0.0.255) t
rue])&]
[s5;:ZDecompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`): [_^int64^ int64]_
[* ZDecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZDecompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* ZDecom
press]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 in
], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false
])&]
[s5;:ZDecompress`(const void`*`,int64`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZDe
compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:ZDecompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* ZDecompr
ess]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^Gate2^ Gate2]<[_^int64^ i
nt64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Разжимает сырые данные, используя 
zlib, из вводного потока [%-*@3 in], памяти 
[%-*@3 data][%- , ][%-*@3 len] или вводной строки [%-*@3 s] 
в выводной поток [%-*@3 out] (в этом случае 
возвращает число записанных байтов 
или отрицательное число, сигнализирующее 
об ошибке) или в возвратное значение 
типа String (при ошибке возвращает IsVoid 
String). [%-*@3 progress] может использоваться 
для отслеживания прогресса операции, 
возврат true отменяет операцию. Параметр 
[%-*@3 hdr] может использоваться для отключения 
заголовочника zlib (подробности ищите 
в документации к zlib).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GZCompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* G
ZCompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 i
n], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 pr
ogress]_`=_[@(0.0.255) false])&]
[s5;:GZCompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* GZCompr
ess]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 in],
 [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])
&]
[s5;:GZCompress`(const void`*`,int`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZComp
ress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [@(0.0.255) int]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZCompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZCompres
s]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^Gate2^ Gate2]<[_^int64^ i
nt64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Сжимает сырые данные в файловый 
формат gzip, из вводного потока [%-*@3 in], 
памяти [%-*@3 data][%- , ][%-*@3 len] или вводной строки 
[%-*@3 s] в выводной поток [%-*@3 out] (в этом 
случае возвращает число записанных 
байтов или отрицательное число, сигнализиру
ющее об ошибке) или в возвратное значение 
типа String (при ошибке возвращает IsVoid 
String). [%-*@3 progress] может использоваться 
для отслеживания прогресса операции, 
возврат true отменяет операцию.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GZDecompress`(Stream`&`,Stream`&`,int64`,Gate2`<int64`,int64`>`): [_^int64^ int64]_
[* GZDecompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in], [_^int64^ int64]_[*@3 size], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZDecompress`(Stream`&`,Stream`&`,Gate2`<int64`,int64`>`): [_^int64^ int64]_[* GZDec
ompress]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 i
n], [_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) fals
e])&]
[s5;:GZDecompress`(const void`*`,int`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZDe
compress]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [@(0.0.255) int]_[*@3 len], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZDecompress`(const String`&`,Gate2`<int64`,int64`>`): [_^String^ String]_[* GZDecom
press]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[_^Gate2^ Gate2]<[_^int64^ int64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Расжимает сырые данные в файловый 
формат gzip, из вводного потока [%-*@3 in], 
памяти [%-*@3 data][%- , ][%-*@3 len] или вводной строки 
[%-*@3 s] в выводной поток [%-*@3 out] (в этом 
случае возвращает число записанных 
байтов или отрицательное число, сигнализиру
ющее об ошибке) или в возвратное значение 
типа String (при ошибке возвращает IsVoid 
String). [%-*@3 progress] может использоваться 
для отслеживания прогресса операции, 
возврат true отменяет операцию.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GZCompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) b
ool]_[* GZCompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZCompressFile`(const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) bool]_[* GZCompre
ssFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ in
t64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Сжимает файл в формат gzip. Если 
[%-*@3 dstfile] отсутствует, имя создаётся 
приставкой расширения .gz к [%-*@3 srcfile]. 
Возвращает true при успехе.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GZDecompressFile`(const char`*`,const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) b
ool]_[* GZDecompressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dstfile], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ int64], 
[_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s5;:GZDecompressFile`(const char`*`,Gate2`<int64`,int64`>`): [@(0.0.255) bool]_[* GZDeco
mpressFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 srcfile], [_^Gate2^ Gate2]<[_^int64^ i
nt64], [_^int64^ int64]>_[*@3 progress]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Расжимает файл в формат gzip. Если 
[%-*@3 dstfile] отсутствует, имя создаётся 
приставкой расширения .gz к [%-*@3 srcfile]. 
Если [%-*@3 srcfile] не имеет расширения .gz, 
функция возвращает false, сигнализируя 
об ошибке, и ничего не делает. Возвращает 
true при успехе.&]
[s0;%RU-RU ]]