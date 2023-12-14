topic "";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s1;%- &]
[s2;:SubpixelFiller`:`:Write`(int`):%- [@(0.0.255) void]_[* Write]([@(0.0.255) int]_[*@3 len])
&]
[s3; Эта функция записывает отфильтрованый 
субпиксельный буфер в вывод RGBA, выполняя 
поканальный (субпиксельный) alpha blending. 
Заметьте, что поканальный блендинг 
можно выполнить правильно, только 
когда цель непрозрачна (opaque) (a `=`= 255). 
Если это не так, то используется обычное 
сглаживание (normal anti`-aliasing), использующее 
среднее (арифметическое) (average).&]
[s3; Важное замечание: из`-за ошибок округления 
при фильтрации, иногда субпиксельное 
значение может быть 257. This is remedied by 
using 257 instead of 256 in the pixel blending command.&]
[s0; &]
[s4; &]
[s1;%- &]
[s2;:SubpixelFiller`:`:RenderN`(int`,int`,int`):%- [@(0.0.255) void]_[* RenderN]([@(0.0.255) i
nt]_[*@3 val], [@(0.0.255) int]_[*@3 h], [@(0.0.255) int]_[*@3 n])&]
[s3; This function adds 1 `- 6 val values to filtered subpixel buffer 
in single pass (6 is just enough needed for Render optimizations). 
Case variants are created by adding shifted Render filtering 
procedures.&]
[s4; &]
[s0; ]]