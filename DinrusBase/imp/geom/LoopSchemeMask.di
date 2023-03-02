﻿
module geom.LoopSchemeMask;

import tpl.singleton;


/** Реализует кэш для весов оригинальных схем цикла (Loop scheme),
    поддерживает:
    $(UL 
      $(LI правило проекции вершины на следующем уровне)
      $(LI правило проекции вершины на органиченной поверхности)
      $(LI правило проекции вершины на  k-ном (level) шаге (Barthe, Kobbelt'2003))
      $(LI тангенсы вершины на ограниченной поверхности)
    )
*/
class МаскаСхемыЦикла(T_, бцел разм_кэша_ = 100)
{
public:
    enum { разм_кэша = разм_кэша_ };
    alias T_                               Скаляр;

protected:

    Скаляр[разм_кэша]                       веса_проекц_;
    Скаляр[разм_кэша]                       веса_пределов_;
    Скаляр[разм_кэша]                       веса_шага_;
    Скаляр[][разм_кэша]                     веса_танг0_;
    Скаляр[][разм_кэша]                     веса_танг1_;

protected:

    static Скаляр                      вычисли_вес_проекц(бцел _валентность);
    static Скаляр                      вычисли_вес_предела(бцел _валентность);
    static Скаляр                      вычисли_вес_шага(бцел _валентность);
    static Скаляр                      вычисли_вес_танг0(бцел _валентность, бцел _ид_вер);
    static Скаляр                      вычисли_вес_танг1(бцел _валентность, бцел _ид_вер);

    проц                                      кэшируй_веса();

public:

    this();

    Скаляр                             вес_проекц(бцел _валентность) ;
    Скаляр                             вес_предела(бцел _валентность) ;
    Скаляр                             вес_шага(бцел _валентность, бцел _шаг) ;
    Скаляр                             вес_танг0(бцел _валентность, бцел _ид_вер) ;
    Скаляр                             вес_танг1(бцел _валентность, бцел _ид_вер) ;
    проц                                      дамп(бцел _макс_валентность = разм_кэша - 1) ;
}

alias МаскаСхемыЦикла!(дво, 100)        МаскаСхемыЦиклаДво;
alias Синглтон!(МаскаСхемыЦиклаДво)    МаскаСхемыЦиклаСинглтон;