module sys.com.activex;
import sys.common, tpl.args, stdrus;

alias sys.WinStructs.ВАРИАНТ АРГВАРИАНТА;
alias sys.WinIfaces.ITypeInfo ИИнфОТипе;
alias sys.WinStructs.ВАРИАНТ Вар, Варарг;

АРГВАРИАНТА вар(...);

extern(D):

class ИсклАктивОбъ: Исключение
{
 this(ткст сооб);
}

class АктивОбъ
{
    this(ткст имяПриложения);
    ~this();
    проц загрузиСостав();
    проц загрузиСостав(ИИнфОТипе иот);
	проц покажиСостав();	
    ВАРИАНТ дай(ткст член);
    проц установи(ткст член, АРГВАРИАНТА арг);
    проц установиПоСсыл(ткст член, АРГВАРИАНТА арг);
    ВАРИАНТ вызови(ткст член,...);
}
АктивОбъ объАктив(ткст арг);

