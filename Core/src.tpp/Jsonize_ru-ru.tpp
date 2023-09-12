topic "Класс JsonIO и фреймворк Jsonize";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс JsonIO и фреймворк Jsonize]]}}&]
[s3; &]
[s1;:JsonIO`:`:class: [@(0.0.255)3 class][3 _][*3 JsonIO]&]
[s2;%RU-RU JsonIO представляет собой единичное 
значение JSON в фреймворке Jsonize. Оно 
используется в качестве параметра 
для глобальной функции Jsonize. Глобальная 
функция Jsonize имеет дефолтное шаблонное 
определение, которое, в свою очередь, 
вызывает метод Jsonize объекта. However, 
when implementation using method is not possible (e.g. for supported 
primitive types), global templated specialization of Jsonize 
can be used.&]
[s2;%RU-RU &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3;%RU-RU &]
[s5;:JsonIO`:`:IsLoading`(`)const: [@(0.0.255) bool]_[* IsLoading]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true when retrieving data from JSON.&]
[s3; &]
[s4; &]
[s5;:JsonIO`:`:IsStoring`(`)const: [@(0.0.255) bool]_[* IsStoring]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true when storing data to JSON.&]
[s3; &]
[s4; &]
[s5;:JsonIO`:`:Get`(`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* Get]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает the value of JSON node when loading.&]
[s3; &]
[s4; &]
[s5;:JsonIO`:`:Set`(const Value`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Устанавливаетthe value of JSON node when 
storing.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:JsonIO`:`:Get`(const char`*`): [_^Upp`:`:Value^ Value]_[* Get]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 key])&]
[s2;%RU-RU Возвращает the value of JSON subnode [%-*@3 key] 
when loading.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:JsonIO`:`:Set`(const char`*`,const Upp`:`:Value`&`): [@(0.0.255) void]_[* Set
]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [@(0.0.255) const]_[_^Upp`:`:Value^ Val
ue][@(0.0.255) `&]_[*@3 v])&]
[s2;%RU-RU Устанавливает the value of JSON subnode [%-*@3 key] 
when storing.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:JsonIO`:`:Put`(Value`&`): [@(0.0.255) void]_[* Put]([_^Value^ Value][@(0.0.255) `&]_[*@3 v
])&]
[s2;%RU-RU Retrieves JSON node data into [%-*@3 v] when storing `- 
invoked to finish jsonization.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:JsonIO`:`:GetResult`(`)const: [_^Value^ Value]_[* GetResult]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает JSON node data into [%-*@3 v] when 
storing `- invoked to finish jsonization.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:JsonIO`:`:operator`(`)`(const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 T]>_[_^JsonIO^ JsonIO][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [*@4 T][@(0.0.255) `&]_[*@3 value])&]
[s2;%RU-RU Устанавливает the JsonIO to represent JSON 
object (if it does not already), creates member [%-*@3 key] and 
jsonizes [%-*@3 value] into it (by calling global Jsonize function, 
whose general form invokes Jsonize method).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:JsonIO`:`:operator`(`)`(const char`*`,T`&`,const T`&`): [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 T]>_[_^Upp`:`:JsonIO^ JsonIO][@(0.0.255) `&]_[* operator()]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 key], [*@4 T][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 defvalue])&]
[s2;%RU-RU Устанавливает the JsonIO to represent JSON 
object (if it does not already), creates member [%-*@3 key] and 
jsonizes [%-*@3 value] into it (by calling global Jsonize function, 
whose general form invokes Jsonize method). When loading and 
[%-*@3 key] is missing in source data, [%-*@3 defvalue] is assigned 
to [%-*@3 value].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:JsonIO`:`:List`(const char`*`,const char`*`,T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[_^Upp`:`:JsonIO^ JsonIO][@(0.0.255) `&]_[* List]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[@(0.0.255) char]_`*, [*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s2;%RU-RU То же, что и operator()([%-*@3 key], [%-*@3 var]). 
Provides compatibility with Xmlize for common template routines 
that provide both Jsonize and Xmlize code.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:JsonIO`:`:Var`(const char`*`,T`&`,X`): [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 T], [@(0.0.255) class]_[*@4 X]>_[_^Upp`:`:JsonIO^ JsonIO][@(0.0.255) `&]_[* Var]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 key], [*@4 T][@(0.0.255) `&]_[*@3 value], 
[*@4 X]_[*@3 item`_jsonize])&]
[s2;%RU-RU Similar to operator()([%-*@3 key], [%-*@3 value]), but instead 
of using global Jsonize function, it uses [%-*@3 item`_jsonize] 
to define the structure.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:JsonIO`:`:Array`(const char`*`,T`&`,X`,const char`*`): [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 X]>_[_^Upp`:`:JsonIO^ JsonIO][@(0.0.255) `&
]_[* Array]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 key], [*@4 T][@(0.0.255) `&]_[*@3 value
], [*@4 X]_[*@3 item`_jsonize], [@(0.0.255) const]_[@(0.0.255) char]_`*_`=_NULL)&]
[s2;%RU-RU Jsonizes array container (must have GetCount, Add and 
operator`[`] methods), using [%-*@3 item`_jsonize] to define the 
structure of elements. The last parameter is ignored and is included 
to provide compatibility with Xmlize code for common template 
routines that provide both Jsonize and Xmlize code.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:JsonIO`:`:JsonIO`(const Value`&`): [* JsonIO]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 src])&]
[s2;%RU-RU Устанавливает JsonIO for retrieving data 
from [%-*@3 src] which is must be Json compatible Value.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:JsonIO`:`:JsonIO`(`): [* JsonIO]()&]
[s2;%RU-RU Создаёт JsonIO для сохранения данных.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4  Функции Store/Load из Jsonize]]}}&]
[s3; &]
[s5;:StoreAsJsonValue`(const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^Value^ V
alue]_[* StoreAsJsonValue]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 var])&]
[s2;%RU-RU Converts [%-*@3 var] into JSON compatible Value (such that 
can be directly used with AsJson function).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFromJsonValue`(T`&`,const Value`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) void]_[* LoadFromJsonValue]([*@4 T][@(0.0.255) `&]_[*@3 var], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 x])&]
[s2;%RU-RU Retrieves variable from JSON compatible Value. Can throw 
JsonizeError on error.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:StoreAsJson`(const T`&`,bool`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^String^ S
tring]_[* StoreAsJson]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 var], 
[@(0.0.255) bool]_[*@3 pretty]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Converts [%-*@3 var] into JSON text. If [%-*@3 pretty] is 
true, adds whitespaces to look better, otherwise the result is 
as compact as possible.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFromJson`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_
[@(0.0.255) bool]_[* LoadFromJson]([*@4 T][@(0.0.255) `&]_[*@3 var], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 json])&]
[s2;%RU-RU Retrieves [%-*@3 var] from [%-*@3 json] text. Does not throw 
JsonizeError, returns false in case of error.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:StoreAsJsonFile`(const T`&`,const char`*`,bool`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) bool]_[* StoreAsJsonFile]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 var], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL, 
[@(0.0.255) bool]_[*@3 pretty]_`=_[@(0.0.255) false])&]
[s2;%RU-RU Converts [%-*@3 var] into JSON file. If [%-*@3 pretty] is 
true, adds whitespaces to look better, otherwise the result is 
as compact as possible. If [%-*@3 file] is NULL, ConfigFile(GetExeTitle() 
`+ `".json`") is used as the file path.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:LoadFromJsonFile`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[@(0.0.255) bool]_[* LoadFromJsonFile]([*@4 T][@(0.0.255) `&]_[*@3 var], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2;%RU-RU Retrieves [%-*@3 var] from [%-*@3 json] file. Does not throw 
JsonizeError, returns false in case of error. If [%-*@3 file] is 
NULL, ConfigFile(GetExeTitle() `+ `".json`") is used as the file 
path.&]
[s3; &]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Стандартные определения шаблона 
Jsonize]]}}&]
[s3; &]
[s5;:Jsonize`(JsonIO`&`,int`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 io], [@(0.0.255) int`&]_[*@3 var])&]
[s5;:Upp`:`:Jsonize`(Upp`:`:JsonIO`&`,Upp`:`:byte`&`): [@(0.0.255) template]_<>_[@(0.0.255) v
oid]_[* Jsonize]([_^Upp`:`:JsonIO^ JsonIO][@(0.0.255) `&]_[*@3 io], [_^Upp`:`:byte^ byte][@(0.0.255) `&
]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,int16`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 io], [_^int16^ int16][@(0.0.255) `&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,int64`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 io], [_^int64^ int64][@(0.0.255) `&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,double`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize](
[_^JsonIO^ JsonIO][@(0.0.255) `&]_[*@3 io], [@(0.0.255) double`&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,bool`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 io], [@(0.0.255) bool`&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,String`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize](
[_^JsonIO^ JsonIO][@(0.0.255) `&]_[*@3 io], [_^String^ String][@(0.0.255) `&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,WString`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize](
[_^JsonIO^ JsonIO][@(0.0.255) `&]_[*@3 io], [_^WString^ WString][@(0.0.255) `&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,Date`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 io], [_^Date^ Date][@(0.0.255) `&]_[*@3 var])&]
[s5;:Jsonize`(JsonIO`&`,Time`&`): [@(0.0.255) template]_<>_[@(0.0.255) void]_[* Jsonize]([_^JsonIO^ J
sonIO][@(0.0.255) `&]_[*@3 io], [_^Time^ Time][@(0.0.255) `&]_[*@3 var])&]
[s2; Предлагают определение шаблона Jsonize 
 для поддержки типа данных [*@3 var].&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Особые варианты Jsonize]]}}&]
[s3; &]
[s5;:StringMap`(JsonIO`&`,VectorMap`<K`,V`>`&`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 K], [@(0.0.255) class]_[*@4 V]>_[@(0.0.255) void]_[* StringMap]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 io], [_^VectorMap^ VectorMap]<[*@4 K], [*@4 V], [*@4 H]>`&_[*@3 map])&]
[s5;:StringMap`(JsonIO`&`,ArrayMap`<K`,V`>`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 K], [@(0.0.255) class]_[*@4 V]>_[@(0.0.255) void]_[* StringMap]([_^JsonIO^ JsonIO][@(0.0.255) `&
]_[*@3 io], [_^ArrayMap^ ArrayMap]<[*@4 K], [*@4 V], [*@4 H]>`&_[*@3 map])&]
[s2;%RU-RU The default Jsonize for Maps has to encode any type as 
key, which leads to somewhat unnatural results when key [%-*@4 K] 
is String or WString. [%-* StringMap ]alternative variant encodes 
map as JSON object with keys equal to map keys.&]
[s3;%RU-RU &]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:JsonizeBySerialize`(JsonIO`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>
_[@(0.0.255) void]_[* JsonizeBySerialize]([_^JsonIO^ JsonIO][@(0.0.255) `&]_[*@3 jio], 
[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%RU-RU This function encodes/retrieves Json by binary serializing 
the object and Jsonizing the hexadecimal data string.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:LambdaIze`(IO`&`,const char`*`,IZE`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 IO], [@(0.0.255) class]_[*@4 IZE]>_[@(0.0.255) void]_[* LambdaIze]([*@4 IO][@(0.0.255) `&
]_[*@3 io], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [*@4 IZE]_[*@3 ize])&]
[s2;%RU-RU This special function uses [%-*@3 ize] to represent the 
value of Jsonize or Xmlize node named [%-*@3 id]. Lambda should 
have one paramter IO`&.&]
[s3;%RU-RU &]
[s3;%RU-RU ]]