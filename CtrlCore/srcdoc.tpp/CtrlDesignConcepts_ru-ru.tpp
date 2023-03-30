topic "Концепты Дизайна Контролов";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%RU-RU 
[s0; [*R@5;6 Концепты Дизайна Контролов]&]
[s0; &]
[s0; При разработке собственных Ctrl`'ов 
часто `'реинвентируют`' `- повторно 
изобретают `- колесо... А ведь есть 
общие образцы, как это делать, и для 
вашего контрола Ultimate`+`+ , вероятно, 
уже имеет класс`-основу. Так, знание 
класса`-основы и некоторых его ключевых 
концептов дизайна может сделать вашу 
жизнь спокойней, а разработку которолов 
быстрее, позволяя более фокусироваться 
на проблеме, нежели на методах её 
решения. Как всегда, лучшая справка 
по Ctrl `- это его исходный код, который 
довольно`-таки не маловат, отчего 
попытаемся `"собрать до кучи`" (суммировать) 
некоторые полезности. В любом случае, 
стоит просмотреть [C virtual ]функции в 
Ctrl, чтобы понять, что подразумевается 
для использования или реализации 
лично вами...&]
[s0; &]
[s0; В целом, Ctrl в U`+`+ `- это визуализатор 
какого`-то рода данных. Данные, однако, 
не статичны, могут меняться из ГИП(GUI) 
(point and click) или из ИПП(API) перспектив, 
с помощью манипуляционных функций. 
Разница в том, что GUI interaction нацелена 
на изменение внутренних данных (или 
состояния) Ctrl`'а, И на уведомление 
приложения об изменениях, но изменение 
этого с помощью ИПП НЕ должно генерировать 
никаких уведомлений об изменении. 
Это очень важное правило дизайна, 
предохраняющее от множества `"головняков`", 
из`-за рекурсивных инвокаций при модификации
 Ctrl`'а через API.&]
[s0; &]
[s0; [* Ctrl Tree]&]
[s0; &]
[s0; В Ultimate`+`+ используется linked list (линкованный 
список) со всеми `"чадами`" (контролами`-отпрыс
ками) Ctrl`'а, которые Add()ированы в нему, 
разделяя с ним его пространство отрисовки. 
Ctrl НЕ `"владеет`" своими отпрысками, 
а просто ссылается на них (Ptr<Ctrl>). Владеть 
ими должно ваше приложение, где`-нибудь 
в контейнере U`+`+, напр. Array<Label>,  либо 
они уже являются членами вашего приложения, 
если используются файлы Layout. Если 
какой`-то Ctrl добавляется к какому`-то 
другому, надо удалить его у предыдущего 
родителя, а то Ctrl может стать частью 
двух деревьев.&]
[s0; &]
[s0; [* GetData / SetData]&]
[s0; &]
[s0; Большинству Ctrl`'ов, создаваемых вами, 
потребуется только одно единственное 
значение, чтобы визуализировать его 
или представить. Это верно для EditFields, 
Buttons, Labels и проч. Чтобы мочь Get / Set это 
единственное значение в/из Ctrl`'а, в 
U`+`+ используется его собственный 
класс `'полиморфного`' Value, который 
даёт контролам способность получать 
и обрабатывать intrinsic типы данных внутри 
себя, через единый интерфейс, не требуя 
от вас никаких преобразований. Для 
этого существует парочка GetData / SetData. 
Это главная дверь в ваш Ctrl. Она есть 
даже у более сложных Ctrl`'ов, типа TreeCtrl, 
 предоставляя текущий выделенный 
индекс. Think of your Ctrl, which information it could 
provide as general through this interface. it makes implicit 
usage easy, also in terms of notification (see next)&]
[s0; &]
[s0; [* WhenAction Callback]&]
[s0; &]
[s0; To notify upper layers of some changes, your Ctrl can use internally 
(or the user externally) the Action() function, which will call 
WhenCallback. and provide the feedback  This is the Callback 
that can be set using `'<<`= THISBACK()`' approach, so using 
it for your own Ctrl is preferable, since it leads to U`+`+ conform 
short syntaxes. Be careful to only call Action() inside your 
code upon graphical user interaction. When modifying your Ctrl 
from API, it should generate no Action(). More or diverse notifications 
can be provided in your controls using other global Callbacks 
(or even Callback1<yourtype> or more), if needed. Use the WhenSomething 
name convention to reflect Event behavior.&]
[s0; &]
[s0; [* Updated(), SetModify(), ResetModify(), ClearModify(), IsModified()]&]
[s0; &]
[s0; Often, the control needs to process or calculate other things 
based on the change of some data inside the control (like maybe 
some results, cached values or the like, NOT graphical helper 
data, this is done using Layout() which is invoked when resizing 
or opening the Ctrl). Use the Updated() virtual function to realize 
this, because it can be triggered from `'outside`' using the 
Update() function. It also SetModified()`'s your Ctrl, so you 
can check for it. Often, when data is changed, Ctrl needs to 
be updated somehow calculating its things and then the user needs 
to be notified. UpdateAction() does this in one step, calling 
both. If graphical data needs change as well, UpdateActionRefresh() 
is the chain to go, which will invoke an additional Paint(). 
ClearModify() acts recursively on all children too.&]
[s0; &]
[s0;* &]
[s0; [* Refresh() strategies]&]
[s0; &]
[s0; Each U`+`+ Ctrl can be scheduled for Refresh() explicitly. This 
does not always cause a Draw() immediately, i.e. in Win32, the 
control draw area is marked for repaint to be processed as soon 
as some time is available (the Message Queue decides and fires 
WM`_PAINT to causes the repaint). Sync() causes a manual repaint 
immediately This is sometimes handy to display immediate changes 
while Main Thread, which would draw it, is known to be locked 
for quite a while. This is used in Progress for example, to let 
the user know, that work in Main Thread is in progress (and cant 
repaint). More on this topic can be found in the Source Code 
documentation and the Manual.&]
[s0; &]
[s0; But When is the right point to call Refresh()? This depends. 
Think of your control and determine logically, what is considered 
representation of your data, and what is only setup or helping 
parametrization.So changing any of the data that renders invalid 
any portion of the Ctrl`'s draw space should trigger a Refresh(). 
Anything else probably not. Helping point: SetData() is probably 
changing your data for sure, it should generally call Refresh() 
after manipulation of the Ctrl`'s data. OTOH, i.e. changing Style 
should not immediately Refresh() because some other Settings 
might be necessary to change as well, and implicit calls to Refresh() 
would equal to performance pain and be in vain.&]
[s0; &]
[s0; User interactions from GUI perspective, changing your data, 
should generally repaint your Ctrl, at least in portions. Changing 
it from API side, should probably not, except for SetData....maybe. 
Because your application can call Refresh() anytime by itself, 
it knows best when and why.&]
[s0; &]
[s0; This all does apply for all Ctrl`'s in this world, cases differ. 
While the more static Ctr`'s like an EditField don`'t really 
care about heavy Refresh()ing (you wouldn`'t feel it), Ctrl`'s 
like a Plot Diagram showing some frequent live Data from some 
Generator would. Thats why in latter case, it might be useful 
to NOT Refresh() on data arrival (SetData) or change, and have 
the API determine the time, when to Refresh(), maybe after having 
inserted or changed a couple of related Plots.&]
[s0; &]
[s0; Refreshing a control just enough can sometimes be crucial. Take 
again the Diagram that plots live data.Refreshing it each and 
every little time due to small bits of info changes can keep 
your application busy and poor in response. Invent means of `'collecting`' 
data without posting it to the Ctrl, and once every 200 ms, do 
your work..and let the Ctrl Refresh(), this still yields a good 
look and keeps the GUI responsive.&]
[s0; &]
[s0; ]]