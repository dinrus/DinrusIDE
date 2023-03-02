﻿module math.linalg.config;

/***
Определен внутренний тип плав по умолчанию для названий типов без явных типовых суффиксов.
Напр., Вектор3 будет иметь компоненты типа т_плав, а Вектор3д всё ещё иметь компоненты типа
дво, поскольку суффикс "д" явно определяет внутренный тип.
*/
alias плав т_плав;

/** Значение, передаваемые по умолчанию функции "равны". */
const цел дефотнпрец = 16;
const цел дефабспрец = 16; /// описано
