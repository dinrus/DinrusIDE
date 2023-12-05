topic "Структуры S_type";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Структуры S`_][*/@7;4 type]]}}&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[s0; S`_[/ type] structures are generated (by multiincluding process) 
from .sch files that describe the database schema. Each TABLE 
or TYPE of .sch has correspending S`_[/ type] structure. This structure 
has member variables corresponding to database columns, with 
the same names and types. `'BOOL`' columns are represented as 
CHAR(1) in database, but as `'bool`' C`+`+ type in S`_[/ type]). 
ARRAY columns are represented as fixed C`+`+ arrays. If TABLE 
or TYPE has any base TYPEs or TABLEs, S`_[/ type] structure contains 
them as well.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Членов]]}}&]
[s3;%- &]
[s5;:S`_type`:`:TableName:%- [@(0.0.255) static]_[@(0.0.255) const]_[@(0.0.255) char]_[* Tabl
eName][@(0.0.255) `[`]]&]
[s2; Имя таблицы или типа.&]
[s3;%- &]
[s4;%- &]
[s5;:S`_type`:`:ColumnSet`(`):%- [@(0.0.255) static] [@(0.0.255) const]_[_^SqlSet^ SqlSet][@(0.0.255) `&
]_[* ColumnSet]()&]
[s2; Полный набор колонок как SqlSet.&]
[s3;%- &]
[s4;%- &]
[s5;:S`_type`:`:ColumnSet`(const String`&`):%- [@(0.0.255) static] 
[_^SqlSet^ SqlSet]_[* ColumnSet]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 pr
efix])&]
[s2; Полный набор колонок как SqlSet, все 
колонки префиксируются префиксом 
[%-*@3 prefix].&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Of`(SqlId`):%- [@(0.0.255) static] [_^SqlSet^ SqlSet]_[* Of]([_^SqlId^ SqlId]_
[*@3 table])&]
[s2; The complete set of columns as SqlSet, all columns expressed 
as part of [%-*@3 table] (TABLE.COLUMN notation).&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:GetColumnIds`(`):%- [@(0.0.255) static] [@(0.0.255) const]_[_^Vector^ Vecto
r]<[_^SqlId^ SqlId]>`&_[* GetColumnIds]()&]
[s2; Возвращает все колонки.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Устанавливает все переменные`-члены 
в Null.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:FieldLayout`(FieldOperator`&`):%- [@(0.0.255) void]_[* FieldLayout]([_^FieldOperator^ F
ieldOperator][@(0.0.255) `&]_[*@3 f])&]
[s2; Вызывает FieldOpertator`::Field(const char `*name, Ref 
f) and Table(const char `*name) methods of [%-*@3 f] to provide 
a visitor pattern for S`_[/ type].&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:operator Fields`(`):%- [* operator_Fields]()&]
[s2; Возвращает callback(this, `&S`_[/ type]`::FieldLayout). 
This method provides unified access to all S`_[/ type]s, which 
is then used in Sql`::Fetch.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:operator`=`=`(const S`_type`&`)const:%- [@(0.0.255) bool]_[* operator`=`=
]([@(0.0.255) const]_[_^S`_type^ S`_][/_^S`_type^ type][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если все поля одинаковы.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:operator`!`=`(const S`_type`&`)const:%- [@(0.0.255) bool]_[* operator!`=](
[@(0.0.255) const]_[_^S`_type^ S`_][/_^S`_type^ type][@(0.0.255) `&]_[*@3 x])_[@(0.0.255) con
st]&]
[s2; То же, что и !operator`=`=(x).&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Converts S`_[/ type] to (multiline) text, mostly for debugging 
purposes.&]
[s3; &]
[s4;%- &]
[s5;:S`_type`:`:GetCount`(`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetCount]()&]
[s2; Возвращает число колонок у этого 
S`_[/ type].&]
[s3;%- &]
[s4;%- &]
[s5;:S`_type`:`:GetId`(int`):%- [@(0.0.255) static] [_^SqlId^ SqlId]_[* GetId]([@(0.0.255) in
t]_[*@3 i])&]
[s2; Возвращает идентификатор ID колонки 
по индексу [%-*@3 i].&]
[s3; &]
[s4;%- &]
[s5;:S`_type`:`:GetIndex`(const String`&`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetIn
dex]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 id])&]
[s5;:S`_type`:`:GetIndex`(const SqlId`&`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetInd
ex]([@(0.0.255) const]_[_^SqlId^ SqlId][@(0.0.255) `&]_[*@3 id])&]
[s2;~~~1792; Возвращает индекс колонки [%-*@3 id] 
или отрицательное число, если она 
не найдена.&]
[s3; &]
[s4;%- &]
[s5;:S`_type`:`:GetWidth`(int`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetWidth]([@(0.0.255) i
nt]_[*@3 i])&]
[s2; Возвращает ширину колонки (обычно 
текста) как определено в схеме. Если 
у колонки нет ширины, возвращает 0.&]
[s3; &]
[s4;%- &]
[s5;:S`_type`:`:GetWidth`(const SqlId`&`):%- [@(0.0.255) static] [@(0.0.255) int]_[* GetWid
th]([@(0.0.255) const]_[_^SqlId^ SqlId][@(0.0.255) `&]_[*@3 id])&]
[s2; Возвращает ширину колонки (обычно 
текста) как определено в схеме.  Если 
[%-*@3 id] отсутствует в S`_[/ type], возвращает 
0.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:GetRef`(int`):%- [_^Ref^ Ref]_[* GetRef]([@(0.0.255) int]_[*@3 i])&]
[s2; Возвращает ссылку на колонку по индексу 
[%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:GetRef`(const SqlId`&`):%- [_^Ref^ Ref]_[* GetRef]([@(0.0.255) const]_[_^SqlId^ S
qlId][@(0.0.255) `&]_[*@3 id])&]
[s2; Возвращает ссылку на колонку [%-*@3 id 
]или пустую ссылку (void), если её нет 
в S`_[/ type].&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Get`(const SqlId`&`)const:%- [_^Value^ Value]_[* Get]([@(0.0.255) const]_[_^SqlId^ S
qlId][@(0.0.255) `&]_[*@3 id])_[@(0.0.255) const]&]
[s2; Возвращает значение колонки [%-*@3 id] 
или void Value, если её нет в S`_[/ type].&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Get`(int`)const:%- [_^Value^ Value]_[* Get]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2; Возвращает значение колонки по индексу 
[%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Get`(`)const:%- [_^ValueMap^ ValueMap]_[* Get]()_[@(0.0.255) const]&]
[s2; Возвращает мап (соответствие) имён 
колонок  их значениям.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Set`(int`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 i
], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2; Устанавливает колонку по индексу 
[%-*@3 i] в значение [%-*@3 v]. Тип значения 
[%-*@3 v] должен быть преобразуем в колонку.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Set`(const SqlId`&`,const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^SqlId^ SqlId][@(0.0.255) `&]_[*@3 id], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 v])&]
[s2; Если [%-*@3 id] есть в S`_[/ type], устанавливает 
его значение в [%-*@3 v], иначе ничего не 
выполняет.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:Set`(const ValueMap`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^ValueMap^ V
alueMap][@(0.0.255) `&]_[*@3 m])&]
[s2; Keys in [%-*@3 m] found as column ids in S`_[/ type] as assigned 
to respective values. Keys not found are ignored, columns not 
present in [%-*@3 m] are left unchanged.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:S`_type`(`):%- [* S`_type]()&]
[s2; Устанавливает все колонки в Null.&]
[s3; &]
[s4; &]
[s5;:S`_type`:`:S`_type`(const ValueMap`&`):%- [* S`_type]([@(0.0.255) const]_[_^ValueMap^ V
alueMap][@(0.0.255) `&]_[*@3 m])&]
[s2; Keys in [%-*@3 m] found as column ids in S`_[/ type] as assigned 
to respective values. Keys not found are ignored, columns not 
present in [%-*@3 m] are set to Null. Overal effect is the same 
as using the default constructor and then Set([%-*@3 m]).&]
[s3; ]]