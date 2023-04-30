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
[s9;%RU-RU A simple date object with up to a day precision.&]
[s3; &]
[s0;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Constructor detail]]}}&]
[s3; &]
[s5;:Date`:`:Date`(`): [* Date]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Date`(const Nuller`&`): [* Date]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Construct an empty date.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Date`(int`,int`,int`): [* Date]([@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 m], [@(0.0.255) int]_[*@3 d])&]
[s2;%RU-RU Constructs a date based on [%-*@3 y] [%-*@3 m] [%-*@3 d].&]
[s3;%RU-RU &]
[s3;%RU-RU &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Date`:`:day: [_^byte^ byte]_[* day]&]
[s2;%RU-RU Day.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:month: [_^byte^ byte]_[* month]&]
[s2;%RU-RU Month.&]
[s3; &]
[s4; &]
[s5;:Date`:`:year: [_^int16^ int16]_[* year]&]
[s2;%RU-RU Year.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%RU-RU Serializes Date to/from [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:IsValid`(`)const: [@(0.0.255) bool]_[* IsValid]()_[@(0.0.255) const]&]
[s2;%RU-RU Returns true if the date is valid.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Set`(int`): [@(0.0.255) void]_[* Set]([@(0.0.255) int]_[*@3 scalar])&]
[s2;%RU-RU Assign a date that is stored in the numeric [%-*@3 scalar].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:Get`(`)const: [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s2;%RU-RU Converts the time into a numeric value.&]
[s3; &]
[s4; &]
[s5;:Date`:`:Compare`(Date`)const: [@(0.0.255) int]_[* Compare]([_^Date^ Date]_[*@3 b])_[@(0.0.255) c
onst]&]
[s2;%RU-RU Compares Date with [%-*@3 b], returns `-1 if <b, 0 if `=`= 
b, 1 if > b.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Date`:`:operator`+`+`(`): [_^Date^ Date][@(0.0.255) `&]_[* operator`+`+]()&]
[s2;%RU-RU Moves to the next day.&]
[s3; &]
[s4; &]
[s5;:Date`:`:operator`-`-`(`): [_^Date^ Date][@(0.0.255) `&]_[* operator`-`-]()&]
[s2;%RU-RU Moves to the previous day.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Date`:`:operator`+`+`(int`): [_^Upp`:`:Date^ Date]_[* operator`+`+]([@(0.0.255) i
nt])&]
[s2;%RU-RU Moves to the next day but returns the current (postfix`+`+).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Date`:`:operator`-`-`(int`): [_^Upp`:`:Date^ Date]_[* operator`-`-]([@(0.0.255) i
nt])&]
[s2;%RU-RU Moves to the previous day but returns the current (postfix`-`-).&]
[s3; &]
[s4; &]
[s5;:Date`:`:Low`(`): [@(0.0.255) static] [_^Date^ Date]_[* Low]()&]
[s2;%RU-RU Returns the lowest possible date (year 4000).&]
[s3; &]
[s4; &]
[s5;:Date`:`:High`(`): [@(0.0.255) static] [_^Date^ Date]_[* High]()&]
[s2;%RU-RU Returns the highest possible date (year 4000).&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура Time]]}}&]
[s3; &]
[s1;:Time`:`:struct: [@(0.0.255)3 struct][3 _][*3 Time][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Date][3 ,
 ][@(0.0.255)3 public][3 _][*@3;3 RelOps][3 <_][*3 Time][3 , ][_^Moveable^3 Moveable][3 <][*3 Time][3 >
_>_]&]
[s9;%RU-RU A simple date time object with up to a second precision.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Constructor detail]]}}&]
[s3; &]
[s5;:Time`:`:Time`(`): [* Time]()&]
[s2;%RU-RU Дефолтный конструктор.&]
[s3; &]
[s4; &]
[s5;:Time`:`:Time`(const Nuller`&`): [* Time]([@(0.0.255) const]_[_^Nuller^ Nuller][@(0.0.255) `&
])&]
[s2;%RU-RU Конструирует пустой time.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Time`:`:Time`(int`,int`,int`,int`,int`,int`): [* Time]([@(0.0.255) int]_[*@3 y], 
[@(0.0.255) int]_[*@3 m], [@(0.0.255) int]_[*@3 d], [@(0.0.255) int]_[*@3 h]_`=_[@3 0], 
[@(0.0.255) int]_[*@3 n]_`=_[@3 0], [@(0.0.255) int]_[*@3 s]_`=_[@3 0])&]
[s2;%RU-RU Constructs a time based on [%-*@3 y] [%-*@3 m] [%-*@3 d] [%-*@3 h] 
[%-*@3 n] [%-*@3 s].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Time`:`:Time`(FileTime`): [* Time]([_^FileTime^ FileTime]_[*@3 filetime])&]
[s2;%RU-RU Constructs based on a platform specific [%-*@3 filetime].&]
[s3;%RU-RU &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:Time`:`:hour: [_^byte^ byte]_[* hour]&]
[s2;%RU-RU Hour.&]
[s3; &]
[s4; &]
[s5;:Time`:`:minute: [_^byte^ byte]_[* minute]&]
[s2;%RU-RU Minute.&]
[s3; &]
[s4; &]
[s5;:Time`:`:second: [_^byte^ byte]_[* second]&]
[s2;%RU-RU Second.&]
[s3; &]
[s4; &]
[s5;:Time`:`:High`(`): [@(0.0.255) virtual] [@(0.0.255) static] [_^Time^ Time]_[* High]()&]
[s2;%RU-RU Returns the highest possible time (year 4000).&]
[s3; &]
[s4; &]
[s5;:Time`:`:Low`(`): [@(0.0.255) virtual] [@(0.0.255) static] [_^Time^ Time]_[* Low]()&]
[s2;%RU-RU Returns the lowest possible time (year `-4000).&]
[s3; &]
[s4; &]
[s5;:Time`:`:Set`(int64`): [@(0.0.255) void]_[* Set]([_^int64^ int64]_[*@3 scalar])&]
[s2;%RU-RU Assign a time that is stored in the numeric [%-*@3 scalar].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Time`:`:Get`(`)const: [@(0.0.255) virtual] [_^int64^ int64]_[* Get]()_[@(0.0.255) const]&]
[s2;%RU-RU Converts the time into a numeric value.&]
[s3; &]
[s4; &]
[s5;:Time`:`:IsValid`(`)const: [@(0.0.255) bool]_[* IsValid]()_[@(0.0.255) const]&]
[s2;%RU-RU Returns true if Time is valid (Date is valid and hour/minute/second 
values are in range).&]
[s3; &]
[s4; &]
[s5;:Time`:`:AsFileTime`(`)const: [_^FileTime^ FileTime]_[* AsFileTime]()_[@(0.0.255) const
]&]
[s2;%RU-RU Converts the time into a system specific value.&]
[s3; &]
[s0; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Функции Date и Time]]}}&]
[s0; &]
[s5;:DayOfWeek`(Date`): [@(0.0.255) int]_[* DayOfWeek]([_^Date^ Date]_[*@3 date])&]
[s2;%RU-RU Day [%-*@3 date] number of day of week 0 Sunday `- 6 Saturday.&]
[s3;%RU-RU &]
[s4; &]
[s5;:IsLeapYear`(int`): [@(0.0.255) bool]_[* IsLeapYear]([@(0.0.255) int]_[*@3 year])&]
[s2;%RU-RU Returns true if [%-*@3 year] is leap (there is 29 days in 
February in that year).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetDaysOfMonth`(int`,int`): [@(0.0.255) int]_[* GetDaysOfMonth]([@(0.0.255) int]_[*@3 mo
nth], [@(0.0.255) int]_[*@3 year])&]
[s2;%RU-RU Returns a number of days in [%-*@3 month] of [%-*@3 year].&]
[s3;%RU-RU &]
[s4; &]
[s5;:LastDayOfMonth`(Date`): [_^Date^ Date]_[* LastDayOfMonth]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Returns day [%-*@3 d] last day of the month.&]
[s0;l288;%RU-RU Example:&]
[s0;l288;%RU-RU [*C@3       ][*C LastDayOfMonth(Date(2011, 9, 23)) `= 
`"30/9/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:FirstDayOfMonth`(Date`): [_^Date^ Date]_[* FirstDayOfMonth]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Returns day [%-*@3 d] first day of the month.&]
[s0;l288;%RU-RU Example:&]
[s0;l288;%RU-RU [*C@3       ][*C LastDayOfYear(Date(2011, 9, 23)) `= 
`"1/9/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:LastDayOfYear`(Date`): [_^Date^ Date]_[* LastDayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Returns day [%-*@3 d] last day of the year.&]
[s0;l288;%RU-RU Example:&]
[s0;l288;%RU-RU [*C@3       ][*C LastDayOfYear(Date(2011, 9, 23)) `= 
`"31/12/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:FirstDayOfYear`(Date`): [_^Date^ Date]_[* FirstDayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Returns day [%-*@3 d ]first day of the year.&]
[s0;l288;%RU-RU Example:&]
[s0;l288;%RU-RU [*C@3       ][*C FirstDayOfYear(Date(2011, 9, 23)) `= 
`"1/1/2011`"]&]
[s3;%RU-RU &]
[s4; &]
[s5;:DayOfYear`(Date`): [@(0.0.255) int]_[* DayOfYear]([_^Date^ Date]_[*@3 d])&]
[s2;%RU-RU Returns day [%-*@3 d ]number of day of year, beginning from 
1.&]
[s0;l288;%RU-RU Example:&]
[s0;l288;%RU-RU [*C@3       ][*C DayOfYear(Date(2011, 1, 2)) `= 2]&]
[s3;%RU-RU &]
[s4; &]
[s5;:AddMonths`(Date`,int`): [_^Date^ Date]_[* AddMonths]([_^Date^ Date]_[*@3 date], 
[@(0.0.255) int]_[*@3 months])&]
[s2;%RU-RU Adds [%-*@3 months] to [%-*@3 date] and returns the result.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetMonths`(Date`,Date`): [@(0.0.255) int]_[* GetMonths]([_^Date^ Date]_[*@3 since], 
[_^Date^ Date]_[*@3 till])&]
[s2;%RU-RU Number of months between two dates. Only whole months 
are counted.&]
[s2;%RU-RU Examples:&]
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
[s2;%RU-RU Number of months between two dates, including partial 
months.&]
[s2;%RU-RU Examples:&]
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
[s2;%RU-RU Adds [%-*@3 years] to [%-*@3 date] and returns the result. 
Note that this is not as simple as adding year attribute of Date 
because of leap years.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetWeekDate`(int`,int`): [_^Date^ Date]_[* GetWeekDate]([@(0.0.255) int]_[*@3 year], 
[@(0.0.255) int]_[*@3 week])&]
[s2;%RU-RU Returns the date of Monday for given [%-*@3 year] and [%-*@3 week] 
according to ISO`-8601. (First week of year is 1).&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetWeek`(Date`,int`&`): [@(0.0.255) int]_[* GetWeek]([_^Date^ Date]_[*@3 d], 
[@(0.0.255) int`&]_[*@3 year])&]
[s2;%RU-RU Returns the week for given date according to ISO`-8601. 
In [%-*@3 year] the corresponding year is returned, as some days 
at the beginning or end of the year can be part of week of another 
week.&]
[s3;%RU-RU &]
[s4; &]
[s5;:EasterDay`(int`): [_^Date^ Date]_[* EasterDay]([@(0.0.255) int]_[*@3 year])&]
[s2;%RU-RU Returns the Easter day for given [%-*@3 year].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTimeZone`(`): [@(0.0.255) int]_[* GetTimeZone]()&]
[s2;%RU-RU Returns a local timezone as offset from GMT in minutes.&]
[s3; &]
[s4; &]
[s5;:FormatTimeZone`(int`): [_^String^ String]_[* FormatTimeZone]([@(0.0.255) int]_[*@3 n])&]
[s2;%RU-RU Formats timezone in common format, used e.g. in internet 
dates.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetTimeZoneText`(`): [_^String^ String]_[* GetTimeZoneText]()&]
[s2;%RU-RU То же, что и FormatTimeZone(GetTimeZone()).&]
[s3; &]
[s4; &]
[s5;:ScanTimeZone`(const char`*`): [@(0.0.255) int]_[* ScanTimeZone]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 s])&]
[s2;%RU-RU Scans timezone text to obtain offset in seconds.&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetLeapSeconds`(Date`): [@(0.0.255) int]_[* GetLeapSeconds]([_^Date^ Date]_[*@3 dt])&]
[s2;%RU-RU Returns the number of leap seconds that has occurred since 
the start of adding leap seconds till [%-*@3 dt].&]
[s3;%RU-RU &]
[s4; &]
[s5;:GetUTCSeconds`(Time`): [_^int64^ int64]_[* GetUTCSeconds]([_^Time^ Time]_[*@3 tm])&]
[s2;%RU-RU Returns the number of seconds since 1970`-1`-1, including 
leap seconds.&]
[s3;%RU-RU &]
[s4; &]
[s5;:TimeFromUTC`(int64`): [_^Time^ Time]_[* TimeFromUTC]([_^int64^ int64]_[*@3 seconds])&]
[s2;%RU-RU Converts [%-*@3 seconds] since 1970`-1`-1, with leap seconds 
included, to Time.&]
[s3;%RU-RU &]
[s0; ]]