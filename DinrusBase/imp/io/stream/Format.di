﻿
module io.stream.Format;

private import io.device.Conduit;

private import text.convert.Layout;

/*******************************************************************************

        Мост между экземпляром Выкладка и потоком. Используется для глобальных
        Стдвыв & Стдош, но может использоваться при желании для общецелевого
        буферного форматирования. Шаблонный тип 'T' диктует размещение
        текста внутри целевого буфера ~ либо сим, либо шим или
        дим (utf8, utf16, либо utf32).

        ФормВывод используется в следующем стиле:
        ---
        auto выведи = new ФормВывод!(сим) (...);

        выведи ("hello");                        => hello
        выведи (1);                              => 1
        выведи (3.14);                           => 3.14
        выведи ('b');                            => b
        выведи (1, 2, 3);                        => 1, 2, 3
        выведи ("abc", 1, 2, 3);                 => abc, 1, 2, 3
        выведи ("abc", 1, 2) ("foo");            => abc, 1, 2foo
        выведи ("abc") ("def") (3.14);           => abcdef3.14

        выведи.фмт ("abc {}", 1);             => abc 1
        выведи.фмт ("abc {}:{}", 1, 2);       => abc 1:2
        выведи.фмт ("abc {1}:{0}", 1, 2);     => abc 2:1
        выведи.фмт ("abc ", 1);               => abc
        ---

        Заметьте, что последний пример не выводит исключений. Есть
        несколько случаев использования, в которых можно опустить аргумент,
        в таком случае мы не активируем никаких механизмов захвата.

        Слив вывода достигается методом слей(), либо посредством
        пустой пары скобок :
         ---
        выведи ("hello world") ();
        выведи ("hello world").слей;

        выведи.фмт ("hello {}", "world") ();
        выведи.фмт ("hello {}", "world").слей;
        ---

        Особые символьные последовательности, такие как "\n", записываются прямо в
        вывод без како-либо трансляции (хотя можно ввести фильтр вывода
        для выполнения этого, при необходимости).Платформо-специфичные
        переходы на строку генерируются методом нс(), который также
        слейает этот вывод, если сконфигурирован на это:
        ---
        выведи ("hello ") ("world").нс;
        выведи.фмт ("hello {}", "world").нс;
        выведи.фмтнс ("hello {}", "world");
        ---

        Метод фмт() поддерживает диапазон опций форматирования,
        выставленных в модуле text.convert.Layout и его расширениях;
        включая полный набор расширений I18N, если он на это сконфигурирован.
        Чтобы создать французский экземпляр ФормВывода, нужно:
        ---
        import text.locale.Locale;

        auto locale = new Локаль (Культура.дайКультуру ("fr-FR"));
        auto выведи = new ФормВывод!(сим) (locale, ...);
        ---

        Заметьте, что ФормВывод *не* обладает потоко-безопасностью.

*******************************************************************************/

class ФормВывод(T) : ФильтрВывода
{
    public  alias ФильтрВывода.слей слей;
	public  alias ФильтрВывода.вывод сток;

    private T[]             кс;
    private Выкладка!(T)      преобразуй;
    private бул            слитьСтроки;

    public alias выведи      opCall;         /// opCall -> выведи
    //  public alias нс    нс;             /// нс -> нс

    version (Win32)
    private const T[] Кс = "\r\n";
    else
        private const T[] Кс = "\n";

    /**********************************************************************

            Конструирует экземпляр ФормВывода, привязывая предоставленный поток
            к форматировщику вывода.
    **********************************************************************/

    this (ИПотокВывода вывод, T[] кс = Кс)
    {
        this (Выкладка!(T).экземпляр, вывод, кс);
    }

    /**********************************************************************

            Конструирует экземпляр ФормВывода, привязывая предоставленный поток
            к форматировщику вывода.

    **********************************************************************/

    this (Выкладка!(T) преобр, ИПотокВывода выв, T[] кс = Кс)
    {
        assert (преобр);
        assert (выв);

        this.преобразуй = преобр;
        this.кс = кс;
        super (выв);
    }

    /**********************************************************************

            Выкладка, использующая предоставленную спецификацию форматирования.

    **********************************************************************/

    final ФормВывод фмт (T[] фмт, ...)
    {
        преобразуй (&излей, _arguments, _argptr, фмт);
        return this;
    }

    /**********************************************************************

            Выкладка, использующая предоставленную спецификацию форматирования.

    **********************************************************************/

    final ФормВывод фмтнс (T[] фмт, ...)
    {
        преобразуй (&излей, _arguments, _argptr, фмт);
        return нс;
    }

    /**********************************************************************

            Неформатированная выкладка, с запятыми между аргументами.
            На данный момент поддерживает максимум 24 аргумента.

    **********************************************************************/

    final ФормВывод выведи (...)
    {
        static  T[] срез =  "{}, {}, {}, {}, {}, {}, {}, {}, "
                                "{}, {}, {}, {}, {}, {}, {}, {}, "
                                "{}, {}, {}, {}, {}, {}, {}, {}, ";

        assert (_arguments.length <= срез.length/4, "ФормВывод :: слишком много аргументов");

        if (_arguments.length is 0)
            сток.слей;
        else
            преобразуй (&излей, _arguments, _argptr, срез[0 .. _arguments.length * 4 - 2]);

        return this;
    }

    /***********************************************************************

            Выводит начало строки и делает дополнительный слей.

    ***********************************************************************/

    final ФормВывод нс ()
    {
        сток.пиши (кс);
        if (слитьСтроки)
            сток.слей;
        return this;
    }

    /**********************************************************************

            Контролирует неявный слей нс(),где да активирует
            слей. Явный слей() всегда производит слей вывода.

    **********************************************************************/

    final ФормВывод слей (бул данет)
    {
        слитьСтроки = данет;
        return this;
    }

    /**********************************************************************

            Возвращает ассоциированный поток вывода.

    **********************************************************************/

    final ИПотокВывода поток ()
    {
        return сток;
    }

    /**********************************************************************

            Устанавливает ассоциированный поток вывода.

    **********************************************************************/

    final ФормВывод поток (ИПотокВывода вывод)
    {
        сток =  вывод;
        return this;
    }

    /**********************************************************************

            Возвращает ассоциированную выкладку.

    **********************************************************************/

    final Выкладка!(T) выкладка ()
    {
        return преобразуй;
    }

    /**********************************************************************

            Установить ассоциированную выкладку.

    **********************************************************************/

    final ФормВывод выкладка (Выкладка!(T) выкладка)
    {
        преобразуй = выкладка;
        return this;
    }

    /**********************************************************************

            Сток для передачи в форматировщик.

    **********************************************************************/

    private final бцел излей (T[] s)
    {
        auto счёт = сток.пиши (s);
        if (счёт is Кф)
            провод.ошибка ("ФормВывод :: неожиданный Кф");
        return счёт;
    }
}


/*******************************************************************************

*******************************************************************************/

debug (Формат)
{
    import io.device.Array, io.stream.Format;

    проц main()
    {
        auto выведи = new ФормВывод!(сим) (new Массив(1024, 1024));

        for (цел i=0; i < 1000; i++)
            выведи(i).нс;
    }
}
