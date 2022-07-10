#include "Painter.h"

namespace РНЦП {

void БуфРисовало::BeginOnPathOp(double q, bool абс)
{
	if(onpath.дайСчёт() == 0)
		RenderPath(ONPATH, Null, обнулиКЗСА());
	старт();
	if(pathlen > 0) {
		if(!абс)
			q *= pathlen;
		ТочкаПЗ pos(0, 0);
		for(int i = 0; i < onpath.дайСчёт(); i++) {
			PathLine& l = onpath[i];
			if(l.len > 0 && (l.len > q || q >= 1.0 && i == onpath.дайСчёт() - 1)) {
				ТочкаПЗ v = l.p - pos;
				Translate(q / l.len * v + pos);
				Rotate(Bearing(v));
				break;
			}
			q -= l.len;
			pos = l.p;
		}
	}
	attrstack.верх().onpath = true;
	onpathstack.добавь() = pick(onpath);
	pathlenstack.добавь(pathlen);
	onpath.очисть();
	pathlen = 0;
}

}
