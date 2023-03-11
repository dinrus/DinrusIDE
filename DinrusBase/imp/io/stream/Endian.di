﻿/*******************************************************************************

        Потоки для обращения эндианного порядка. Этот поток понимается как
        набор элементов одинакового размера. Частичные элементы не меняются.

*******************************************************************************/

module io.stream.Endian;

private import stdrus :ПерестановкаБайт;
private import io.device.Conduit;
private import io.stream.Buffered;
private import io.model;

/*******************************************************************************

        Тип T - это тип элемента.

*******************************************************************************/

class ЭндианВвод(T) : ФильтрВвода, ФильтрВвода.Переключатель
{       
        static if ((T.sizeof != 2) && (T.sizeof != 4) && (T.sizeof != 8)) 
                    pragma (msg, "ЭндианВвод :: нужен тип длиной 2, 4 или 8 байтов");

        /***********************************************************************

        ***********************************************************************/

        this (ИПотокВвода поток)
        {
                super (Бввод.создай (поток));
        }
        
        /***********************************************************************

                Читает из провода, преобразуя в целевой массив. Предоставленный 
				приёмник наполняется контентом из провода. 

                Возвращает число считанных байтов, которое может быть меньше
                запрощенного в приёмн (или ВВПоток.Кф в конце потока).
                Заключительный частичный элемент помещается в приёмн,
                но возвращаемая длина эффективно его игнорирует.

        ***********************************************************************/

        final override т_мера читай (проц[] приёмн)
        {
                auto длин = исток.читай (приёмн[0 .. приёмн.length & ~(T.sizeof-1)]);
                if (длин != Кф)
                   {
                   // этот финализованный читай может быть неправильно разложен ...
                   длин &= ~(T.sizeof - 1);

                   static if (T.sizeof == 2)
                              ПерестановкаБайт.своп16 (приёмн.ptr, длин);

                   static if (T.sizeof == 4)
                              ПерестановкаБайт.своп32 (приёмн.ptr, длин);

                   static if (T.sizeof == 8)
                              ПерестановкаБайт.своп64 (приёмн.ptr, длин);
                   }
                return длин;
        }
}



/*******************************************************************************
        
         Тип T - это тип элемента.

*******************************************************************************/

class ЭндианВывод (T) : ФильтрВывода, ФильтрВывода.Переключатель
{       
        static if ((T.sizeof != 2) && (T.sizeof != 4) && (T.sizeof != 8)) 
                    pragma (msg, "ЭндианВывод :: нужен тип длиной 2, 4 или 8 байтов");

        private БуферВывода вывод;


        БуферВывода дайВывод()
		{
            return this.вывод;
		}

        /***********************************************************************

        ***********************************************************************/

        this (ИПотокВывода поток)
        {
                super (вывод = Бвыв.создай (поток));
        }

        /***********************************************************************
        
                Пишет в поток вывода из массива-источника. Предоставленный контент
                ист будет потреблён и оставлен нетронутым.

                Возвращает число байтов, записанных из ист, которое может быть
                меньше предоставленного количества. Всякме неполные элементы 
                потреблены не будут.

        ***********************************************************************/

        final override т_мера пиши (проц[] ист)
        {
                т_мера писатель (проц[] приёмн)
                {
                        auto длин = приёмн.length;
                        if (длин > ист.length)
                            длин = ист.length;

                        длин &= ~(T.sizeof - 1);
                        приёмн [0..длин] = ист [0..длин];

                        static if (T.sizeof == 2)
                                   ПерестановкаБайт.своп16 (приёмн.ptr, длин);

                        static if (T.sizeof == 4)
                                   ПерестановкаБайт.своп32 (приёмн.ptr, длин);

                        static if (T.sizeof == 8)
                                   ПерестановкаБайт.своп64 (приёмн.ptr, длин);

                        return длин;
                }

                return вывод.писатель (&писатель);
        }
}


/*******************************************************************************
        
*******************************************************************************/
        
debug (UnitTest)
{
        import io.device.Array;
        
        unittest
        {
                auto inp = new ЭндианВвод!(дим)(new Массив("hello world"d));
                auto oot = new ЭндианВывод!(дим)(new Массив(64));
                oot.копируй (inp);
                assert (oot.вывод.срез == "hello world"d);
        }
}