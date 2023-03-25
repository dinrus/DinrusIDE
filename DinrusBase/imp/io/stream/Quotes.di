﻿module io.stream.Quotes;

private import io.stream.Iterator;

/*******************************************************************************

        Итерирует по набору разграниченных, необязательно взятых в кавычки,
		текстовых полей.

        Каждое поле представлено клиенту как срез исходного контента,
        где срез транзитивен. Если требуется придержать выставленный
        контент, следует произвести с ним соответствующий .dup.

        Контент, выставленный через обходчик, предполагается имеющим всецело
		доступ только для чтения. Все текущие обходчики придерживаются этого
		правила, но пользователь может менять контент через срез получи().
		Чтобы усилить желаемый аспект только чтения, в этот код пришлось быть
		ввести повторное копирование, либо же компилятор должен поддерживать
		массивы только для чтения (сейчас это уже есть в D2).

        Применение:
        ---
        auto f = new Файл ("my.csv");
        auto l = new Строки (f);
        auto b = new Массив (0);
        auto q = new Кавычки!(сим)(",", b);

        foreach (строка; l)
                {
                b.присвой (строка);
                foreach (поле, индекс; q)
                         Стдвыв (индекс, поле);
                Стдвыв.нс;
                }
        ---

        Смотрите Обходчик, Строки, Образцы, Разграничители.

*******************************************************************************/

class Кавычки(T) : Обходчик!(T)
{
    private T[] разделитель;

    /***********************************************************************

            Разбивается только по разграничителям. Если есть кавычки,
            разбиение по разграничителям не производится до завершения кавычек.

    ***********************************************************************/

    this (T[] разделитель, ИПотокВвода поток = пусто)
    {
        super.установи(поток);
        this.разделитель = разделитель;
    }

    /***********************************************************************

            Разбивается только по разграничителям. Если есть кавычки,
            разбиение по разграничителям не производится до завершения кавычек.

    ***********************************************************************/

    protected т_мера скан (проц[] данные)
    {
        T    quote = 0;
        цел  искейп = 0;
        auto контент = (cast(T*) данные.ptr) [0 .. данные.length / T.sizeof];

        foreach (i, c; контент)
        // within a quote block?
        if (quote)
        {
            if (c is '\\')
                ++искейп;
            else
            {
                // matched the начальное quote сим?
                if (c is quote && искейп % 2 is 0)
                    quote = 0;
                искейп = 0;
            }
        }
        else
            // начало a quote block?
            if (c is '"' || c is '\'')
                quote = c;
            else if (есть (разделитель, c))
                return найдено (установи (контент.ptr, 0, i));
        return неНайдено;
    }
}


/*******************************************************************************

*******************************************************************************/

debug (UnitTest)
{
    private import io.Stdout, io.stream.Quotes;
    private import text.Util;
    private import io.device.Array;

    unittest
    {
        ткст[] ожидалось =
        [
            `0`
            ,``
            ,``
            ,`"3"`
            ,`""`
            ,`5`
            ,`",6"`
            ,`"7,"`
            ,`8`
            ,`"9,\\\","`
            ,`10`
            ,`',11",'`
            ,`"12"`
        ];

        auto b = new Массив (ожидалось.объедини (","));
        foreach (i, f; new Кавычки!(сим)(",", b))
        if (i >= ожидалось.length)
            Стдвыв.фмтнс ("ах-ха-ха: неожижанное совпадение: {}, {}", i, f);
        else if (f != ожидалось[i])
            Стдвыв.фмтнс ("ах-ха-ха: плохое совпадение: {}, {}, {}", i, f, ожидалось[i]);
    }
}

