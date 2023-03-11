﻿
module col.CircularList;

private import col.Clink;
public  import  col.Container;
private import col.model.IContainer;
/*******************************************************************************

        Циркулярно связанный список

        ---
        Обходчик обходчик ()
        цел opApply (цел delegate(ref З значение) дг)

        ЦиркулярныйСписок добавь (З элемент)
        ЦиркулярныйСписок добавьПо (т_мера индекс, З элемент)
        ЦиркулярныйСписок добавь (З элемент)
        ЦиркулярныйСписок приставь (З элемент)
        т_мера добавьПо (т_мера индекс, ИКонтейнер!(З) e)
        т_мера добавь (ИКонтейнер!(З) e)
        т_мера приставь (ИКонтейнер!(З) e)

        бул возьми (ref З знач)
        бул содержит (З элемент)
        З получи (т_мера индекс)
        т_мера первый (З элемент, т_мера стартовыйИндекс = 0)
        т_мера последний (З элемент, т_мера стартовыйИндекс = 0)

        З голова ()
        З хвост ()
        З голова (З элемент)
        З хвост (З элемент)
        З удалиГолову ()
        З удалиХвост ()

        бул удалиПо (т_мера индекс)
        т_мера удали (З элемент, бул все)
        т_мера удалиДиапазон (т_мера отИндекса, т_мера доИндекса)

        т_мера замени (З старЭлемент, З новЭлемент, бул все)
        бул замениПо (т_мера индекс, З элемент)

        т_мера размер ()
        бул пуст_ли ()
        З[] вМассив (З[] приёмн)
        ЦиркулярныйСписок dup ()
        ЦиркулярныйСписок поднабор (т_мера из_, т_мера длина)
        ЦиркулярныйСписок очисть ()
        ЦиркулярныйСписок сбрось ()
        ЦиркулярныйСписок проверь ()
        ---

*******************************************************************************/

class ЦиркулярныйСписок (З, alias Извл = Контейнер.извлеки,
            alias Куча = Контейнер.ДефСбор)
        : ИКонтейнер!(З)
{
    // Это тип используется для конфигурации Разместителя
    public alias ЦСвязка!(З)  Тип;

    private alias Тип      *Реф;

    private alias Куча!(Тип) Размест;

    // число содежащихся элементов
    private т_мера          счёт;

    // конфигурируемый менеджер кучи
    private Размест           куча;

    // тэг изменения, обновляемый при каждом изменении
    private т_мера          изменение;

    // голова этого списка. Пусто, если он пустой
    private Реф             список;


    /***********************************************************************

            Делает пустой список.

    ***********************************************************************/

    this ()
    {
        this (пусто, 0);
    }

    /***********************************************************************

            Делает конфигурируемый список.

    ***********************************************************************/

    protected this (Реф в, т_мера c)
    {
        список = в;
        счёт = c;
    }

    /***********************************************************************

            Очищает при удалении.

    ***********************************************************************/

    ~this ()
    {
        сбрось;
    }

    /***********************************************************************

            Возвращает генерный обходчик для содержимых элементов.

    ***********************************************************************/

    final Обходчик обходчик ()
    {
        // использован в_ be Обходчик i =void, but that doesn't инициализуй
        // fields that are not инициализован here.
        Обходчик i;
        i.хозяин = this;
        i.изменение = изменение;
        i.ячейка = i.голова = список;
        i.счёт = счёт;
        i.индекс = 0;
        return i;
    }

    /***********************************************************************

            Конфигурирует назначенный разместитель размером каждого
            блока размещения (числом узлов, размещённых одновременно)
            и числом узлов, которыми преднаполняется кэш.

            Временная ёмкость: O(n)

    ***********************************************************************/

    final ЦиркулярныйСписок кэш (т_мера чанк, т_мера счёт=0)
    {
        куча.конфиг (чанк, счёт);
        return this;
    }

    /***********************************************************************


    ***********************************************************************/

    final цел opApply (цел delegate(ref З значение) дг)
    {
        return обходчик.opApply (дг);
    }

    /***********************************************************************

            Возвращает число содежащихся элементов.

    ***********************************************************************/

    final т_мера размер ()
    {
        return счёт;
    }

    /***********************************************************************

            Создаёт независимую копию этого списка. Сами элементы
            не клонируются.

    ***********************************************************************/

    final ЦиркулярныйСписок dup ()
    {
        return new ЦиркулярныйСписок!(З, Извл, куча) (список ? список.копируйСписок(&куча.размести) : пусто, счёт);
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final бул содержит (З элемент)
    {
        if (список)
            return список.найди (элемент) !is пусто;
        return нет;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final З голова ()
    {
        return перваяЯчейка.значение;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final З хвост ()
    {
        return последняяЯчейка.значение;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final З получи (т_мера индекс)
    {
        return ячейкаПо(индекс).значение;
    }

    /***********************************************************************

            Временная ёмкость: O(n)
            Возвращает т_мера.max, если элементов не найдено.

    ***********************************************************************/

    final т_мера первый (З элемент, т_мера стартовыйИндекс = 0)
    {
        if (стартовыйИндекс < 0)
            стартовыйИндекс = 0;

        auto p = список;
        if (p is пусто)
            return т_мера.max;

        for (т_мера i = 0; да; ++i)
        {
            if (i >= стартовыйИндекс && элемент == p.значение)
                return i;

            p = p.следщ;
            if (p is список)
                break;
        }
        return т_мера.max;
    }

    /***********************************************************************

            Временная ёмкость: O(n)
            Возвращает т_мера.max, если элементов не найдено.

    ***********************************************************************/

    final т_мера последний (З элемент, т_мера стартовыйИндекс = 0)
    {
        if (счёт is 0)
            return т_мера.max;

        if (стартовыйИндекс >= счёт)
            стартовыйИндекс = счёт - 1;

        if (стартовыйИндекс < 0)
            стартовыйИндекс = 0;

        auto p = ячейкаПо (стартовыйИндекс);
        т_мера i = стартовыйИндекс;
        for (;;)
        {
            if (элемент == p.значение)
                return i;
            else if (p is список)
                break;
            else
            {
                p = p.предш;
                --i;
            }
        }
        return т_мера.max;
    }

    /***********************************************************************

            Временная ёмкость: O(длина)

    ***********************************************************************/

    final ЦиркулярныйСписок поднабор (т_мера из_, т_мера длина)
    {
        Реф новый_список = пусто;

        if (длина > 0)
        {
            проверьИндекс (из_);
            auto p = ячейкаПо (из_);
            auto текущ = новый_список = куча.размести.установи (p.значение);

            for (т_мера i = 1; i < длина; ++i)
            {
                p = p.следщ;
                if (p is пусто)
                    длина = i;
                else
                {
                    текущ.добавьСледщ (p.значение, &куча.размести);
                    текущ = текущ.следщ;
                }
            }
        }

        return new ЦиркулярныйСписок (новый_список, длина);
    }

    /***********************************************************************

             Временная ёмкость: O(1)

    ***********************************************************************/

    final ЦиркулярныйСписок очисть ()
    {
        return очисть (нет);
    }

    /***********************************************************************

            Reset the ХэшКарта contents и optionally конфигурируй a new
            куча manager. This releases ещё память than очисть() does

            Временная ёмкость: O(n)

    ***********************************************************************/

    final ЦиркулярныйСписок сбрось ()
    {
        return очисть (да);
    }

    /***********************************************************************

            Временная ёмкость: O(n)

            Takes the последний элемент on the список

    ***********************************************************************/

    final бул возьми (ref З знач)
    {
        if (счёт)
        {
            знач = хвост;
            удалиХвост ();
            return да;
        }
        return нет;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final ЦиркулярныйСписок приставь (З элемент)
    {
        if (список is пусто)
            список = куча.размести.установи (элемент);
        else
            список = список.добавьПредш (элемент, &куча.размести);
        инкремент;
        return this;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final З голова (З элемент)
    {
        auto p = перваяЯчейка;
        auto знач = p.значение;
        p.значение = элемент;
        измени;
        return знач;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final З удалиГолову ()
    {
        auto p = перваяЯчейка;
        if (p.синглтон)
            список = пусто;
        else
        {
            auto n = p.следщ;
            p.отвяжи;
            список = n;
        }

        auto знач = p.значение;
        декремент (p);
        return знач;
    }

    /***********************************************************************

             Временная ёмкость: O(1)

     ***********************************************************************/

    final ЦиркулярныйСписок добавь (З элемент)
    {
        return добавь (элемент);
    }

    /***********************************************************************

             Временная ёмкость: O(1)

     ***********************************************************************/

    final ЦиркулярныйСписок добавь (З элемент)
    {
        if (список is пусто)
            приставь (элемент);
        else
        {
            список.предш.добавьСледщ (элемент, &куча.размести);
            инкремент;
        }
        return this;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final З хвост (З элемент)
    {
        auto p = последняяЯчейка;
        auto знач = p.значение;
        p.значение = элемент;
        измени;
        return знач;
    }

    /***********************************************************************

            Временная ёмкость: O(1)

    ***********************************************************************/

    final З удалиХвост ()
    {
        auto p = последняяЯчейка;
        if (p is список)
            список = пусто;
        else
            p.отвяжи;

        auto знач = p.значение;
        декремент (p);
        return знач;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final ЦиркулярныйСписок добавьПо (т_мера индекс, З элемент)
    {
        if (индекс is 0)
            приставь (элемент);
        else
        {
            ячейкаПо(индекс - 1).добавьСледщ(элемент, &куча.размести);
            инкремент;
        }
        return this;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final ЦиркулярныйСписок замениПо (т_мера индекс, З элемент)
    {
        ячейкаПо(индекс).значение = элемент;
        измени;
        return this;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final ЦиркулярныйСписок удалиПо (т_мера индекс)
    {
        if (индекс is 0)
            удалиГолову;
        else
        {
            auto p = ячейкаПо(индекс);
            p.отвяжи;
            декремент (p);
        }
        return this;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final т_мера удали (З элемент, бул все)
    {
        auto c = счёт;
        if (список)
        {
            auto p = список;
            for (;;)
            {
                auto n = p.следщ;
                if (элемент == p.значение)
                {
                    p.отвяжи;
                    декремент (p);
                    if (p is список)
                    {
                        if (p is n)
                        {
                            список = пусто;
                            break;
                        }
                        else
                            список = n;
                    }

                    if (! все)
                        break;
                    else
                        p = n;
                }
                else if (n is список)
                    break;
                else
                    p = n;
            }
        }
        return c - счёт;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final т_мера замени (З старЭлемент, З новЭлемент, бул все)
    {
        т_мера c;
        if (список)
        {
            auto p = список;
            do
            {
                if (старЭлемент == p.значение)
                {
                    ++c;
                    измени;
                    p.значение = новЭлемент;
                    if (! все)
                        break;
                }
                p = p.следщ;
            }
            while (p !is список);
        }
        return c;
    }

    /***********************************************************************

            Временная ёмкость: O(число элементов в e)

    ***********************************************************************/

    final т_мера приставь (ИКонтейнер!(З) e)
    {
        Реф hd = пусто;
        Реф текущ = пусто;
        auto c = счёт;

        foreach (элемент; e)
        {
            инкремент;

            if (hd is пусто)
            {
                hd = куча.размести.установи(элемент);
                текущ = hd;
            }
            else
            {
                текущ.добавьСледщ (элемент, &куча.размести);
                текущ = текущ.следщ;
            }
        }

        if (список is пусто)
            список = hd;
        else if (hd)
        {
            auto tl = список.предш;
            текущ.следщ = список;
            список.предш = текущ;
            tl.следщ = hd;
            hd.предш = tl;
            список = hd;
        }
        return счёт - c;
    }

    /***********************************************************************

            Временная ёмкость: O(число элементов в e)

    ***********************************************************************/

    final т_мера добавь (ИКонтейнер!(З) e)
    {
        auto c = счёт;
        if (список is пусто)
            приставь (e);
        else
        {
            auto текущ = список.предш;
            foreach (элемент; e)
            {
                инкремент;
                текущ.добавьСледщ (элемент, &куча.размести);
                текущ = текущ.следщ;
            }
        }
        return счёт - c;
    }

    /***********************************************************************

            Временная ёмкость: O(размер() + число элементов в e)

    ***********************************************************************/

    final т_мера добавьПо (т_мера индекс, ИКонтейнер!(З) e)
    {
        auto c = счёт;
        if (список is пусто || индекс is 0)
            приставь (e);
        else
        {
            auto текущ = ячейкаПо (индекс - 1);
            foreach (элемент; e)
            {
                инкремент;
                текущ.добавьСледщ (элемент, &куча.размести);
                текущ = текущ.следщ;
            }
        }
        return счёт - c;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    final т_мера удалиДиапазон (т_мера отИндекса, т_мера доИндекса)
    {
        auto p = ячейкаПо (отИндекса);
        auto последний = список.предш;
        auto c = счёт;
        for (т_мера i = отИндекса; i <= доИндекса; ++i)
        {
            auto n = p.следщ;
            p.отвяжи;
            декремент (p);
            if (p is список)
            {
                if (p is последний)
                {
                    список = пусто;
                    break;
                }
                else
                    список = n;
            }
            p = n;
        }
        return c - счёт;
    }

    /***********************************************************************

            Копирует и возвращает содержащийся в Массиве набор значений,
            используя опциональный приёмн как реципиент (который меняется в размере
            по необходимости).

            Возвращает срез приёмн, представляющий значения контейнера.

            Временная ёмкость: O(n)

    ***********************************************************************/

    final З[] вМассив (З[] приёмн = пусто)
    {
        if (приёмн.length < счёт)
            приёмн.length = счёт;

        т_мера i = 0;
        foreach (знач; this)
        приёмн[i++] = знач;
        return приёмн [0 .. счёт];
    }

    /***********************************************************************

            Пуст ли этот контейнер?

            Временная ёмкость: O(1)

    ***********************************************************************/

    final бул пуст_ли ()
    {
        return счёт is 0;
    }

    /***********************************************************************


    ***********************************************************************/

    final ЦиркулярныйСписок проверь()
    {
        assert(((счёт is 0) is (список is пусто)));
        assert((список is пусто || список.размер is счёт));

        if (список)
        {
            т_мера c = 0;
            auto p = список;
            do
            {
                assert(p.предш.следщ is p);
                assert(p.следщ.предш is p);
                assert(экземпляры(p.значение) > 0);
                assert(содержит(p.значение));
                p = p.следщ;
                ++c;
            }
            while (p !is список);
            assert(c is размер);
        }
        return this;
    }

    /***********************************************************************

            Временная ёмкость: O(n)

    ***********************************************************************/

    private т_мера экземпляры (З элемент)
    {
        if (список)
            return список.счёт (элемент);
        return 0;
    }

    /***********************************************************************


    ***********************************************************************/

    private проц проверьИндекс (т_мера i)
    {
        if (i >= счёт)
            throw new Исключение ("вне диапазона");
    }

    /***********************************************************************

        Возвращает первую ячейку, либо выводит исключение, если пуста.

    ***********************************************************************/

    private Реф перваяЯчейка ()
    {
        проверьИндекс (0);
        return список;
    }

    /***********************************************************************

        Возвращает последнюю ячейку, либо выводит исключение, если пуста.

    ***********************************************************************/

    private Реф последняяЯчейка ()
    {
        проверьИндекс (0);
        return список.предш;
    }

    /***********************************************************************

    Возвращает индекс'ную ячейку, либо выводит исключение, если неверный индекс.

    ***********************************************************************/

    private Реф ячейкаПо (т_мера индекс)
    {
        проверьИндекс (индекс);
        return список.н_ый (индекс);
    }

    /***********************************************************************

             Временная ёмкость: O(1)

    ***********************************************************************/

    private ЦиркулярныйСписок очисть (бул все)
    {
        измени;

        // собери each узел if we can't собери все at once
        if (куча.собери(все) is нет && счёт)
        {
            auto p = список;
            do
            {
                auto n = p.следщ;
                декремент (p);
                p = n;
            }
            while (p != список);
        }

        список = пусто;
        счёт = 0;
        return this;
    }

    /***********************************************************************

      Добавлен новый элемент.

    ***********************************************************************/

    private проц инкремент ()
    {
        ++изменение;
        ++счёт;
    }

    /***********************************************************************

       Удалён элемент.

    ***********************************************************************/

    private проц декремент (Реф p)
    {
        Извл (p.значение);
        куча.собери (p);
        ++изменение;
        --счёт;
    }

    /***********************************************************************

       Набор изменён.

    ***********************************************************************/

    private проц измени ()
    {
        ++изменение;
    }

    /***********************************************************************

            Обходчик без фильтрации.

    ***********************************************************************/

    private struct Обходчик
    {
        бул              рев;
        Реф               ячейка,
        голова,
        приор;
        ЦиркулярныйСписок      хозяин;
        т_мера            индекс,
        счёт;
        т_мера            изменение;

        /***************************************************************

            Не изменён ли контейнер без нашего ведома?

        ***************************************************************/

        бул действителен ()
        {
            return хозяин.изменение is изменение;
        }

        /***************************************************************

            Получает доступ к следующему значению, и возвращает нет, когда
            больше не осталось значений для обхода.

        ***************************************************************/

        бул следщ (ref З знач)
        {
            auto n = следщ;
            return (n) ? знач = *n, да : нет;
        }

        /***************************************************************

             Возвращает указатель на следующее значение, либо пусто, когда
             больше не осталось значений для обхода.

        ***************************************************************/

        З* следщ ()
        {
            З* r;

            if (индекс < счёт)
            {
                ++индекс;
                приор = ячейка;
                r = &ячейка.значение;
                ячейка = (рев ? ячейка.предш : ячейка.следщ);
            }
            else
                ячейка = пусто;
            return r;
        }

        /***************************************************************

                Поддержка Foreach.

        ***************************************************************/

        цел opApply (цел delegate(ref З значение) дг)
        {
            цел результат;
            auto c = ячейка;

            while (индекс < счёт)
            {
                ++индекс;
                приор = c;
                c = (рев ? c.предш : c.следщ);
                if ((результат = дг(приор.значение)) != 0)
                    break;
            }
            ячейка = пусто;
            return результат;
        }

        /***************************************************************

            Удаляет значение, которое только-что итерировано.

        ***************************************************************/

        бул удали ()
        {
            if (приор)
            {
                auto следщ = (рев ? приор.предш : приор.следщ);
                if (приор is голова)
                {
                    if (приор is следщ)
                        хозяин.список = пусто;
                    else
                        голова = хозяин.список = следщ;
                }

                приор.отвяжи;
                хозяин.декремент (приор);
                приор = пусто;

                --счёт;
                // ignore this change
                ++изменение;
                return да;
            }
            return нет;
        }

        /***************************************************************

            Вставляет новое значение перед узлом, который предстоит
            итерировать (или после узла, который только-что итерирован).

            Возвращает: копию этого обходчика для сцепки.

        ***************************************************************/

        Обходчик вставь (З значение)
        {
            // Note: this needs some attention, not sure как
            // в_ укз when обходчик is in реверс.
            if (ячейка is пусто)
                приор.добавьСледщ (значение, &хозяин.куча.размести);
            else
                ячейка.добавьПредш (значение, &хозяин.куча.размести);
            хозяин.инкремент;

            ++счёт;
            // ignore this change
            ++изменение;
            return *this;
        }

        /***************************************************************

                Меняет направление следщ() и opApply(), и
                сбрасывает точку терминации, так что можно выполнять
                другой полный обход.

        ***************************************************************/

        Обходчик реверс ()
        {
            рев ^= да;
            следщ;
            индекс = 0;
            return *this;
        }
    }
}

/*******************************************************************************

*******************************************************************************/

debug (UnitTest)
{
    unittest
    {
        auto список = new ЦиркулярныйСписок!(цел);
        список.добавь(1);
        список.добавь(2);
        список.добавь(3);

        цел i = 1;
        foreach(знач; список)
        {
            assert(знач == i);
            i++;
        }

        auto итер = список.обходчик;
        итер.следщ();
        итер.удали();                          // delete the первый item

        i = 2;
        foreach(знач; список)
        {
            assert(знач == i);
            i++;
        }

        // тест вставь functionality
        итер = список.обходчик;
        итер.следщ;
        итер.вставь(4);

        цел[] compareto = [2, 4, 3];
        i = 0;
        foreach(знач; список)
        {
            assert(знач == compareto[i++]);
        }
    }
}

/*******************************************************************************

*******************************************************************************/

debug (ЦиркулярныйСписок)
{
    import io.Stdout;
    import thread;
    import time.StopWatch;

    проц main()
    {
        // usage examples ...
        auto список = new ЦиркулярныйСписок!(ткст);
        foreach (значение; список)
        Стдвыв (значение).нс;

        список.добавь ("foo");
        список.добавь ("bar");
        список.добавь ("wumpus");

        // implicit генерный iteration
        foreach (значение; список)
        Стдвыв (значение).нс;

        // явный генерный iteration
        foreach (значение; список.обходчик.реверс)
        Стдвыв.фмтнс ("> {}", значение);

        // генерный iteration with optional удали
        auto s = список.обходчик;
        foreach (значение; s)
        {} //s.удали;

        // incremental iteration, with optional удали
        ткст знач;
        auto обходчик = список.обходчик;
        while (обходчик.следщ(знач))
        {}//обходчик.удали;

        // incremental iteration, with optional failfast
        auto it = список.обходчик;
        while (it.действителен && it.следщ(знач))
        {}

        // удали specific элемент
        список.удали ("wumpus", нет);

        // удали первый элемент ...
        while (список.возьми(знач))
            Стдвыв.фмтнс ("taking {}, {} лево", знач, список.размер);


        // установи for benchmark, with набор of целыйs. We
        // use a чанк разместитель, и presize the бакет[]
        auto тест = new ЦиркулярныйСписок!(бцел, Контейнер.извлеки, Контейнер.Чанк);
        тест.кэш (1000, 1_000_000);
        const счёт = 1_000_000;
        Секундомер ш;

        // benchmark добавим
        ш.старт;
        for (бцел i=счёт; i--;)
            тест.добавь(i);
        Стдвыв.фмтнс ("{} добавьs: {}/s", тест.размер, тест.размер/ш.stop);

        // benchmark добавим without allocation overhead
        тест.очисть;
        ш.старт;
        for (бцел i=счёт; i--;)
            тест.добавь(i);
        Стдвыв.фмтнс ("{} добавьs (после очисть): {}/s", тест.размер, тест.размер/ш.stop);

        // benchmark duplication
        ш.старт;
        auto dup = тест.dup;
        Стдвыв.фмтнс ("{} элемент dup: {}/s", dup.размер, dup.размер/ш.stop);

        // benchmark iteration
        ш.старт;
        foreach (значение; тест) {}
        Стдвыв.фмтнс ("{} элемент iteration: {}/s", тест.размер, тест.размер/ш.stop);

        тест.проверь;
    }
}