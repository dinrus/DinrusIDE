﻿/*******************************************************************************

        Простая сериализация для пар имя/значение на основе текста.

*******************************************************************************/

module io.stream.Map;

private import io.stream.Lines,
               io.stream.Buffered;

private import Текст = text.Util;

private import io.device.Conduit;

/*******************************************************************************

        Предоставляет загрузочные средства для потока свойств. То есть, файл
        или другой носитель, содержащий строки текста с выкладкой имя=значение.

*******************************************************************************/

class КартВвод(T) : Строки!(T)
{
        /***********************************************************************

                Распространить конструктор на суперкласс.

        ***********************************************************************/

        this (ИПотокВвода поток)
        {
                super (поток);
        }

        /***********************************************************************

                Загружает свойства из предоставленного потока, посредством foreach.

                Для перемещения по строкам текста используется обходчик,
			    из каждой строки извлекается пара lValue и rValue (левое и правое
				значение), ожидаемый формат файла следующий:

                <pre>
                x = y
                abc = 123
                x.y.z = это единичное свойство

                # это строка комментария
                </pre>

                Заметьте, что в действительности предоставленные имя и значение
				являются срезами и их нужно скопировать, если есть необходимость
				"придержать" их (используя имя.dup и значение.dup).

        ***********************************************************************/

        final цел opApply (цел delegate(ref T[] имя, ref T[] значение) дг)
        {
                цел возвр;

                foreach (строка; super)
                        {
                        auto текст = Текст.убери (строка);

                        // для комментария нужен '#' как первый непробельный сим
                        if (текст.length && (текст[0] != '#'))
                           {
                           // найдём '=' сим
                           auto i = Текст.местоположение (текст, cast(T) '=');

                           // игнорируем, если не найден ...
                           if (i < текст.length)
                              {
                              auto имя = Текст.убери (текст[0 .. i]);
                              auto значение = Текст.убери (текст[i+1 .. $]);
                              if ((возвр = дг (имя, значение)) != 0)
                                   break;
                              }
                           }
                        }
                return возвр;
        }

        /***********************************************************************

                Загружает поток ввода, преобразуя его в AA.

        ***********************************************************************/

        final КартВвод загрузи (ref T[][T[]] свойства)
        {
                foreach (имя, значение; this)
                         свойства[имя.dup] = значение.dup;  
                return this;
        }
}


/*******************************************************************************

        Предоставляет средства записи для потока свойств. То есть, файл
        или другой носитель, который может содержать строки текста
		с выкладкой  имя=значение. 
        
*******************************************************************************/

class КартВывод(T) : ФильтрВывода
{
        private T[] кс;

        private const T[] префикс = "# ";
        private const T[] равно = " = ";
        version (Win32)
                 private const T[] NL = "\r\n";
        version (Posix)
                 private const T[] NL = "\n";

        /***********************************************************************

                Распространить конструктор на суперкласс.

        ***********************************************************************/

        this (ИПотокВывода поток, T[] нс = NL)
        {
                super (Бвыв.создай (поток));
                кс = нс;
        }

        /***********************************************************************

                Приставить нс к предоставленному потоку.

        ***********************************************************************/

        final КартВывод нс ()
        {
                сток.пиши (кс);
                return this;
        }

        /***********************************************************************

                Приставить комментарий к предоставленному потоку.

        ***********************************************************************/

        final КартВывод коммент (T[] текст)
        {
                сток.пиши (префикс);
                сток.пиши (текст);
                сток.пиши (кс);
                return this;
        }

        /***********************************************************************

                Приставить имя & значение к предоставленному потоку.

        ***********************************************************************/

        final КартВывод добавь (T[] имя, T[] значение)
        {
                сток.пиши (имя);
                сток.пиши (равно);
                сток.пиши (значение);
                сток.пиши (кс);
                return this;
        }

        /***********************************************************************

                Приставить AA свойств к предоставленному потоку.

        ***********************************************************************/

        final КартВывод добавь (T[][T[]] свойства)
        {
                foreach (ключ, значение; свойства)
                         добавь (ключ, значение);
                return this;
        }
}



/*******************************************************************************
        
*******************************************************************************/
        
debug (UnitTest)
{
        import io.Stdout;
        import io.device.Array;
        
        unittest
        {
                auto буф = new Массив(200);
                auto ввод = new КартВвод!(сим)(буф);
                auto вывод = new КартВывод!(сим)(буф);

                ткст[ткст] карта;
                карта["foo"] = "bar";
                карта["foo2"] = "bar2";
                вывод.добавь(карта).слей;

                карта = карта.init;
                ввод.загрузи (карта);
                assert (карта["foo"] == "bar");
                assert (карта["foo2"] == "bar2");
        }
}