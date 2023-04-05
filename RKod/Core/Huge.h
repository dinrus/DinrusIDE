#ifndef _Core_Huge_h_
#define _Core_Huge_h_

class Huge {
public:
	enum { CHUNK = 1024 * 1024 };

private:
	struct Block {
		byte данные[CHUNK];
	};
	
	Массив<Block>  данные;
	size_t        size;

public:
	byte  *AddChunk();
	void   финиш(int last_chunk_size);
	
	size_t дайРазм() const                              { return size; }
	void   дай(void *t, size_t pos, size_t sz) const;
	void   дай(void *t)                                 { дай(t, 0, дайРазм()); }
	Ткст дай() const;
	
	Huge();
};

#endif
