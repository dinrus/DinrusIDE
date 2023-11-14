topic "Структуры Date и Time";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура Date]]}}&]
[s3;%RU-RU &]
[s1;:Date`:`:struct: [@(0.0.255)3 struct][3 _][*3 Date][3 _:_][@(0.0.255)3 public][3 _][*@3;3 RelOps
][3 <_][*3 Date][3 , ][_^Moveable^3 Moveable][3 <][*3 Date][3 >_>_]&]
[s9;%RU-RU Простой объект даты с точностью 
до дня.&]
[s3; &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:Date`:`:Date`(`): [* Date]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Date`(const Nuller`&`): [* Date]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Конструирует пустую дату.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Date`(int`,int`,int`): [* Date]([@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 m], [@(0.0.255) int]_[*@3 d])&]
[s2;%RU-RU Конструирует дату, основанную 
на [%-*@3 y] [%-*@3 m] [%-*@3 d].&]
[s3;%RU-RU &]
[s3;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Date`:`:day: [_^byte^ byte]_[* day]&]
[s2;%RU-RU День.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:month: [_^byte^ byte]_[* month]&]
[s2;%RU-RU Месяц.&]
[s3; &]
[s4; &]
[s5;:Date`:`:year: [_^int16^ int16]_[* year]&]
[s2;%RU-RU Год.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Сериализует Date в/из [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:IsValid`(`)const: [@(0.0.255) bool]_[* IsValid]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если дата полноценна.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Set`(int`): [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 scalar])&]
[s2;%RU-RU Присвоить дату, сохраненную в 
числовом значении [%-*@3 scalar].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:Get`(`)const: [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s2;%RU-RU Преобразует время в числовое 
значение.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Compare`(Date`)const: [@(0.0.255) int]_[* Compare]([_^Date^ Date]_[*@3 b])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Сравнивает Date с [%-*@3 b], возвращает 
`-1, если <b, или 0, если `=`= b, или 1, если 
> b.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:operator`+`+`(`): [_^Date^ Date][@(0.0.255) `&]_[* operator`+`+]()&]
[s2;%RU-RU Перемещает к следующему дню.&]
[s3; &]
[s4; &]
[s5;:Date`:`:operator`-`-`(`): [_^Date^ Date][@(0.0.255) `&]_[* operator`-`-]()&]
[s2;%RU-RU Перемещает к предыдущему дню.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Date`:`:operator`+`+`(int`): [_^Upp`:`:Date^ Date]_[* operator`+`+]([@(0.0.255) i
nt])&]
[s2;%RU-RU Перемещает к следующему дню, но 
возвращает текущий (постфикс`+`+).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Date`:`:operator`-`-`(int`): [_^Upp`:`:Date^ Date]_[* operator`-`-]([@(0.0.255) i
nt])&]
[s2;%RU-RU Перемещает к предыдущему дню, 
но возвращает текущий (постфикс`-`-).&]
[s3; &]
[s4; &]
[s5;:Date`:`:Low`(`): [@(0.0.255) static] [_^Date^ Date]_[* Low]()&]
[s2;%RU-RU Возвращает наименьшую возможную 
дату (год 4000).&]
[s3; &]
[s4; &]
[s5;:Date`:`:High`(`): [@(0.0.255) static] [_^Date^ Date]_[* High]()&]
[s2;%RU-RU Возвращает наибольшую возможную 
дату (год 4000).&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура Time]]}}&]
[s3; &]
[s1;:Time`:`:struct: [@(0.0.255)3 struct][3 _][*3 Time][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Date][3 ,
 ][@(0.0.255)3 public][3 _][*@3;3 RelOps][3 <_][*3 Time][3 , ][_^Moveable^3 Moveable][3 <][*3 Time][3 >
_>_]&]
[s9;%RU-RU Простой объект даты`-времени с 
точностью до секунды.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Детали Конструктора]]}}&]
[s3; &]
[s5;:Time`:`:Time`(`): [* Time]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s3; &]
[s4; &]
[s5;:Time`:`:Time`(const Nuller`&`): [* Time]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Конструирует пустое время.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Time`:`:Time`(int`,int`,int`,int`,int`,int`): [* Time]([@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 m], [@(0.0.255) int]_[*@3 d], [@(0.0.255) int]_[*@3 h]_`=_[@3 0], 
[@(0.0.255) int]_[*@3 n]_`=_[@3 0], [@(0.0.255) int]_[*@3 s]_`=_[@3 0])&]
[s2;%RU-RU Конструирует время, основанное 
на [%-*@3 y] [%-*@3 m] [%-*@3 d] [%-*@3 h] [%-*@3 n] [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Time`:`:Time`(FileTime`): [* Time]([_^FileTime^ FileTime]_[*@3 filetime])&]
[s2;%RU-RU Конструирует на основе платформо`-специф
ичного [%-*@3 filetime].&]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Time`:`:hour: [_^byte^ byte]_[* hour]&]
[s2;%RU-RU Час.&]
[s3; &]
[s4; &]
[s5;:Time`:`:minute: [_^byte^ byte]_[* minute]&]
[s2;%RU-RU Минута.&]
[s3; &]
[s4; &]
[s5;:Time`:`:second: [_^byte^ byte]_[* second]&]
[s2;%RU-RU Секунда.&]
[s3; &]
[s4; &]
[s5;:Time`:`:High`(`): [@(0.0.255) virtual] [@(0.0.255) static] [_^Time^ Time]_[* High]()&]
[s2;%RU-RU Возвращает наибольшее возможное 
время (год 4000).&]
[s3; &]
[s4; &]
[s5;:Time`:`:Low`(`): [@(0.0.255) virtual] [@(0.0.255) static] [_^Time^ Time]_[* Low]()&]
[s2;%RU-RU Возвращает наименьшее возможное 
время (год `-4000).&]
[s3; &]
[s4; &]
[s5;:Time`:`:Set`(int64`): [@(0.0.255) void]_[* Set]([_^int64^ int64]_[*@3 scalar])&]
[s2;%RU-RU Присваивает время, сохранённое 
в числовом значении [%-*@3 scalar].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Time`:`:Get`(`)const: [@(0.0.255) virtual] [_^int64^ int64]_[* Get]()_[@(0.0.255) const]&]
[s2;%RU-RU Преобразует время в числовое 
значение.&]
[s3; &]
[s4; &]
[s5;:Time`:`:IsValid`(`)const: [@(0.0.255) bool]_[* IsValid]()_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает true, если Time полноценен 
(полноценна Date и значения hour/minute/second 
попадают в диапазон).&]
[s3; &]
[s4; &]
[s5;:Time`:`:AsFileTime`(`)const: [_^FileTime^ FileTime]_[* AsFileTime]()_[@(0.0.255) const
]&]
[s2;%RU-RU Преобразует время в системно`-специфично
е значение.&]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции Date и Time]]}}&]
[s0; &]
[s5;:DayOfWeek`(Date`): [@(0.0.255) int]_[* DayOfWeek]([_^Date^ Date]_[*@3 date])&]
[s2;%RU-RU Номер числа дня недели для даты 
[%-*@3 date :] 0 Воскресенье `- 6 Суббота.&]
[s3;%RU-RU &]
[s4; &]
[s5;:IsLeapYear`(int`): [@(0.0.255) bool]_[* IsLeapYear]([@(0.0.255) int]_[*@3 year])&]
[s2;%RU-RU Возвращает true, если год [%-*@3 year] 
високосный (в этом году у Февраля 
29 дней).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetDaysOfMonth`(int`,int`): [@(0.0.255) int]_[* GetDaysOfMonth]([@(0.0.255) int]_[*@3 mo
nth], [@(0.0.255) int]_[*@3 year])&]
[s2;%RU-RU Возвращает число дней в месяце 
[%-*@3 month] года [%-*@3 year].&]
[s3;%RU-RU &]
[s4; &]
[s5;:LastDayOfMonth`(Date`): [_^Date^ Date]_[* LastDayOfMonth]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Возвращает последний день месяца 
для даты [%-*@3 d].&]
[s0;l288;%RU-RU Пример:&]
[s0;l288;%RU-RU [*C@3       ][*C LastDayOfMonth(Date(2011, 9, 23)) `= 
`"30/9/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:FirstDayOfMonth`(Date`): [_^Date^ Date]_[* FirstDayOfMonth]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Возвращает первый день месяца 
для даты [%-*@3 d].&]
[s0;l288;%RU-RU Пример:&]
[s0;l288;%RU-RU [*C@3       ][*C LastDayOfYear(Date(2011, 9, 23)) `= 
`"1/9/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:LastDayOfYear`(Date`): [_^Date^ Date]_[* LastDayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Возвращает последний день года 
для даты [%-*@3 d].&]
[s0;l288;%RU-RU Пример:&]
[s0;l288;%RU-RU [*C@3       ][*C LastDayOfYear(Date(2011, 9, 23)) `= 
`"31/12/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:FirstDayOfYear`(Date`): [_^Date^ Date]_[* FirstDayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Возвращает первый день года для 
даты [%-*@3 d].&]
[s0;l288;%RU-RU Пример:&]
[s0;l288;%RU-RU [*C@3       ][*C FirstDayOfYear(Date(2011, 9, 23)) `= 
`"1/1/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:DayOfYear`(Date`): [@(0.0.255) int]_[* DayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Возвращает номер дня года для 
даты [%-*@3 d], начиная с 1.&]
[s0;l288;%RU-RU Пример:&]
[s0;l288;%RU-RU [*C@3       ][*C DayOfYear(Date(2011, 1, 2)) `= 2]&]
[s3;%RU-RU &]
[s4; &]
[s5;:AddMonths`(Date`,int`): [_^Date^ Date]_[* AddMonths]([_^Date^ Date]_[*@3 date], 
[@(0.0.255) int]_[*@3 months])&]
[s2;%RU-RU Прибавляет число месяцев [%-*@3 months] 
к дате [%-*@3 date] и возвращает результат.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetMonths`(Date`,Date`): [@(0.0.255) int]_[* GetMonths]([_^Date^ Date]_[*@3 since], 
[_^Date^ Date]_[*@3 till])&]
[s2;%RU-RU Число месяцев между двумя датами. 
Считаются только полные месяцы.&]
[s2;%RU-RU Примеры:&]
[s2;%RU-RU [*C@3       ][*C GetMonths(Date(2013, 2, 5), Date(2013, 3, 
4)) `= 0]&]
[s2;%RU-RU [*C@3       ][*C GetMonths(Date(2013, 2, 5), Date(2013, 3, 
5)) `= 1]&]
[s2;%RU-RU [*C@3       ][*C GetMonths(Date(2013, 8, 31), Date(2013, 9, 
30)) `= 0]&]
[s2;%RU-RU [*C@3       ][*C GetMonths(Date(2013, 8, 31), Date(2013, 10, 
1)) `= 1]&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetMonthsP`(Upp`:`:Date`,Upp`:`:Date`): [@(0.0.255) int]_[* GetMonthsP]([_^Upp`:`:Date^ D
ate]_[*@3 since], [_^Upp`:`:Date^ Date]_[*@3 till])&]
[s2;%RU-RU Число месяцев между двумя датами, 
включая неполные.&]
[s2;%RU-RU Примеры:&]
[s2;%RU-RU [*C       GetMonthsP(Date(2016, 1, 1), Date(2016, 1, 1)) 
`= 0]&]
[s2;%RU-RU [*C       GetMonthsP(Date(2016, 1, 1), Date(2016, 1, 2)) 
`= 1 // partial]&]
[s2;%RU-RU [*C       GetMonthsP(Date(2016, 1, 1), Date(2016, 2, 1)) 
`= 1 // exact one]&]
[s2;%RU-RU [*C       GetMonthsP(Date(2016, 1, 1), Date(2016, 2, 2)) 
`= 2 // partial 2nd]&]
[s3;%RU-RU &]
[s4; &]
[s5;:AddYears`(Date`,int`): [_^Date^ Date]_[* AddYears]([_^Date^ Date]_[*@3 date], 
[@(0.0.255) int]_[*@3 years])&]
[s2;%RU-RU Прибавляет годы [%-*@3 years] к дате 
[%-*@3 date] и возвращает результат. Заметьте, 
что это не так уж просто, так как надо 
учитывать високосные годы.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetWeekDate`(int`,int`): [_^Date^ Date]_[* GetWeekDate]([@(0.0.255) int]_[*@3 year], 
[@(0.0.255) int]_[*@3 week])&]
[s2;%RU-RU Возвращает дату Понедельника 
для данного года [%-*@3 year ]и недели [%-*@3 week] 
в соответствии с ISO`-8601. (Первая неделя 
года равна 1).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetWeek`(Date`,int`&`): [@(0.0.255) int]_[* GetWeek]([_^Date^ Date]_[*@3 d], 
[@(0.0.255) int`&]_[*@3 year])&]
[s2;%RU-RU Возвращает неделю для заданной 
даты в соответствии с ISO`-8601. В [%-*@3 year] 
возвращается соответствующий год, 
так как несколько дней в начале или 
конце года могут быть частью другой 
недели.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EasterDay`(int`): [_^Date^ Date]_[* EasterDay]([@(0.0.255) int]_[*@3 year])&]
[s2;%RU-RU Возвращает день Пасхи для заданного 
года [%-*@3 year].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTimeZone`(`): [@(0.0.255) int]_[* GetTimeZone]()&]
[s2;%RU-RU Возвращает локальный часовой 
пояс как смещение от GMT в минутах.&]
[s3; &]
[s4; &]
[s5;:FormatTimeZone`(int`): [_^String^ String]_[* FormatTimeZone]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Форматирует часовой пояс в общем 
формате, что используется, например, 
в датах Интернета.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTimeZoneText`(`): [_^String^ String]_[* GetTimeZoneText]()&]
[s2;%RU-RU То же, что и FormatTimeZone(GetTimeZone()).&]
[s3; &]
[s4; &]
[s5;:ScanTimeZone`(const char`*`): [@(0.0.255) int]_[* ScanTimeZone]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%RU-RU Сканирует текст часового пояса, 
чтобы получить смещение в секундах.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetLeapSeconds`(Date`): [@(0.0.255) int]_[* GetLeapSeconds]([_^Date^ Date]_[*@3 dt])&]
[s2;%RU-RU Возвращает число leap (високосных?) 
секунд, имевшее место быть с начала 
добавления leap`-секунд до даты [%-*@3 dt].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetUTCSeconds`(Time`): [_^int64^ int64]_[* GetUTCSeconds]([_^Time^ Time]_[*@3 tm])&]
[s2;%RU-RU Возвращает число секунд с 1970`-1`-1, 
включая leap`-секунды.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TimeFromUTC`(int64`): [_^Time^ Time]_[* TimeFromUTC]([_^int64^ int64]_[*@3 seconds])&]
[s2;%RU-RU Преобразует секунды [%-*@3 seconds] с 
1970`-1`-1, включая leap`-секунды, до времени 
Time.&]
[s3;%RU-RU &]
[s0; ]]