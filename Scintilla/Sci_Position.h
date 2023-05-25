// Контрол редактирования исходного кода Scintilla
/** @file Sci_Position.h
 ** Определяет тип Sci_Position, используемый во внешних интерфейсах Scintilla'ы.
 ** Требуется для клиентского кода, написанного на Си, поэтому не входит в C++ namespace.
 **/
// Copyright 2015 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SCI_POSITION_H
#define SCI_POSITION_H

#include <stddef.h>

// Базовый тип со знаком, используемый по всему интерфейсу
typedef ptrdiff_t Sci_Position;

// Беззначный вариант для ILexer::Lex и ILexer::Fold
typedef size_t Sci_PositionU;

// Для Sci_CharacterRange, определённого как long, и для совместимости с Win32 CHARRANGE
typedef intptr_t Sci_PositionCR;

#ifdef _WIN32
	#define SCI_METHOD __stdcall
#else
	#define SCI_METHOD
#endif

#endif
