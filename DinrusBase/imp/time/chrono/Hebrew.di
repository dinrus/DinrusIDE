﻿module time.chrono.Hebrew;
private import time.chrono.Calendar;



/**
 * $(ANCHOR _Hebrew)
 * Представляет Hebrew Календарь.
 */
extern(D) class Еврейский : Календарь {

  /**
   * Представляет текущ эра.
   */
  public const бцел ЕВРЕЙСКИЙ_ЭРА = 1;

  /**
   * Переписано. Возвращает Значение Времени, устновленное в заданную дата и время в указанном _эра.
   * Параметры:
   *   год = Целое, представляющее _год.
   *   месяц = Целое, представляющее _месяц.
   *   день = Целое, представляющее _день.
   *   час = Целое, представляющее _час.
   *   минута = Целое, представляющее _минута.
   *   секунда = Целое, представляющее _секунда.
   *   миллисекунда = Целое, представляющее _миллисекунда.
   *   эра = Целое, представляющее _эра.
   * Возвращает: Время, установленное в указанную дата и время.
   */
   override Время воВремя(бцел год, бцел месяц, бцел день, бцел час, бцел минута, бцел секунда, бцел миллисекунда, бцел эра) ;

  /**
   * Переписано. Возвращает день недели в указанном Время.
   * Параметры: время = Значение Времени.
   * Возвращает: Значение ДняНедели, представляющее день недели времени.
   */
   override ДеньНедели дайДеньНедели(Время время) ;

  /**
   * Переписано. Возвращает день месяца в указанном Время.
   * Параметры: время = Значение Времени.
   * Возвращает: Целое, представляющее день месяца времени.
   */
   override бцел дайДеньМесяца(Время время) ;

  /**
   * Переписано. Возвращает день годв в указанном Время.
   * Параметры: время = Значение Времени.
   * Возвращает: Целое, представляющее день годв времени.
   */
   override бцел дайДеньГода(Время время);

  /**
   * Переписано. Возвращает месяц в указанном Время.
   * Параметры: время = Значение Времени.
   * Возвращает: Целое, представляющее месяц во времени.
   */
   override бцел дайМесяц(Время время);

  /**
   * Переписано. Возвращает год в указанном Время.
   * Параметры: время = Значение Времени.
   * Возвращает: Целое, представляющее год во времени.
   */
   override бцел дайГод(Время время);

  /**
   * Переписано. Возвращает эра в указанном Время.
   * Параметры: время = Значение Времени.
   * Возвращает: Целое, представляющее эру во времени.
   */
   override бцел дайЭру(Время время) ;

  /**
   * Переписано. Возвращает число дней в указанном _год и _месяц указанного _эра.
   * Параметры:
   *   год = Целое, представляющее _год.
   *   месяц = Целое, представляющее _месяц.
   *   эра = Целое, представляющее _эра.
   * Возвращает: Число дней в указанном _год и _месяц указанного _эра.
   */
   override бцел дайДниМесяца(бцел год, бцел месяц, бцел эра) ;

  /**
   * Переписано. Возвращает число дней в указанном _год указанного _эра.
   * Параметры:
   *   год = Целое, представляющее _год.
   *   эра = Целое, представляющее _эра.
   * Возвращает: The число дней в указанном _год в указанном _эра.
   */
   override бцел дайДниГода(бцел год, бцел эра) ;

  /**
   * Переписано. Возвращает число месяцев в указанном _год указанного _эра.
   * Параметры:
   *   год = Целое, представляющее _год.
   *   эра = Целое, представляющее _эра.
   * Возвращает: The число месяцев в указанном _год в указанном _эра.
   */
   override бцел дайМесяцыГода(бцел год, бцел эра);

  /**
   * Переписано. Показывает, является ли указанный _год в указанном _эра високосным _год.
   * Параметры: год = Целое, представляющее _год.
   * Параметры: эра = Целое, представляющее _эра.
   * Возвращает: да, указанный _год високосный _год; иначе, нет.
   */
   override бул високосен_ли(бцел год, бцел эра);

  /**
   * $(I Свойство.) Переписано. Получает список эр в текущ Календарь.
   * Возвращает: Массив целых чисел, представляющий эру в текущ Календарь.
   */
   override бцел[] эры();

  /**
   * $(I Свойство.) Переписано. Выводит определитель, связанный с текущ Календарь.
   * Возвращает: Целое, представляющее определитель текущ Календарь.
   */
   override бцел ид() ;

 // private проц проверьГод(бцел год, бцел эра) ;

 // private бцел дайТипГода(бцел год);

 // private бцел дайНачалоГода(бцел год);

 // private Время дайГрегорианскоеВремя(бцел год, бцел месяц, бцел день, бцел час, бцел минута, бцел секунда, бцел миллисекунда) ;

}

