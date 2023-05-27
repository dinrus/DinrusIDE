#ifndef _Core_Huge_h_
#define _Core_Huge_h_

class Huge {
public:
	enum { CHUNK = 1024 * 1024 };

private:
	struct Block {
		ббайт данные[CHUNK];
	};
	
	Массив<Block>  данные;
	т_мера        size;

public:
	ббайт  *AddChunk();
	проц   финиш(цел last_chunk_size);
	
	т_мера дайРазм() const                              { return size; }
	проц   дай(ук t, т_мера pos, т_мера sz) const;
	проц   дай(ук t)                                 { дай(t, 0, дайРазм()); }
	Ткст дай() const;
	
	Huge();
};

#endif
