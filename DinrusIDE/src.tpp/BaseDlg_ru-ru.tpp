topic "Базовый Диалог";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[i448;a25;kKO9; $$5,0#37138531426314131252341829483380:structitem]
[ $$0,0#00000000000000000000000000000000:Дефолт]
[{_} 
[s1;%RU-RU &]
[s2;:sPi: [@(0.0.255) static]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^PackageInfo^ PackageInfo]>_[* sPi]&]
[s3;%RU-RU Мап вектора, содержащий информацию 
о пакетах.&]
[s4; &]
[s1; &]
[s2;:InvalidatePackageInfo`(const Upp`:`:String`&`): [@(0.0.255) void]_[* InvalidatePacka
geInfo]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 name])&]
[s2;:InvalidatePackageInfo`(const Upp`:`:String`&`): -|Удаляет 
информацию о пакете.&]
[s2;O_;%RU-RU [%-*@3 name]..............................Имя пакета&]
[s1; &]
[s2;:GetPackageInfo`(const Upp`:`:String`&`): [_^PackageInfo^ PackageInfo]_[* GetPackageI
nfo]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 name])&]
[s0; -|[2 Получает информацию о пакете.]&]
[s3;%RU-RU [%-*@3 name] ...............................Имя пакета&]
[s4; &]
[s1; &]
[s2;:AddAssemblyPaths`(Upp`:`:FileSel`&`): [@(0.0.255) void]_[* AddAssemblyPaths]([_^Upp`:`:FileSel^ F
ileSel][@(0.0.255) `&]_[*@3 dir])&]
[s0; -|[2 Получает ссылку на директорию сборки 
и добавляет сборку в список. Используется 
при ]&]
[s0; [2 -|выводе диалогового окна NestEditorDlg. 
]&]
[s0;%RU-RU  [%-*@3 dir] ........................[2 Папка, выбранная 
в ФайлВыборке.]&]
[s4;%RU-RU &]
[ {{10000@3 [s0; [%RU-RU@2 Структура ][*@2 NestEditorDlg]]}}&]
[s5;:NestEditorDlg`:`:struct: [@(0.0.255) struct]_[* NestEditorDlg]_:_[@(0.0.255) public]_[*@3 W
ithNestEditorLayout]<[_^Upp`:`:TopWindow^ TopWindow]>_&]
[s0; [2 Эта структура служит для вывода 
диалогового окна с редактором гнёзд.]&]
[s0;2 &]
[s1; &]
[s2;:NestEditorDlg`:`:Set`(const Upp`:`:String`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 nests])&]
[s0; -|[2 Устанавливает список гнёзд.]&]
[s3;%RU-RU  [%-*@3 nests] .... Разделённый точкой 
с запятой список гнёзд.&]
[s4;%RU-RU &]
[s1; &]
[s2;:NestEditorDlg`:`:Get`(`)const: [_^Upp`:`:String^ String]_[* Get]()_[@(0.0.255) const]&]
[s0; [@(0.0.255) -|][2 Возвращает список гнёзд в 
виде строки, состоящей из разделённых 
точкой с запятой ]&]
[s0; [2 -|путей к гнёздам сборок.]&]
[s3;%RU-RU &]
[s4; &]
[s1; &]
[s2;:NestEditorDlg`:`:NestEditorDlg`(`): [* NestEditorDlg]()&]
[s3;%RU-RU Конструктор.&]
[s0;%RU-RU &]
[s4; &]
[s0;%RU-RU ]]