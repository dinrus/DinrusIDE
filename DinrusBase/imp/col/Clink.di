﻿/*******************************************************************************
        Based upon Doug Lea's Java collection package
*******************************************************************************/

module col.Clink;

/*******************************************************************************

        ЦСвязки - это линки, всегда организуемые в циркулярные списки.

*******************************************************************************/

struct ЦСвязка (З)
{
        alias ЦСвязка!(З)    Тип;
        alias Тип         *Реф;

        Реф     предш,           // указатель на предш
                следщ;           // указатель на следщ
        З       значение;          // значение элемента

        /***********************************************************************

                 Установить значение в точку относительно самого себя.
                        
        ***********************************************************************/

        Реф установи (З знач)
        {
                return установи (знач, this, this);
        }

        /***********************************************************************

                 Установить в точку, где n как следщ ячейка и p как предш ячейка.

                 Параметр: n, новая следщ ячейка
                 Параметр: p, навая предш ячейка
                        
        ***********************************************************************/

        Реф установи (З знач, Реф p, Реф n)
        {
                значение = знач;
                предш = p;
                следщ = n;
                return this;
        }

        /**
         * Возвращает да, если текущ ячейка является единственной на этот список.
        **/

        бул синглтон()
        {
                return следщ is this;
        }

        проц вяжиСледщ (Реф p)
        {
                if (p)
                   {
                   следщ.предш = p;
                   p.следщ = следщ;
                   p.предш = this;
                   следщ = p;
                   }
        }

        /**
         * Создаёт ячейку, содержащую знач и компонует её сразу же после текущ ячейки.
        **/

        проц добавьСледщ (З знач, Реф delegate() размести)
        {
                auto p = размести().установи (знач, this, следщ);
                следщ.предш = p;
                следщ = p;
        }

        /**
         * Создаёт узел, содержащий знач, компонует его перед текущ ячейка, и возвращает его.
		 **/

        Реф добавьПредш (З знач, Реф delegate() размести)
        {
                auto p = предш;
                auto c = размести().установи (знач, p, this);
                p.следщ = c;
                предш = c;
                return c;
        }

        /**
         * Компонует p перед текущ ячейкой.
        **/

        проц вяжиПредш (Реф p)
        {
                if (p)
                   {
                   предш.следщ = p;
                   p.предш = предш;
                   p.следщ = this;
                   предш = p;
                   }
        }

        /**
         * Возвращает число ячеек в этом списке.
        **/

        цел размер()
        {
                цел c = 0;
                auto p = this;
                do {
                   ++c;
                   p = p.следщ;
                   } while (p !is this);
                return c;
        }

        /**
         * Возвращает первый ячейку, содержащую элемент, найденный при круговом обходе,
         * начиная с текущ ячейки, либо пусто, если таковая отсутствует.
        **/

        Реф найди (З элемент)
        {
                auto p = this;
                do {
                   if (элемент == p.значение)
                       return p;
                   p = p.следщ;
                   } while (p !is this);
                return пусто;
        }

        /**
         * Возвращает число ячеек, содержащих элемент, найденное
		 * при круговом обходе.
        **/

        цел счёт (З элемент)
        {
                цел c = 0;
                auto p = this;
                do {
                   if (элемент == p.значение)
                       ++c;
                   p = p.следщ;
                   } while (p !is this);
                return c;
        }

        /**
         * Возвращает н_ую ячейку, траверсируя отсюда. Она может обматываться.
        **/

        Реф н_ый (цел n)
        {
                auto p = this;
                for (цел i = 0; i < n; ++i)
                     p = p.следщ;
                return p;
        }


        /**
         * Отвязать следщ ячейку.
         * Это не влияет на данный список при синглтон_ли().
        **/

        проц отвяжиСледщ ()
        {
                auto nn = следщ.следщ;
                nn.предш = this;
                следщ = nn;
        }

        /**
         * Отвязать предш ячейку.
         * Это не влияет на данный список при синглтон_ли().
        **/

        проц отвяжиПредш ()
        {
                auto pp = предш.предш;
                pp.следщ = this;
                предш = pp;
        }


        /**
         * Отвязать this от списка, в котором он находится.
         * Делает его синглтоном.
        **/

        проц отвяжи ()
        {
                auto p = предш;
                auto n = следщ;
                p.следщ = n;
                n.предш = p;
                предш = this;
                следщ = this;
        }

        /**
         * Создаёт копию этого списка и возвращает новую голову. 
        **/

        Реф копируйСписок (Реф delegate() размести)
        {
                auto hd = this;

                auto новый_список = размести().установи (hd.значение, пусто, пусто);
                auto текущ = новый_список;

                for (auto p = следщ; p !is hd; p = p.следщ)
                     {
                     текущ.следщ = размести().установи (p.значение, текущ, пусто);
                     текущ = текущ.следщ;
                     }
                новый_список.предш = текущ;
                текущ.следщ = новый_список;
                return новый_список;
        }
}


