﻿module col.model.BagView;

private import col.model.View;

/**
 *
 * Рюкзак (Bag) - это коллекция, поддерживающая множество одинаковых элементов.
 *
**/

public interface ОбзорРюкзака(З) : Обзор!(З)
{
    public override ОбзорРюкзака!(З) дубликат();
    public alias дубликат dup;

    version (VERBOSE)
    {
        public alias добавим opCat;

        /**
         * Конструирует новый Рюкзак, клон этого, за исключением того, что
         * в нём будет указанный элемент. Это можно использовать
         * для создания серии Рюкзаков, отличающихся друго от друга только
         * тем, что содержат дополнительные элементы.
         *
         * @param добавляемый в новый рюкзак элемент
         * Возвращает: новый Рюкзак c, совпадающий с этим, за исключением
         * c.случаи(элемент) == случаи(элемент)+1
         * Выводит исключение: ИсклНедопустимыйЭлемент, если !можноВключить(элемент)
        **/

        public Рюкзак добавим(З элемент);

        /**
         * Конструирует новую Коллекция, клон этой, за исключением того, что
         * дабавлен указанный элемент, если его ещё не было до этого.
		 * Это можно использовать для создания серии коллекций, отличающихся
         * друг от друга только тем, что содержат дополнительные элементы.
         *
         * @param элемент элемент, включаемый в новую коллекцию
         * Возвращает: новую коллекцию c, совпадающую с этой, за исключением
         * c.случаи(элемент) = мин(1, случаиЭлемента)
         * Выводит исключение: ИсклНедопустимыйЭлемент if !можноВключить(элемент)
        **/

        public Рюкзак добавимЕслиНет(З элемент);
    } // version

}
