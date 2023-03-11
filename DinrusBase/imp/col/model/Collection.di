﻿module col.model.Collection;

public import col.model.Iterator;

/**
 * Интерфейс собраний (коллекций) определяет основной интерфейс для всех коллекций.
 *
 * Базовая коллекция должна позволять обход всех её элементов, сообщать о том,
 * содержится ли в ней данный элемент, а также удалять элементы. Добавление элементов
 * в данном случае не поддерживается, поскольку элементы не всегда добавляются простым
 * приёмом. Например, карта (мэп) требует добавления в обходчик и ключа, и самого элемента.
 */
interface Коллекция(З) : Обходчик!(З), Чистящий!(З) 
{
    /**
     *Очистить контейнер от всех значений
     */
    Коллекция!(З) зачисть();

    /**
     * Удалить элемент с определённым значением. Это может быть операцией O(n).
     * Если коллекция с ключами, первый элемент, чьё значение совпадает,
     * будет удалён.
     *
     * Возвращает this.
     */
    Коллекция!(З) удали(З з);

    /**
     * Удалить элемент с определённым значением. Это может быть операцией O(n).
     * Если коллекция с ключами, первый элемент, чьё значение совпадает,
     * будет удалён.
     *
     * Возвращает this.
     *
     * Устанавливает былУдалён в да, если этот элемент существовал и был удалён.
     */
    Коллекция!(З) удали(З з, ref бул былУдалён);

    /**
     * Возвращает да, если эта коллекция содержит данное значение. Может быть O(n).
     */
    бул содержит(З з);

    /**
     * Делает копию этой коллекции. Глубокая копия элементов не выполняется,
     * если они представляют собой ссылки или указательный тип.
     */
    Коллекция!(З) dup();
}