topic "Класс SqlSession";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Класс SqlSession]]}}&]
[s3; &]
[s1;:SqlSession`:`:class: [@(0.0.255)3 class][3 _][*3 SqlSession][3 _:_][@(0.0.255)3 public][3 _][*@3;3 S
qlSource]&]
[s2;%RU-RU SqlSesion represents single database session. SqlSession 
has ability to start (Begin) and finish (Commit or Rollback) 
transactions; when no transaction is started, each SQL statement 
is treated as single transaction. SqlSession is used as constructor 
parameter for Sql context. As most application usually work with 
single default session, or alternatively read/write sessions, 
this is reflected by existence of default sessions represented 
by global pseodovariables SQL and SQLR.&]
[s2;%RU-RU SqlSesion serves as base class for database specific session 
classes, which provide their unique methods for connection and 
other tasks.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Методов]]}}&]
[s3;%RU-RU &]
[s5;:SqlSession`:`:Begin`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Begin]()&]
[s2;%RU-RU Начинает транзакцию.&]
[s3; &]
[s4; &]
[s5;:SqlSession`:`:Commit`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Commit]()&]
[s2;%RU-RU Коммитит транзакцию.&]
[s3; &]
[s4; &]
[s5;:SqlSession`:`:Rollback`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Rollback]()&]
[s2;%RU-RU Откатывает транзакцию.&]
[s3; &]
[s4; &]
[s5;:SqlSession`:`:GetTransactionLevel`(`)const: [@(0.0.255) virtual] 
[@(0.0.255) int]_[* GetTransactionLevel]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает 0, если транзакции 
нет, или 1, если транзакция активна. 
(Изначально, U`+`+ позволял встроенные 
(`"гнездовые`") транзакции, теперь депрекирова
л их).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:IsOpen`(`)const: [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если сессия подключена 
и готова принимать инструкции.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumUsers`(`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ String
]>_[* EnumUsers]()&]
[s2;%RU-RU Перечисливает всех пользователей 
БД. Точная функция зависит от БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumDatabases`(`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ St
ring]>_[* EnumDatabases]()&]
[s2;%RU-RU Перечисливает все базы данных. 
Точная функция зависит от БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumTables`(String`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ S
tring]>_[* EnumTables]([_^String^ String]_[*@3 database])&]
[s2;%RU-RU Перечисливает все таблицы БД 
[%-*@3 database]. Точная функция зависит от 
БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumViews`(String`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ S
tring]>_[* EnumViews]([_^String^ String]_[*@3 database])&]
[s2;%RU-RU Перечисливает все обзоры БД [%-*@3 database]. 
Точная функция зависит от БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumSequences`(String`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ S
tring]>_[* EnumSequences]([_^String^ String]_[*@3 database])&]
[s2;%RU-RU Перечисливает все последовательности 
БД [%-*@3 database].Точная функция зависит 
от БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumColumns`(String`,String`): [@(0.0.255) virtual] 
[_^Vector^ Vector]<[_^SqlColumnInfo^ SqlColumnInfo]>_[* EnumColumns]([_^String^ String]_[*@3 d
atabase], [_^String^ String]_[*@3 table])&]
[s2;%RU-RU Перечисливает все колонки таблицы 
БД [%-*@3 database] [%-*@3 table]. Точная функция 
зависит от БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:EnumPrimaryKey`(String`,String`): [@(0.0.255) virtual] 
[_^Vector^ Vector]<[_^String^ String]>_[* EnumPrimaryKey]([_^String^ String]_[*@3 database],
 [_^String^ String]_[*@3 table])&]
[s2;%RU-RU Перечисливает все первичные ключи 
таблицы БД [%-*@3 database] [%-*@3 table]. Точная 
функция зависит от БД.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetDialect`(`)const: [@(0.0.255) int]_[* GetDialect]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает диалект SQL сессии (ORACLE, 
SQLITE3, MY`_SQL, MSSQL, PGSQL).&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:SetTrace`(Stream`&`): [@(0.0.255) void]_[* SetTrace]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s]_`=_VppLog())&]
[s2;%RU-RU Активирует трассировку инструкций 
SQL, выданных за эту сессию, и ошибок 
SQL в поток [%-*@3 s].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetTrace`(`)const: [_^Stream^ Stream]_`*[* GetTrace]()_[@(0.0.255) const
]&]
[s2;%RU-RU Возвращает текущий поток трассировки 
или NULL, если нет активного.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:KillTrace`(`): [@(0.0.255) void]_[* KillTrace]()&]
[s2;%RU-RU Дезактивирует трассировку.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:LogErrors`(Stream`&`): [@(0.0.255) void]_[* LogErrors]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s]_`=_VppLog())&]
[s2;%RU-RU Активирует трассировку ошибок 
SQL только в поток [%-*@3 s].&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:LogErrors`(bool`): [@(0.0.255) void]_[* LogErrors]([@(0.0.255) bool]_[*@3 b
])&]
[s2;%RU-RU Если [%-*@3 b] равно true, то же что и 
LogErrors(), если false, дезактивирует трассировку 
ошибок SQL.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:TraceTime`(bool`): [@(0.0.255) void]_[* TraceTime]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Добавляет к трассируемым командам 
время выполнения.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:IsTraceTime`(`)const: [@(0.0.255) bool]_[* IsTraceTime]()_[@(0.0.255) co
nst]&]
[s2;%RU-RU Возвращает статус режима TraceTime.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:TraceSlow`(int`): [_^SqlSession^ SqlSession][@(0.0.255) `&]_[* TraceSlow
]([@(0.0.255) int]_[*@3 ms]_`=_[@3 5000])&]
[s2;%RU-RU Активирует трассировку инструкций 
SQL длинее [%-*@3 ms] как ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:ThrowOnError`(bool`): [_^SqlSession^ SqlSession][@(0.0.255) `&]_[* Throw
OnError]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU Если этот режим активен, SqlExc выводится 
каждый раз при ошибке SQL.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:IsThrowOnError`(`)const: [@(0.0.255) bool]_[* IsThrowOnError]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает true, если ThrowOnError активен.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:WasError`(`)const: [@(0.0.255) bool]_[* WasError]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, еслиимелась ошибка 
с начала сессии или с после очистки 
ClearError.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:SetError`(String`,String`,int`,const char`*`,ERRORCLASS`): [@(0.0.255) v
oid]_[* SetError]([_^String^ String]_[*@3 error], [_^String^ String]_[*@3 stmt], 
[@(0.0.255) int]_[*@3 code]_`=_[@3 0], [@(0.0.255) const]_[@(0.0.255) char]_`*_[*@3 scode]_`=
_NULL, Sql`::ERRORCLASS_[*@3 clss]_`=_Sql`::ERROR`_UNSPECIFIED)&]
[s2;%RU-RU Внешне устанавливает сессию в 
состояние ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetLastError`(`)const: [_^String^ String]_[* GetLastError]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает описание последней 
ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetErrorStatement`(`)const: [_^String^ String]_[* GetErrorStatement]()
_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает инструкцию, вызвавшую 
последнюю ошибку.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetErrorCode`(`)const: [@(0.0.255) int]_[* GetErrorCode]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает код последней ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetErrorCodeString`(`)const: [_^String^ String]_[* GetErrorCodeString](
)_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает текст кода ошибки.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:GetErrorClass`(`)const: Sql`::ERRORCLASS_[* GetErrorClass]()_[@(0.0.255) c
onst]&]
[s2;%RU-RU Возвращает класс ошибки: ERROR`_UNSPECIFIED 
или CONNECTION`_BROKEN.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:ClearError`(`): [@(0.0.255) void]_[* ClearError]()&]
[s2;%RU-RU Очищает последнюю ошибку. Вызывается 
при обработке ошибок.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:InstallErrorHandler`(bool`(`*`)`(String error`,String stmt`,int code`,const char`*scode`,ERRORCLASS clss`)`): [@(0.0.255) v
oid]_[* InstallErrorHandler]([@(0.0.255) bool]_(`*[*@3 handler])(String_error, 
String_stmt, [@(0.0.255) int]_code, [@(0.0.255) const]_[@(0.0.255) char]_`*scode, 
Sql`::ERRORCLASS_clss))&]
[s2;%RU-RU Устанавливает альтернативный 
обработчик ошибок, вызываемый при 
ошибке.&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%RU-RU То же, что и IsOpen().&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:SqlSession`:`:PerThread`(bool`): [@(0.0.255) static] [@(0.0.255) void]_[* PerThread]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%RU-RU In multithreded mode, activates mode where each thread 
has assigned unique per`-thread default session (otherwise default 
session is shared).&]
[s3;%RU-RU &]
[s0;%RU-RU ]]