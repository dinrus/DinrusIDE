typedef std::atomic<int> Атомар;

inline int  атомнИнк(volatile Атомар& t)             { return ++t; }
inline int  атомнДек(volatile Атомар& t)             { return --t; }
