#include "umake.h"

void Иср::ConsoleShow()
{
}

void Иср::ConsoleSync()
{
}

Вектор<Ткст> Иср::PickErrors()
{
	return console.PickErrors();
}

void Иср::DoProcessСобытиеs()
{
}

void Иср::ReQualifyCodeBase()
{
}

Ткст Иср::GetMain()
{
	return main;
}

void Иср::BeginBuilding(bool clear_console)
{
	SetupDefaultMethod();
	SetHdependDirs();
	HdependTimeDirty();
	build_time = msecs();
}

void Иср::EndBuilding(bool ok)
{
	console.EndGroup();
	console.жди();
	Вектор<Ткст> errors = console.PickErrors();
	for(Ткст p: errors)
		DeleteFile(p);
	if(!errors.пустой())
		ok = false;
	вКонсоль("");
	вКонсоль((ok ? "OK. " : "There were errors. ") + GetPrintTime(build_time));
}

void Иср::ConsoleClear()
{
}

void Иср::SetupDefaultMethod()
{
}

void Иср::ClearErrorEditor()
{
}

void Иср::SetErrorEditor()
{
}

