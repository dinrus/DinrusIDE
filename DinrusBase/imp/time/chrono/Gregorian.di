﻿module time.chrono.Gregorian;
private import time.chrono.Calendar;


/**
 * $(ANCHOR _Gregorian)
 * Представляет Грегорианский Календарь.
 *
 * Заметьте, что это Пролептический Грегорианский Календарь.  Большинство
 * календарей подразумевает, что даты до 9/14/1752 были Юлианскими Датами.
 * Юлиан отличается от Грегориана тем, что високосные годы бывают каждые 4 года,
 * даже при приращении 100 лет.
 * Пролептический Грегорианский Календарь применяет правила Грегорианского
 * високосного года к датам до 9/14/1752, что облегчает расчёт дат.
 */
extern(D) class Грегориан : Календарь 
{
        // import baseclass воВремя()
        alias Календарь.воВремя воВремя;

        /// static /*shared*/ экземпляр
        public static Грегориан генерный;

        enum Тип 
        {
                Локализованный = 1, /// Ссылается на локализованную версию Грегорианского календаря.
                АнглСША = 2, /// Ссылается на англоамериканскую версию Грегорианского календаря.
                СреднеВостФранц = 9,/// Ссылается на средневосточную французскую версию Грегорианского календаря.
                Арабский = 10, /// Ссылается на арабскую версию Грегорианского календаря.
                ТранслитерАнгл = 11,  /// Ссылается на транслитерированную английскую версию Грегорианского календаря.
                ТранслитерФранц = 12    /// Ссылается на транслитерированную французскую версию Грегорианского календаря.
        }      

        /**
        * Представляет текущ эру.
        */
        enum {НАША_ЭРА = 1, ДО_НАШЕЙ_ЭРЫ = 2, МАКС_ГОД = 9999};

        /**
        * Создает генерный экземпляр this Календарь
        */
         static this()
        {       
                генерный = new Грегориан;
        }

        /**
        * Инициализует экземпляр класса Грегориан, используя указанное значение Грегориан.Тип. Если значение 
        * не указано, дефолтом (умолчанием) является Грегориан.Types.Локализованный.
        */
        this (Тип тип = Тип.Локализованный) ;

        /**
        * Переписано. Возвращает Значение Времени, установленное в указанный дата и время в указанном _эра.
        * Параметры:
        *   год = Целое, представляющее _год.
        *   месяц = Целое, представляющее _месяц.
        *   день = Целое, представляющее _день.
        *   час = Целое, представляющее _час.
        *   минута = Целое, представляющее _минута.
        *   секунда = Целое, представляющее _секунда.
        *   миллисекунда = Целое, представляющее _миллисекунда.
        *   эра = Целое, представляющее _эра.
        * Возвращает: Время, установленное в указанную дата и время.
        */
        override Время воВремя (бцел год, бцел месяц, бцел день, бцел час, бцел минута, бцел секунда, бцел миллисекунда, бцел эра);

        /**
        * Переписано. Возвращает день недели в указанном Время.
        * Параметры: время = Значение Времени.
        * Возвращает: Значение ДняНедели  representing день недели времени.
        */
        override ДеньНедели дайДеньНедели(Время время) ;

        /**
        * Переписано. Возвращает день месяца в указанном Время.
        * Параметры: время = Значение Времени.
        * Возвращает: Целое, представляющее день месяца времени.
        */
        override бцел дайДеньМесяца(Время время) ;

        /**
        * Переписано. Возвращает день года в указанном Время.
        * Параметры: время = Значение Времени.
        * Возвращает: Целое, представляющее день годв времени.
        */
        override бцел дайДеньГода(Время время) ;

        /**
        * Переписано. Возвращает этот месяц в указанном Время.
        * Параметры: время = Значение Времени.
        * Возвращает: Целое, представляющее месяц во времени.
        */
        override бцел дайМесяц(Время время) ;

        /**
        * Переписано. Возвращает этот год в указанном Время.
        * Параметры: время = Значение Времени.
        * Возвращает: Целое, представляющее год во времени.
        */
        override бцел дайГод(Время время) ;

        /**
        * Переписано. Возвращает эту эру в указанном Время.
        * Параметры: время = Значение Времени.
        * Возвращает: Целое, представляющее эра во времени.
        */
        override бцел дайЭру(Время время) ;

        /**
        * Переписано. Возвращает число дней в указанном _год и _месяц указанной _эра.
        * Параметры:
        *   год = Целое, представляющее _год.
        *   месяц = Целое, представляющее _месяц.
        *   эра = Целое, представляющее _эра.
        * Возвращает: The число дней в указанном _год и _месяц указанной _эра.
        */
        override бцел дайДниМесяца(бцел год, бцел месяц, бцел эра) ;

        /**
        * Переписано. Возвращает число дней в указанном _год указанной _эра.
        * Параметры:
        *   год = Целое, представляющее _год.
        *   эра = Целое, представляющее _эра.
        * Возвращает: The число дней в указанном _год в указанной _эра.
        */
        override бцел дайДниГода(бцел год, бцел эра) ;

        /**
        * Переписано. Возвращает число месяцев в указанном _год указанной _эра.
        * Параметры:
        *   год = Целое, представляющее _год.
        *   эра = Целое, представляющее _эра.
        * Возвращает: The число месяцев в указанном _год в указанной _эра.
        */
        override бцел дайМесяцыГода(бцел год, бцел эра);

        /**
        * Переписано. Показывает, является ли указанный _год в указанной _эра високосным _год.
        * Параметры: год = Целое, представляющее _год.
        * Параметры: эра = Целое, представляющее _эра.
        * Возвращает: да, если указанный _год високосный _год; иначе, нет.
        */
        override бул високосен_ли(бцел год, бцел эра) ;

        /**
        * $(I Свойство.) Выводит значение Грегориан.Тип, указывающее языковую версию этого Грегориан.
        * Возвращает: Значение Грегориан.Тип, указывающее языковую версию этого Грегориан.
        */
        Тип типКалендаря();

        /**
        * $(I Свойство.) Переписано. Выводит список эр в текущ Календарь.
        * Возвращает: Массив целых чисел, представляющий эры в текущ Календарь.
        */
        override бцел[] эры();

        /**
        * $(I Свойство.) Переписано. Выводит определитель, связанный с текущ Календарь.
        * Возвращает: Целое, представляющее определитель текущ Календаря.
        */
        override бцел ид() ;

        /**
         * Переписано.  Получить компоненты структуры Время, используя
         * правила этого Календаря.  Полезно, если нужно более одного из
         * указанных компонентов. Заметьте, что время дня не указывается,
         * так как оно вычисляется непосредственно из структуры Время.
         */
        override проц разбей(Время время, ref бцел год, ref бцел месяц, ref бцел день, ref бцел деньгода, ref бцел деньнед, ref бцел эра);

        /**
         * Переписано. Возвращает новое Время с дабавлением заданного числа месяцев.
         * Если месяцы отрицательны, эти месяцы отнимаются.
         *
         * Если целевой месяц не поддерживает компонента день введённого
         * времени, то выводится ошибка, если при этом обрезатьДень не установлен в
         * да.  Если обрезатьДень установлен в да, то этот день уменьшается на_
         * максимум дней в этом месяце.
         *
         * Например, добавим один месяц к 1/31/2000 с обрезатьДень, установленным в
         * да, что приведёт к результату 2/28/2000.
         *
         * Параметры: t = Время, к которому добавляются месяцы.
         * Параметры: члоМес = Число добавляемых месяцев. Может быть
         * отрицательным.
         * Параметры: обрезатьДень = Округлить день (вниз) на максимум дней
         * целевого месяца (при необходимости).
         *
         * Возвращает: Время, представляющее предоставленное время с добавкой
         * числа месяцев.
         */
        override Время добавьМесяцы(Время t, цел члоМес, бул обрезатьДень=нет);

        /**
         * Переписано.  Добавить указанное число лет к заданному Время.
         *
         * Заметьте, что Грегорианский Календарь принимает во внимание,что время до
         * н.э. отрицательно, и поддерживает перевод из "до н.э."" в "н.э.""
         *
         * Параметры: t = Время, к которому добавляются годы.
         * Параметры: члоЛет = Число добавляемых лет. Может быть отрицательным.
         *
         * Возвращает: Время, представляющее предоставленное время с добавкой
         * числа лет.
         */
        override Время добавьГоды(Время t, цел члоЛет);
      //  package static проц разбейДату (дол тики, ref бцел год, ref бцел месяц, ref бцел день, ref бцел деньГода, ref бцел эра) ;
      //  package static бцел откиньЧасть (дол тики, ЧастьДаты часть) ;
      //  package static дол дайТикиДаты (бцел год, бцел месяц, бцел день, бцел эра);
      //  package static бул статВисокосен_ли(бцел год, бцел эра);
      //  package static проц ошАрга(ткст стр);
}