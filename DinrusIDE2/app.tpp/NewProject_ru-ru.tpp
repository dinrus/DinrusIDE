topic "Новый Проект";
[l288;i704;a17;O9;~~~.992; $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,0#31310162474203024125188417583966:caption]
[b83;* $$3,5#07864147445237544204411237157677:title]
[b167;a42;C $$4,6#40027414424643823182269349404212:item]
[b42;a42; $$5,5#45413000475342174754091244180557:text]
[l288;a17; $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,0#20902679421464641399138805415013:code]
[b2503; $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255) $$9,10#83433469410354161042741608181528:base]
[t4167;C+117 $$10,10#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i416;b42;a42;O9;~~~.416; $$12,12#10566046415157235020018451313112:tparam]
[b167;C $$13,13#92430459443460461911108080531343:item1]
[a42;C $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128) $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,4#88603949442205825958800053222425:package`-title]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s2; [@5 Создание нового приложения (не используя 
шаблон пакета)]&]
[s0; -|В этом примере мы создадим простое 
приложение `"Hello world`", которое состоит 
из двух пакетов, с названиями [* Main] 
и [* Util].&]
[s0; &]
[s0; -|U`+`+ требует, чтобы исходники сохранялись 
под корневой директорией, называемой 
[* гнездом]. В гнездовой директории 
будут содержаться другие директории 
с исходниками. Директории, находящиеся 
в корневой, станут директориями пакетов, 
когда мы начнём делать наше приложение.&]
[s0; &]
[s0; -|U`+`+ нужно, чтобы один из пакетов был 
[* главным пакетом]. Скоро мы узнаем, 
как создать главный пакет.&]
[s0; &]
[s0; -|Запустим DinrusIDE. Появится диалоговое 
окно [* Открыть главный пакет]. Слева 
появится список [* сборок]. Кликнет 
правой кнопкой над списком сборок 
и выберем [* Новая сборка].&]
[s0; &]
[s0; -|A dialog will appear titled [* Assembly setup]. First, give the 
assembly a name in the field titled [* Assembly name]. A good choice 
is the name of the application, which we will call `"Hello`".&]
[s0; -|Next, provide a list of nest directories in the field titled 
[* Package nests]. The first directory must be the directory we 
want to contain the application`'s files. If you needed to use 
packages stored elsewhere, you would provide the names of the 
nests that contain them, separating the name of each directory 
with a semi`-colon.&]
[s0; -|We don`'t want to do this, so you should only provide the name 
of our application`'s source nest. So in the [* Package nest ]field, 
type `"c:/Hello`".&]
[s0; -|The two other fields in this dialog can be left with their 
default settings. Exit the dialog by clicking the [* OK] button.&]
[s0; -|At this stage, U`+`+ will have created the nest directory `"c:/Hello`". 
We now want to create two packages called [* Main] and [* Util]. 
Click on the [* New] button to start the [* Create new package ]dialog 
and give the package the name `"Main`" . DinrusIDE will start, 
showing package [* Main ]at the top left of the display.&]
[s0; -|We now want to make [* Main] a main package, because it is this 
package that must produce our target application. Right click 
on [* Main] and select [* Main package configuration] or select [* Workspace`|Main 
package configuration...] through the menu bar.&]
[s0; -|Now let`'s create package [* Util ]and add it to package [* Main].&]
[s0; ]]