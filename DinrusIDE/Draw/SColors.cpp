#include "Draw.h"

namespace РНЦП {

CH_COLOR(SBlack, AdjustIfDark(чёрный()));
CH_COLOR(SGray, AdjustIfDark(серый()));
CH_COLOR(SLtGray, AdjustIfDark(светлоСерый()));
CH_COLOR(SWhiteGray, AdjustIfDark(белоСерый()));
CH_COLOR(SWhite, AdjustIfDark(белый()));
CH_COLOR(SRed, AdjustIfDark(красный()));
CH_COLOR(SGreen, AdjustIfDark(зелёный()));
CH_COLOR(SBrown, AdjustIfDark(коричневый()));
CH_COLOR(SBlue, AdjustIfDark(синий()));
CH_COLOR(SMagenta, AdjustIfDark(магента()));
CH_COLOR(SCyan, AdjustIfDark(цыан()));
CH_COLOR(SYellow, AdjustIfDark(жёлтый()));
CH_COLOR(SLtRed, AdjustIfDark(светлоКрасный()));
CH_COLOR(SLtGreen, AdjustIfDark(светлоЗелёный()));
CH_COLOR(SLtYellow, AdjustIfDark(светлоЖёлтый()));
CH_COLOR(SLtBlue, AdjustIfDark(светлоСиний()));
CH_COLOR(SLtMagenta, AdjustIfDark(светлоМагента()));
CH_COLOR(SLtCyan, AdjustIfDark(светлоЦыан()));

bool dark_theme__;

#define CH_END } // to avoid } highlighting problem

CH_VAR0(ChColor, Цвет, SColorPaper, белый())
	dark_theme__ = тёмен(SColorPaper());
	Iml::ResetAll();
CH_END

CH_COLOR(SColorFace, SLtGray());
CH_COLOR(SColorText, SBlack());
CH_COLOR(SColorHighlight, SBlue());
CH_COLOR(SColorHighlightText, SWhite());
CH_COLOR(SColorMenu, SLtGray());
CH_COLOR(SColorMenuText, SBlack());
CH_COLOR(SColorInfo, SLtYellow());
CH_COLOR(SColorInfoText, SBlack());
CH_COLOR(SColorDisabled, SGray());
CH_COLOR(SColorLight, SWhite());
CH_COLOR(SColorShadow, SGray());
CH_COLOR(SColorMark, тёмен(SColorPaper()) ? тёмнаяТема(светлоСиний()) : светлоСиний());
CH_COLOR(SColorMenuMark, тёмен(SColorMenu()) ? тёмнаяТема(светлоСиний()) : светлоСиний());

CH_COLOR(SColorLtFace, смешай(SColorFace, SColorLight));
CH_COLOR(SColorDkShadow, смешай(SColorShadow, SBlack));
CH_COLOR(SColorLabel, SColorText());

}
