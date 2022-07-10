#include "Painter.h"

namespace РНЦП {

void LinearPathConsumer::стоп()
{
}

void LinearPathFilter::стоп()
{
	PutEnd();
}

void Transformer::Move(const ТочкаПЗ& p)
{
	PutMove(xform.Transform(p));
}

void Transformer::Строка(const ТочкаПЗ& p)
{
	PutLine(xform.Transform(p));
}

}
