topic "SqlColumnInfo";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 SqlColumnInfo]]}}&]
[s3; &]
[s1;:SqlColumnInfo`:`:struct: [@(0.0.255)3 struct][3 _][*3 SqlColumnInfo][3 _:_][@(0.0.255)3 pub
lic][3 _][*@3;3 Moveable][3 <][*3 SqlColumnInfo][3 >_]&]
[s2;%% This strusture is used to describe a column of SQL database 
table. Validity and exact meaning of attributes depends on database 
used, except for [* name], [* type ]and [* nullable ]which are always 
valid.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Список Публичных Членов]]}}&]
[s3; &]
[s5;:SqlColumnInfo`:`:name: [_^String^ String]_[* name]&]
[s2;%% The name of column.&]
[s3; &]
[s4; &]
[s5;:SqlColumnInfo`:`:type: [@(0.0.255) int]_[* type]&]
[s2;%% The type of column as Value type id. One of INT`_V, DOUBLE`_V, 
DATE`_V, TIME`_V, WSTRING`_V, STRING`_V, SQLRAW`_V, INT64`_V, 
BOOL`_V; exact set of values permitted depends on database engine 
used. &]
[s3; &]
[s4; &]
[s5;:SqlColumnInfo`:`:width: [@(0.0.255) int]_[* width]&]
[s2;%% Size of column.&]
[s3; &]
[s4; &]
[s5;:SqlColumnInfo`:`:precision: [@(0.0.255) int]_[* precision]&]
[s2;%% Number of total digits in numeric types.&]
[s3; &]
[s4; &]
[s5;:SqlColumnInfo`:`:scale: [@(0.0.255) int]_[* scale]&]
[s2;%% Number of digits after comma in numeric types.&]
[s3; &]
[s4; &]
[s5;:SqlColumnInfo`:`:nullable: [@(0.0.255) bool]_[* nullable]&]
[s2;%% Column can contain NULL values.&]
[s3; &]
[s4; &]
[s5;:SqlColumnInfo`:`:binary: [@(0.0.255) bool]_[* binary]&]
[s2;%% Column contains binary data.&]
[s3; &]
[s0;%% ]]