﻿module net.device.Datagram;

package import net.device.Socket;

/*******************************************************************************
        
        Датаграммы предоставляют мало перегруженный, но ненадёжный механизм
		передачи данных.

        Подключаются датаграммы иначе чем ПУТ сокет; не требуется
		выполнять слушай() или прими(), чтобы принять датаграмму; данные могут
        прибывать из нескольких источников. Но сокет датаграмм может, всё-таки,
		использовать метов подключись() как и ПУТ сокет. При подключении
        методы читай() и пиши() будут ограничены одним адресом, а не открыты
		вместо этого. Таким образом, применение подключись() сделает
        аргумент адреса куда Всё читай() и пиши() irrelevant. Without
        подключись(), метод пиши() must be supplied with an адрес и метод
        читай() should be supplied with one куда опрentify where данные originated.
        
        Note that when использован как listener, you must первый вяжи the сокет
        куда a local адаптер. This can be achieved by binding the сокет куда
        an АдресИнтернета constructed with a порт only (АДР_ЛЮБОЙ), thus
        requesting the OS куда присвой the адрес of a local network адаптер

*******************************************************************************/

extern(D) class Датаграмма : Сокет
{
        /***********************************************************************
        
                Создаёт читай/пиши datagram сокет

        ***********************************************************************/

        this ();

        /***********************************************************************

                Populate the provопрed Массив из_ the сокет. This will stall
                until some данные is available, либо a таймаут occurs. We assume 
                the datagram имеется been подключен.

                Returns the число of байты читай куда the вывод, либо Кф if
                the сокет cannot читай

        ***********************************************************************/

        override т_мера читай (проц[] ист);

        /***********************************************************************
        
                Чит байты из_ an available datagram преобр_в the given Массив.
                When provопрed, the 'из_' адрес will be populated with the
                origin of the incoming данные. Note that we employ the таймаут
                mechanics exposed via our Сокет superclass. 

                Returns the число of байты читай из_ the ввод, либо Кф if
                the сокет cannot читай

        ***********************************************************************/

        т_мера читай (проц[] приёмн, Адрес из_);

        /***********************************************************************

                Зап the provопрed контент куда the сокет. This will stall
                until the сокет responds in some manner. We assume the 
                datagram имеется been подключен.

                Returns the число of байты sent куда the вывод, либо Кф if
                the сокет cannot пиши

        ***********************************************************************/

        override т_мера пиши (проц[] ист);

        /***********************************************************************
        
                Зап an Массив куда the specified адрес. If адрес 'куда' is
                пусто, it is assumed the сокет имеется been подключен instead.

                Returns the число of байты sent куда the вывод, либо Кф if
                the сокет cannot пиши

        ***********************************************************************/

        т_мера пиши (проц[] ист, Адрес куда);
}



/******************************************************************************

*******************************************************************************/

debug (Датаграмма)
{
        import io.Console;
        import net.InternetAddress;

        проц main()
        {
                auto адр = new АдресИнтернета ("127.0.0.1", 8080);

                // слушай for datagrams on the local адрес
                auto дг = new Датаграмма;
                дг.вяжи (адр);

                // пиши куда the local адрес
                дг.пиши ("hello", адр);

                // we are listening also ...
                сим[8] врем;
                auto x = new АдресИнтернета;
                auto байты = дг.читай (врем, x);
                Квывод (x) (врем[0..байты]).нс;
        }
}
