﻿module io.device.TempFile;

import Путь = io.Path;
import math.random.Kiss : Kiss;
import io.device.Device : Устройство;
import io.device.File;

/******************************************************************************
 *
 * Класс ВремФайл стремится предоставить безопасный способ создания и удаления
 * временных файлов. Класс ВремФайл автоматически закроет временные файлы,
 * когда этот объект разрушен, поэтому рекомендуется выполнить соответствующие
 * деструкции в масштабе scope (exit).
 *
 * Временные файлы создаются в одном из нескольких стилей, во многом подобно
 * обычным файлам. Стили ВремФайл имеют следующие свойства:
 *
 * $(UL
 * $(LI $(B ОпцУдаления): Этим определяется, будет ли файл удалён сразу же
 * после его закрытия (transient,) или продолжит существование даже после
 * того, как приложение завершило работу (permanent.))
 * )
 *
 * В последующем этот класс можно развернуть, чтобы добавить больший контроль
 * над свойствами временных файлов.
 *
 * При типичном использовании ( создания временного файла для вмешения 
 * сохраняемых данных) достаточно следующего:
 *
 * -----
 *  {
 *      scope temp = new ВремФайл;
 *      
 *      // Используется temp как обычный провод; он будет закрыт автоматически,
 *      // когда выйдет из масштаба (scope).
 *  }
 * -----
 *
 * $(B Важно):
 * Рекомендуется $(I НЕ) использовать файлы, создаваемые этим классом для
 * хранения необходимой информации. С текущей реализацией замечено несколько
 * проблем, которые позволяют злоумышленникам получать доступ к этим
 * временным файлам.
 *
 * $(B Todo): Детализировать свойства безопасности и гарантии.
 *
 ******************************************************************************/


extern(D) 
class ВремФайл : Файл
{

    /**************************************************************************
     * 
     * Этот перечень используется для контролирования персистентности
	 * временного файла после удаления объекта ВремФайл.
     *
     **************************************************************************/

    enum ОпцУдаления : ббайт
    {
        /**
         * Временный файл должен удаляться вместе с объектом-хозяином.
         */
        ВКорзину,
        /**
         * Временный файл должен оставаться полсле удаления объекта.
         */
        Навсегда
    }

    /**************************************************************************
     * 
     * Эта структура используется для определения того, как временные файлы
     * должны будут открываться и использоваться.
     *
     **************************************************************************/
    align(1) struct СтильВремфл
    {
        //Visibility visibility;      ///
        ОпцУдаления удаление;        ///
        //Sensitivity sensitivity;    ///
        //Общ совместно;                ///
        //Кэш кэш;                ///
        ббайт попытки = 10;          ///
    }

    /**
     * СтильВремфл для создания транзитивного временного файла, к которому
     * есть доступ только у текущего пользователя.
     */
    static const СтильВремфл ВКорзину = {ОпцУдаления.ВКорзину};
    /**
     * СтильВремфл для создания перманентного временного файла, к которому
     * есть доступ только у текущего пользователя.
     */
    static const СтильВремфл Навсегда = {ОпцУдаления.Навсегда};


    ///
    this(СтильВремфл стиль = СтильВремфл.init);

    ///
    this(ткст префикс, СтильВремфл стиль = СтильВремфл.init);

    СтильВремфл стильВремфл();

    static ткст времфлПуть();
   
    override проц открепи();
}