﻿/**
 * Модуль tuple определяет шаблонную структуру, используемую для группирования произвольных данных.
 *
 */
module tpl.tuple;


/**
 * Кортеж - это набор типизированных значений.  Кортежи используются для возврата
 * набора значений из функции или передачи набора параметров в
 * функцию.
 *
 * ПРИМЕЧАНИЕ: Со времени перехода от пользовательских к встроенным кортежам, способность
 *       возвращать кортежи из функции была утрачена. Пока эта проблема существует
 *       в языке, кортежи следует заключать в структуру,
 *       если их нужно возвращать из функции.
 *
 * Пример:
 * ----------------------------------------------------------------------
 *
 * alias Кортеж!(цел, реал) T1;
 * alias Кортеж!(цел, дол) T2;
 * struct Оберни( Знач... )
 * {
 *     Знач знач;
 * }
 *
 * Оберни!(T2) функц( T1 знач )
 * {
 *     Оберни!(T2) возвр;
 *     возвр.знач[0] = знач[0];
 *     возвр.знач[1] = знач[0] * cast(дол) знач[1];
 *     return возвр;
 * }
 *
 * ----------------------------------------------------------------------
 *
 * Это оригинальный пример кортежа, он демонстрирует возможные действия с
 * кортежами. Надеемся, что скоро для этой фичи будет добавлена языковая
 * поддержка.
 *
 * Пример:
 * ----------------------------------------------------------------------
 *
 * alias Кортеж!(цел, реал) T1;
 * alias Кортеж!(цел, дол) T2;
 *
 * T2 функц( T1 знач )
 * {
 *     T2 возвр;
 *     возвр[0] = знач[0];
 *     возвр[1] = знач[0] * cast(дол) знач[1];
 *     return возвр;
 * }
 *
 *
 * // кортежи можно компоновать
 * alias Кортеж!(цел) ЦелКортеж;
 * alias Кортеж!(ЦелКортеж, дол) ВозврКортеж;
 *
 * // кортежи равнозначны набору параметров функции такого же типа
 * ВозврКортеж t = функц( 1, 2.3 );
 *
 * ----------------------------------------------------------------------
 */
template Кортеж( СписокТ... )
{
    alias СписокТ Кортеж;
}


/**
 * Возвращает индекс первого случая T в СписокТ или Tlist.length, если он
 * не найден.
 */
template ИндексУ( T, СписокТ... )
{
    static if( СписокТ.length == 0 )
        const т_мера ИндексУ = 0;
    else static if( is( T == СписокТ[0] ) )
        const т_мера ИндексУ = 0;
    else
        const т_мера ИндексУ = 1 + ИндексУ!( T, СписокТ[1 .. $] );
}


/**
 * Возвращает Кортеж с первым случаем T удалённым из СписокТ.
 */
template Удали( T, СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ Удали;
    else static if( is( T == СписокТ[0] ) )
        alias СписокТ[1 .. $] Удали;
    else
        alias Кортеж!( СписокТ[0], Удали!( T, СписокТ[1 .. $] ) ) Удали;
}


/**
 * Возвращает Кортеж со всеми случаями T удалёнными из СписокТ.
 */
template УдалиВсе( T, СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ УдалиВсе;
    else static if( is( T == СписокТ[0] ) )
        alias .УдалиВсе!( T, СписокТ[1 .. $] ) УдалиВсе;
    else
        alias Кортеж!( СписокТ[0], .УдалиВсе!( T, СписокТ[1 .. $] ) ) УдалиВсе;
}


/**
 * Возвращает Кортеж с первым случаем T заменённым на U.
 */
template Замени( T, U, СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ Замени;
    else static if( is( T == СписокТ[0] ) )
        alias Кортеж!(U, СписокТ[1 .. $]) Замени;
    else
        alias Кортеж!( СписокТ[0], Замени!( T, U, СписокТ[1 .. $] ) ) Замени;
}


/**
 * Возвращает Кортеж со всеми случаями T заменёнными на U.
 */
template ЗамениВсе( T, U, СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ ЗамениВсе;
    else static if( is( T == СписокТ[0] ) )
        alias Кортеж!( U, ЗамениВсе!( T, U, СписокТ[1 .. $] ) ) ЗамениВсе;
    else
        alias Кортеж!( СписокТ[0], ЗамениВсе!( T, U, СписокТ[1 .. $] ) ) ЗамениВсе;
}


/**
 * Возвращает Кортеж с типами из СписокТ декларированнымии в реверсном порядке.
 */
template Реверсни( СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ Реверсни;
    else
        alias Кортеж!( Реверсни!( СписокТ[1 .. $]), СписокТ[0] ) Реверсни;
}


/**
 * Возвращает Кортеж с удалением всех типов-дубликатов.
 */
template Уникум( СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ Уникум;
    else
        alias Кортеж!( СписокТ[0],
                      Уникум!( УдалиВсе!( СписокТ[0],
                                           СписокТ[1 .. $] ) ) ) Уникум;
}


/**
 * Возвращает тип из СписокТ, самый производный от T. Если такого типа
 * не найдено, тогда будет возвращён T.
 */
template ФинПроизводный( T, СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias T ФинПроизводный;
    else static if( is( СписокТ[0] : T ) )
        alias ФинПроизводный!( СписокТ[0], СписокТ[1 .. $] ) ФинПроизводный;
    else
        alias ФинПроизводный!( T, СписокТ[1 .. $] ) ФинПроизводный;
}


/**
 * Возвращает Кортеж с типоами, отсортированными так, что самые производные типы
 * упорядочены перед остальными типами.
 */
template ПроизводныеВперёд( СписокТ... )
{
    static if( СписокТ.length == 0 )
        alias СписокТ ПроизводныеВперёд;
    else
        alias Кортеж!( ФинПроизводный!( СписокТ[0], СписокТ[1 .. $] ),
                      ПроизводныеВперёд!( ЗамениВсе!( ФинПроизводный!( СписокТ[0], СписокТ[1 .. $] ),
                                                    СписокТ[0],
                                                    СписокТ[1 .. $] ) ) ) ПроизводныеВперёд;
}


/*
 * Краткий тест шаблонов, расположенных выше.
 */
static assert( 0 == ИндексУ!(цел, цел, плав, сим));
static assert( 1 == ИндексУ!(плав, цел, плав, сим));
static assert( 3 == ИндексУ!(дво, цел, плав, сим));

static assert( is( Удали!(цел, цел, плав, цел) == Удали!(проц, плав, цел) ) );
static assert( is( УдалиВсе!(цел, цел, плав, цел) == Удали!(проц, плав) ) );
static assert( is( Удали!(плав, цел, плав, цел) == Удали!(проц, цел, цел) ) );
static assert( is( Удали!(дво, цел, плав, цел) == Удали!(проц, цел, плав, цел) ) );

static assert( is( Замени!(цел, сим, цел, плав, цел) == Удали!(проц, сим, плав, цел) ) );
static assert( is( ЗамениВсе!(цел, сим, цел, плав, цел) == Удали!(проц, сим, плав, сим) ) );
static assert( is( Замени!(плав, сим, цел, плав, цел) == Удали!(проц, цел, сим, цел) ) );
static assert( is( Замени!(дво, сим, цел, плав, цел) == Удали!(проц, цел, плав, цел) ) );

static assert( is( Реверсни!(плав, плав[], дво, сим, цел) ==
                   Уникум!(цел, сим, дво, плав[], сим, цел, плав, дво) ) );

static assert( is( ФинПроизводный!(цел, дол, крат) == крат ) );