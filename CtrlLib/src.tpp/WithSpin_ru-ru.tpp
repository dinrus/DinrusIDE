topic "Шаблонный класс WithSpin и редакторы Value со спин-кнопками";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Шаблонный класс WithSpin]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@3;3 Base][3 , ][@(0.0.255)3 class][3 _][*@4;3 IncType][3 _`=_][*@4;3 Da
taType][3 >]&]
[s1;:WithSpin`:`:class:%- [@(0.0.255) class]_[* WithSpin]_:_[@(0.0.255) public]_[*@3 Base]&]
[s2;%- &]
[s2;%- 
@@image:369&148
(A0cAHAAAAAAAAAAAAHic+/BhFIyCQQHWHrkeM3mPbfXaIYeAzgY6Hquntp5/fuzeu/9DEACdveTkY1ye+v7rz0A7EARWr938hzgAVAlUD3Q2Hk/BjTWeiY4YGBjo7KlPhADcU0AwJDz1+/fv96jAN6wCTQSoZsh56h0SiM9bCkHIgiR7avX/h3DOu/9RdPfUz58/X8BAQetFZAQXB6ohw1P1AxdTnz9/vkgIANUMOU8RA8hOfgtWD4CniEckeAoWQVHnQNz64eUpIDoIjiz611PEV75DyFO0q3zr7w1MkQ6sgz7CAFqRDhcntZ5aAG/Ygn00TCrfgW5R/Pr16w0qADaT0ESAaoaWp5BbFLgAyS2KgfYUFSvfX3/+0s3leACp9RTQ2Xg8Nfx6vreevN1+c6gioOOxemoUjIJRQDkAAEn0LCc=)
&]
[s2; &]
[s2; Этот шаблон добавляет спин`-кнопки 
к виджету [%-*@3 Base], которыми инкрементируется 
/декрементируется его значение. [%-*@3 Base] 
должен быть потомком класса Ctrl и иметь 
определения методов [* MinMax]([%-*@4 DataType,] 
[%-*@4 DataType]), [* GetMin ]и [* GetMax]. [%-*@4 DataType] `- это 
тип значения, содержащегося в [%-*@3 Base], 
[%-*@4 IncType] `- тип инкрементного значения.&]
[s2; Процесс инкрементации /декрементации 
можно кастомизировать двумя глобальными 
шаблонными функциями:  [* WithSpin`_DefaultIncValue] 
определяет дефолтное значение инкремента, 
а [* WithSpin`_Add] определяет то, каким образом 
значение инкрементируется /декрементируетс
я.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Список Публичных Методов]]}}&]
[s3;%- &]
[s5;:WithSpin`:`:SetInc`(IncType`):%- [_^WithSpin^ WithSpin][@(0.0.255) `&]_[* SetInc]([*@4 I
ncType]_[*@3 `_inc]_`=_[@3 1])&]
[s2; Устанавливает значение инкремента.&]
[s3; &]
[s4; &]
[s5;:WithSpin`:`:GetInc`(`)const:%- [*@4 DataType]_[* GetInc]()_[@(0.0.255) const]&]
[s2; Возвращает текущее значение инкремента.&]
[s3; &]
[s4; &]
[s5;:WithSpin`:`:OnSides`(bool`):%- [_^WithSpin^ WithSpin][@(0.0.255) `&]_[* OnSides]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Вместо помещения обеих кнопок с правой 
стороны виджета, помещает кнопку 
декремента слева, а кнопку инкремента 
`- справа.&]
[s3; &]
[s4; &]
[s5;:WithSpin`:`:IsOnSides`(`)const:%- [@(0.0.255) bool]_[* IsOnSides]()_[@(0.0.255) const]&]
[s2; True, если OnSides активен.&]
[s3; &]
[s4; &]
[s5;:WithSpin`:`:ShowSpin`(bool`):%- [_^WithSpin^ WithSpin][@(0.0.255) `&]_[* ShowSpin]([@(0.0.255) b
ool]_[*@3 s]_`=_[@(0.0.255) true])&]
[s2; Показывает/скрывает спин`-кнопки.&]
[s3; &]
[s4; &]
[s5;:WithSpin`:`:IsSpinVisible`(`)const:%- [@(0.0.255) bool]_[* IsSpinVisible]()_[@(0.0.255) c
onst]&]
[s2; Возвращает true, если спин`-кнопки показаны.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:WithSpin`:`:RoundFromMin`(bool`):%- [_^Upp`:`:WithSpin^ WithSpin][@(0.0.255) `&
]_[* RoundFromMin]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Если эта опция активна, инкременты 
округляются в сторону минимального 
значения. Напр.,  обычно (когда неактивно), 
если минимальное значение равно 3, 
рарастающая от минимума проходит 
через значения 3, 5, 10, ..., котрая при 
активном RoundFromMin, идёт через 3, 8, 13, 
...&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:WithSpin`:`:MouseWheelSpin`(bool`):%- [_^Upp`:`:WithSpin^ WithSpin][@(0.0.255) `&
]_[* MouseWheelSpin]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Если активен, значение можно менять 
колёсиком мыши `- прокручивание колеса 
равносильно нажатию стрелок вверх/вниз. 
Дефолтно активен.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:WithSpin`:`:NoMouseWheelSpin`(`):%- [_^Upp`:`:WithSpin^ WithSpin][@(0.0.255) `&
]_[* NoMouseWheelSpin]()&]
[s2; То же, что и MouseWheelSpin(false).&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:WithSpin`:`:KeySpin`(bool`):%- [_^Upp`:`:WithSpin^ WithSpin][@(0.0.255) `&]_[* K
eySpin]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Если активен, значение можно менять 
кнопками Вверх и Вниз. Дефолтно активен.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:WithSpin`:`:NoKeySpin`(`):%- [_^Upp`:`:WithSpin^ WithSpin][@(0.0.255) `&]_[* No
KeySpin]()&]
[s2; То же, что и KeySpin(false).&]
[s3;%- &]
[s4; &]
[s5;:WithSpin`:`:SpinButtonsObject`(`):%- [_^SpinButtons^ SpinButtons][@(0.0.255) `&]_[* Sp
inButtonsObject]()&]
[s5;:WithSpin`:`:SpinButtonsObject`(`)const:%- [@(0.0.255) const]_[_^SpinButtons^ SpinBut
tons][@(0.0.255) `&]_[* SpinButtonsObject]()_[@(0.0.255) const]&]
[s2; Возвращает ссылку на внутренний 
объект SpinButtons.&]
[s0; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Предопределённые значения WithSpin]]}}&]
[s3;%- &]
[s5;:EditIntSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[@(0.0.255) int],
 [_^EditInt^ EditInt]>_[* EditIntSpin]&]
[s5;:EditInt64Spin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[_^int64^ int6
4], [_^EditInt64^ EditInt64]>_[* EditInt64Spin]&]
[s5;:EditDoubleSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[@(0.0.255) d
ouble], [_^EditDouble^ EditDouble]>_[* EditDoubleSpin]&]
[s5;:EditDateSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[_^Date^ Date],
 [_^EditDate^ EditDate], [@(0.0.255) int]>_[* EditDateSpin]&]
[s5;:EditTimeSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[_^Time^ Time],
 [_^EditTime^ EditTime], [@(0.0.255) int]>_[* EditTimeSpin]&]
[s5;:EditIntNotNullSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[@(0.0.255) i
nt], [_^EditIntNotNull^ EditIntNotNull]>_[* EditIntNotNullSpin]&]
[s5;:EditInt64NotNullSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[_^int64^ i
nt64], [_^EditInt64NotNull^ EditInt64NotNull]>_[* EditInt64NotNullSpin]&]
[s5;:EditDoubleNotNullSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[@(0.0.255) d
ouble], [_^EditDoubleNotNull^ EditDoubleNotNull]>_[* EditDoubleNotNullSpin]&]
[s5;:EditDateNotNullSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[_^Date^ D
ate], [_^EditDateNotNull^ EditDateNotNull], [@(0.0.255) int]>_[* EditDateNotNullSpin]&]
[s5;:EditTimeNotNullSpin`:`:typedef:%- [@(0.0.255) typedef]_[_^WithSpin^ WithSpin]<[_^Time^ T
ime], [_^EditTimeNotNull^ EditTimeNotNull], [@(0.0.255) int]>_[* EditTimeNotNullSpin]&]
[s2; Эти typedefs предопределяют общие, основанные 
на EditField, редакторы значений со спин`-кнопкам
и.&]
[s3; ]]