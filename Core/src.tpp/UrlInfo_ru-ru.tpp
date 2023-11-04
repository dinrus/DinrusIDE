topic "Структура UrlInfo";
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
[ {{10000@(113.42.0) [s0;%RU-RU [*@7;4 Структура UrlInfo]]}}&]
[s0; &]
[s1;:Upp`:`:UrlInfo`:`:struct: [@(0.0.255)3 struct][3 _][*3 UrlInfo]&]
[s2;%RU-RU Parses URL into parts and provides easy access to query 
parameters.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%RU-RU [* Список Публичных Членов]]}}&]
[s0;%RU-RU &]
[s0;%RU-RU Example values are based on parsing URL&]
[s0;%RU-RU &]
[s0;%RU-RU [@3 http://username:password`@hostname:9090/path?arg`=value`&aarg`[`]`=item1`&a
arg`[`]`=item2#anchor]&]
[s0;%RU-RU &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:url: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* url]&]
[s2;%RU-RU Original url.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:scheme: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* scheme
]&]
[s2;%RU-RU URL scheme: [@3 http]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:host: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* host]&]
[s2;%RU-RU Host: [@3 hostname]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:port: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* port]&]
[s2;%RU-RU Port: [@3 9090]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:username: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* user
name]&]
[s2;%RU-RU Authentication username: [@3 username]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:password: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* pass
word]&]
[s2;%RU-RU Authentication password: [@3 password]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:path: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* path]&]
[s2;%RU-RU Path: [@3 /path]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:query: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* query]&]
[s2;%RU-RU Query: [@3 arg`=value`&aarg`[`]`=item1`&aarg`[`]`=item2]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:fragment: [_^topic`:`/`/Core`/src`/String`$en`-us^ String]_[* frag
ment]&]
[s2;%RU-RU Fragment: [@3 anchor]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:parameters: [_^topic`:`/`/Core`/src`/VectorMap`$en`-us^ VectorMa
p]<[_^topic`:`/`/Core`/src`/String`$en`-us^ String], [_^topic`:`/`/Core`/src`/String`$en`-us^ S
tring]>_[* parameters]&]
[s2;%RU-RU Scalar query parameters: [@(0.0.255) `{`"][@3 arg][@(0.0.255) `": 
`"][@3 value][@(0.0.255) `"`}]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:array`_parameters: [_^topic`:`/`/Core`/src`/VectorMap`$en`-us^ V
ectorMap]<[_^topic`:`/`/Core`/src`/String`$en`-us^ String], [_^topic`:`/`/Core`/src`/Vector`$en`-us^ V
ector]<[_^topic`:`/`/Core`/src`/String`$en`-us^ String]>>_[* array`_parameters]&]
[s2;%RU-RU Array query parameters: [@(0.0.255) `{`"][@3 aarg][@(0.0.255) `": 
`[`"][@3 item1][@(0.0.255) `", `"][@3 item2][@(0.0.255) `"`]`}]&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%RU-RU Clears the URL to initial states (everything is empty).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:Parse`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Parse]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 url])&]
[s2;%RU-RU Parses [%-*@3 url].&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:operator`[`]`(const char`*`)const: [_^topic`:`/`/Core`/src`/String`$en`-us^ S
tring]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает value of scalar parameter or empty 
string if not present.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:GetArray`(const char`*`)const: [@(0.0.255) const]_[_^topic`:`/`/Core`/src`/Vector`$en`-us^ V
ector]<[_^topic`:`/`/Core`/src`/String`$en`-us^ String]>`&_[* GetArray]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%RU-RU Возвращает reference to array parameter or reference 
to empty array if not present.&]
[s3;%RU-RU &]
[s4; &]
[s5;:Upp`:`:UrlInfo`:`:UrlInfo`(const Upp`:`:String`&`): [* UrlInfo]([@(0.0.255) const]_[_^topic`:`/`/Core`/src`/String`$en`-us^ S
tring][@(0.0.255) `&]_[*@3 url])&]
[s2;%RU-RU Constructs UrlInfo by parsing [%-*@3 url].&]
[s3;%RU-RU &]
[s0;%RU-RU ]]