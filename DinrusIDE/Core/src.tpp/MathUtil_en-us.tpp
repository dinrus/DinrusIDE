topic "Math utility";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Math utility]]}}&]
[s3; &]
[s0;%% A number of mathematical functions and constants.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constant List]]}}&]
[s3; &]
[s5;:M`_E: [* M`_E]&]
[s2;%% [/ e-|-|-|]2.71828182845904523536&]
[s3;%% &]
[s4; &]
[s5;:M`_LOG2E: [* M`_LOG2E]&]
[s2;%% [/ log][/, 2][/ e-|-|-|]1.44269504088896340736&]
[s3;%% &]
[s4; &]
[s5;:M`_LOG10E: [* M`_LOG10E]&]
[s2;%% [/ log10e]-|-|-|0.434294481903251827651&]
[s3;%% &]
[s4; &]
[s5;:M`_LN2: [* M`_LN2]&]
[s2;%% [/ ln2]-|-|-|0.693147180559945309417&]
[s3;%% &]
[s4; &]
[s5;:M`_LN10: [* M`_LN10]&]
[s2;%% [/ ln10]-|-|-|2.30258509299404568402&]
[s3;%% &]
[s4; &]
[s5;:M`_PI: [* M`_PI]&]
[s2;%% [/ Pi]-|-|-|3.14159265358979323846&]
[s3;%% &]
[s4; &]
[s5;:M`_PI`_2: [* M`_PI`_2]&]
[s2;%% [/ Pi / 2]-|-|-|1.57079632679489661923&]
[s3;%% &]
[s4; &]
[s5;:M`_PI`_4: [* M`_PI`_4]&]
[s2;%% [/ Pi / 4]-|-|-|0.785398163397448309616&]
[s3;%% &]
[s4; &]
[s5;:M`_1`_PI: [* M`_1`_PI]&]
[s2;%% [/ 1 / Pi]-|-|-|0.318309886183790671538&]
[s3;%% &]
[s4; &]
[s5;:M`_2`_PI: [* M`_2`_PI]&]
[s2;%% [/ 2 / Pi]-|-|-|0.636619772367581343076&]
[s3;%% &]
[s4; &]
[s5;:M`_1`_SQRTPI: [* M`_1`_SQRTPI]&]
[s2;%% [/ 1 / sqrt(Pi)]-|-|0.564189583547756286948&]
[s3;%% &]
[s4; &]
[s5;:M`_2`_SQRTPI: [* M`_2`_SQRTPI]&]
[s2;%% [/ 2 / sqrt(Pi)]-|-|1.12837916709551257390&]
[s3;%% &]
[s4; &]
[s5;:M`_SQRT2: [* M`_SQRT2]&]
[s2;%% 1.41421356237309504880&]
[s3;%% &]
[s4; &]
[s5;:M`_SQRT`_2: [* M`_SQRT`_2]&]
[s2;%% 0.707106781186547524401&]
[s3;%% &]
[s4; &]
[s5;:M`_2PI: [* M`_2PI]&]
[s2;%% [/ 2Pi]&]
[s3; &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:Random`(`): [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:dword`:`:typedef^ d
word]_[* Random]()&]
[s2;%% Returns the next random generated number. Algorithm used is 
xoshiro256`*`*, with seed obtained from host platform specific 
resources (`'/dev/urandom`' in Posix systems, GUID generator 
in Win32).&]
[s3;%% &]
[s4; &]
[s5;:Random`(dword`): [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:dword`:`:typedef^ d
word]_[* Random]([_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:dword`:`:typedef^ d
word]_[*@3 n])&]
[s2; Returns random generated number smaller than [*@3 n].&]
[s3;%% &]
[s4; &]
[s5;:Random64`(`): [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:qword`:`:typedef^ q
word]_[* Random64]()&]
[s2;%% Returns 64bit random number.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Random64`(Upp`:`:uint64`): [_^Upp`:`:uint64^ uint64]_[* Random64]([_^Upp`:`:uint64^ u
int64]_[*@3 n])&]
[s2; [%% Returns the 64bit random number] smaller than [*@3 n].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Random64`(Upp`:`:uint64`*`,int`): [@(0.0.255) void]_[* Random64]([_^Upp`:`:uint64^ u
int64]_`*[*@3 t], [@(0.0.255) int]_[*@3 n])&]
[s2;%% Sets [%-*@3 n] random 64bit numbers to [%-*@3 t] array.&]
[s3; &]
[s4; &]
[s5;:Randomf`(`): [@(0.0.255) double]_[* Randomf]()&]
[s2;%% Returns the floating point number <0, 1).&]
[s3;%% &]
[s4; &]
[s5;:SeedRandom`(dword`): [@(0.0.255) void]_[* SeedRandom]([_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:dword`:`:typedef^ d
word]_[*@3 seed])&]
[s2;%% Seeds random with single value [%-*@3 seed]. This is good to 
get always the same sequence of numbers (for the same seed). 
Usually used for testing.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SeedRandom`(`): [@(0.0.255) void]_[* SeedRandom]()&]
[s2;%% Seeds random using the host platform entropy. Good to `"re`-randomize`" 
Random after seeding it with fixed value.&]
[s3; &]
[s4; &]
[s5;:ZeroArray`(x`): [* ZeroArray]([*@3 x])&]
[s2;%% Fills C array [%-*@3 x] with zeros.&]
[s3; &]
[s4; &]
[s5;:sqr`(double`): [@(0.0.255) double]_[* sqr]_([@(0.0.255) double]_[*@3 a])&]
[s2;%% Returns the square of [%-*@3 a].&]
[s3;%% &]
[s4; &]
[s5;:argsinh`(double`): [@(0.0.255) double]_[* argsinh]_([@(0.0.255) double]_[*@3 s])&]
[s2;%% Returns the hyperbolic arcsin of [%-*@3 s].&]
[s3;%% &]
[s4; &]
[s5;:argcosh`(double`): [@(0.0.255) double]_[* argcosh]_([@(0.0.255) double]_[*@3 c])&]
[s2;%% Returns the hyperbolic arcsin of [%-*@3 c].&]
[s3;%% &]
[s4; &]
[s5;:argtanh`(double`): [@(0.0.255) double]_[* argtanh]_([@(0.0.255) double]_[*@3 t])&]
[s2;%% Returns the hyperbolic arctan of [%-*@3 t].&]
[s3;%% &]
[s4; &]
[s5;:iscale`(int`,int`,int`): [@(0.0.255) int]_[* iscale]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 z])&]
[s2;%% Returns [%-*@3 x] `* [%-*@3 y] / [%-*@3 z].&]
[s3;%% &]
[s4; &]
[s5;:iscalefloor`(int`,int`,int`): [@(0.0.255) int]_[* iscalefloor]([@(0.0.255) int]_[*@3 x],
 [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 z])&]
[s2;%% Returns [%-*@3 x] `* [%-*@3 y] / [%-*@3 z][%-* ,][%-  rounded towards 
negative infinity].&]
[s3;%% &]
[s4; &]
[s5;:iscaleceil`(int`,int`,int`): [@(0.0.255) int]_[* iscaleceil]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 z])&]
[s2;%% Returns [%-*@3 x] `* [%-*@3 y] / [%-*@3 z][%-* ,][%-  rounded towards 
positive infinity].&]
[s3;%% &]
[s4; &]
[s5;:idivfloor`(int`,int`): [@(0.0.255) int]_[* idivfloor]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y])&]
[s2;%% Returns [%-*@3 x] / [%-*@3 y], truncated towards [%- negative infinity].&]
[s3;%% &]
[s4; &]
[s5;:idivceil`(int`,int`): [@(0.0.255) int]_[* idivceil]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y])&]
[s2;%% Returns [%-*@3 x] / [%-*@3 y], truncated towards [%- positive infinity].&]
[s3;%% &]
[s4; &]
[s5;:itimesfloor`(int`,int`): [@(0.0.255) int]_[* itimesfloor]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y])&]
[s2; Moves [*@3 x] to nearest lower multiple of [*@3 y].&]
[s3;%% &]
[s4; &]
[s5;:itimesceil`(int`,int`): [@(0.0.255) int]_[* itimesceil]([@(0.0.255) int]_[*@3 x], 
[@(0.0.255) int]_[*@3 y])&]
[s2; Moves [*@3 x] to nearest higher multiple of [*@3 y].&]
[s3;%% &]
[s4; &]
[s5;:fround`(double`): [@(0.0.255) int]_[* fround]([@(0.0.255) double]_[*@3 x])&]
[s2; Converts double [*@3 x ]to nearest integer. Checks integer limits.&]
[s3;%% &]
[s4; &]
[s5;:ffloor`(double`): [@(0.0.255) int]_[* ffloor]([@(0.0.255) double]_[*@3 x])&]
[s2; Converts double [*@3 x ]to integer rounding towards negative infinity. 
Checks integer limits.&]
[s3;%% &]
[s4; &]
[s5;:fceil`(double`): [@(0.0.255) int]_[* fceil]([@(0.0.255) double]_[*@3 x])&]
[s2; Converts double [*@3 x ]to integer rounding towards `+infinity. 
Checks integer limits.&]
[s3;%% &]
[s4; &]
[s5;:fround64`(double`): [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:int64`:`:typedef^ i
nt64]_[* fround64]([@(0.0.255) double]_[*@3 x])&]
[s2; Converts double [*@3 x ]to nearest integer. Checks integer limits.&]
[s3;%% &]
[s4; &]
[s5;:ffloor64`(double`): [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:int64`:`:typedef^ i
nt64]_[* ffloor64]([@(0.0.255) double]_[*@3 x])&]
[s2; Converts double [*@3 x ]to integer rounding towards negative infinity. 
Checks integer limits.&]
[s3;%% &]
[s4; &]
[s5;:fceil64`(double`): [_^topic`:`/`/Core`/src`/PrimitiveDataTypes`$en`-us`#Upp`:`:int64`:`:typedef^ i
nt64]_[* fceil64]([@(0.0.255) double]_[*@3 x])&]
[s2; Converts double [*@3 x ]to integer rounding towards `+infinity. 
Checks integer limits.&]
[s3;%% &]
[s4; &]
[s5;:modulo`(double`,double`): [@(0.0.255) double]_[* modulo]([@(0.0.255) double]_[*@3 x], 
[@(0.0.255) double]_[*@3 y])&]
[s2;%% Returns the floating`-point remainder of [%-*@3 x]/[%-*@3 y].&]
[s2;%% &]
[s2;%% The remainder of a division operation is the result of subtracting 
the integral quotient multiplied by the denominator from the 
numerator:&]
[s2;%% &]
[s2;%% remainder `= numerator `- quotient `* denominator.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SolveQuadraticEquation`(double`,double`,double`,double`*`): [@(0.0.255) int
]_[* SolveQuadraticEquation]([@(0.0.255) double]_[*@3 A], [@(0.0.255) double]_[*@3 B], 
[@(0.0.255) double]_[*@3 C], [@(0.0.255) double]_`*[*@3 r])&]
[s2;%% Solves quadratic equation. Returns number of solutions (0, 
1 or 2), stores solutions to array r (which must have at least 
two elements).&]
[s3;%% &]
[s4; &]
[s5;:AsString`(double`,int`): [_^topic`:`/`/Core`/src`/String`$en`-us`#String`:`:class^ S
tring]_[* AsString]([@(0.0.255) double]_[*@3 x], [@(0.0.255) int]_[*@3 nDigits])&]
[s2;%% [%- Returns the string representation of ][%-*@3 x][%-  with] [%-*@3 nDigits][%-  
precision].&]
[s3; &]
[s0; ]]