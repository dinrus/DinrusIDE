﻿module io.stream.Zlib;

private import io.device.Conduit : ФильтрВвода, ФильтрВывода;
private import io.model : ИПотокВвода, ИПотокВывода, ИПровод;

/* Этой константой контролируется размер буферов ввода/вывода, используемых
 * внутренне. Она должна иметь ясное и здравое значение (так рекомендует
 * документация zlib), и меняться она может только на платформах с
 * ограничениями памяти или в зависимости от целей применения.
 */

private enum { РАЗМЕР_ЧАНКА = 256 * 1024 };

/* Эта константа определяет дефолтное значение окноБиты. Она взята из локументации
 * в zlib.в. Она не должна ничего нарушать, если zlib изменится вдруг на
 * иные дефолты.
 */

private enum { ОКНОБИТЫ_ДЕФОЛТ = 15 };

extern(D):

/*******************************************************************************

    Этот фильтр ввода может использоваться для расжатия потоков zlib.

*******************************************************************************/

class ВводЗлиб : ФильтрВвода
{
    /***************************************************************************

        В этом перечне определяется кодировка сжатого потока.

    ***************************************************************************/

    enum Кодировка : цел
    {
        /**
         *  Код попытается автоматически определить, какой будет кодировка
         *  потока. Если поток был сжат без кодировки, то в этом случае
		 *  автоматическое определение невозможно!
         */
        Авто,
        /**
         *  Поток в кодировке zlib.
         */
        Злиб,
        /**
         *  Поток в кодировке gzip.
         */
        Гзип,
        /**
         *  Поток без кодировки.
         */
        Нет
    }


    /***************************************************************************

        Конструирует новый декомпрессионный фильтр zlib. Требуется пароль к потоку,
        из которого будет производиться чтение декомпрессиооным фильтром. Если этот
        фильтр используется с каким-то проводом, то используется идиома:

        ---
        auto ввод = new ВводЗлиб(мойПровод.ввод));
        ввод.читай(мойКонтент);
        ---

        Опциональный параметр окноБиты  лежит в основе двух логарифмов размера
        окна, должен быть в диапазоне 8-15, по умолчанию равен 15, если не
        указано иного.  В дополнение, параметр окноБиты  может быть отрицательным,
		что указывает на то, что zlib должен пропустить стандартные заголовок
		и "трейлер" zlib, с размером окна, равным -окноБиты.

      Параметры:
        поток = сжатый поток ввода.

        кодировка =
            кодировка потока.  По умолчанию равен Кодировка.Авто, чего
            должно быть достаточно, если поток не был сжат без кодировки;
			а в случае его сжатия без кодировки нужно вручную задать
            Кодировка.Нет.

        окноБиты =
            два основных логарифма размера окна, должно быть в
            диапазоне 8-15, если не указано, то по умолчанию равно 15.

    ***************************************************************************/

    this(ИПотокВвода поток, Кодировка кодировка,
         цел окноБиты = ОКНОБИТЫ_ДЕФОЛТ);

    /// описано ранее
    this(ИПотокВвода поток);

    ~this();

    /***************************************************************************

        Сбрасывает и ре-инициализует этот экземпляр.

        При создании сжатых потоков в цикле, этот метод может использоваться
        повторного применения единичного экземпляра, что предотвращает
        потенциально нагрузочную реаллокацию внутренних буферов.

        Этот поток должен быть закрытым уже до вызова метода сбрось.

    ***************************************************************************/

    проц сбрось(ИПотокВвода поток, Кодировка кодировка,
                          цел окноБиты = ОКНОБИТЫ_ДЕФОЛТ);

    /// описано ранее

    проц сбрось(ИПотокВвода поток);

    /***************************************************************************

        Разжимает данные из низлежащего провода, преобразованнного в целевой массив.

        Возвращает число байтов, сохранённых в приёмн, которое может быть меньше
        требуемого.
    ***************************************************************************/

    override т_мера читай(проц[] приёмн);

    /***************************************************************************

        Закрывает поток сжатия.

    ***************************************************************************/

    override проц закрой();

    // Отключить seeking
    override дол сместись(дол смещение, Якорь якорь = Якорь.Нач);

}

/*******************************************************************************

    Этот фильтр вывода может использоваться для выполнения сжатия данных,
	преобразуемых в поток zlib.

*******************************************************************************/

class ВыводЗлиб : ФильтрВывода
{
    /***************************************************************************

        В перечне представлено несколько предопределённых уровней сжатия.

        Любое целое между -1 и 9 включительно может быть использовано как уровень,
        хотя символов в данном перечне должно быть достаточно для большинства
		случаев.

    ***************************************************************************/

    enum Уровень : цел
    {
        /**
         * Дефолтный уровень сжатия.  Его выбирают для получения отличного компромисса
         * между скоростьи и сжатием, а нужный уровень сжатия определяется низлежащей
         * библиотекой zlib.  Он, грубо говоря, эквивалентен
         *  уровню сжатия 6.
         */
        Нормальный = -1,
        /**
         * Не выполнять сжатия. Это вызывает лёгкое расширение потока для
         * вмещения поточных метаданных.
         */
        Нет = 0,
        /**
         * Минимальное сжатие; самый скорый уровень, выполняющий самую
         * невысокую компрессию.
         */
        Быстрый = 1,
        /**
         * Максимальное сжатие.
         */
        Наилучший = 9
    }

    /***************************************************************************

        Этот перечень позволяет указать кодировку сжатого потока.

    ***************************************************************************/

    enum Кодировка : цел
    {
        /**
         *  Поток будет использовать кодировку zlib.
         */
        Злиб,
        /**
         *  Поток будет использовать кодировку gzip.
         */
        Гзип,
        /**
         *  Поток без кодировки.
         */
        Нет
    }

    /***************************************************************************

        Конструирует новый компрессионный фильтр zlib. Требуется пароль к потоку,
        в который будет производиться запись компрессиооным фильтром. Если этот
        фильтр используется с каким-то проводом, то используется идиома:

        ---
        auto вывод = new ВыводЗлиб(мойПровод.вывод);
        вывод.пиши(мойКонтент);
        ---

        Опциональный параметр окноБиты  лежит в основе двух логарифмов размера
        окна, должен быть в диапазоне 8-15, по умолчанию равен 15, если не
        указано иного.  В дополнение, параметр окноБиты  может быть отрицательным,
		что указывает на то, что zlib должен пропустить стандартные заголовок
		и "трейлер" zlib, с размером окна, равным -окноБиты.

    ***************************************************************************/

    this(ИПотокВывода поток, Уровень уровень, Кодировка кодировка,
         цел окноБиты = ОКНОБИТЫ_ДЕФОЛТ);

    /// описано ранее
    this(ИПотокВывода поток, Уровень уровень = Уровень.Нормальный);


    ~this();

    /***************************************************************************

        Сбрасывает и ре-инициализует этот экземпляр.

        При создании сжатых потоков в цикле, этот метод может использоваться
        повторного применения единичного экземпляра, что предотвращает
        потенциально нагрузочную реаллокацию внутренних буферов.

        Этот поток должен быть закрытым уже до вызова метода сбрось.

    ***************************************************************************/

    проц сбрось(ИПотокВывода поток, Уровень уровень, Кодировка кодировка,
                          цел окноБиты = ОКНОБИТЫ_ДЕФОЛТ);

    /// описано ранее
    проц сбрось(ИПотокВывода поток, Уровень уровень = Уровень.Нормальный);

    /***************************************************************************

        Сжимает данные в низлежащем проводе.

        Возвращает число сжатых байтов из ист; при записи должны всегда
		использоваться все предоставленные данные; хотя они не всегда
        тот час же записываются в низлежащий поток вывода.

    ***************************************************************************/

    override т_мера пиши(проц[] ист);

    /***************************************************************************

        Это свойство только для чтения возвращает число сжатых байтов, записанных
        в низлежащий поток. Следуя а вызовом закрой или подай, оно содержит
		общий сжатый размер потока ввода данные.

    ***************************************************************************/

    т_мера записано();

    /***************************************************************************

        Закрывает поток сжатия. Весь буферированный контент передаётся
        в низлежащий поток.

    ***************************************************************************/

    override проц закрой();

    /***************************************************************************

        Протолкнуть любой буферированный контент.  При вызове метода косвенно закрывается
		поток zlib, поэтому его нельзя вызывать до завершения сжатия данных.
		При вызовах пиши или подай после того, как компрессионный фильтр
        был подан, выводится исключение.

        Единственная разница между этим методом и методом закрой состоит в том,
        что низлежащий поток остаётся открытым.

    ***************************************************************************/

    проц подай();

    // Disable seeking
    override дол сместись(дол смещение, Якорь якорь = Якорь.Нач);

}