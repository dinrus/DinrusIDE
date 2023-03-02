﻿module io.FileScan;

public  import io.FilePath;

/*******************************************************************************

        Рекурсивно сканирует файлы и папки, добавляя отфильтрованные файлы в
        структуру вывода. Может использоваться для создания списка подпапок
        и содержащихся в них файлов. Следующий пример создаёт список всех файлов
        с суффиксом ".d", включающий текущую папку со всеми, расположенными в ней,
        подпапками:
        ---
        auto скан = new СканФайл;

        скан (".", ".d");

        Стдвыв.фмтнс ("{} Папок", скан.папки.length);
        foreach (папка; скан.папки)
                 Стдвыв.фмтнс ("{}", папка);

        Стдвыв.фмтнс ("\n{} Файлов", скан.файлы.length);
        foreach (файл; скан.файлы)
                 Стдвыв.фмтнс ("{}", файл);
        ---

        Это непохоже на большинство эффективных методов сканирования большого числа
        файлов, но работает в удобной манере.
        
*******************************************************************************/

extern(D)
class СканФайл
{       
        alias смети     opCall;
        
        /***********************************************************************

            Псевдоним для делегата от Фильтр.Принимает ФПуть & бул в 
            качестве аргументов и возвращает булево значение.

            Аргумент ФПуть представляет файл, найденный при сканировании, 
            и бул, если ФПуть является папкой.

            Этот фильтр возвращает да, если находит совпадения. Заметьте,
            возврат нет, когда путь представляет папку, приводит к тому, 
            что все содержащиеся в ней файлы игнорируются. Чтобы всегда 
            проводить рекурсию папкок, делаем следующее:
            ---
            return (папка_ли || свер (fp.имя));
            ---

        ***********************************************************************/

        alias ФПуть.Фильтр Фильтр;

       /***********************************************************************

            Возвращает все ошибки, найденные при последнем сканировании.

        ***********************************************************************/

        public ткст[] ошибки ();

        /***********************************************************************

            Возвращает все файлы, найденные при последнем сканировании.

        ***********************************************************************/

        public ФПуть[] файлы ();

        /***********************************************************************
        
            Возвращает все папки, найденные при последнем сканировании.

        ***********************************************************************/

        public ФПуть[] папки ();

        /***********************************************************************

                Сметает набор файлов и папок из заданного родительского пути,
                без применения фильтрации.
        
        ***********************************************************************/
        
        СканФайл смети (ткст путь, бул рекурсия = да);

        /***********************************************************************
                Сметает набор файлов и папок из заданного родительского пути,
                с применения фильтрации элементов по указанному суффиксу.
        
        ***********************************************************************/
        
        СканФайл смети (ткст путь, ткст свер, бул рекурсия = да);

        /***********************************************************************
                Сметает набор файлов и папок из заданного родительского пути,
                с применения фильтрации элементов по заданному делегату.

        ***********************************************************************/
        
        СканФайл смети (ткст путь, Фильтр фильтр, бул рекурсия=да);



}


/*******************************************************************************

*******************************************************************************/

debug (СканФайл)
{
        import io.Stdout;
        import io.FileScan;

        проц main()
        {
                auto скан = new СканФайл;

                скан (".");

                Стдвыв.фмтнс ("{} Папки", скан.папки.length);
                foreach (папка; скан.папки)
                         Стдвыв (папка).нс;

                Стдвыв.фмтнс ("\n{} Файлы", скан.файлы.length);
                foreach (файл; скан.файлы)
                         Стдвыв (файл).нс;

                Стдвыв.фмтнс ("\n{} Ошибки", скан.ошибки.length);
                foreach (ошибка; скан.ошибки)
                         Стдвыв (ошибка).нс;
        }
}
