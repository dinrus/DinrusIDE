﻿module io.stream.TextFile;

public  import io.device.File;

private import io.stream.Text;

extern(D):

/*******************************************************************************

        Создаёт файл со строчно-ориентированным вводом. Ввод буферируемый.

*******************************************************************************/

class ТекстФайлВвод : ТекстВвод
{
        /***********************************************************************

                Создаёт FileStream.              

        ***********************************************************************/

        this (ткст путь, Файл.Стиль стиль = Файл.ЧитСущ);

        /***********************************************************************

                Оборачивает экземпляр FileConduit. 

        ***********************************************************************/

        this (Файл файл);
}


/*******************************************************************************
       
        Создаёт файл с форматированным текстовым выводом. Вывод буферируемый.

*******************************************************************************/

class ТекстФайлВывод : ТекстВывод
{
        /***********************************************************************

                Создаёт FileStream.             

        ***********************************************************************/

        this (ткст путь, Файл.Стиль стиль = Файл.ЗапСозд);

        /***********************************************************************

                Оборачивает экземпляр Файл.

        ***********************************************************************/

        this (Файл файл);
 }


/*******************************************************************************

*******************************************************************************/

debug (TextFile)
{
        import io.Console;

        проц main()
        {
                auto t = new ТекстФайлВвод ("TextFile.d");
                foreach (строка; t)
                         Квывод(строка).нс;                  
        }
}
