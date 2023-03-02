class Ткст;

#if defined(PLATFORM_POSIX) || defined(PLATFORM_WINCE)
inline int tmGetTime() {
	return GetTickCount();
}
#else
inline int tmGetTime() {
	return timeGetTime();
}
#endif

class ТаймингИнспектор {
protected:
	static bool active;

	const char *имя;
	int         call_count;
	dword       total_time;
	dword       min_time;
	dword       max_time;
	int         max_nesting;
	int         all_count;
	СтатическийСтопор mutex;

public:
	ТаймингИнспектор(const char *имя = NULL); // Not Ткст !!!
	~ТаймингИнспектор();

	void   добавь(dword time, int nesting);

	Ткст дамп();

	class Routine {
	public:
		Routine(ТаймингИнспектор& stat, int& nesting)
		: nesting(nesting), stat(stat) {
			start_time = tmGetTime();
			nesting++;
		}

		~Routine() {
			nesting--;
			stat.добавь(start_time, nesting);
		}

	protected:
		dword start_time;
		int& nesting;
		ТаймингИнспектор& stat;
	};

	static void активируй(bool b)                    { active = b; }
};

class ИнспекторСчётаНажатий
{
public:
	ИнспекторСчётаНажатий(const char *имя, int hitcount = 0) : имя(имя), hitcount(hitcount) {}
	~ИнспекторСчётаНажатий();

	void Step()              { hitcount++; }
	void добавь(int i)          { hitcount += i; }
	void operator ++ ()      { Step(); }
	void operator += (int i) { добавь(i); }

private:
	const char *имя;
	int         hitcount;
};

#define RTIMING(x) \
	static РНЦП::ТаймингИнспектор КОМБИНИРУЙ(sTmStat, __LINE__)(x); \
	static thread_local int КОМБИНИРУЙ(sTmStatNesting, __LINE__); \
	РНЦП::ТаймингИнспектор::Routine КОМБИНИРУЙ(sTmStatR, __LINE__)(КОМБИНИРУЙ(sTmStat, __LINE__), КОМБИНИРУЙ(sTmStatNesting, __LINE__))

#define RACTIVATE_TIMING()    ТаймингИнспектор::активируй(true);
#define RDEACTIVATE_TIMING()  ТаймингИнспектор::активируй(false);

#define RHITCOUNT(n) \
	{ static ИнспекторСчётаНажатий hitcount(n); hitcount.Step(); }
