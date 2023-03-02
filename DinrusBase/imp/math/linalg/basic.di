﻿/***
* В модуле содержатся основные числовые процедуры, часто используемые
* в прочих модулях, либо применяемые в проектах, где используется линейная алгебра.
*/
module math.linalg.basic;

//import stdrus;
import math.linalg.config;

/***
Функция приблизительного равенства. Фактически копия функции feqrel,
но с видоизменениями, делающими её пригодной для сравнения почти нулевых чисел.

Params:
    x, y        = Сравниваемые числа.
    отнпрец     = Minimal number of mantissa bits that have to be _equal in x and y
                  to suppose their's equality. Makes sense in comparisons of values
                  enough far from ноль.
    абспрец     = If absolute difference between x and y is меньше than 2^(-абспрец)
                  they supposed to be _equal. Makes sense in comparisons of values
                  enough near to ноль.
                
Возвращает:
    true if x and y are supposed to be _equal, false otherwise.
*/
бул равны(реал x, реал y, цел отнпрец = дефотнпрец, цел абспрец = дефабспрец);

template РавенствоПоНорм(Т)
{
    бул равны(Т a, Т b, цел отнпрец = дефотнпрец, цел абспрец = дефабспрец)
    {
        return .равны((b - a).квадратНорм, 0.L, отнпрец, абспрец);
    }
}


бул меньше(реал a, реал b, цел отнпрец = дефотнпрец, цел абспрец = дефабспрец);


бул больше(реал a, реал b, цел отнпрец = дефотнпрец, цел абспрец = дефабспрец);

/***
Функция линейной интерполяции.
Возвращает:
    Значение, интерполированное из a в b по значению t. Если t не входит в диапазон [0; 1],
    то применяется линейная экстраполяция.
*/
реал лининтерп(реал a, реал b, реал t);

template Лининтерп(Т)
{
    Т лининтерп(Т a, Т b, реал t)
    {
        return a * (1 - t) + b * t;
    }
}

/***
Содержит функции мин и макс для генерных типов, обеспечивающих
opCmp.
*/
template МинМакс(Т)
{
    /***
    Возвращает:
        Наибольшее из a и b.
    */
    Т макс(Т a, Т b)
    {
        return (a > b) ? a : b;
    }

    /***
    Возвращает:
        Наименьшее из a и b.
    */
    Т мин(Т a, Т b)
    {
        return (a < b) ? a : b;
    }
}

/// Вводим функции мин и макс для основных числовых типов.
alias МинМакс!(бул).мин     мин;
alias МинМакс!(бул).макс     макс; /// описано
alias МинМакс!(байт).мин     мин; /// описано
alias МинМакс!(байт).макс     макс; /// описано
alias МинМакс!(ббайт).мин    мин; /// описано
alias МинМакс!(ббайт).макс    макс; /// описано
alias МинМакс!(крат).мин    мин; /// описано
alias МинМакс!(крат).макс    макс; /// описано
alias МинМакс!(бкрат).мин   мин; /// описано
alias МинМакс!(бкрат).макс   макс; /// описано
alias МинМакс!(цел).мин      мин; /// описано
alias МинМакс!(цел).макс      макс; /// описано
alias МинМакс!(бцел).мин     мин; /// описано
alias МинМакс!(бцел).макс     макс; /// описано
alias МинМакс!(дол).мин     мин; /// описано
alias МинМакс!(дол).макс     макс; /// описано
alias МинМакс!(бдол).мин    мин; /// описано
alias МинМакс!(бдол).макс    макс; /// описано
alias МинМакс!(плав).мин    мин; /// описано
alias МинМакс!(плав).макс    макс; /// описано
alias МинМакс!(дво).мин   мин; /// описано
alias МинМакс!(дво).макс   макс; /// описано
alias МинМакс!(реал).мин     мин; /// описано
alias МинМакс!(реал).макс     макс; /// описано


/***
Contains clamping functions for generic types to which мин and макс
functions can be applied.
*/
template Закрепи(Т)
{
    /***
    Makes value of x to be not меньше than inf. Method can change value of x.
    Возвращает:
        Copy of x после clamping is applied.
    */
    Т закрепиПод(inout Т x, Т inf)
    {
        return x = макс(x, inf);
    }

    /***
    Makes value of x to be not больше than sup. Method can change value of x.
    Возвращает:
        Copy of x после clamping is applied.
    */
    Т закрепиНад(inout Т x, Т sup)
    {
        return x = мин(x, sup);
    }

    /***
    Makes value of x to be nor меньше than inf nor больше than sup.
    Method can change value of x.
    Возвращает:
        Copy of x после clamping is applied.
    */
    Т закрепи(inout Т x, Т inf, Т sup)
    {
        закрепиПод(x, inf);
        return закрепиНад(x, sup);
    }
}

/// Introduce clamping functions for basic numeric types.
alias Закрепи!(бул).закрепиПод   закрепиПод;
alias Закрепи!(бул).закрепиНад   закрепиНад; /// описано
alias Закрепи!(бул).закрепи        закрепи;      /// описано
alias Закрепи!(байт).закрепиПод   закрепиПод; /// описано
alias Закрепи!(байт).закрепиНад   закрепиНад; /// описано
alias Закрепи!(байт).закрепи        закрепи;      /// описано
alias Закрепи!(ббайт).закрепиПод  закрепиПод; /// описано
alias Закрепи!(ббайт).закрепиНад  закрепиНад; /// описано
alias Закрепи!(ббайт).закрепи       закрепи;      /// описано
alias Закрепи!(крат).закрепиПод  закрепиПод; /// описано
alias Закрепи!(крат).закрепиНад  закрепиНад; /// описано
alias Закрепи!(крат).закрепи       закрепи;      /// описано
alias Закрепи!(бкрат).закрепиПод закрепиПод; /// описано
alias Закрепи!(бкрат).закрепиНад закрепиНад; /// описано
alias Закрепи!(бкрат).закрепи      закрепи;      /// описано
alias Закрепи!(цел).закрепиПод    закрепиПод; /// описано
alias Закрепи!(цел).закрепиНад    закрепиНад; /// описано
alias Закрепи!(цел).закрепи         закрепи;      /// описано
alias Закрепи!(бцел).закрепиПод   закрепиПод; /// описано
alias Закрепи!(бцел).закрепиНад   закрепиНад; /// описано
alias Закрепи!(бцел).закрепи        закрепи;      /// описано
alias Закрепи!(дол).закрепиПод   закрепиПод; /// описано
alias Закрепи!(дол).закрепиНад   закрепиНад; /// описано
alias Закрепи!(дол).закрепи        закрепи;      /// описано
alias Закрепи!(бдол).закрепиПод  закрепиПод; /// описано
alias Закрепи!(бдол).закрепиНад  закрепиНад; /// описано
alias Закрепи!(бдол).закрепи       закрепи;      /// описано
alias Закрепи!(плав).закрепиПод  закрепиПод; /// описано
alias Закрепи!(плав).закрепиНад  закрепиНад; /// описано
alias Закрепи!(плав).закрепи       закрепи;      /// описано
alias Закрепи!(дво).закрепиПод закрепиПод; /// описано
alias Закрепи!(дво).закрепиНад закрепиНад; /// описано
alias Закрепи!(дво).закрепи      закрепи;      /// описано
alias Закрепи!(реал).закрепиПод   закрепиПод; /// описано
alias Закрепи!(реал).закрепиНад   закрепиНад; /// описано
alias Закрепи!(реал).закрепи        закрепи;      /// описано

/** Contains переставь function for generic types. */
template ПростойПерестанов(Т)
{
    /** Swaps values of a and b. */
    void переставь(inout Т a, inout Т b)
    {
        Т temp = a;
        a = b;
        b = temp;
    }
}

/// Introduces переставь function for basic numeric types.
alias ПростойПерестанов!(бул).переставь   переставь;
alias ПростойПерестанов!(байт).переставь   переставь; /// описано
alias ПростойПерестанов!(ббайт).переставь  переставь; /// описано
alias ПростойПерестанов!(крат).переставь  переставь; /// описано
alias ПростойПерестанов!(бкрат).переставь переставь; /// описано
alias ПростойПерестанов!(цел).переставь    переставь; /// описано
alias ПростойПерестанов!(бцел).переставь   переставь; /// описано
alias ПростойПерестанов!(дол).переставь   переставь; /// описано
alias ПростойПерестанов!(бдол).переставь  переставь; /// описано
alias ПростойПерестанов!(плав).переставь  переставь; /// описано
alias ПростойПерестанов!(дво).переставь переставь; /// описано
alias ПростойПерестанов!(реал).переставь   переставь; /// описано
