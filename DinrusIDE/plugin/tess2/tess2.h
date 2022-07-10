#ifndef _plugin_tessel_tessel_h_
#define _plugin_tessel_tessel_h_

#include <Core/Core.h>

namespace РНЦП {

void Tesselate(const Вектор<Вектор<ТочкаПЗ>>& shape, Вектор<ТочкаПЗ>& vertex, Вектор<Кортеж<int, int, int>>& triangle, bool evenodd = false);
void Tesselate(const Вектор<Вектор<ТочкаПЗ>>& shape, Вектор<ТочкаПЗ>& vertex, Вектор<int>& Индекс, bool evenodd = false);

};

#endif
