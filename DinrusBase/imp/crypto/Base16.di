﻿/*******************************************************************************

    Этот модуль используется для раскодировки и кодировки гекс-массивов типа ткст.

    Пример:
    ---
    ткст blah = "Hello there, my имя is Jeff.";

    scope encodebuf = new сим[вычислиРазмерКодир(cast(ббайт[])blah)];
    ткст кодирован = кодируй(cast(ббайт[])blah, encodebuf);

    scope decodebuf = new ббайт[кодирован.length];
    if (cast(ткст)раскодируй(кодирован, decodebuf) == "Hello there, my имя is Jeff.")
        Стдвыв("yay").нс;
    ---

    Since v1.0

*******************************************************************************/

module crypto.Base16;

extern(D):

/*******************************************************************************

    Вычисляет и возвращает размер, необходимый для кодирования переданного
    массива.

    Параметры:
    данные = Массив, который будет кодирован

*******************************************************************************/
 бцел вычислиРазмерКодир(ббайт[] данные);

/*******************************************************************************

    Вычисляет и возвращает размер, необходимый для кодирования переданной длины.

    Параметры:
    длина = Число кодируемых байтов.

*******************************************************************************/
бцел вычислиРазмерКодир(бцел длина);

/*******************************************************************************

    Кодирует данные и возвращает как ASCII hex ткст.

    Параметры:
    данные = что is в_ be кодирован
    buff = буфер large enough в_ hold кодирован данные

    Пример:
    ---
    сим[512] encodebuf;
    ткст myEncodedString = кодируй(cast(ббайт[])"Hello, как are you today?", encodebuf);
    Стдвыв(myEncodedString).нс; // 48656C6C6F2C20686F772061726520796F7520746F6461793F
    ---


*******************************************************************************/
ткст кодируй(ббайт[] данные, ткст буф);

/*******************************************************************************

    Кодирует данные и возвращает как ASCII hex ткст.

    Параметры:
    данные = кодируемое

    Пример:
    ---
    ткст myEncodedString = кодируй(cast(ббайт[])"Hello, how are you today?");
    Стдвыв(myEncodedString).нс; // 48656C6C6F2C20686F772061726520796F7520746F6461793F
    ---


*******************************************************************************/
 ткст кодируй(ббайт[] данные);

/*******************************************************************************

    Раскодирует ASCII hex ткст и возвращает его как ббайт[] данные. Преаллоцирует
    размер массива.

    Этот декодер игнорирует не-гекс символы. Так:
    SGVsbG8sIGhvd
    yBhcmUgeW91IH
    RvZGF5Pw==

    действительно.

    Параметры:
    данные = раскодируемое

    Пример:
    ---
    ткст myDecodedString = cast(ткст)раскодируй("48656C6C6F2C20686F772061726520796F7520746F6461793F");
    Стдвыв(myDecodeString).нс; // Hello, how are you today?
    ---

*******************************************************************************/
 ббайт[] раскодируй(ткст данные);

/*******************************************************************************

    Раскодирует ASCII hex ткст и возвращает его как ббайт[] данные.

    Этот декодер игнорирует не-гекс символы. Так:
    SGVsbG8sIGhvd
    yBhcmUgeW91IH
    RvZGF5Pw==

    действительно.

    Параметры:
    данные = раскодируемое
    buff = массив, достаточный для вмещения раскодированных данных

    Пример:
    ---
    ббайт[512] decodebuf;
    ткст myDecodedString = cast(ткст)раскодируй("48656C6C6F2C20686F772061726520796F7520746F6461793F", decodebuf);
    Стдвыв(myDecodeString).нс; // Hello, how are you today?
    ---

*******************************************************************************/
 ббайт[] раскодируй(ткст данные, ббайт[] буф);