topic "О Фреймах (Рамках)";
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C+117 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i416;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[a42;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; [@5 О Фреймах (Рамках)]&]
[s0; &]
[s0; Каждый U`+`+ Ctrl (widget) имеет один или 
более фреймов. Фреймы `- это объекты, 
производные от класса CtrlFrame, которые 
формируют внешний вид и функционал 
области, расположенной между внешней 
границей Ctrl и его отображением.&]
[s0; &]
[s0; Фреймы упорядочиваются `- у каждого 
имеется собственный индекс....У Ctrl 
всегда есть фрейм с индексом 0, и, как 
правило, он используется для обеспечения 
декоративного бордюра Ctrl. Если Ctrl 
не требуется никакого декоративного 
бордюра, фрейм 0 можно установить 
в NullFrame(). Например,  StaticRect по умолчанию 
имеет NullFrame() в качестве frame0:&]
[s0; &]
[s7; -|TopWindow  win;&]
[s7; -|StaticRect ctrl;&]
[s7; -|ctrl.Color(SLtCyan);&]
[s7; -|win.Add(ctrl.SizePos());&]
[s7; -|win.SetRect(0, 0, 100, 100);&]
[s7; -|win.Run();&]
[s7; &]
[s0; &]
[s0;= 
@@image:662&825
(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)
&]
[s0;= &]
[s0; Можно изменить frame 0, используя метод 
[* SetFrame]:&]
[s0; &]
[s7; -|ctrl.SetFrame(InsetFrame());&]
[s7; -|win.Run();&]
[s7; &]
[s0;= 
@@image:662&825
(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)
&]
[s0;= &]
[s0; Помните, что интерфейс класса CtrlFrame 
разработан так, что в случае специфичных 
Фреймов (типа InsetFrame и других декоративных 
статических фреймов) один экземпляр 
объекта`-фрейма может использоваться 
для любого числа экземпляров Ctrl. В 
иных случаях, как, например, со ScrollBar 
и прочими, содержащими субконтролы`-фреймы, 
нужно использовать один экземпляр 
класса Frame на каждое единичное применение.&]
[s0; &]
[s0; Теперь можем добавить другой декоративный 
фрейм в свой пример:&]
[s0; &]
[s7; -|ctrl.AddFrame(BlackFrame());&]
[s7; -|win.Run();&]
[s7; &]
[s0;= 
@@image:662&825
(A2oAhAAAAHz/AAAAAHic7d17TFRXHgfwO7AFbaPbQjW+KSpq8VHAtziKIqDAIK9BUBhBnUKpQqW23b7T3TRNHybtbpum3WxS0qRZW7S1qfvIbjZp/2iarg2IFakLdZiZO3dmoFAq2pUV2d9vzp1z7zzQmQl0huR3883NnTN3Xh/O656p09FRj03I+0aT3wYRci5icjvlFJqEkh5hr1UwSGyvqXEKBoewVxIqHXhQ6U55bzDpDyIV9kDj/RKeL1rEIuI+T3IdiK4DuOkqybMI2VYMHOBNCcuzncLW9tExNoYWUzsU2zQS/9TonN9iFr04uvT4aOpro6lvjm56A6N9C7P17THDTggwt3ie8X1mKGFvnu/HCn5YzwACBExiDt8AQCH1S1+6qCoH0EUdHow9dhPO5ICJL2PAcPnrSgCTRV0YOYF3y6K+yRBYfWAfigcK1TfZB+dhGpDYxhGIUH5FSFcqobDta6HQLJRc0hh7IULtDywgGd1wNbpxGDyFR0eim0Yh7Bj2vJAdsMKYozeh6rIzxz2awzcDDDuZvT323lg09cNRxuvRdUPRxkFN9VVh/0/CXtd+309wUy6p7pdT2Yd7KNE75SYPJZBSi5B5XtB2KXp53wh7LyuBzu2Ag0mCIQtKjhG8F94Sj3Ew4nKwD1PTC901tDII9tjlNoxelAMsLNDDQ6CrLzRpdF1Cbhf2/HwgALr1/xZmfYR0aZ/jvaBXckmOCpAbynnoJyW8sPYHPEf9JwsijokK4HjFBRVXaTnz5cDQtZGx+v+xNnhU3K42QXsW6QANMuM9+WDb1zjOjqHH4yWGJXACG3wNqvfM/qZhDK9F6koFvVOhCRCGe7p6X33U8XS1dFRvq9eJD+601my3Htxh3b8FYqlMh5jLVmNKUky6ZV1ZCW0lawY62+GxMGQgV/JfhSUtqLf4A396Vr96XEyFJrnmMJKHG28Ltw3/mIGENaigwh/oooNmCLWo7/jjA++//uMHb/a/+2Lv8cecLzU4X6hzPP+g45kD9if325802B+rkJrKbA2FYt0ua/VWkz6tLXvRX/JSoMbKegv/jm7Tn4X2ize1Z6OyvPXQhylxwxqnUmjwbCC+f/dxT1Bu2GuZ3W5m3omBnuN5448n3h5sebf/Ty/3vfFU76tNzhePOF6osz9nBEDH0zX2J6rsxypsjcVifb71YGZPxfr23GUfrp6BHR3orfjKVw+7RHXFMziYFQvrb1mXqzRVv6018Io3cXVPFlPHJORhQMB+rBzcBpqP//DW872vHHX+rt7x7CH7bwzSoxVSY6ntSJFYlyfW5WJbrt5qqdxkKlvTnp/csm4W6kGnB3r3faXogSdMAtkFBbxJfvmgAgQ99YCFY5bfuhcU3cQFrh3UdDoLxq0nNRb3vfFk3x+eAbqfz36BJU0V0pESsU5nPZRt3b/Nsm+TpWIDlF/5x0mzPvVyScr5ghUnN81HvXXtsh70e6A355/omd6OIy/81eClXXrMR03HwwHlcf8X0ysKPszNUw9Git6XGnpfeQRqHR9SXXQ51v0ZQGcuW8PLzcUrLxevOq/z0WN1D/SgJMM1q2GDFFwnVsp0Xm6+egioXF1GTPW7pZ5ozIZeDhvsc0b7EwYOZa3JtFZpLeXrFLqCpebCZFPRivO65JMb58l697d51D2uVyS69fr4/I3NM+842M8iG1b1qYPakQao99TL89CzP1VthxEW9sf2SQ2lnAt6OYVOt9ict9CsSzIV3v9t/v2nNs5FvbWd/vWgf4APziqeWy+6epC7eQB66/XhhQyvhHBpEwnhNTBP4oZIVLUZhlQcWB+vgh7P1lgiPlzgNTe2Fi217l5kyb/PolvYszvpQm7Sx+tny3qs+sGUD/SAcUMnrsZgxXPKV3MuN4z7SiemdogFjhGQ3QvxAgw2E6vX6xmsiqhXsV46UmRv2mOHQfboHltDMc6Za3OUPnDPCrFkiVi4UNQtEHUJ5oLEjl2JH6+diXqbvpOrH9S9+Fc99Nx0cGnMrpqRznXpyvW4oQJYPQhX2ZigFusmOvpBJSpDbJKlaeJDedIjpdLRMvsjehiCbYd1XnVP0i+RihNtBfNsunmW/Hkd2fM+WR2Heukm1FvUgXUP9BLaBW0X6undei666JprmkPXo4zXXYsSwzH115UwwJprPLg6sdcVxjhBKb8SShTDHyEg0LN7OczlbA8XgJvUWGJrKFLQjFp2cPPqoL000V44V9LNsubOurh95qepv0a9jB4EXNWFejN+j3pwE/WcjA71VG4sHnquQKF8jsow2MjmExpPQxAw5SVZDFqxdqftcIHtSKFCV7ddMm62H1DGXEfhbHv+TGtOfOfWu0+vvBP1ttmFdLOiB5UQ9Vy9BKODz3XITffw/6Lqb8I+5vAN3zBYZggPgQjVkZFKVbihfggCApezFpjL11kPbBdrd3EoW12WVLtNOpRuP7DWaVipAObeY9sx7WL6XaeXx6AeQDFAXGNx68GoVN7L2yynU7uxlVV1ohtGwRaiqR+GCMYRCGOciAjVNwIOY7zhgVnxX6H8ZxD4PmNWT9EqmJwA4NXPzyCdMdNm3GY7pJUObLBXpzkqk50Vi6H85781O3ZOFzOmXlx3xydLo1AvZwD1Njhxziy3XDN+61EuDxZedKAUc1ReNGZLynDASiAACGErukxy4sL+OkGHk7oMQaB7873QeHtKUy37NloNWut+rdWQLho2iob1tqo0ad8K+54kR2kCNFuoePYdd1m1d3Ssjvp4iUbWg+oHenB5C3pzWrmeV8WLaRz2Q+eOrMdXzhuUTDRjKNpuQxDoXD/9P5nzuvOXfV/8wGX9atOeNSZ9qqkstadslVmfbC1JshYmiPmzxZ3xYuY065ZY04aocw8IHy4SUC//CgKCHmu5nnoMUB4peGttGvHVuzVggPnlAYeujXyza9nZDfFnt8xt3ZHYmpPUmrOkNXtxW86ic9mJ7VkLLmTNvbB9ZkdG/AXt9I6NUy+s/dW5FM0XycJHmxO89WDGotJjX46w6qdMVLwMVc3WL13Yq9yYLd2ld+bLgf6OtjM7ln2wfFpLyj0fpcadSLv3REpcS1r8ybT4U6l3n06Z9umqOz9bOeV0cszpZdGfLdWcWiy0bE5wftuKa8t+9Excj+3laTBWwiGP6QofbV29olfYe+aTnEASYlcWXI/nMRDHVV8BhNEgN6ix+L2GfkDp97z0dK4VSHmRU7noUL6WUn9rVjfMxll1+IAbzOwi8GH0RhBPW3ldHiZgnGXTlT3XYMDFwAFMXYqG5D074IEZdf4A7tm9UNNYZfOKl166a+lVe1bIbNXsaIvKwvD/DEOj62LB5VOdRVl19F3FVa8IjXuyTYEGl1PGOD+zC5dEIFp3tlySb2a4w0pgD1e1ELiSZZe3cAB7sPKrl9mKei5Abug37AQMP1/1wMDDXjHAaIJJUM8cXNiVmmvMjV1zaWZ9/xR9d8uf36EEEtdawSUh8YQQ/5paT6Dtdlufo5P0Qt689Kb66sG9FL/hejDEzH+P9EgvjHpTUr8jPdIjvUgP6Y2rXmxqJ+mForeA9EiP9CZJSI/0IkZvCumFoLcFZizNpEd6YdSjfi8UPW23sID0SI/0Jkl89GjUCEmPr1CRHumFS49WR0PSW0B6pBdOvdg00gtBr5v0SC+8ejRjCUmPVkdJL8x61O+RHulNgpDeuOrRqBGK3pYuWh0lvXDrUb9HeqQX8SE90iO9yRjSIz3Sm4whPdIjvckY0iM90puMIT3SC79et/tfu5Ae6ZFexMfjG0nSIz3SmyTx0YslPdIjvcgP6Y2PXrf7l5RIj/RIL+Lj+69dSI/0SC/y47E6SnqkR3qTJOrVUbrWID3Smyzx0aPV0RD16PdYSC+sevQbaKHoKaujpEd64dGjfi9UPRo1SC+cevS7BKHo8d9bJj3SI73Ij5/fmSe94PVozCW9CNSjbeztFr+3DHdRbhu/erf5f97Tptq4njC7FQDjikVmGLvbCRF2DlLGDNCt/JesN/MUdoCYZveBOs1+yud7xqtcfkiz9035nGaM9/Pzh3hG/ZD5zd4nB/JW/Rd6fRzPl/N+Fd8XfR8P5v4R9e59B/eUkPJ/ENThSQ==)
&]
[s0;= &]
[s0; Как видите, фреймы добавляются от 
внешнего бордюра в область видимости 
Ctrl. Для исследования интерфейса класса 
CtrlFrame, создадим простой фрейм с самого 
начала:&]
[s0; &]
[s7; struct Frame1 : CtrlFrame `{&]
[s7; -|virtual void FrameLayout(Rect`& r) `{&]
[s7; -|-|r.left `+`= 20;&]
[s7; -|`}&]
[s7; -|virtual void FrameAddSize(Size`& sz) `{&]
[s7; -|-|sz.cx `+`= 20;&]
[s7; -|`}&]
[s7; -|virtual void FramePaint(Draw`& w, const Rect`& r) `{&]
[s7; -|-|w.DrawRect(r.left, r.top, 20, r.Height(), SLtGreen);&]
[s7; -|`}&]
[s7; `};&]
[s0; &]
[s0;~~~736; [* FrameLayout] устанавливает внутреннюю 
выкладку рамки и уменьшает размер 
прямоугольника Ctrl (контрола). U`+`+ стартует 
от внешнего прямоугольника контрола 
(Ctrl) и вызывает FrameLayout (выкладку рамки) 
для каждого фрейма. Результирующий 
прямоугольник представляет внешний 
вид контрола. [* FrameAddSize] представляет 
собой противоположный процесс: он 
используется для определения требуемого 
размера Ctrl, чтобы получить заданый 
размер внешнего вида, другими словами, 
для вычисления размера контрола от 
размера его вида с текущим набором 
фреймов. И, наконец, [* FramePaint] позволяет 
рисовать в области рамки.&]
[s0; &]
[s0; Теперь перейдём к нашему фрейму:&]
[s0; &]
[s7; -|Frame1 frame1;&]
[s7; -|ctrl.InsertFrame(0, frame1);&]
[s7; -|win.Run();&]
[s7; &]
[s7; &]
[s0;= 
@@image:662&825
(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)
&]
[s0;= &]
[s0; В более сложных случаях, когда рамка 
должна обеспечивать интерактивное 
поведение `- как, например, ScrollBar или 
HeaderCtrl, нужно добавить некоторые субконтролы
 в область рамки. Это можно сделать 
методами [* FrameAdd] и [* FrameRemove] (FrameRemove зарезервиров
ан для удаления из фреймов), использующими 
[* SetFrameRect]:&]
[s0; &]
[s7; struct Frame2 : Frame1 `{&]
[s7; -|Button x;&]
[s7; &]
[s7; -|virtual void FrameLayout(Rect`& r) `{&]
[s7; -|-|x.SetFrameRect(r.left, r.top, 16, 16);&]
[s7; -|-|r.left `+`= 16;&]
[s7; -|`}&]
[s7; -|virtual void FrameAddSize(Size`& sz) `{&]
[s7; -|-|sz.cx `+`= 16;&]
[s7; -|`}&]
[s7; -|virtual void FramePaint(Draw`& w, const Rect`& r) `{&]
[s7; -|-|w.DrawRect(r.left, r.top `+ 16, 20, r.Height() `- 16, SLtMagenta);&]
[s7; -|`}&]
[s7; -|virtual void FrameAdd(Ctrl`& parent) `{&]
[s7; -|-|parent.Add(x);&]
[s7; -|`}&]
[s7; -|virtual void FrameRemove() `{&]
[s7; -|-|x.Remove();&]
[s7; -|`}&]
[s7; -|&]
[s7; -|Frame2() `{&]
[s7; -|-|x.LeftPos(0, 16).TopPos(0, 16).NoWantFocus();&]
[s7; -|`}&]
[s7; `};&]
[s7; &]
[s7; &]
[s7; .......&]
[s7; &]
[s7; &]
[s7; -|Frame2 frame2;&]
[s7; -|ctrl.AddFrame(frame2);&]
[s7; -|win.Run();&]
[s7; &]
[s0; &]
[s0;= 
@@image:662&825
(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)
&]
[s0; &]
[s0; Многие контролы Ctrl могут работать 
как обычные контролы области обзора, 
так и как рамки: это достигается их 
произведением как от Ctrl, так и от CtrlFrame. 
Примерами являются ScrollBar или HeaderCtrl.&]
[s0; &]
[s0; Можете также использовать вспомогательные 
шаблоны [*^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameLeft`:`:class^ Fram
eLeft], [*^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameRight`:`:class^ FrameRight
], [*^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameTop`:`:class^ FrameTop] 
и [*^topic`:`/`/CtrlCore`/src`/Frame`_ru`-ru`#FrameBottom`:`:class^ FrameBottom], 
чтобы добавить функциональности 
фрейма любому существующему контролу 
Ctrl.&]
[s0;3 ]]