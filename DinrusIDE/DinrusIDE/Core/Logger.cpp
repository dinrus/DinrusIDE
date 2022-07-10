#include "Logger.h"

#include <chrono>

using namespace РНЦП;

Logger::Logger(LoggingLevel level, const Ткст& tag)
	: level(level)
	, tag(tag)
{
}

Logger::~Logger()
{
	Log();
}

void Logger::Log()
{
	ТкстПоток ss;
	
	Ткст output = outputStream.дайРез();
	if (output.пустой())
		return;
	
	ss << GetCurrentTime();
	ss << " " << level;
	if (!tag.пустой())
	{
		ss << "/" << tag;
	}
	ss << " " << output;
	
	VppLog() << ss.дайРез() << EOL;
}

Ткст Logger::GetCurrentTime()
{
	using namespace std::chrono;
	using days = duration<int, std::ratio_multiply<hours::period, std::ratio<24>>::type>;
	
	auto tp = system_clock::now().time_since_epoch();
	tp += minutes(дайЧПояс());
	
	auto d  = duration_cast<days>(tp);    tp -= d;
	auto h  = duration_cast<hours>(tp);   tp -= h;
	auto m  = duration_cast<minutes>(tp); tp -= m;
	auto s  = duration_cast<seconds>(tp); tp -= s;
	auto ms = duration_cast<milliseconds>(tp);
	
	return фмт("%02d:%02d:%02d:%03d", (int)h.count(), (int)m.count(), (int)s.count(), (int)ms.count());
}

Поток& РНЦП::operator<<(Поток& s, Logger::LoggingLevel level)
{
	switch(level)
	{
		case (Logger::LoggingLevel::DEBUG):
			return s << "DEBUG";
		case (Logger::LoggingLevel::INFO):
			return s << "ИНФО ";
		case (Logger::LoggingLevel::WARN):
			return s << "WARN ";
		case (Logger::LoggingLevel::ERROR_LEVEL):
			return s << "Ошибка";
		case (Logger::LoggingLevel::NONE):
			return s << "NONE ";
	}
	return s;
}
