﻿/*******************************************************************************

    Модуль предоставляет систему общецелевого форматирования для
    преобразования значений в текст, удобный для отображения. Есть поддержка
    размещению, выравниванию и общих определителей формата для
    чисел.

    Выкладка может быть адаптирована путём конфигурирования различных указателей
    и связанных с ней метаданных. Её можно применять для plug in текст.локаль
    для обработки частных форматов, дата/время и специфичных для культуры
    преобразований.

    На стиль форматирования больше всего повлияли используемые в фреймворках .NET
    и ICU , а не C-стильное printf или D-стильное
    пишиф.

******************************************************************************/

module text.convert.Layout;

private import  exception;
private import  Utf = text.convert.Utf;
private import  Плав = text.convert.Float,
Целое = text.convert.Integer;

private import  io.model: ИПотокВывода;

private import tpl.variant;

 private import time.Time;
 private import text.convert.DateTime;

//version = GNU;

/*******************************************************************************

        Платформные вопросы ...

*******************************************************************************/

version (GNU)
{
    private import tpl.args;
    alias ук Арг;
    alias спис_ва АргСписок;
}
else version(LDC)
{
    private import tpl.args;
    alias ук Арг;
    alias спис_ва АргСписок;
}
else
{
    alias ук Арг;
    alias ук АргСписок;
}

/*******************************************************************************

Содержит методы для замены элементов формата в ткст на ткст
эквиваленты каждому аргументу.

*******************************************************************************/
alias Выкладка!(сим).экземпляр фмт;

class Выкладка(T)
{
    public alias преобразуй opCall;
    public alias бцел delegate (T[]) Сток;

/*
    static if (is (typeof(МестнДатаВремя)))
        private МестнДатаВремя* датаВремя = &ДефДатаВремя;
		*/
		
		alias text.convert.DateTime.ДефДатаВремя датаВремя;

    /**********************************************************************

            Возвращает разделяемый экземпляр.

            Он не является потокобезопасным,и его статический конструктор
            не вызывается соответствующе (баг компилятора).

    **********************************************************************/

    static Выкладка экземпляр ()
    {
        static Выкладка общая;

        if (общая is пусто)
            общая = new Выкладка!(T);
        return общая;
    }

    /**********************************************************************

    **********************************************************************/

    public final T[] sprint (T[] результат, T[] строкаФмт, ...)
    {
        return vprint (результат, строкаФмт, _arguments, _argptr);
    }

    /**********************************************************************

    **********************************************************************/

    public final T[] vprint (T[] результат, T[] строкаФмт, ИнфОТипе[] аргументы, АргСписок арги)
    {
        T*  p = результат.ptr;
        цел естьВНаличии = результат.length;

        бцел сток (T[] s)
        {
            цел длин = s.length;
            if (длин > естьВНаличии)
                длин = естьВНаличии;

            естьВНаличии -= длин;
            p [0..длин] = s[0..длин];
            p += длин;
            return длин;
        }

        преобразуй (&сток, аргументы, арги, строкаФмт);
        return результат [0 .. p-результат.ptr];
    }

    /**********************************************************************

            Заменяет элемент _format в ткст на ткст
            эквивалент каждому аргументу.

            Параметры:
              строкаФмт  = Ткст, содержащий элементы форматирования _format.
              арги       = Список аргументов.

            Возвращает: Копию строкаФмт, элементы в которой заменены
            на ткст эквивалент аргументов.

            Примечания: Параметр строкаФмт внедрён с элементами _format
            в форме: $(BR)$(BR)
              {индекс[,alignment][:_format-ткст]}$(BR)$(BR)
              $(UL $(LI индекс $(BR)
                Целое число, указывающее элемент в списке для форматирования.)
              $(LI alignment $(BR)
                Необязательное целое, указывающее минимальную ширину.
                Результат - псеп_в_конце с пробелами, если длина значения
                меньше, чем alignment.)
              $(LI _format-ткст $(BR)
                Опциональный ткст колов форматирования.)
            )$(BR)

            Необходимы вводные и завершающие фигурные скобки. Чтобы включить
            литеральный символ фигурных скобок, используются два символа вводной и
            закрывающей скобок.$(BR)$(BR)
            Если строкаФмт равна "{0} бутылок пива на столе" и
            аргумент равен целому со значением 99, то возвратное значение
            будет:$(BR) "99 бутылок пива на столе".

    **********************************************************************/

    public final T[] преобразуй (T[] строкаФмт, ...)
    {
  		return преобразуй (_arguments, _argptr, строкаФмт);
/+
        ИнфОТипе[] аргументы = _arguments;
        АргСписок арги  = _argptr;
		assert (строкаФмт, "пустой определитель формата");
		assert (аргументы.length < 64, "слишком много аргов в Выкладка.преобразуй");
		T[] вывод;

        бцел сток (T[] s)
        {
            вывод ~= s;
            return s.length;
        }

        //Сток сток = &сток_;
		Арг[64] аргсписок = void;
		foreach (i, арг; аргументы)
		{
			аргсписок[i] = арги;
			арги += (арг.tsize + т_мера.sizeof - 1) & ~ (т_мера.sizeof - 1);
		}  	
	    разбор (строкаФмт, аргументы, аргсписок, &сток);
        return вывод;
        +/
    }

    /**********************************************************************

    **********************************************************************/

    public final бцел преобразуй (Сток сток, T[] строкаФмт, ...)
    {
        return преобразуй (сток, _arguments, _argptr, строкаФмт);
    }

    /**********************************************************************

        Tentative преобразуй, использующий ИПотокВывода как сток.
		
    **********************************************************************/

    public final бцел преобразуй (ИПотокВывода вывод, T[] строкаФмт, ...)
    {
        бцел сток (T[] s)
        {
            return вывод.пиши(s);
        }

        return преобразуй (&сток, _arguments, _argptr, строкаФмт);
    }

    /**********************************************************************

    **********************************************************************/

    public final T[] преобразуй (ИнфОТипе[] аргументы, АргСписок арги, T[] строкаФмт)
    {
        T[] вывод;

        бцел сток (T[] s)
        {
            вывод ~= s;
            return s.length;
        }

        преобразуй (&сток, аргументы, арги, строкаФмт);
        return вывод;
    }


    /**********************************************************************

    **********************************************************************/

    public final бцел преобразуй (Сток сток, ИнфОТипе[] аргументы, АргСписок арги, T[] строкаФмт)
    {
        assert (строкаФмт, "пустой определитель формата");
        assert (аргументы.length < 64, "слишком много аргов в Выкладка.преобразуй");

        version (GNU)
        {
            union ArgU
            {
                цел i;
                байт b;
                дол l;
                крат s;
                проц[] a;
                реал r;
                плав f;
                дво d;
                кплав cf;
                кдво cd;
                креал вк;
            }

            Арг[64] аргсписок = void;
            ArgU[64] сохранённыеАрги = void;

            foreach (i, арг; аргументы)
            {
                static if (is(typeof(арги.ptr)))
                    аргсписок[i] = арги.ptr;
                else
                    аргсписок[i] = арги;

                /* Поскольку типы с плавающей запятой "не живут"
                 * на стэке, они должны использоваться
                 * надлежащим типом. */
                бул преобразованый = нет;
                switch (арг.classinfo.имя[9])
                {
                case КодТипа.ПЛАВ, КодТипа.МПЛАВ:
                    сохранённыеАрги[i].f = ва_арг!(плав)(арги);
                    аргсписок[i] = &(сохранённыеАрги[i].f);
                    преобразованый = да;
                    break;

                case КодТипа.КПЛАВ:
                    сохранённыеАрги[i].cf = ва_арг!(кплав)(арги);
                    аргсписок[i] = &(сохранённыеАрги[i].cf);
                    преобразованый = да;
                    break;

                case КодТипа.ДВО, КодТипа.МДВО:
                    сохранённыеАрги[i].d = ва_арг!(дво)(арги);
                    аргсписок[i] = &(сохранённыеАрги[i].d);
                    преобразованый = да;
                    break;

                case КодТипа.КДВО:
                    сохранённыеАрги[i].cd = ва_арг!(кдво)(арги);
                    аргсписок[i] = &(сохранённыеАрги[i].cd);
                    преобразованый = да;
                    break;

                case КодТипа.РЕАЛ, КодТипа.МРЕАЛ:
                    сохранённыеАрги[i].r = ва_арг!(реал)(арги);
                    аргсписок[i] = &(сохранённыеАрги[i].r);
                    преобразованый = да;
                    break;

                case КодТипа.КРЕАЛ:
                    сохранённыеАрги[i].вк = ва_арг!(креал)(арги);
                    аргсписок[i] = &(сохранённыеАрги[i].вк);
                    преобразованый = да;
                    break;

                default:
                    break;
                }
                if (! преобразованый)
                {
                    switch (арг.tsize)
                    {
                    case 1:
                        сохранённыеАрги[i].b = ва_арг!(байт)(арги);
                        аргсписок[i] = &(сохранённыеАрги[i].b);
                        break;
                    case 2:
                        сохранённыеАрги[i].s = ва_арг!(крат)(арги);
                        аргсписок[i] = &(сохранённыеАрги[i].s);
                        break;
                    case 4:
                        сохранённыеАрги[i].i = ва_арг!(цел)(арги);
                        аргсписок[i] = &(сохранённыеАрги[i].i);
                        break;
                    case 8:
                        сохранённыеАрги[i].l = ва_арг!(дол)(арги);
                        аргсписок[i] = &(сохранённыеАрги[i].l);
                        break;
                    case 16:
                        assert((проц[]).sizeof==16,"Размер структуры не поддерживается");
                        сохранённыеАрги[i].a = ва_арг!(проц[])(арги);
                        аргсписок[i] = &(сохранённыеАрги[i].a);
                        break;
                    default:
                        assert (нет, "Неизвестный размер: " ~ Целое.вТкст (арг.tsize));
                    }
                }
            }
        }
        else
        {
            Арг[64] аргсписок =void;
            foreach (i, арг; аргументы)
            {
                аргсписок[i] = арги;
                арги += (арг.tsize + т_мера.sizeof - 1) & ~ (т_мера.sizeof - 1);
            }
        }
        return разбор (строкаФмт, аргументы, аргсписок, сток);
    }

    /**********************************************************************

            Разбирает формат-ткст, выводя в сток форматированные арги и текст
            фрагменты по ходу.

    **********************************************************************/

    public бцел разбор (T[] выкладка, ИнфОТипе[] иот, Арг[] арги, Сток сток)
    {
        T[512] результат = void;
        цел length, следщИндекс;


        T* s = выкладка.ptr;
        T* фрагмент = s;
        T* конец = s + выкладка.length;

        while (да)
        {
            while (s < конец && *s != '{')
                ++s;

            // излить фрагмент
            length += сток (фрагмент [0 .. s - фрагмент]);

            // все готово?
            if (s is конец)
                break;

            // проверить на "{{" и пропустить, если так
            if (*++s is '{')
            {
                фрагмент = s++;
                continue;
            }

            цел индекс = 0;
            бул indexed = нет;

            // выкинуть индекс
            while (*s >= '0' && *s <= '9')
            {
                индекс = индекс * 10 + *s++ -'0';
                indexed = да;
            }

            // пропустить пробелы
            while (s < конец && *s is ' ')
                ++s;

            бул crop;
            бул лево;
            бул право;
            цел  ширина;

            // есть минимальная или же максимальна ширина?
            if (*s is ',' || *s is '.')
            {
                if (*s is '.')
                    crop = да;

                while (++s < конец && *s is ' ') {}
                if (*s is '-')
                {
                    лево = да;
                    ++s;
                }
                else
                    право = да;

                // получить ширину
                while (*s >= '0' && *s <= '9')
                    ширина = ширина * 10 + *s++ -'0';

                // пропустить пробелы
                while (s < конец && *s is ' ')
                    ++s;
            }

            T[] формат;

            // имеется ткст формата?
            if (*s is ':' && s < конец)
            {
                T* fs = ++s;

                // съесть всё до закрывающей скобки
                while (s < конец && *s != '}')
                    ++s;
                формат= fs [0 .. s - fs];
            }

            // настоять на закрывающей скобке
            if (*s != '}')
            {
                length += сток ("{ошибочный формат}");
                continue;
            }

            // проверить на дефолтный индекс & установить следщ дефолтный счётчик
            if (! indexed)
                индекс = следщИндекс;
            следщИндекс = индекс + 1;

            // следщ сим - старт последующего фрагмента
            фрагмент = ++s;

            // укз alignment
            проц излей (T[] стр)
            {
                цел pading = ширина - стр.length;

                if (crop)
                {
                    if (pading < 0)
                    {
                        if (лево)
                        {
                            length += сток ("...");
                            length += сток (Utf.отрежьЛево (стр[-pading..$]));
                        }
                        else
                        {
                            length += сток (Utf.отрежьПраво (стр[0..ширина]));
                            length += сток ("...");
                        }
                    }
                    else
                        length += сток (стр);
                }
                else
                {
                    // if право aligned, пад out with пробелы
                    if (право && pading > 0)
                        length += пробелы (сток, pading);

                    // излей formatted аргумент
                    length += сток (стр);

                    // finally, пад out on право
                    if (лево && pading > 0)
                        length += пробелы (сток, pading);
                }
            }

            // an astonishing число of typehacks needed в_ укз массивы :(
            проц process (ИнфОТипе _ti, Арг _arg)
            {
                // Because Вариантs can contain AAs (и maybe
                // even static массивы someday), we need в_
                // process them here.

                if (_ti is typeid(Вариант))
                {
                        // Unpack the variant и вперёд
                        auto vptr = cast(Вариант*)_arg;
                        auto innerTi = vptr.тип;
                        auto innerArg = vptr.ptr;
                        process (innerTi, innerArg);
                }                
                else if (_ti.classinfo.имя.length is 20 && _ti.classinfo.имя[9..$] == "StaticArray" )
                {
                    auto tiStat = cast(TypeInfo_StaticArray)_ti;
                    auto p = _arg;
                    length += сток ("[");
                    for (цел i = 0; i < tiStat.длин; i++)
                    {
                        if (p !is _arg )
                            length += сток (", ");
                        process (tiStat.значение, p);
                        p += tiStat.tsize/tiStat.длин;
                    }
                    length += сток ("]");
                }
                else if (_ti.classinfo.имя.length is 25 && _ti.classinfo.имя[9..$] == "AssociativeArray")
                {
                    auto tiAsso = cast(TypeInfo_AssociativeArray)_ti;
                    auto tiKey = tiAsso.ключ;
                    auto tiVal = tiAsso.следщ();
                    // the knowledge of the internal ключ/знач storage is использован
                    // so this might break if, that internal storage changes
                    alias ббайт AV; // any тип for ключ, значение might be ok, the размеры are corrected later
                    alias ббайт AK;
                    auto aa = *cast(AV[AK]*) _arg;

                    length += сток ("{");
                    бул первый = да;

                    цел roundUp (цел sz)
                    {
                        return (sz + (ук).sizeof -1) & ~((ук).sizeof - 1);
                    }

                    foreach (ref знач; aa)
                    {
                        // the ключ is befor the значение, so substrace with fixed ключ размер из_ above
                        auto pk = cast(Арг)( &знач - roundUp(AK.sizeof));
                        // сейчас the реал значение поз is plus the реал ключ размер
                        auto pv = cast(Арг)(pk + roundUp(tiKey.tsize()));

                        if (!первый)
                            length += сток (", ");
                        process (tiKey, pk);
                        length += сток (" => ");
                        process (tiVal, pv);
                        первый = нет;
                    }
                    length += сток ("}");
                }
                else if (_ti.classinfo.имя[9] is КодТипа.МАССИВ)
                {
                    if (_ti is typeid(ткст))
                        излей (Utf.изТкст8 (*cast(ткст*) _arg, результат));
                    else if (_ti is typeid(шим[]))
                        излей (Utf.изТкст16 (*cast(шим[]*) _arg, результат));
                    else if (_ti is typeid(дим[]))
                        излей (Utf.изТкст32 (*cast(дим[]*) _arg, результат));
                    else
                    {
                        // для всех нетекстовых Массив типов (включая ткст[])
                        auto масс = *cast(проц[]*)_arg;
                        auto длин = масс.length;
                        auto ptr = cast(Арг) масс.ptr;
                        auto elTi = _ti.следщ();
                        auto размер = elTi.tsize();
                        length += сток ("[");
                        while (длин > 0)
                        {
                            if (ptr !is масс.ptr)
                                length += сток (", ");
                            process (elTi, ptr);
                            длин -= 1;
                            ptr += размер;
                        }
                        length += сток ("]");
                    }
                }
                else
                    // стандартная обработка
                    излей (депешируй (результат, формат, _ti, _arg));
            }


            // обработать данный аргумент
            if (индекс >= иот.length)
                излей ("{неверный индекс}");
            else
                process (иот[индекс], арги[индекс]);
        }
        return length;
    }

    /***********************************************************************

    ***********************************************************************/

    private T[] депешируй (T[] результат, T[] формат, ИнфОТипе тип, Арг p)
    {
        switch (тип.classinfo.имя[9])
        {
        case КодТипа.БУЛ:
            static T[] t = "да";
            static T[] f = "нет";
            return (*cast(бул*) p) ? t : f;

        case КодТипа.БАЙТ:
            return целое (результат, *cast(байт*) p, формат, ббайт.max);

        case КодТипа.ПРОЦ:
        case КодТипа.ББАЙТ:
            return целое (результат, *cast(ббайт*) p, формат, ббайт.max, "u");

        case КодТипа.КРАТ:
            return целое (результат, *cast(крат*) p, формат, бкрат.max);

        case КодТипа.БКРАТ:
            return целое (результат, *cast(бкрат*) p, формат, бкрат.max, "u");

        case КодТипа.ЦЕЛ:
            return целое (результат, *cast(цел*) p, формат, бцел.max);

        case КодТипа.БЦЕЛ:
            return целое (результат, *cast(бцел*) p, формат, бцел.max, "u");

        case КодТипа.БДОЛ:
            return целое (результат, *cast(дол*) p, формат, бдол.max, "u");

        case КодТипа.ДОЛ:
            return целое (результат, *cast(дол*) p, формат, бдол.max);

        case КодТипа.ПЛАВ:
            return плавающее (результат, *cast(плав*) p, формат);

        case КодТипа.МПЛАВ:
            return мнимое (результат, *cast(вплав*) p, формат);

        case КодТипа.МДВО:
            return мнимое (результат, *cast(вдво*) p, формат);

        case КодТипа.МРЕАЛ:
            return мнимое (результат, *cast(вреал*) p, формат);

        case КодТипа.КПЛАВ:
            return комплексное (результат, *cast(кплав*) p, формат);

        case КодТипа.КДВО:
            return комплексное (результат, *cast(кдво*) p, формат);

        case КодТипа.КРЕАЛ:
            return комплексное (результат, *cast(креал*) p, формат);

        case КодТипа.ДВО:
            return плавающее (результат, *cast(дво*) p, формат);

        case КодТипа.РЕАЛ:
            return плавающее (результат, *cast(реал*) p, формат);

        case КодТипа.СИМ:
            return Utf.изТкст8 ((cast(сим*) p)[0..1], результат);

        case КодТипа.ШИМ:
            return Utf.изТкст16 ((cast(шим*) p)[0..1], результат);

        case КодТипа.ДИМ:
            return Utf.изТкст32 ((cast(дим*) p)[0..1], результат);

        case КодТипа.УКЗ:
            return целое (результат, *cast(т_мера*) p, формат, т_мера.max, "x");

        case КодТипа.КЛАСС:
            auto c = *cast(Объект*) p;
            if (c)
                return Utf.изТкст8 (c.вТкст, результат);
            break;

        case КодТипа.СТРУКТ:
            auto s = cast(TypeInfo_Struct) тип;
            if (s.xtoString)
            {
                ткст delegate() вТкст;
                вТкст.ptr = p;
                вТкст.funcptr = cast(ткст function())s.xtoString;
                return Utf.изТкст8 (вТкст(), результат);
            }
            goto default;

        case КодТипа.ИНТЕРФЕЙС:
            auto x = *cast(ук*) p;
            if (x)
            {
                auto пи = **cast(Interface ***) x;
                auto o = cast(Объект)(*cast(ук*)p - пи.смещение);
                return Utf.изТкст8 (o.вТкст, результат);
            }
            break;

        case КодТипа.ПЕРЕЧЕНЬ:
            return депешируй (результат, формат, (cast(TypeInfo_Enum) тип).основа, p);

        case КодТипа.ТИПДЕФ:
            return депешируй (результат, формат, (cast(TypeInfo_Typedef) тип).основа, p);

        default:
            return неизвестное (результат, формат, тип, p);
        }

        return cast(T[]) "{пусто}"w;
    }

    /**********************************************************************

            Указывает на ошибку "неизвестный тип".

    **********************************************************************/

    protected T[] неизвестное (T[] результат, T[] формат, ИнфОТипе тип, Арг p)
    {
            if (тип is typeid(Время))
            {
                static if (is (T == сим))
                    return датаВремя.фмт(результат, *cast(Время*) p, формат);
                else
                {
                    // TODO: this needs в_ be cleaned up
                    сим[128] tmp0 =void;
                    сим[128] tmp1 =void;
                    return Utf.изТкст8(датаВремя.фмт(tmp0, *cast(Время*) p, Utf.вТкст(формат, tmp1)), результат);
                }
            }
        
        return "{необрабатываемый тип аргумента: " ~ Utf.изТкст8 (тип.вТкст, результат) ~ "}";
    }

    /**********************************************************************

            Форматирует целое значение.

    **********************************************************************/

    protected T[] целое (T[] вывод, дол знач, T[] формат, бдол маска = бдол.max, T[] def="d")
    {
        if (формат.length is 0)
            формат = def;
        if (формат[0] != 'd')
            знач &= маска;

        return Целое.фмт (вывод, знач, формат);
    }

    /**********************************************************************

            Форматирует значение с плавающей точкой. Дефолт установлен на 2 
            десятичных места.

    **********************************************************************/

    protected T[] плавающее (T[] вывод, реал знач, T[] формат)
    {
        бцел dec = 2,
                 эксп = 10;
        бул пад = да;

        for (auto p=формат.ptr, e=p+формат.length; p < e; ++p)
            switch (*p)
            {
            case '.':
                пад = нет;
                break;
            case 'e':
            case 'Е':
                эксп = 0;
                break;
            case 'x':
            case 'X':
                дво d = знач;
                return целое (вывод, *cast(дол*) &d, "x#");
            default:
                auto c = *p;
                if (c >= '0' && c <= '9')
                {
                    dec = c - '0', c = p[1];
                    if (c >= '0' && c <= '9' && ++p < e)
                        dec = dec * 10 + c - '0';
                }
                break;
            }

        return Плав.фмт (вывод, знач, dec, эксп, пад);
    }

    /**********************************************************************

    **********************************************************************/

    private проц ошибка (ткст сооб)
    {
        throw new ИсклНелегальногоАргумента (сооб);
    }

    /**********************************************************************

    **********************************************************************/

    private бцел пробелы (Сток сток, цел счёт)
    {
        бцел возвр;

        static const T[32] Spaces = ' ';
        while (счёт > Spaces.length)
        {
            возвр += сток (Spaces);
            счёт -= Spaces.length;
        }
        return возвр + сток (Spaces[0..счёт]);
    }

    /**********************************************************************

            Форматирует мнимое значение.

    **********************************************************************/

    private T[] мнимое (T[] результат, вреал знач, T[] формат)
    {
        return плавающийХвост (результат, знач.im, формат, "*1i");
    }

    /**********************************************************************

            Форматирует комплексное значение.

    **********************************************************************/

    private T[] комплексное (T[] результат, креал знач, T[] формат)
    {
        static бул signed (реал x)
        {
            static if (реал.sizeof is 4)
                return ((*cast(бцел *)&x) & 0x8000_0000) != 0;
            else
                static if (реал.sizeof is 8)
                    return ((*cast(бдол *)&x) & 0x8000_0000_0000_0000) != 0;
                else
                {
                    auto pe = cast(ббайт *)&x;
                    return (pe[9] & 0x80) != 0;
                }
        }
        static T[] plus = "+";

        auto длин = плавающийХвост (результат, знач.re, формат, signed(знач.im) ? пусто : plus).length;
        return результат [0 .. длин + плавающийХвост (результат[длин..$], знач.im, формат, "*1i").length];
    }

    /**********************************************************************

            Форматирует значения с плавающей точкой,  и добавляет их в хвост.

    **********************************************************************/

    private T[] плавающийХвост (T[] результат, реал знач, T[] формат, T[] хвост)
    {
        assert (результат.length > хвост.length);

        auto рез = плавающее (результат[0..$-хвост.length], знач, формат);
        auto длин=рез.length;
        if (рез.ptr!is результат.ptr)
            результат[0..длин]=рез;
        результат [длин .. длин + хвост.length] = хвост;
        return результат [0 .. длин + хвост.length];
    }
}


/*******************************************************************************

*******************************************************************************/

private enum КодТипа
{
    ПУСТОЙ = 0,
    ПРОЦ = 'v',
    БУЛ = 'b',
    ББАЙТ = 'h',
    БАЙТ = 'g',
    БКРАТ = 't',
    КРАТ = 's',
    БЦЕЛ = 'k',
    ЦЕЛ = 'i',
    БДОЛ = 'm',
    ДОЛ = 'l',
    РЕАЛ = 'e',
    ПЛАВ = 'f',
    ДВО = 'd',
    СИМ = 'a',
    ШИМ = 'u',
    ДИМ = 'w',
    МАССИВ = 'A',
    КЛАСС = 'C',
    СТРУКТ = 'S',
    ПЕРЕЧЕНЬ = 'Е',
    КОНСТ = 'x',
    ИНВАРИАНТ = 'y',
    ДЕЛЕГАТ = 'D',
    ФУНКЦИЯ = 'F',
    УКЗ = 'P',
    ТИПДЕФ = 'T',
    ИНТЕРФЕЙС = 'I',
    КПЛАВ = 'q',
    КДВО = 'r',
    КРЕАЛ = 'c',
    МПЛАВ = 'o',
    МДВО = 'p',
    МРЕАЛ = 'j'
}



/*******************************************************************************

*******************************************************************************/

debug (UnitTest)
{
    unittest
    {
       // auto фмт = Выкладка!(сим).экземпляр;

        // basic выкладка tests
        assert( фмт( "abc" ) == "abc" );
        assert( фмт( "{0}", 1 ) == "1" );
        assert( фмт( "{0}", -1 ) == "-1" );

        assert( фмт( "{}", 1 ) == "1" );
        assert( фмт( "{} {}", 1, 2) == "1 2" );
        assert( фмт( "{} {0} {}", 1, 3) == "1 1 3" );
        assert( фмт( "{} {0} {} {}", 1, 3) == "1 1 3 {не_годится индекс}" );
        assert( фмт( "{} {0} {} {:x}", 1, 3) == "1 1 3 {не_годится индекс}" );

        assert( фмт( "{0}", да ) == "да", фмт( "{0}", да ));
        assert( фмт( "{0}", нет ) == "нет" );

        assert( фмт( "{0}", cast(байт)-128 ) == "-128" );
        assert( фмт( "{0}", cast(байт)127 ) == "127" );
        assert( фмт( "{0}", cast(ббайт)255 ) == "255" );

        assert( фмт( "{0}", cast(крат)-32768  ) == "-32768" );
        assert( фмт( "{0}", cast(крат)32767 ) == "32767" );
        assert( фмт( "{0}", cast(бкрат)65535 ) == "65535" );
        assert( фмт( "{0:x4}", cast(бкрат)0xafe ) == "0afe" );
        assert( фмт( "{0:X4}", cast(бкрат)0xafe ) == "0AFE" );

        assert( фмт( "{0}", -2147483648 ) == "-2147483648" );
        assert( фмт( "{0}", 2147483647 ) == "2147483647" );
        assert( фмт( "{0}", 4294967295 ) == "4294967295" );

        // large целыйs
        assert( фмт( "{0}", -9223372036854775807L) == "-9223372036854775807" );
        assert( фмт( "{0}", 0x8000_0000_0000_0000L) == "9223372036854775808" );
        assert( фмт( "{0}", 9223372036854775807L ) == "9223372036854775807" );
        assert( фмт( "{0:X}", 0xFFFF_FFFF_FFFF_FFFF) == "FFFFFFFFFFFFFFFF" );
        assert( фмт( "{0:x}", 0xFFFF_FFFF_FFFF_FFFF) == "ffffffffffffffff" );
        assert( фмт( "{0:x}", 0xFFFF_1234_FFFF_FFFF) == "ffff1234ffffffff" );
        assert( фмт( "{0:x19}", 0x1234_FFFF_FFFF) == "00000001234ffffffff" );
        assert( фмт( "{0}", 18446744073709551615UL ) == "18446744073709551615" );
        assert( фмт( "{0}", 18446744073709551615UL ) == "18446744073709551615" );

        // fragments перед и после
        assert( фмт( "d{0}d", "s" ) == "dsd" );
        assert( фмт( "d{0}d", "1234567890" ) == "d1234567890d" );

        // brace escaping
        assert( фмт( "d{0}d", "<ткст>" ) == "d<ткст>d");
        assert( фмт( "d{{0}d", "<ткст>" ) == "d{0}d");
        assert( фмт( "d{{{0}d", "<ткст>" ) == "d{<ткст>d");
        assert( фмт( "d{0}}d", "<ткст>" ) == "d<ткст>}d");

        // hex conversions, where ширина indicates leading zeroes
        assert( фмт( "{0:x}", 0xafe0000 ) == "afe0000" );
        assert( фмт( "{0:x7}", 0xafe0000 ) == "afe0000" );
        assert( фмт( "{0:x8}", 0xafe0000 ) == "0afe0000" );
        assert( фмт( "{0:X8}", 0xafe0000 ) == "0AFE0000" );
        assert( фмт( "{0:X9}", 0xafe0000 ) == "00AFE0000" );
        assert( фмт( "{0:X13}", 0xafe0000 ) == "000000AFE0000" );
        assert( фмт( "{0:x13}", 0xafe0000 ) == "000000afe0000" );

        // десяток ширина
        assert( фмт( "{0:d6}", 123 ) == "000123" );
        assert( фмт( "{0,7:d6}", 123 ) == " 000123" );
        assert( фмт( "{0,-7:d6}", 123 ) == "000123 " );

        // ширина & знак combinations
        assert( фмт( "{0:d7}", -123 ) == "-0000123" );
        assert( фмт( "{0,7:d6}", 123 ) == " 000123" );
        assert( фмт( "{0,7:d7}", -123 ) == "-0000123" );
        assert( фмт( "{0,8:d7}", -123 ) == "-0000123" );
        assert( фмт( "{0,5:d7}", -123 ) == "-0000123" );

        // Negative numbers in various bases
        assert( фмт( "{:b}", cast(байт) -1 ) == "11111111" );
        assert( фмт( "{:b}", cast(крат) -1 ) == "1111111111111111" );
        assert( фмт( "{:b}", cast(цел) -1 )
        == "11111111111111111111111111111111" );
        assert( фмт( "{:b}", cast(дол) -1 )
        == "1111111111111111111111111111111111111111111111111111111111111111" );

        assert( фмт( "{:o}", cast(байт) -1 ) == "377" );
        assert( фмт( "{:o}", cast(крат) -1 ) == "177777" );
        assert( фмт( "{:o}", cast(цел) -1 ) == "37777777777" );
        assert( фмт( "{:o}", cast(дол) -1 ) == "1777777777777777777777" );

        assert( фмт( "{:d}", cast(байт) -1 ) == "-1" );
        assert( фмт( "{:d}", cast(крат) -1 ) == "-1" );
        assert( фмт( "{:d}", cast(цел) -1 ) == "-1" );
        assert( фмт( "{:d}", cast(дол) -1 ) == "-1" );

        assert( фмт( "{:x}", cast(байт) -1 ) == "ff" );
        assert( фмт( "{:x}", cast(крат) -1 ) == "ffff" );
        assert( фмт( "{:x}", cast(цел) -1 ) == "ffffffff" );
        assert( фмт( "{:x}", cast(дол) -1 ) == "ffffffffffffffff" );

        // аргумент индекс
        assert( фмт( "a{0}b{1}c{2}", "x", "y", "z" ) == "axbycz" );
        assert( фмт( "a{2}b{1}c{0}", "x", "y", "z" ) == "azbycx" );
        assert( фмт( "a{1}b{1}c{1}", "x", "y", "z" ) == "aybycy" );

        // alignment does not ограничь the length
        assert( фмт( "{0,5}", "hellohello" ) == "hellohello" );

        // alignment fills with пробелы
        assert( фмт( "->{0,-10}<-", "hello" ) == "->hello     <-" );
        assert( фмт( "->{0,10}<-", "hello" ) == "->     hello<-" );
        assert( фмт( "->{0,-10}<-", 12345 ) == "->12345     <-" );
        assert( фмт( "->{0,10}<-", 12345 ) == "->     12345<-" );

        // chop at maximum specified length; вставь ellИПses when chopped
        assert( фмт( "->{.5}<-", "hello" ) == "->hello<-" );
        assert( фмт( "->{.4}<-", "hello" ) == "->hell...<-" );
        assert( фмт( "->{.-3}<-", "hello" ) == "->...llo<-" );

        // ширина определитель indicates число of десяток places
        assert( фмт( "{0:f}", 1.23f ) == "1.23" );
        assert( фмт( "{0:f4}", 1.23456789L ) == "1.2346" );
        assert( фмт( "{0:e4}", 0.0001) == "1.0000e-04");

        assert( фмт( "{0:f}", 1.23f*1i ) == "1.23*1i");
        assert( фмт( "{0:f4}", 1.23456789L*1i ) == "1.2346*1i" );
        assert( фмт( "{0:e4}", 0.0001*1i) == "1.0000e-04*1i");

        assert( фмт( "{0:f}", 1.23f+1i ) == "1.23+1.00*1i" );
        assert( фмт( "{0:f4}", 1.23456789L+1i ) == "1.2346+1.0000*1i" );
        assert( фмт( "{0:e4}", 0.0001+1i) == "1.0000e-04+1.0000e+00*1i");
        assert( фмт( "{0:f}", 1.23f-1i ) == "1.23-1.00*1i" );
        assert( фмт( "{0:f4}", 1.23456789L-1i ) == "1.2346-1.0000*1i" );
        assert( фмт( "{0:e4}", 0.0001-1i) == "1.0000e-04-1.0000e+00*1i");

        // 'f.' & 'e.' фмт truncates zeroes из_ floating десятки
        assert( фмт( "{:f4.}", 1.230 ) == "1.23" );
        assert( фмт( "{:f6.}", 1.230 ) == "1.23" );
        assert( фмт( "{:f1.}", 1.230 ) == "1.2" );
        assert( фмт( "{:f.}", 1.233 ) == "1.23" );
        assert( фмт( "{:f.}", 1.237 ) == "1.24" );
        assert( фмт( "{:f.}", 1.000 ) == "1" );
        assert( фмт( "{:f2.}", 200.001 ) == "200");

        // Массив вывод
        цел[] a = [ 51, 52, 53, 54, 55 ];
        assert( фмт( "{}", a ) == "[51, 52, 53, 54, 55]" );
        assert( фмт( "{:x}", a ) == "[33, 34, 35, 36, 37]" );
        assert( фмт( "{,-4}", a ) == "[51  , 52  , 53  , 54  , 55  ]" );
        assert( фмт( "{,4}", a ) == "[  51,   52,   53,   54,   55]" );
        цел[][] b = [ [ 51, 52 ], [ 53, 54, 55 ] ];
        assert( фмт( "{}", b ) == "[[51, 52], [53, 54, 55]]" );

        бкрат[3] c = [ cast(бкрат)51, 52, 53 ];
        assert( фмт( "{}", c ) == "[51, 52, 53]" );

        // целое AA
        бкрат[дол] d;
        d[234] = 2;
        d[345] = 3;
        assert( фмт( "{}", d ) == "{234 => 2, 345 => 3}" ||
        фмт( "{}", d ) == "{345 => 3, 234 => 2}");

        // бул/ткст AA
        бул[ткст] e;
        e[ "ключ".dup ] = да;
        e[ "значение".dup ] = нет;
        assert( фмт( "{}", e ) == "{ключ => да, значение => нет}" ||
        фмт( "{}", e ) == "{значение => нет, ключ => да}");

        // ткст/дво AA
        ткст[ дво ] f;
        f[ 1.0 ] = "one".dup;
        f[ 3.14 ] = "ПИ".dup;
        assert( фмт( "{}", f ) == "{1.00 => one, 3.14 => ПИ}" ||
        фмт( "{}", f ) == "{3.14 => ПИ, 1.00 => one}");
    }
}



debug (Выкладка)
{
    import io.Console;

    static if (is (typeof(Время)))
        import time.WallClock;

    проц main ()
    {
        //auto фмт = Выкладка!(сим).экземпляр;

        фмт.преобразуй (Квывод.поток, "hi {}", "there\n");

        Квывод (фмт.sprint (new сим[1], "hi")).нс;
        Квывод (фмт.sprint (new сим[10], "{.4}", "hello")).нс;
        Квывод (фмт.sprint (new сим[10], "{.-4}", "hello")).нс;

        Квывод (фмт ("{:f1}", 3.0)).нс;
        Квывод (фмт ("{:g}", 3.00)).нс;
        Квывод (фмт ("{:f1}", -0.0)).нс;
        Квывод (фмт ("{:g1}", -0.0)).нс;
        Квывод (фмт ("{:d2}", 56)).нс;
        Квывод (фмт ("{:d4}", cast(байт) -56)).нс;
        Квывод (фмт ("{:f4}", 1.0e+12)).нс;
        Квывод (фмт ("{:f4}", 1.23e-2)).нс;
        Квывод (фмт ("{:f8}", 3.14159)).нс;
        Квывод (фмт ("{:e20}", 1.23e-3)).нс;
        Квывод (фмт ("{:e4.}", 1.23e-07)).нс;
        Квывод (фмт ("{:.}", 1.2)).нс;
        Квывод (фмт ("ptr:{}", &фмт)).нс;
        Квывод (фмт ("бдол.max {}", бдол.max)).нс;

        struct S
        {
            ткст вТкст ()
            {
                return "foo";
            }
        }

        S s;
        Квывод (фмт ("struct: {}", s)).нс;

        static if (is (typeof(Время)))
            Квывод (фмт ("время: {}", Куранты.сейчас)).нс;
    }
}
