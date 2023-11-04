// Контрол редактирования исходного кода Scintilla
/** @file ILoader.h
 ** Интерфейс для загрузки в документ Scintilla из фонового потока.
 **/
// Copyright 1998-2017 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef ILOADER_H
#define ILOADER_H

#include "Sci_Position.h"

namespace Scintilla {

class ILoader {
public:
	virtual int SCI_METHOD Release() = 0;
	// Returns a status code from SC_STATUS_*
	virtual int SCI_METHOD AddData(const char *data, Sci_Position length) = 0;
	virtual void * SCI_METHOD ConvertToDocument() = 0;
};

}

#endif
