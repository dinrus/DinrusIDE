class Ткст;

#if defined(PLATFORM_POSIX) || defined(PLATFORM_WINCE)
inline цел tmGetTime() {
	return GetTickCount();
}
#else
inline цел tmGetTime() {
	return timeGetTime();
}
#endif

class ТаймингИнспектор {
protected:
	static бул active;

	кткст0 имя;
	цел         call_count;
	бцел       total_time;
	бцел       min_time;
	бцел       max_time;
	цел         max_nesting;
	цел         all_count;
	СтатическийСтопор mutex;

public:
	ТаймингИнспектор(кткст0 имя = NULL); // Not Ткст !!!
	~ТаймингИнспектор();

	проц   добавь(бцел time, цел nesting);

	Ткст дамп();

	class Routine {
	public:
		Routine(ТаймингИнспектор& stat, цел& nesting)
		: nesting(nesting), stat(stat) {
			start_time = tmGetTime();
			nesting++;
		}

		~Routine() {
			nesting--;
			stat.добавь(start_time, nesting);
		}

	protected:
		бцел start_time;
		цел& nesting;
		ТаймингИнспектор& stat;
	};

	static проц активируй(бул b)                    { active = b; }
};

class ИнспекторСчётаНажатий
{
public:
	ИнспекторСчётаНажатий(кткст0 имя, цел hitcount = 0) : имя(имя), hitcount(hitcount) {}
	~ИнспекторСчётаНажатий();

	проц Step()              { hitcount++; }
	проц добавь(цел i)          { hitcount += i; }
	проц operator ++ ()      { Step(); }
	проц operator += (цел i) { добавь(i); }

private:
	кткст0 имя;
	цел         hitcount;
};

#define RTIMING(x) \
	static ТаймингИнспектор КОМБИНИРУЙ(sTmStat, __LINE__)(x); \
	static thread_local цел КОМБИНИРУЙ(sTmStatNesting, __LINE__); \
	ТаймингИнспектор::Routine КОМБИНИРУЙ(sTmStatR, __LINE__)(КОМБИНИРУЙ(sTmStat, __LINE__), КОМБИНИРУЙ(sTmStatNesting, __LINE__))

#define RACTIVATE_TIMING()    ТаймингИнспектор::активируй(true);
#define RDEACTIVATE_TIMING()  ТаймингИнспектор::активируй(false);

#define RHITCOUNT(n) \
	{ static ИнспекторСчётаНажатий hitcount(n); hitcount.Step(); }
