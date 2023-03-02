﻿module col.model.MapX;

private import  col.model.MapView,
        col.model.Dispenser;

/**
 *
 * MutableMap поддерживает стандартные операции обновления карт.
 *
**/

public interface Карта(К, T) : ОбзорКарты!(К, T), Расходчик!(T)
{
    public override Карта!(К,T) дубликат();
    public alias дубликат dup;
	
    /**
     * Включает указанную пару в Карту.
     * Если имелась другая пара с таким же ключом,
     * она заменяется новой парой.
     *
     * @param ключ - ключ для включаемого элемента
     * @param элемент - включаемый элемент
     * Возвращает: condition:
     * <PRE>
     * имеется(ключ, элемент) &&
     * без эффекта &&
     * Версия меняется, если !PREV(this).содержит(ключ, элемент))
     * </PRE>
    **/

    public проц добавь (К ключ, T элемент);

    /**
     * Включает указанную пару в Карту.
     * Если имелась другая пара с таким же ключом,
     * она заменяется новой парой.
     *
     * @param элемент включаемый элемент
     * @param ключ ключ для включаемого элемента
     * Возвращает: условие:
     * <PRE>
     * имеется(ключ, элемент) &&
     * без эффекта &&
     * Версия меняется, если !PREV(this).содержит(ключ, элемент))
     * </PRE>
    **/

    public проц opIndexAssign (T элемент, К ключ);


    /**
     * Удаляет пару с заданным ключом.
     * @param  ключ ключ
     * Возвращает: условие:
     * <PRE>
     * !содержитКлюч(ключ)
     * foreach (ключ in ключи()) at(ключ).равно(PREV(this).at(ключ)) &&
     * foreach (ключ in PREV(this).ключи()) (!ключ.равно(ключ)) --> at(ключ).равно(PREV(this).at(ключ))
     * (version() != PREV(this).version()) ==
     * содержитКлюч(ключ) !=  PREV(this).содержитКлюч(ключ))
     * </PRE>
    **/

    public проц удалиКлюч (К ключ);


    /**
     * Заменяет старую пару на новую с тем же ключом.
     * Действие отсутствует, когда пары нет. (То же самое,
     * если ключ есть, но привязан к другому значению.)
     * @param ключ ключ к удаляемой паре
     * @param старЭлемент существующий элемент
     * @param новЭлемент значение, которое заменит предыдущий параметр
     * Возвращает: условие:
     * <PRE>
     * !содержит(ключ, старЭлемент) || содержит(ключ, новЭлемент);
     * эффект отсутствует &&
     * Версия меняется, если PREV(this).содержит(ключ, старЭлемент))
     * </PRE>
    **/

    public проц замениПару (К ключ, T старЭлемент, T новЭлемент);
}

