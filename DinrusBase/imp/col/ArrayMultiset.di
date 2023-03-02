﻿module col.ArrayMultiset;

public import col.model.Multiset;

private import col.Link;
private import col.DefaultAllocator;

/+ ИНТЕРФЕЙС:

class МассивМультинабор(З, alias Разместитель=ДефолтныйРазместитель) : Мультинабор!(З)
{
    alias МассивМультинабор!(З, Разместитель) ТипМассивМультинабор;

    struct курсор
    {
        З значение();
        З значение(З з);
        курсор opPostInc();
        курсор opPostDec();
        курсор opAddAssign(цел прир);
        курсор opSubAssign(цел прир);
        бул opEquals(курсор обх);
    }

    final цел очистить(цел delegate(ref бул чистить_ли, ref З з) дг);
    цел opApply(цел delegate(ref З з) дг);
    this(бцел gs = 31);
    ТипМассивМультинабор зачисть();
    бцел длина();
    курсор начало();
    курсор конец();
    курсор удали(курсор обх);
    курсор найди(З з);
    бул содержит(З з);
    ТипМассивМультинабор удали(З з);
    ТипМассивМультинабор удали(З з, ref бул былУдалён);
    ТипМассивМультинабор добавь(З з);
    ТипМассивМультинабор добавь(З з, ref бул былДобавлен);
    ТипМассивМультинабор добавь(Обходчик!(З) обх);
    ТипМассивМультинабор добавь(Обходчик!(З) обх, ref бцел члоДобавленных);
    ТипМассивМультинабор добавь(З[] массив);
    ТипМассивМультинабор добавь(З[] массив, ref бцел члоДобавленных);
    бцел счёт(З з);
    ТипМассивМультинабор удалиВсе(З з);
    ТипМассивМультинабор удалиВсе(З з, ref бцел члоУдалённых);
    ТипМассивМультинабор dup();
    З дай();
    З изыми();
}
+///===============================================================================

/**
 * Этот класс реализует интерфейс мультинабор, содержа линкованный список
 * массивов для сохранения элементов. Так как набору не нужно поддерживать
 * определённый порядок, удаление и добавление является операцией O(1).
 *
 * Удаление элемента приводит к повреждению всех курсоров.
 *
 * Добавление элемента курсоров не повреждает.
 */
class МассивМультинабор(З, alias Разместитель=ДефолтныйРазместитель) : Мультинабор!(З)
{
    private alias Связка!(З[]).Узел узел;
    private alias Разместитель!(Связка!(З[])) разместитель;
    private разместитель разм;
    private узел _голова;
    private бцел _счёт;

    private бцел _размерРоста;

    private узел размести()
    {
        return разм.размести;
    }

    private узел размести(З[] з)
    {
        auto n = размести;
        n.значение = з;
        return n;
    }

    alias МассивМультинабор!(З, Разместитель) ТипМассивМультинабор;

    /**
     * Курсор подобен указателю в коллекции МассивМультинабор.
     */
    struct курсор
    {
        private узел укз; alias укз ptr;
        private бцел инд;

        /**
         * Возвращает значение, на которое указывает курсор.
         */
        З значение()
        {
            return укз.значение[инд];
        }

        /**
         * Устанавливает значение, на которое указывает курсор.
         */
        З значение(З з)
        {
            return (укз.значение[инд] = з);
        }

        /**
         * Увеличивает на один курсор, возвращает курсор, каким он был перед
         * увеличением.
         */
        курсор opPostInc()
        {
            курсор врм = *this;
            инд++;
            if(инд >= укз.значение.length)
            {
                инд = 0;
                укз = укз.следщ;
            }
            return врм;
        }

        /**
         * Уменьшает на один курсор, возвращает курсор, каким он был перед
         * уменьшением.
         */
        курсор opPostDec()
        {
            курсор врм = *this;
            if(инд == 0)
            {
                укз = укз.предш;
                инд = укз.значение.length - 1;
            }
            else
                инд--;
            return врм;
        }

        /**
         * Добавляет заданное значение к курсору.
         *
         * Выполняется за время O(n), но константа равна < 1.
         */
        курсор opAddAssign(цел прир)
        {
            if(прир < 0)
                return opSubAssign(-прир);
            while(прир >= укз.значение.length - инд)
            {
                прир -= (укз.значение.length - инд);
                укз = укз.следщ;
                инд = 0;
            }
            инд += прир;
            return *this;
        }

        /**
         * Отнимает заданное значение от курсора.
         *
         * Выполняется за время O(n), но константа равна < 1.
         */
        курсор opSubAssign(цел прир)
        {
            if(прир < 0)
                return opAddAssign(-прир);
            while(прир > инд)
            {
                прир -= инд;
                укз = укз.предш;
                инд = укз.значение.length;
            }
            инд -= прир;
            return *this;
        }

        /**
         * Сравнивает два курсора на равенство.
         */
        бул opEquals(курсор обх)
        {
            return обх.ptr is укз && обх.инд is инд;
        }
    }

    /**
     * Итерирует по элементам в этом МассивМультинаборе, определяя, какие
     * элементы следует удалить.
     *
     * Используется так:
     * ----------
     * // удалить все нечётные элементы
     * foreach(ref чистить_ли, элт; &МассивМультинабор.очистить)
     * {
     *    чистить_ли = ((элт & 1) == 1)
     * }
     */
    final цел очистить(цел delegate(ref бул чистить_ли, ref З з) дг)
    {
        return _примени(дг);
    }

    private цел _примени(цел delegate(ref бул чистить_ли, ref З з) дг)
    {
        курсор обх = начало;
        бул чистить_ли;
        цел возврдг = 0;
        курсор _конец = конец; // cache конец so обх isn'т всегда being generated
        while(!возврдг && обх != _конец)
        {
            чистить_ли = нет;
            if((возврдг = дг(чистить_ли, обх.ptr.значение[обх.инд])) != 0)
                break;
            if(чистить_ли)
                обх = удали(обх);
            else
                обх++;
        }
        return возврдг;
    }

    /**
     * Итерирует по этой коллекции.
     */
    цел opApply(цел delegate(ref З з) дг)
    {
        цел _дг(ref бул чистить_ли, ref З з)
        {
            return дг(з);
        }
        return _примени(&_дг);
    }

    /**
     * Создаёт МассивМультинабор с указанным размером роста. Размер роста
     * используется для размещения новых массивов при добавлении в линкованный список.
     */
    this(бцел gs = 31)
    {
        _размерРоста = gs;
        _голова = разм.размести();
        узел.крепи(_голова, _голова);
        _счёт = 0;
    }

    /**
     * Очистить коллекцию от всех значений.
     */
    ТипМассивМультинабор зачисть()
    {
        static if(разместитель.нужноСвоб)
        {
            разм.освободиВсе();
            _голова = размести;
        }
        узел.крепи(_голова, _голова);
        return this;
    }

    /**
     * Возвращает число элементов в коллекции.
     */
    бцел длина()
    {
        return _счёт;
    }
	alias длина length;

    /**
     * Возвращает курсор, указывающий на первый элемент этой коллекции.
     */
    курсор начало()
    {
        курсор обх;
        обх.ptr = _голова.следщ;
        обх.инд = 0;
        return обх;
    }

    /**
     * Возвращает курсор, указывающий за последний элемент
     * коллекции.
     */
    курсор конец()
    {
        курсор обх;
        обх.ptr = _голова;
        обх.инд = 0;
        return обх;
    }

    /**
     * Удаляет элемент, перед которым показывает курсор. Возвращает валидный
     * курсор, указывающий на другой элемент или конец, если удалённый элемент
     * был последним.
     *
     * Выполняется за время O(1).
     */
    курсор удали(курсор обх)
    {
        узел последн = _голова.предш;
        if(обх.ptr is последн && обх.инд is последн.значение.length - 1)
        {
            обх = конец;
        }
        else
        {
            обх.значение = последн.значение[$-1];
        }
        последн.значение.length = последн.значение.length - 1;
        if(последн.значение.length == 0)
        {
            последн.открепи;
            static if(разместитель.нужноСвоб)
                разм.освободи(последн);
        }
        _счёт--;
        return обх;
    }

    /**
     * Возвращает курсор, указывающий на первый случай з.
     *
     * Выполняется за время O(n).
     */
    курсор найди(З з)
    {
        курсор обх = начало;
        курсор _конец = конец;
        while(обх != _конец && обх.значение != з)
            обх++;
        return обх;
    }

    /**
     * Возвращает да, если з является элементом в этом наборе.
     *
     * Выполняется за время O(n).
     */
    бул содержит(З з)
    {
        return найди(з) != конец;
    }

    /**
     * Удаляет заданный элемент из этого набора. Удаляется только первый
     * случай.
     *
     * Возвращает да, если элемент был найден и удалён.
     *
     * Выполняется за время O(n).
     */
    ТипМассивМультинабор удали(З з)
    {
        бул пропущен;
        return удали(з, пропущен);
    }

    /**
     * Удаляет заданный элемент из этого набора. Удаляется только первый
     * случай.
     *
     * Возвращает да, если элемент был найден и удалён.
     *
     * Выполняется за время O(n).
     */
    ТипМассивМультинабор удали(З з, ref бул былУдалён)
    {
        курсор обх = найди(з);
        if(обх == конец)
            былУдалён = нет;
        else
        {
            удали(обх);
            былУдалён = да;
        }
        return this;
    }

    /**
     * Добавляет данный элемент в этот набор.
     *
     * Возвращает да.
     *
     * Выполняется за время O(1).
     */
    ТипМассивМультинабор добавь(З з)
    {
        бул пропущен;
        return добавь(з, пропущен);
    }
    /**
     * Добавляет данный элемент в этот набор.
     *
     * Возвращает да.
     *
     * Выполняется за время O(1).
     */
    ТипМассивМультинабор добавь(З з, ref бул былДобавлен)
    {
        узел последн = _голова.предш;
        if(последн is _голова || последн.значение.length == _размерРоста)
        {
            //
            // pre-размести массив длина, then Устанавливает длина to 0
            //
            auto массив = new З[_размерРоста];
            массив.length = 0;
            _голова.надставь(размести(массив));
            последн = _голова.предш;
        }

        последн.значение ~= з;
        былДобавлен = да;
        _счёт++;
        return this;
    }

    //
    // these can probably be optimized more
    //

    /**
     * добавляет все значения из заданного итератора в этот набор.
     *
     * Возвращает число добавленных элементов.
     */
    ТипМассивМультинабор добавь(Обходчик!(З) обх)
    {
        бцел пропущен;
        return добавь(обх, пропущен);
    }

    /**
     * Добавляет все значения из заданного итератора в набор.
     *
     * Возвращает число добавленных элементов.
     */
    ТипМассивМультинабор добавь(Обходчик!(З) обх, ref бцел члоДобавленных)
    {
        бцел исхдлина = длина;
        foreach(з; обх)
            добавь(з);
        члоДобавленных = длина - исхдлина;
        return this;
    }

    /**
     * Добавляет все значения из заданного массива в набор.
     *
     * Возвращает число добавленных элементов.
     */
    ТипМассивМультинабор добавь(З[] массив)
    {
        бцел пропущен;
        return добавь(массив, пропущен);
    }

    /**
     * Добавляет все значения из заданного массива в набор.
     *
     * Возвращает число добавленных элементов.
     */
    ТипМассивМультинабор добавь(З[] массив, ref бцел члоДобавленных)
    {
        бцел исхдлина = длина;
        foreach(з; массив)
            добавь(з);
        члоДобавленных = длина - исхдлина;
        return this;
    }

    /**
     * Вычисляет число случаев з.
     *
     * Выполняется за время O(n).
     */
    бцел счёт(З з)
    {
        бцел возврзнач = 0;
        foreach(x; this)
            if(з == x)
                возврзнач++;
        return возврзнач;
    }

    /**
     * Удаляет все случаи з.  Возвращает число удалённых экземпляров.
     *
     * Выполняется за время O(n).
     */
    ТипМассивМультинабор удалиВсе(З з)
    {
        бцел пропущен;
        return удалиВсе(з, пропущен);
    }
    /**
     * Удаляет все случаи з.  Возвращает число удалённых экземпляров.
     *
     * Выполняется за время O(n).
     */
    ТипМассивМультинабор удалиВсе(З з, ref бцел члоУдалённых)
    {
        бцел исхдлина = длина;
        foreach(ref dp, x; &очистить)
        {
            dp = cast(бул)(з == x);
        }
        члоУдалённых = исхдлина - длина;
        return this;
    }

    /**
     * Дублирует этот контейнер. Глубокая копия элементов ('deep' copy)
     * не выполняется.
     */
    ТипМассивМультинабор dup()
    {
        auto возврзнач = new ТипМассивМультинабор(_размерРоста);
        узел n = _голова.следщ;
        while(n !is _голова)
        {
            узел x;
            if(n.значение.length == _размерРоста)
                x = возврзнач.размести(n.значение.dup);
            else
            {
                auto массив = new З[_размерРоста];
                массив.length = n.значение.length;
                массив[0..$] = n.значение[];
                x = возврзнач.размести(массив);
            }
            возврзнач._голова.надставь(x);
        }
        возврзнач._счёт = _счёт;
        return возврзнач;
    }

    /**
     * Даёт наиболее удобный элемент в этом наборе. Этот элемент обычно
     * итерируется первым.  Следовательно, вызов удали(дай())
     * гарантированно будет менее, чем операция O(n).
     */
    З дай()
    {
        return начало.значение;
    }

    /**
     * Удаляет наиболее удобный элемент из набора и возвращает его значение.
     * Это равнозначно удали(дай()), за исключением того, что выполняется
     * один этап поиска.
     */
    З изыми()
    {
        auto c = начало;
        auto возврзнач = c.значение;
        удали(c);
        return возврзнач;
    }
}

version(UnitTest)
{
    void main()
    {
        auto ms = new МассивМультинабор!(бцел);
        ms.добавь([0U, 1, 2, 3, 4, 5]);
        assert(ms.length == 6);
        ms.удали(1);
        assert(ms.length == 5);
        assert(ms._голова.следщ.значение == [0U, 5, 2, 3, 4]);
        foreach(ref бул очистить_ли, бцел з; &ms.очистить)
            очистить_ли = (з % 2 == 1);
        assert(ms.length == 3);
        assert(ms._голова.следщ.значение == [0U, 4, 2]);
    }
}
