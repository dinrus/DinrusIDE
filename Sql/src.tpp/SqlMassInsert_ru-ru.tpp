topic "Класс SqlMassInsert";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Класс SqlMassInsert]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:SqlMassInsert`:`:class:%- [@(0.0.255)3 class][3 _][*3 SqlMassInsert]&]
[s2; Этот класс предназначен на случай, 
когда в базу данных нужно ввести множемтво 
данных командами SQL. Использование 
отдельных инструкций [/ insert] для каждого 
ряда может оказаться медленным, главным 
образом, из`-за сетевого трафика. SqlMassInsert 
буферирует единичные втавки и, пользуясь 
спецификой диалекта SQL, вставляет 
много рядов одной инструкцией SQL.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:SqlMassInsert`:`:operator`(`)`(SqlId`,const Value`&`):%- [_^SqlMassInsert^ SqlMass
Insert][@(0.0.255) `&]_[* operator()]([_^SqlId^ SqlId]_[*@3 col], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 val])&]
[s2; Добавляет единичную вставляемую 
колонку.&]
[s3; &]
[s4;%- &]
[s5;:SqlMassInsert`:`:operator`(`)`(const ValueMap`&`):%- [_^SqlMassInsert^ SqlMassInse
rt][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_
[*@3 data])&]
[s2; Вставляет набор колонок, определённый 
парами ключ`-значение данных [%-*@3 data] 
(ключ `- это имя колонки, значение `- 
это вставляемое значение).&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:EndRow`(SqlBool`):%- [_^SqlMassInsert^ SqlMassInsert][@(0.0.255) `&
]_[* EndRow]([_^SqlBool^ SqlBool]_[*@3 remove]_`=_[_^SqlBool^ SqlBool]())&]
[s2; Завершает один ряд. Заметьте, что 
в каждый ряд должны добавиться одни 
и те же колонки в одном и том же порядке. 
Если [%-*@3 remove] не пуст, it is combined using [/ or] 
and if combined condition is not empty during Flush, [/ delete] 
statement is issued before inserting rows, using combined condition 
in the [/ where] clause. This is intended for situation where rows 
are replaced (in addition to be inserted) `- this it is possible 
to specify condition for row being inserted, so that it is removed 
(if exists) before inserting a new value. Rows [/ must] always 
have the same columns before EndRow is invoked, and exception 
to this rule is when EndRow is invoked without [/ any] columns 
added `- in that case, nothing is inserted, only [%-*@3 remove] 
condition is used.&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:Flush`(`):%- [@(0.0.255) void]_[* Flush]()&]
[s2; Вставляет все буферированные ряды. 
SqlMassInsert вызывает этот метод, когда 
буфер достигает порогового размера, 
и ещё в деструкторе.Обычно нет необходимости
 вызывать Flush непосредственно, за 
исключением, может быть, в конце вставки, 
чтобы получить финальное IsError.&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:IsError`(`)const:%- [@(0.0.255) bool]_[* IsError]()_[@(0.0.255) const
]&]
[s2; Возвращает true, если была обнаружена 
ошибка SQL.&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:UseTransaction`(bool`):%- [_^SqlMassInsert^ SqlMassInsert][@(0.0.255) `&
]_[* UseTransaction]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вставка набора рядов (при Flush) использует 
транзакцию. Это дефолт.&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:NoUseTransaction`(`):%- [_^SqlMassInsert^ SqlMassInsert][@(0.0.255) `&
]_[* NoUseTransaction]()&]
[s2; То же, что и UseTransaction(false).&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:SqlMassInsert`(Sql`&`,SqlId`):%- [* SqlMassInsert]([_^Sql^ Sql][@(0.0.255) `&
]_[*@3 sql], [_^SqlId^ SqlId]_[*@3 table])&]
[s2; Создаёт экземпляр для контекста 
[%-*@3 sql] и таблицы [%-*@3 table] .&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:SqlMassInsert`(SqlId`):%- [* SqlMassInsert]([_^SqlId^ SqlId]_[*@3 tab
le])&]
[s2; Создаёт экземпляр для дефолтного 
контекста (SQL) и [%-*@3 table].&]
[s3; &]
[s4; &]
[s5;:SqlMassInsert`:`:`~SqlMassInsert`(`):%- [@(0.0.255) `~][* SqlMassInsert]()&]
[s2; Вызывает Flush.&]
[s3; ]]