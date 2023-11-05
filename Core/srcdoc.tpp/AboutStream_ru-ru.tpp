topic "О Потоке (класс Stream)";
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
[s0; [*R@5;6 О классе Stream(Поток)]&]
[s0; [/ (some loosely organized comments about Stream by Konstantin 
Hartwich)]&]
[s0;/ &]
[s0;i150;O0; Stream представляет собой только 
логическую `'cut out piece`'(вырезку) или 
конечный снэпшот (снимок) of a per definition 
сопротекающего, бесконечного потока 
данных, обрабатываемого, управляемого 
или доступного в какой`-то степени; 
поскольку переменная pos указывает 
на текущее смещение или поз`'ицию 
куска данных, представленного экземпляром 
Stream, от логического начала или старта 
потока.&]
[s0; &]
[s0;i150;O0; Stream в основном только класс`-интерфейс 
с указателями для ссылки на какую`-то 
область памяти. Сам он обычно никаких 
данных (`"собственных`", в понятиях 
U`+`+) не имеет. Поэтому он может быть 
MemStream, StringStream, FileStream и т.д... `'Они`' получают 
доступ, либо содержат (StringBuffer) буфер, 
и используют указатели из базы Stream, 
чтобы управлять им. Буфер, на который 
указывает Stream,`- это текущий отрезок 
данных, `*всецело`* доступный в вашем 
потоке.&]
[s0; &]
[s0;i150;O0; Stream по определению однонаправленный, 
как таковой. В отличие от других реализаций 
Stream, Upp Stream имеет всё для того, чтобы 
использоваться как Input (вводный) или 
Output (выводящий) поток. Эти два режима 
поддерживаются в одном экземпляре, 
но одновременно использоваться не 
могут. Тем не менее, не будет производиться 
никаких ASSERT, Exception или сообщений об 
ошибке, если кто`-то попытается сделать 
Put и Get из одного и того же Stream. Может 
быть, это просто вне логики и не то, 
чего вам хотелось бы, но Stream использует 
только один ptr для представления текущей 
`'head`' позиции чтения или записи. (так 
что `"интринсикально`" никак не возможно 
использовать MemStream как Circular Buffer, что 
было бы неплохо... (btw (между прочим), 
а не реализовать бы такое?)  Эти 2 Modes 
(режима) можно `"разнить`" посредством 
функций API (ИПП) IsStoring() / IsLoading(). Эти 
Modes устанавливаются с помощью SetStoring() 
/ SetLoading() и, как правило, устанавливаются 
автоматически, в зависимости от того, 
как вами был создан экземпляр потока.&]
[s0; &]
[s0;i150;O0; В обох режимах, расширение буфера 
обозначает доступное пространство 
для `*всего`* чтения или записи, используя 
указатели rdlim и wrlim. Для чтения это 
означает текущий доступный, читаемый 
чанк (сегмент) данных. Для записи `- 
это `*уже аллоцированные`* данные, 
в которые можно писать. При чтении, 
ptr означает текущую позицию чтения. 
Буфер to ptr `- это данные, которыми уже 
`"овладело`" чтение. Пространство от 
ptr до rdlim означает данные, которые 
ещё надлежит прочесть. При записи, 
буфер до ptr означает данные, уже записанные 
в данное пространство, ptr до wrlim  `- 
пространство, свободное для заполнения 
(перед тем как нужно будет триггировать 
Flush, если он поддерживается, или он 
тригируется автоматически, как у 
FileStream)&]
[s0; &]
[s0;i150;O0; Сериализация stuff`'а в Stream довольно 
крута в иных реализациях, у Stream в наличии 
есть сплит`-интерфейс для сериализации 
и десериализации `"стаффа`", так, чтобы 
пользователю нужно было отслеживать, 
в каком порядке идут элементы, и использоват
ь в основном две функции, для Сериализации 
и Десериализации. Поток upp снимает 
эту головную боль. Он использует 1 
интерфейс, и внутренне и косвенно 
обрабатывает разницу, относящуюся 
к сериализации/десериализации, с 
помощью IsLoading() / IsStoring().Выгода пользователю 
от этого,`-  ему достаточно указать 
`*одно`* функциональное место, определяющее 
порядок сериализации и она одинаково 
будет поддерживаться в обоих направлениях, 
hurray. (drawback, нельзя десериализовать 
из `'const Stream `&`', так как функции требуется 
`'Stream `&`'). Поскольку это как`-то необычно, 
надо же знать, что такое Stream!&]
[s0; &]
[s0;i150;O0; Глобальный шаблонный оператор 
operator%(Stream `&, T `&) позволяет сериализовать 
ваш класс, как и пользователю, простым 
вызовом `"поток % ваш`_класс`". Предоставьте 
в своём классе функцию `'void Serialize(Stream`& 
s)`', где определены поведение и порядок 
де/сериализации контента (сериализуются 
и иные данные). После десериализации 
вашего класса следует выполнить завершающий
 init вашего класса. Это делает обработку 
транспорта данных с помощью Socket довольно 
простой, но пока не реализована Smile&]
[s0; &]
[s0;i150;O0; Можно легко реализовать свои 
собственные буфферированные потоки, 
предоставлятся довольно защищённый 
интерфес. Stream предоставляет богатый 
интерфейс дефолтной сериализации 
всякого рода стаффа, включая контейнеры 
NTL...(затем, к тому же, рекурсивно вызывая 
над ними serialize..)&]
[s0; &]
[s0;i150;O0; Нужно хорошо знать, какой вид 
потока Stream использовать и для чего.. 
т.е. MemStream нельзя использовать как 
`"самонарастающий`" Buffer, он обрабатывает 
только ранее предоставленный кусок 
памяти, который не умеет расти (так 
как куски памяти не растут!). Для этого 
юзайте StringStream.&]
[s0; &]
[s0;i150;O0; Базовая [/ имплементация потока] 
Stream такая: Stream ссылается на кусок памяти, 
представленный указателем на `'buffer`'. 
Он неявно сохраняет его размер указателями 
rdlim и wrlim, представляющими протяженность 
буфера для операций чтения и записи. 
Каждая функция API полагает, что может 
сохранять или читать свои данные 
прямо в этом куске памяти, по местонахождени
ю ptr, как правило, не вызывая никакого 
flush или типа того, продвигая далее 
и далее ptr. Но рано или поздно они потребят 
всё пространство (reacing respective rdlim, means 
having read all, or wrlim, meaning having written all). then 
it will claim some `'upper level`' action to either provide more 
data, done by advanceing the snapshot position in the read case, 
or writing out stored data and mark it as free again. this is 
done invoking `_Get(..) or `_Put(..). in other words... `_Put 
normally should take care of processing the full buffer by flushing 
it somehow, process the data provided, that didnt fit in the 
full buffer, and rewinding the ptr and adjusting wrlim, declaring 
buffer empty. `_Get typically claims some more data to be made 
available inside the Stream, maybe by copying some data in provided 
empty buffer first, then remainder in buffer and again rewinding 
the ptr and adjusting rdlim. this behaviour is to be defined 
somehow, and is special for any kind of stream.&]
[s0; &]
[s0;i150;O0; Flushing behavior is only used for write side, or out 
buffers. it is not invoked by generic Stream implementation by 
default. but higher level Streams use it in to do exacely this. 
flushing queued data to the underying destination (File Streams 
only so far).&]
[s0; &]
[s0;i150;O0; there are several helper functions around handling Streams, 
even copying, which is normally not possible just like that, 
or stuff like providing a version for streamed data and some 
constraints on version (min, max) when serializing. take a look 
in Stream.cpp.&]
[s0; &]
[s0;i150;O0; OutStream uses a small internal buffer to accumulate 
things, before it forwards them to a to be user implemented Out(..) 
function, which should take care of processing it somehow (sending 
somewhere or what ever..)&]
[s0; &]
[s0;i150;O0; TeeStream is an OutStream that uses internal buffer 
again, and when time has come, pushes it to 2 other streams..(so 
a little `'data latency`' is expected..if you want to make the 
data be available at once, call Flush() after your operations..&]
[s0; ]]