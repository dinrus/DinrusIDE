topic "Класс Range";
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
[{_}%RU-RU 
[ {{10000@(113.42.0) [s0; [*@7;4 Класс Range]]}}&]
[s1;O_; &]
[s0; Алгоритмы U`+`+ предназначены для работы 
с [*/ Диапазонами]. Range `- это тип, у которого 
есть (как минимум):&]
[s0; &]
[s0;i150;O0; Станадартные методы begin() / end().&]
[s0;i150;O0; GetCount(), возвращающий число элементов 
в Range (может быть реализован как end 
`- begin)&]
[s0;i150;O0; operator`[`] (может быть реализован 
как begin()`[i`])&]
[s0; &]
[s0; Стандартные Ranges обычно реализуют 
ещё функцию ToString и сравнения (если 
это возможно).&]
[s0; &]
[s0; Обычно, Range является либо контейнером 
U`+`+, или просто какой`-то его частью.&]
[s0; &]
[s0; U`+`+ предоставляет такие определения 
типов, касающиеся Range, и шаблонные 
функции:&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ValueTypeOf`:`:typedef:%- [@(0.0.255) template <class ][*@4 Range][@(0.0.255) >
 using]_[* ValueTypeOf];&]
[s2; Возвращает тип элементов Range.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:IteratorOf`:`:typedef:%- [@(0.0.255) template <class ][*@4 Range][@(0.0.255) > 
using]_[* IteratorOf];&]
[s5;:Upp`:`:ConstIteratorOf`:`:typedef:%- [@(0.0.255) template <class 
][*@4 Range][@(0.0.255) > using]_[* ConstIteratorOf]`'&]
[s2; Возвращает тип Iterator / ConstIterator диапазона.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SubRange`(I`,I`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I]>_&]
[s5;:Upp`:`:SubRange`(I`,I`):%- [_^Upp`:`:SubRangeClass^ SubRangeClass]<[*@4 I]>_[* SubRang
e]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end])&]
[s2; Делает Range, основанный на итераторах 
begin/end.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SubRange`(I`,int`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I]>_&]
[s5;:Upp`:`:SubRange`(I`,int`):%- [_^Upp`:`:SubRangeClass^ SubRangeClass]<[*@4 I]>_[* SubRa
nge]([*@4 I]_[*@3 begin], [@(0.0.255) int]_[*@3 count])&]
[s2; Делает Range, основанный на итераторе 
begin и счёте count.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SubRange`(C`&`&`,int`,int`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
]>&]
[s5;:Upp`:`:SubRange`(C`&`&`,int`,int`):%- [@(0.0.255) auto]_[* SubRange]([*@4 C][@(0.0.255) `&
`&]_[*@3 c], [@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count]);&]
[s2; Делает Range как поддиапазон другого 
Range (напр., контейнера).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SubRangeOf`:`:typedef:%- [@(0.0.255) template <class ][*@4 Range][@(0.0.255) > 
using]_[* SubRangeOf];&]
[s2; Возвращает тип SubRange какого`-л. Range.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConstRange`(const T`&`,int`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_&]
[s5;:Upp`:`:ConstRange`(const T`&`,int`):%- [_^Upp`:`:ConstRangeClass^ ConstRangeClass]<
[*@4 T]>_[* ConstRange]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) int]_[*@3 count])&]
[s2; Создаёт Range из [%-*@3 count] элементов, равный 
[%-*@3 value].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ConstRange`(int`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_&]
[s5;:Upp`:`:ConstRange`(int`):%- [_^Upp`:`:ConstRangeClass^ ConstRangeClass]<[*@4 T]>_[* Co
nstRange]([@(0.0.255) int]_[*@3 count])&]
[s2; Создаёт Range из [%-*@3 count] дефолтно сконструирова
нных элементов [%-*@4 T].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ReverseRange`(BaseRange`&`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 B
aseRange]>_&]
[s5;:Upp`:`:ReverseRange`(BaseRange`&`&`):%- [_^Upp`:`:ReverseRangeClass^ ReverseRangeC
lass]<[*@4 BaseRange]>_[* ReverseRange]([*@4 BaseRange][@(0.0.255) `&`&]_[*@3 r])&]
[s2; Делает Range, реверсирующий порядок 
элементов [%-*@3 r]. Первый элемент [%-*@3 r] 
становится последним элементом ReverseRange 
и т.д.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:ViewRange`(BaseRange`&`&`,Upp`:`:Vector`<int`>`&`&`):%- [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 BaseRange]>_&]
[s5;:Upp`:`:ViewRange`(BaseRange`&`&`,Upp`:`:Vector`<int`>`&`&`):%- [_^Upp`:`:ViewRangeClass^ V
iewRangeClass]<[*@4 BaseRange]>_[* ViewRange]([*@4 BaseRange][@(0.0.255) `&`&]_[*@3 r], 
[_^Upp`:`:Vector^ Vector]<[@(0.0.255) int]>`&`&_[*@3 ndx])&]
[s2; Создаёт видимую область [%-*@4 BaseRange 
][%-*@3 r], на основе мапирования [%-*@3 ndx]. 
Элемент по [%-*@3 ndx]`[0`] становится первым 
элементом нового Range, [%-*@3 ndx]`[1`] вторым 
и т.д.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:FilterRange`(BaseRange`&`&`,Predicate`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 BaseRange], [@(0.0.255) class]_[*@4 Predicate]>_&]
[s5;:Upp`:`:FilterRange`(BaseRange`&`&`,Predicate`):%- [_^Upp`:`:ViewRangeClass^ ViewRa
ngeClass]<[*@4 BaseRange]>_[* FilterRange]([*@4 BaseRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Predicate]_[*@3 p])&]
[s2; То же, что и ViewRangeClass<BaseRange>([%-*@3 r], FindAll([%-*@3 r], 
[%-*@3 p])). Создаёт область видимости элементов 
из мастер`-Range, удовлетворяющего условию 
[%-*@3 p].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SortedRange`(BaseRange`&`&`,Predicate`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 BaseRange], [@(0.0.255) class]_[*@4 Predicate]>_&]
[s5;:Upp`:`:SortedRange`(BaseRange`&`&`,Predicate`):%- [_^Upp`:`:ViewRangeClass^ ViewRa
ngeClass]<[*@4 BaseRange]>_[* SortedRange]([*@4 BaseRange][@(0.0.255) `&`&]_[*@3 r], 
[*@4 Predicate]_[*@3 p])&]
[s2; Возвращает вид диапазона [%-*@3 r], отсортирован
ного по предикату [%-*@3 p].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SortedRange`(BaseRange`&`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 B
aseRange]>_&]
[s5;:Upp`:`:SortedRange`(BaseRange`&`&`):%- [_^Upp`:`:ViewRangeClass^ ViewRangeClass]<[*@4 B
aseRange]>_[* SortedRange]([*@4 BaseRange][@(0.0.255) `&`&]_[*@3 r])&]
[s2; Возвращает вид диапазона [%-*@3 r], отсортирован
ного по предикату std`::less.&]
[s0; ]]