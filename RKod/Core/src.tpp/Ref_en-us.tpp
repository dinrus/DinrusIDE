topic "Ref";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Ref]]}}&]
[s1;@(0.0.255)3 &]
[s1;:Ref`:`:class: [@(0.0.255)3 class][3 _][*3 Ref][3 _:_][@(0.0.255)3 private][3 _][*@3;3 Moveable][3 <
][*3 Ref][3 >_]&]
[s2;%% Generic reference to rich Value types and Value. Constructor 
from various rich Value creates Ref, which then can be used to 
assign values to original. Note that assigning Ref to Ref assigns 
the reference, not the value (in this regard, it works more like 
C`+`+ pointer than C`+`+ reference).&]
[s3; &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Список Публичных Методов]]}}&]
[s3; &]
[s5;:Ref`:`:GetType`(`)const: [_^dword^ dword]_[* GetType]()_[@(0.0.255) const]&]
[s2;%% Returns the type of referenced variable (same codes as for 
Value, value itself has VALUE`_V type).&]
[s3; &]
[s4; &]
[s5;:Ref`:`:IsNull`(`)const: [@(0.0.255) bool]_[* IsNull]()_[@(0.0.255) const]&]
[s2;%% Возвращает true, если referenced variable contains Null.&]
[s3; &]
[s4; &]
[s5;:Ref`:`:Is`(`)const: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) bool]_
[* Is]()_[@(0.0.255) const]&]
[s2;%% Возвращает true, если referenced variable is of type [%-*@4 T].&]
[s3; &]
[s4; &]
[s5;:Ref`:`:Get`(`)const: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[*@4 T][@(0.0.255) `&
]_[* Get]()_[@(0.0.255) const]&]
[s2; [%% Returns reference to referenced variable (Is<][*@4 T]> must 
be true, otherwise undefined).&]
[s3; &]
[s4; &]
[s5;:Ref`:`:SetNull`(`): [@(0.0.255) void]_[* SetNull]()&]
[s2;%% Sets Null to referenced variable.&]
[s3; &]
[s4; &]
[s5;:Ref`:`:GetValue`(`)const: [_^Value^ Value]_[* GetValue]()_[@(0.0.255) const]&]
[s2;%% Returns the content of referenced variable as Value.&]
[s3; &]
[s4; &]
[s5;:Ref`:`:SetValue`(const Value`&`): [@(0.0.255) void]_[* SetValue]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Sets the content of referenced variable to [%-*@3 v].&]
[s3;%% &]
[s4; &]
[s5;:Ref`:`:operator Value`(`)const: [* operator_Value]()_[@(0.0.255) const]&]
[s2;%% То же, что и GetValue().&]
[s3; &]
[s4; &]
[s5;:Ref`:`:operator`~`(`)const: [_^Value^ Value]_[* operator`~]()_[@(0.0.255) const]&]
[s2;%% То же, что и GetValue().&]
[s3; &]
[s4; &]
[s5;:Ref`:`:operator`=`(const Value`&`): [_^Ref^ Ref][@(0.0.255) `&]_[* operator`=]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% То же, что и SetValue([%-*@3 v]).&]
[s3;%% &]
[s4; &]
[s5;:Ref`:`:Ref`(String`&`): [* Ref]([_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s5;:Ref`:`:Ref`(WString`&`): [* Ref]([_^WString^ WString][@(0.0.255) `&]_[*@3 s])&]
[s5;:Ref`:`:Ref`(int`&`): [* Ref]([@(0.0.255) int`&]_[*@3 i])&]
[s5;:Ref`:`:Ref`(int64`&`): [* Ref]([_^int64^ int64][@(0.0.255) `&]_[*@3 i])&]
[s5;:Ref`:`:Ref`(double`&`): [* Ref]([@(0.0.255) double`&]_[*@3 d])&]
[s5;:Ref`:`:Ref`(bool`&`): [* Ref]([@(0.0.255) bool`&]_[*@3 b])&]
[s5;:Ref`:`:Ref`(Date`&`): [* Ref]([_^Date^ Date][@(0.0.255) `&]_[*@3 d])&]
[s5;:Ref`:`:Ref`(Time`&`): [* Ref]([_^Time^ Time][@(0.0.255) `&]_[*@3 t])&]
[s5;:Ref`:`:Ref`(Value`&`): [* Ref]([_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Support for standard Value types and Value.&]
[s3;%% &]
[s4; &]
[s5;:Ref`:`:Ref`(`): [* Ref]()&]
[s2;%% Дефолтный конструктор, constructs empty Ref (no variable referenced, 
no value can be assigned).&]
[s0;%% ]]