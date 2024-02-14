topic "Информация о Пакетах";
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s4;%RU-RU &]
[s5;:MoveFile`(const char`*`,const char`*`): [@(0.0.255) bool]_[* MoveFile]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 oldpath], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 newpath])
&]
[s2;%RU-RU Перемещает файл из [%-*@3 oldpath ][%- в] 
[%-*@3 newpath] .&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s1;:PackageInfo`:`:struct: [@(0.0.255) struct]_[* PackageInfo]_:_[@(0.0.255) public]_[*@3 Mov
eable]<[* PackageInfo]>_&]
[s2;%RU-RU Структура, содержащая информацию 
о пакете.&]
[s3; &]
[s4; &]
[s5;:PackageInfo`:`:stamp: [_^Time^ Time]_[* stamp]&]
[s2;%RU-RU Штамп времени.&]
[s3; &]
[s4; &]
[s5;:PackageInfo`:`:path: [_^String^ String]_[* path]&]
[s2;%RU-RU Путь к директории пакета.&]
[s3; &]
[s4; &]
[s5;:PackageInfo`:`:ink: [_^Color^ Color]_[* ink]&]
[s2;%RU-RU Цвет, которым пакет выделяется 
в списке.&]
[s3; &]
[s4; &]
[s5;:PackageInfo`:`:italic: [@(0.0.255) bool]_[* italic]&]
[s5;:PackageInfo`:`:italic: [@(0.0.255) bool]_[* bold]&]
[s2;%RU-RU Название полужирным, если [%-* italic 
`= ]true. Название курсивом, если [%-* bold 
`=] true.&]
[s3;%RU-RU &]
[s4; &]
[s5;:InvalidatePackageInfo`(const String`&`): [@(0.0.255) void]_[* InvalidatePackageInfo](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 name])&]
[s2;%RU-RU Удаляет информацию о пакете с 
именем [%-*@3 name] .&]
[s3;%RU-RU &]
[s4;%RU-RU &]
[s5;:GetPackageInfo`(const String`&`): [_^PackageInfo^ PackageInfo]_[* GetPackageInfo]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 name])&]
[s2;%RU-RU Выводит информацию о пакете с 
именем [%-*@3 name] .&]
[s3;%RU-RU &]
[s4; &]
[s5;:AddAssemblyPaths`(Upp`:`:FileSel`&`): [@(0.0.255) void]_[* AddAssemblyPaths]([_^Upp`:`:FileSel^ F
ileSel][@(0.0.255) `&]_[*@3 dir])&]
[s2;%RU-RU Добавляет директорию в пути к 
сборкам, давая возможность указать 
её с помощью диалога файлвыбора.&]
[s2;%RU-RU  [%-*@3 dir]       Директория, выбраная 
посредством файлвыборки.&]
[s3;%RU-RU &]
[s0;%RU-RU ]]