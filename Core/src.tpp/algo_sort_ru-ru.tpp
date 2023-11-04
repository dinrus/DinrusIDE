topic "Сортировка Шаблонных Функций";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Сортировка Шаблонных Функций]]}}&]
[s3; &]
[s5;:Upp`:`:Sort`(Range`&`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 R
ange], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:Sort`(Range`&`&`,const Less`&`): [@(0.0.255) void]_[* Sort]([*@4 Range][@(0.0.255) `&
`&]_[*@3 c], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует диапазон [%-@3 c, ][%- упорядочивание
 определено] [%-*@3 less]. Порядок элементов 
с одинаковыми значениями может быть 
изменён (нестабильная сортировка).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Sort`(Range`&`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>&]
[s5;:Upp`:`:Sort`(Range`&`&`): [@(0.0.255) void]_[* Sort]([*@4 Range][@(0.0.255) `&`&]_[*@3 c])
&]
[s2;%RU-RU Сортирует диапазон [%-@3 c, ][%- упорядочивание
 определено] [*/ operator<]. Порядок элементов 
с одинаковыми значениями может быть 
изменён (нестабильная сортировка).&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:StableSort`(Range`&`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:StableSort`(Range`&`&`,const Less`&`): [@(0.0.255) void]_[* StableSort]([*@4 Ra
nge][@(0.0.255) `&`&]_[*@3 r], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует контейнер [%-*@3 r ], упорядочивани
е определено [%-*@3 less]. Порядок элементов 
с одинаковыми значениями остаётся 
неизменным (стабильная сортировка).&]
[s6;%RU-RU &]
[s4; &]
[s5;:Upp`:`:StableSort`(Range`&`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Range]>
&]
[s5;:Upp`:`:StableSort`(Range`&`&`): [@(0.0.255) void]_[* StableSort]([*@4 Range][@(0.0.255) `&
`&]_[*@3 r])&]
[s2;%RU-RU Сортирует контейнер [%-*@3 r ], упорядочивани
е определено [*/ operator<].  Порядок элементов 
с одинаковыми значениями остаётся 
неизменным (стабильная сортировка).&]
[s6;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:IndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) void]_[* I
ndexSort]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r2],
 [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s5;:Upp`:`:IndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:IndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* IndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_
[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_
[*@3 less])&]
[s5;:Upp`:`:IndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4], [@(0.0.255) class]_[*@4 Les
s]>&]
[s5;:Upp`:`:IndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* IndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_
[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], [*@4 Range4][@(0.0.255) `&`&]_[*@3 r4], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует 2, 3 или 4 контейнера. 
Все контейнеры должны иметь одинаковое 
число элементов. Итоговый порядок 
определяется контейнером [%-*@3 r]. Упорядочиван
ие определено [* less].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:IndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2]>&]
[s5;:Upp`:`:IndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) void]_[* IndexSort]([*@4 M
asterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r2])&]
[s5;:Upp`:`:IndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3]>&]
[s5;:Upp`:`:IndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) void]_[* I
ndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r2],
 [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3])&]
[s5;:Upp`:`:IndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4]>&]
[s5;:Upp`:`:IndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) v
oid]_[* IndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_
[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], [*@4 Range4][@(0.0.255) `&`&]_[*@3 r4])&]
[s2;%RU-RU Сортирует 2, 3 или 4 контейнера. 
Все контейнеры должны иметь одинаковое 
число элементов. Итоговый порядок 
определяется контейнером [%-*@3 r]. Упорядочиван
ие определено [*/ operator<].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:StableIndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:StableIndexSort`(MasterRange`&`&`,Range2`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* StableIndexSort]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])
&]
[s5;:Upp`:`:StableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3],[@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:StableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* StableIndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s5;:Upp`:`:StableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4], [@(0.0.255) class]_[*@4 Les
s]>&]
[s5;:Upp`:`:StableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`,const Less`&`): [@(0.0.255) v
oid]_[* StableIndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], 
[*@4 Range4][@(0.0.255) `&`&]_[*@3 r4], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])
&]
[s2;%RU-RU Сортирует 2, 3 или 4 контейнера. 
Все контейнеры должны иметь одинаковое 
число элементов. Итоговый порядок 
определяется контейнером [%-*@3 r]. Упорядочиван
ие определено [* less]. Порядок элементов 
с одинаковыми значениями остаётся 
неизменным (стабильная сортировка)&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:StableIndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2]>&]
[s5;:Upp`:`:StableIndexSort`(MasterRange`&`&`,Range2`&`&`): [@(0.0.255) void]_[* StableIn
dexSort]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], [*@4 Range2][@(0.0.255) `&`&]_[*@3 r2])&]
[s5;:Upp`:`:StableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3]>&]
[s5;:Upp`:`:StableIndexSort2`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`): [@(0.0.255) vo
id]_[* StableIndexSort2]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3])&]
[s5;:Upp`:`:StableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 MasterRange], [@(0.0.255) class]_[*@4 Range2], 
[@(0.0.255) class]_[*@4 Range3], [@(0.0.255) class]_[*@4 Range4]>&]
[s5;:Upp`:`:StableIndexSort3`(MasterRange`&`&`,Range2`&`&`,Range3`&`&`,Range4`&`&`): [@(0.0.255) v
oid]_[* StableIndexSort3]([*@4 MasterRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Range2][@(0.0.255) `&`&]_[*@3 r2], [*@4 Range3][@(0.0.255) `&`&]_[*@3 r3], 
[*@4 Range4][@(0.0.255) `&`&]_[*@3 r4])&]
[s2;%RU-RU Сортирует пару контейнеров. Оба 
контейнера должны иметь одинаковое 
число элементов. Итоговый порядок 
определяется контейнером [%-*@3 r]. Упорядочиван
ие определено [*/ operator<].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetSortOrder`(const Range`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:GetSortOrder`(const Range`&`,const Less`&`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_[* GetSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Создаёт восходящий порядок значений 
в контейнере [%-*@3 r]. Упорядочивание 
определятся [%-*@3 less]. Порядок элементов 
с одинаковыми значениями может быть 
изменён (нестабильная сортировка).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetSortOrder`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 R
ange]>&]
[s5;:Upp`:`:GetSortOrder`(const Range`&`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) int]>_[* G
etSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%RU-RU Создаёт восходящий порядок значений 
в контейнере [%-*@3 r]. Упорядочивание 
определятся [*/ operator<]. Порядок элементов 
с одинаковыми значениями может быть 
изменён (нестабильная сортировка).&]
[s3;%RU-RU &]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:GetStableSortOrder`(const Range`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 Range], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:Upp`:`:GetStableSortOrder`(const Range`&`,const Less`&`): [_^Upp`:`:Vector^ Vector
]<[@(0.0.255) int]>_[* GetStableSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r
], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Создаёт восходящий порядок значений 
в контейнере [%-*@3 r]. Упорядочивание 
определятся [%-*@3 less]. Порядок элементов 
с одинаковыми значениями остаётся 
неизменным (стабильная сортировка)&]
[s3;%RU-RU &]
[s3;%RU-RU -|&]
[s4; &]
[s5;:Upp`:`:GetStableSortOrder`(const Range`&`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 Range]>&]
[s5;:Upp`:`:GetStableSortOrder`(const Range`&`): [_^Upp`:`:Vector^ Vector]<[@(0.0.255) in
t]>_[* GetStableSortOrder]([@(0.0.255) const]_[*@4 Range][@(0.0.255) `&]_[*@3 r])&]
[s2;%RU-RU Создаёт восходящий порядок значений 
в контейнере [%-*@3 r]. Упорядочивание 
определятся [*/ operator<]. Порядок элементов 
с одинаковыми значениями остаётся 
неизменным (стабильная сортировка)&]
[s3;%RU-RU &]
[s4; &]
[s5;:SortByKey`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map],
 [@(0.0.255) class]_[*@4 Less]>&]
[s5;:SortByKey`(Map`&`,const Less`&`): [@(0.0.255) void]_[* SortByKey]([*@4 Map][@(0.0.255) `&
]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающтй необходимые интерфейсы) 
[%-*@3 map] по ключам, используя [%-*@3 less] как 
предикат сортировки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SortByKey`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>&]
[s5;:SortByKey`(Map`&`): [@(0.0.255) void]_[* SortByKey]([*@4 Map][@(0.0.255) `&]_[*@3 map])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающтй необходимые интерфейсы) 
[%-*@3 map] по ключам, используя operator< как 
предикат сортировки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SortByValue`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map
], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:SortByValue`(Map`&`,const Less`&`): [@(0.0.255) void]_[* SortByValue]([*@4 Map][@(0.0.255) `&
]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающтй необходимые интерфейсы) 
[%-*@3 map] по значениям, используя [%-*@3 less] 
как предикат сортировки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SortByValue`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>&]
[s5;:SortByValue`(Map`&`): [@(0.0.255) void]_[* SortByValue]([*@4 Map][@(0.0.255) `&]_[*@3 map])
&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающтй необходимые интерфейсы) 
[%-*@3 map] по значениям, используя operator< 
как предикат сортировки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StableSortByKey`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 M
ap], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:StableSortByKey`(Map`&`,const Less`&`): [@(0.0.255) void]_[* StableSortByKey]([*@4 Map
][@(0.0.255) `&]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающтй необходимые интерфейсы) 
[%-*@3 map] по ключам, используя [%-*@3 less] как 
предикат сортировки. Стабилен: сохраняет 
порядок одинаковых элементов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StableSortByKey`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>&]
[s5;:StableSortByKey`(Map`&`): [@(0.0.255) void]_[* StableSortByKey]([*@4 Map][@(0.0.255) `&]_
[*@3 map])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающий необходимые интерфейсы) 
[%-*@3 map] по ключам, используя operator<  как 
предикат сортировки. Стабилен: сохраняет 
порядок равных элементов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StableSortByValue`(Map`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Map], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:StableSortByValue`(Map`&`,const Less`&`): [@(0.0.255) void]_[* StableSortByValue]([*@4 M
ap][@(0.0.255) `&]_[*@3 map], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающий необходимые интерфейсы) 
[%-*@3 map] по значениям, используя [%-*@3 less] 
как предикат сортировки. Стабилен: 
сохраняет порядок равных элементов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StableSortByValue`(Map`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Map]>&]
[s5;:StableSortByValue`(Map`&`): [@(0.0.255) void]_[* StableSortByValue]([*@4 Map][@(0.0.255) `&
]_[*@3 map])&]
[s2;%RU-RU Сортирует VectorMap или ArrayMap (или любой 
другой гипотетический контейнер, 
поддерживающий необходимые интерфейсы) 
[%-*@3 map] по значениям, используя operator< 
 как предикат сортировки. Стабилен: 
сохраняет порядок равных элементов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SortIndex`(Index`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Ind
ex], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:SortIndex`(Index`&`,const Less`&`): [@(0.0.255) void]_[* SortIndex]([*@4 Index][@(0.0.255) `&
]_[*@3 index], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует Index или ArrayIndex, используя 
[%-*@3 less ]как предикат сортировки.&]
[s3;%RU-RU &]
[s4; &]
[s5;:SortIndex`(Index`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index]>&]
[s5;:SortIndex`(Index`&`): [@(0.0.255) void]_[* SortIndex]([*@4 Index][@(0.0.255) `&]_[*@3 inde
x])&]
[s2;%RU-RU Сортирует Index или ArrayIndex.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StableSortIndex`(Index`&`,const Less`&`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 Index], [@(0.0.255) class]_[*@4 Less]>&]
[s5;:StableSortIndex`(Index`&`,const Less`&`): [@(0.0.255) void]_[* StableSortIndex]([*@4 I
ndex][@(0.0.255) `&]_[*@3 index], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s2;%RU-RU Сортирует Index или ArrayIndex, используя 
[%-*@3 less ]как предикат сортировки.  Стабилен: 
сохраняет порядок равных элементов.&]
[s3;%RU-RU &]
[s4; &]
[s5;:StableSortIndex`(Index`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 Index]>&]
[s5;:StableSortIndex`(Index`&`): [@(0.0.255) void]_[* StableSortIndex]([*@4 Index][@(0.0.255) `&
]_[*@3 index])&]
[s2;%RU-RU Сортирует Index или ArrayIndex. Стабилен: 
сохраняет порядок равных элементов.&]
[s3;%RU-RU ]]