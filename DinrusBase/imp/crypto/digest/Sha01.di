﻿/*******************************************************************************
        This module реализует common части of the SHA-0 и SHA-1 algoritms

*******************************************************************************/

module crypto.digest.Sha01;

private import stdrus;
private import crypto.digest.MerkleDamgard;

/*******************************************************************************

*******************************************************************************/

extern(D) package abstract class Sha01 : МерклеДамгард
{
        protected бцел[5]               контекст;
        private static const ббайт      padChar = 0x80;
        package static const бцел       маска = 0x0000000F;
    
        /***********************************************************************

                The дайджест размер of Sha-0 и Sha-1 is 20 байты

        ***********************************************************************/

        final бцел размерДайджеста() ;

        /***********************************************************************

                Initialize the cipher

                Примечания:
                Возвращает the cipher состояние в_ it's начальное значение

        ***********************************************************************/

        final protected override проц сбрось();

        /***********************************************************************

                Obtain the дайджест

                Возвращает:
                the дайджест

                Примечания:
                Возвращает дайджест of the текущ cipher состояние, this may be the
                final дайджест, либо a дайджест of the состояние between calls в_ обнови()

        ***********************************************************************/

        final protected override проц создайДайджест(ббайт[] буф);

        /***********************************************************************

                 блок размер

                Возвращает:
                the блок размер

                Примечания:
                Specifies the размер (в байтах) of the блок of данные в_ пароль в_
                each вызов в_ трансформируй(). For SHA0 the размерБлока is 64.

        ***********************************************************************/

        final protected override бцел размерБлока();

        /***********************************************************************

                Length паддинг размер

                Возвращает:
                the length паддинг размер

                Примечания:
                Specifies the размер (в байтах) of the паддинг which uses the
                length of the данные which имеется been ciphered, this паддинг is
                carried out by the падДлин метод. For SHA0 the добавьРазмер is 0.

        ***********************************************************************/

        final protected override бцел добавьРазмер() ;

        /***********************************************************************

                Pads the cipher данные

                Параметры:
                данные = a срез of the cipher буфер в_ заполни with паддинг

                Примечания:
                Fills the passed буфер срез with the appropriate паддинг for
                the final вызов в_ трансформируй(). This паддинг will заполни the cipher
                буфер up в_ размерБлока()-добавьРазмер().

        ***********************************************************************/

        final protected override проц падСооб(ббайт[] данные);

        /***********************************************************************

                Performs the length паддинг

                Параметры:
                данные   = the срез of the cipher буфер в_ заполни with паддинг
                length = the length of the данные which имеется been ciphered

                Примечания:
                Fills the passed буфер срез with добавьРазмер() байты of паддинг
                based on the length в байтах of the ввод данные which имеется been
                ciphered.

        ***********************************************************************/

        final protected override проц падДлин(ббайт[] данные, бдол length);


        /***********************************************************************

        ***********************************************************************/

        protected static бцел f(бцел t, бцел B, бцел C, бцел D);

        /***********************************************************************

        ***********************************************************************/

        protected static const бцел[] К =
        [
                0x5A827999,
                0x6ED9EBA1,
                0x8F1BBCDC,
                0xCA62C1D6
        ];

}
