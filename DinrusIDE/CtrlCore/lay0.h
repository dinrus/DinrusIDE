//#BLITZ_APPROVE

#define LAYOUT(имя, x, y)       struct имя##__layid {};
#define UNTYPED(variable, param)
#define ЭЛТ(classname, var, param)
#define END_LAYOUT

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ЭЛТ
#undef  END_LAYOUT

#define LAYOUT(имя, x, y)       template<class T> \
	                             struct With##имя : public T, public имя##__layid { \
										static РНЦП::Размер GetLayoutSize() { return РНЦП::Ктрл::LayoutZoom(x, y); }
#define UNTYPED(variable, param)
#define ЭЛТ(classname, var, param)     classname var;
#define END_LAYOUT               };

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ЭЛТ
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template<class T> inline void SetLayout_##nm(T& parent, bool add = false, bool show = false) { \
                                  SetLayout_Size(parent, РНЦП::Zx(x), РНЦП::Zy(y));
#define UNTYPED(var, param)       parent.var.param; if(add) parent.добавь(parent.var); if(show) parent.var.покажи();
#define ЭЛТ(clss, var, param)    UNTYPED(var, param);
#define END_LAYOUT             }

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ЭЛТ
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template<class T, class D> inline void SetLayout_##nm(T& ctrl, D& parent, bool add = false, bool show = false) { \
                                  SetLayout_Size(ctrl, РНЦП::Zx(x), РНЦП::Zy(y));
#define UNTYPED(var, param)       parent.var.param; if(add) ctrl.добавь(parent.var); if(show) parent.var.покажи();
#define ЭЛТ(clss, var, param)    UNTYPED(var, param);
#define END_LAYOUT             }

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ЭЛТ
#undef  END_LAYOUT

#define LAYOUT(nm, x, y)       template <class L, class D> \
                               void InitLayout(РНЦП::Ктрл& parent, L& layout, D& uts, nm##__layid&) { \
                                  parent.LayoutId(#nm);
#define UNTYPED(var, param)       uts.var.param; uts.var.LayoutId(#var); parent.добавь(uts.var);
#define ЭЛТ(clss, var, param)    layout.var.param; layout.var.LayoutId(#var); parent.добавь(layout.var);
#define END_LAYOUT             }

#include LAYOUTFILE

#undef  LAYOUT
#undef  UNTYPED
#undef  ЭЛТ
#undef  END_LAYOUT

#undef  LAYOUTFILE
