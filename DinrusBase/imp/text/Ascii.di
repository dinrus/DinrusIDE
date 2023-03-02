﻿/*******************************************************************************
        Placeholder for a выделение of ASCII utilities. These generally will
        not work with utf8, и cannot be easily extended в_ utf16 or utf32        
*******************************************************************************/

module text.Ascii;

extern(D):

/******************************************************************************

        Convert в_ lowercase. Возвращает the преобразованый контент in приёмн,
        performing an in-place conversion if приёмн is пусто

******************************************************************************/

ткст вПроп (ткст ист, ткст приёмн = пусто);

/******************************************************************************

        Преобразовать в верхний регистр. Возвращает на приёмник преобразованный
		контент, выполняя преобразование на месте, если приёмник == пусто

******************************************************************************/

ткст вЗаг (ткст ист, ткст приёмн = пусто);

/******************************************************************************

        Сравнить два текста, игнорируя регистр. Возвращает 0, если они равны
        
******************************************************************************/

цел сравнилюб (ткст s1, ткст s2);


/******************************************************************************

        Сравнивает два ткст с регистром. Возвращает 0, если равны
        
******************************************************************************/

цел сравни (ткст s1, ткст s2);



/******************************************************************************

        Возвращает индекс позиция of a текст образец внутри ист, or
        ист.length upon failure.

        This is a case-insensitive ищи (with thanks в_ Nietsnie)
        
******************************************************************************/

static цел найдилюб (in ткст ист, in ткст образец);



/******************************************************************************

******************************************************************************/

debug (UnitTest)
{       
        unittest
        {
        сим[20] врем;
        
        assert (вПроп("1bac", врем) == "1bac");
        assert (вПроп("1BAC", врем) == "1bac");
        assert (вЗаг("1bac", врем) == "1BAC");
        assert (вЗаг("1BAC", врем) == "1BAC");
        assert (сравнилюб ("ABC", "abc") is 0);
        assert (сравнилюб ("abc", "abc") is 0);
        assert (сравнилюб ("abcd", "abc") > 0);
        assert (сравнилюб ("abc", "abcd") < 0);
        assert (сравнилюб ("ACC", "abc") > 0);

        assert (найдилюб ("ACC", "abc") is 3);
        assert (найдилюб ("ACC", "acc") is 0);
        assert (найдилюб ("aACC", "acc") is 1);
        }
}

debug (Ascii)
{
        проц main() {}
}
