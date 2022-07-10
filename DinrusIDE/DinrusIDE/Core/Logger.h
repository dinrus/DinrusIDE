#ifndef _Logger_Logger_h
#define _Logger_Logger_h

#include <Core/Core.h>

#define UPP_FUNCTION_NAME Ткст(__func__)
#define UPP_METHOD_NAME(ClassName) \
	Ткст(ClassName) << "::" << UPP_FUNCTION_NAME << "(this = " << this << "): "

namespace РНЦП {

// TODO: добавь support for thread names ids.
// TODO: добавь logger config to select global logging level.
class Logger {
public:
	enum class LoggingLevel {
		DEBUG,
		INFO,
		WARN,
		ERROR_LEVEL,
		NONE
	};

	Logger(LoggingLevel level, const Ткст& tag = "");
	virtual ~Logger();
	
	template <typename T>
	Logger& operator<<(T const& значение) {
		outputStream << значение;
		return *this;
	}
	
private:
	void Log();
	
	Ткст GetCurrentTime();
	
private:
	ТкстПоток outputStream;
	
	LoggingLevel level;
	Ткст       tag;
};

Поток& operator<<(Поток& s, Logger::LoggingLevel level);

#define LOGGER(CLASS_NAME, LEVEL) \
class CLASS_NAME : public Logger { \
public: \
	CLASS_NAME(const Ткст& tag = "") \
		: Logger(LEVEL, tag) \
	{} \
};

LOGGER(Logd, Logger::LoggingLevel::DEBUG)
LOGGER(Logi, Logger::LoggingLevel::INFO)
LOGGER(Logw, Logger::LoggingLevel::WARN)
LOGGER(Loge, Logger::LoggingLevel::ERROR_LEVEL)

#undef LOGGER

}

#endif
