﻿/*******************************************************************************

        Стандартные, глобальные форматёры для консольного вывода.
		Если форматированный вывод или трансляция в Юникод не требуется,
		тогда лучше использовать непосредственно модуль io.Console.
		Если же требуется форматирование, но без вывода в консоль,
		то вместо этого лучше применять text.convert.Format.

        Стдвыв & Стдош используются в следующем стиле:
        ---
        Стдвыв ("hello");                       => hello
        Стдвыв (1);                             => 1
        Стдвыв (3.14);                          => 3.14
        Стдвыв ('b');                           => b
        Стдвыв (1, 2, 3);                       => 1, 2, 3
        Стдвыв ("abc", 1, 2, 3);                => abc, 1, 2, 3
        Стдвыв ("abc", 1, 2) ("foo");           => abc, 1, 2foo
        Стдвыв ("abc") ("def") (3.14);          => abcdef3.14

        Стдвыв.фмт ("abc {}", 1);            => abc 1
        Стдвыв.фмт ("abc {}:{}", 1, 2);      => abc 1:2
        Стдвыв.фмт ("abc {1}:{0}", 1, 2);    => abc 2:1
        Стдвыв.фмт ("abc ", 1);              => abc
        ---

        Заметьте, последний пример не выводит исключение. Существует
        несколько случаев применения, в которых вброс аргумента допустим,
        поэтому не приходится применять дополнительных механизмов "отлова".

        Очистка вывода достигается слей() методом или
        посредством пустой пары скобок:
        ---
        Стдвыв ("hello world") ();
        Стдвыв ("hello world").слей;

        Стдвыв.фмт ("hello {}", "world") ();
        Стдвыв.фмт ("hello {}", "world").слей;
        ---

        Особые символьные цепочки, такие как "\n", записываются прямо в
        вывод, без всяких трансляций (хотя можно вставить вывод-фильтр,
        чтобы выаолнять нужные трансляции). Платформно-специфичные
        "новые строки" генерируются методом нс(), который также очищает
        вывод, если он на это сконфигурирован:
        ---
        Стдвыв ("hello ") ("world").нс;
        Стдвыв.фмт ("hello {}", "world").нс;
        Стдвыв.фмтнс ("hello {}", "world");
        ---

        Метод фмт() у Стдош и Стдвыв поддерживает ряд опций
        форматирования, предоставленный text.convert.Layout и его
        расширениями; включая все расширения I18N, чтобы активировать
        францизский Стдвыв, сделайте следуюшее:
        ---
        import text.locale.Locale;

        Стдвыв.выкладка = new Локаль (Культура.дайКультуру ("fr-FR"));
        ---

        Отметим, что Стдвыв - сущность совместного использования,
        поэтому каждое его использование зависит от примера выше.
        Для приложений, поддерживающих несколько регионов,
        создаётся несколько экземпляров Локаль, которые кэшируются
        соответствующим образом.

        Стдвыв.выкладка также может использоваться для форматирования без
        вывода в консоль, как в следующем примере:
        ---
        ткст ткт = Стдвыв.выкладка.преобразуй("{} и {}", 42, "abc");
        //ткт равно "42 и abc"
        ---
        Это удобно в случае, если Стдвыв уже импортирован.

        Заметьте также, что используемый вывод-поток предоставляется
        глобальными экземплярами ~ tего можно применить, например,
        для копирования файла в стандартный вывод:
        ---
        Стдвыв.копируй (new Файл ("myfile"));
        ---

        Заметьте, что Стдвыв *не* потокобезопасен. Используйте либо
        io.log.Trace, либо стандартные средства логирования, чтобы
        активировать ввод-вывод через атомарную консоль.

*******************************************************************************/
module io.Stdout;

private import io.Console;
private import io.stream.Format;
private import text.convert.Layout;

/*******************************************************************************

        Конструирует Стдвыв & Стдош при загрузке этого модуля

*******************************************************************************/

private alias ФормВывод!(сим) Вывод;

public static Вывод Стдвыв,      /// глобальный стандартный вывод
       Стдош;      /// глобальный вывод ошибок
public alias Стдвыв  стдвыв;      /// альтернатива
public alias Стдош  стдош;      /// альтернатива

static this ()
{
    //Статический конструктор внутри класса Выкладка 
    // нельзя вызвать до выполнения этого (bug)
    auto выкладка = Выкладка!(сим).экземпляр;

    Стдвыв = new Вывод (выкладка, Квывод.поток);
    Стдош = new Вывод (выкладка, Кош.поток);

    Стдвыв.слей = !Квывод.перенаправленый;
    Стдош.слей = !Кош.перенаправленый;
}