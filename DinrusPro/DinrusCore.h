#ifndef _DinrusPro_DinrusCore_h_
#define _DinrusPro_DinrusCore_h_

#include <DinrusPro/DinrusPro.h>
#include <DinrusPro/Funcs.h>
#include <DinrusPro/Tpl.h>
#include <DinrusPro/NonTpl.h>


#define СОВМЕСТИМОСТЬ_С_БИ_СТЛ(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	проц                  clear()                { очисть(); } \
	size_type             size() const           { return дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	бул                  empty() const          { return пустой(); } \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \

#define СОВМЕСТИМОСТЬ_С_МАП_СТЛ(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return B::begin(); } \
	const_iterator        стоп() const            { return B::end(); } \
	проц                  clear()                { B::очисть(); } \
	size_type             size() const           { return B::дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	бул                  empty() const          { return B::пустой(); } \
	iterator              старт()                { return B::begin(); } \
	iterator              стоп()                  { return B::end(); } \

#define СОВМЕСТИМОСТЬ_С_ОТСОРТМАП_СТЛ(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	проц                  clear()                { B::очисть(); } \
	size_type             size() const           { return B::дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	бул                  empty() const          { return B::дайСчёт() == 0; } \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \

#define СОВМЕСТИМОСТЬ_С_ВЕКТОРОМ_СТЛ(C) \
	typedef T             value_type; \
	typedef КонстОбходчик const_iterator; \
	typedef const T&      const_reference; \
	typedef цел           size_type; \
	typedef цел           difference_type; \
	const_iterator        старт() const          { return begin(); } \
	const_iterator        стоп() const            { return end(); } \
	проц                  clear()                { очисть(); } \
	size_type             size() const           { return дайСчёт(); } \
	typedef Обходчик      iterator; \
	typedef T&            reference; \
	iterator              старт()                { return begin(); } \
	iterator              стоп()                  { return end(); } \
	reference             front()                { return (*this)[0]; } \
	const_reference       front() const          { return (*this)[0]; } \
	reference             back()                 { return верх(); } \
	const_reference       back() const           { return верх(); } \
	бул                  empty() const          { return пустой(); } \
	проц                  push_back(const T& x)  { добавь(x); } \
	проц                  pop_back()             { сбрось(); } \

//////////////////////////////

#define E__Value(I)   Значение p##I
#define E__Ref(I)     Реф p##I

// ---------------------- Значение Массив

#include "Value.hpp"
#include "ValueUtil.hpp"
///////////////////////
#include <DinrusPro/Core/Obsolete.h>
#include <DinrusPro/Core/FixedMap.h>
#include <DinrusPro/Core/SplitMerge.h>
#include <DinrusPro/Core/Tuple.h>
#include <DinrusPro/Core/Util.h>
#include <DinrusPro/Core/Profile.h>
#include <DinrusPro/Core/FilterStream.h>
#include <DinrusPro/Core/Format.h>
#include <DinrusPro/Core/z.h>

#include <DinrusPro/Core/XML.h>
#include <DinrusPro/Core/Xmlize.h>

#include <DinrusPro/Core/Gtypes.h>
#include <DinrusPro/Core/i18n.h>
#include <DinrusPro/Core/Topic.h>


#include <DinrusPro/Core/LocalProcess.h>

#include <DinrusPro/Core/Inet.h>

#include <DinrusPro/Core/Win32Util.h>

#include <DinrusPro/Core/Vcont.hpp>
#include <DinrusPro/Core/Index.hpp>
#include <DinrusPro/Core/Map.hpp>
#include <DinrusPro/Core/ВхоВектор.hpp>
#include <DinrusPro/Core/InMap.hpp>

#include <DinrusPro/Core/Хьюдж.h>


#endif
