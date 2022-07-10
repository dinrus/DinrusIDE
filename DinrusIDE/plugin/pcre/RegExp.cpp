#include "Pcre.h"

namespace РНЦП {


RegExp::RegExp(int options)
{
	очисть();
	SetOptions(options);
}

RegExp::RegExp(const char * p, int options)
{
	очисть();
	SetOptions(options);
	SetPattern(p);
}

RegExp::RegExp(const Ткст &p, int options)
{
	очисть();
	SetOptions(options);
	SetPattern(p);
}

RegExp::~RegExp()
{
	очисть(true);
}

void RegExp::очисть(bool freemem)
{
	if(freemem && study)
		pcre_free(study);
	if(freemem && cpattern)
		pcre_free(cpattern);

	first = false;
	cpattern = NULL;
	study = NULL;
	rc = 0;
	compile_options = 0;
	execute_options = 0;
	error_code = 0;
}

void RegExp::SetOptions(int options)
{
	compile_options = options & COMPILE_OPTIONS;
	execute_options = options & EXECUTE_OPTIONS;
}

void RegExp::SetPattern(const char * p)
{
	pattern = p;
}

void RegExp::SetPattern(const Ткст &p)
{
	pattern = p;
}

bool RegExp::Compile(bool recompile)
{
	if(cpattern)
	{
		if(recompile)
			pcre_free(cpattern);
		else
			return true;
	}
	cpattern = pcre_compile2(pattern, compile_options, &error_code, &error_string, &error_offset, NULL);
	return error_code == 0;
}

bool RegExp::Study(bool restudy)
{
	if(!cpattern)
		Compile();
	if(study){
		if(restudy)
			pcre_free(study);
		else
			return true;
	}
	study = pcre_study(cpattern, 0, &error_string);
	if(error_string != NULL)
		error_code = -1; // unfortunatelly, pcre_study doesn't return Ошибка codes...
	return error_code == 0;
}

int RegExp::выполни(const Ткст &t, int offset)
{
	rc = pcre_exec(cpattern, study, t, t.дайДлину(), offset, execute_options, pos, __countof(pos));
	if(rc <= 0)
		first = false;
	return rc;
}

bool RegExp::сверь(const Ткст &t, bool copy)
{
	if(copy)
		text = t;
	if(!Compile())
		return false;
	return выполни(t) > 0;
}

bool RegExp::FastMatch(const Ткст &t)
{
	return сверь(t, false);
}

bool RegExp::GlobalMatch(const Ткст &t)
{
	if(!first)
	{
		first = true;
		return сверь(t);
	}
	int offset = pos[1];
	if(pos[0] == pos[1])
	{
		if(pos[0] == t.дайДлину())
		{
			first = false;
			return false;
		}
		execute_options |= PCRE_NOTEMPTY | PCRE_ANCHORED;
	}
	return выполни(t, offset) > 0;
}

Ткст RegExp::operator[](const int i)
{
	return дайТкст(i);
}

int RegExp::дайСчёт()
{
	return rc - 1;
}

Вектор<Ткст> RegExp::GetStrings()
{
	Вектор<Ткст> subs;
	for(int i = 0; i < дайСчёт(); i++)
		subs.добавь(дайТкст(i));
	return subs;
}

Ткст RegExp::дайТкст(int i)
{
	i = 2 * (i + 1);
	return text.середина(pos[i], pos[i + 1] - pos[i]);
}

void RegExp::GetMatchPos(int i, int& iPosStart, int& iPosAfterEnd)
{
	i = 2 * (i + 1);
	iPosStart = pos[i];
	iPosAfterEnd = pos[i + 1];
}

int RegExp::дайСмещ() const
{
	return rc > 0 ? pos[0] : -1;
}

int RegExp::дайДлину() const
{
	return rc > 0 ? pos[1] - pos[0] : -1;
}

int RegExp::GetSubOffset(int i) const
{
	ПРОВЕРЬ(i < rc - 1);
	return pos[2 * i + 2];
}

int RegExp::GetSubLength(int i) const
{
	return pos[2 * i + 3] - pos[2 * i + 2];
}

bool RegExp::ReplacePos(Ткст& t, int p, int q, const Ткст& r)
{
	if(p>=0 && q<=t.дайДлину() && q>p){		
		
		t.удали(p, q-p);
		if(!r.пустой()) t.вставь(p, r);	
		
		return true;
	}
	
	return false;					
}

Вектор<Ткст> RegExp::Make_rv(const Ткст& r)
{
	RegExp reg("\\(((\r|\n|.)*?)\\)");
	
	Вектор<Ткст> rv;
	
	while(reg.GlobalMatch(r)){
		rv.добавь(reg.дайТкст(0));
	}
	
	return rv;
}

Вектор<Ткст> RegExp::ResolveBackRef(const Вектор<Ткст>& rv)
{	
	Вектор<Ткст> new_rv;
	
	int rv_count = rv.дайСчёт();
	int match_count = дайСчёт();
	
	RegExp reg("\\\\(\\d+)");
	
	Ткст log;
		
	if(match_count>0 && rv_count>0) {
		
		// deep copy rv vector.
		new_rv<<=rv;
		
		// loop throug number of replace string
		// run reg regexp on each to find if they
		// have any "match referance" ie /1 /2 /3 etc
		for(int i=0; i<rv_count; i++){
						
			while(reg.GlobalMatch(new_rv[i])){				
				
				// for each matched "match ref"
				for(int j=0, jMx=reg.дайСчёт(); j< jMx; j++){
					
					Ткст p;
					
					// convert the ref to number
					int m=тктЦелЗнач(reg.дайТкст(j));
					
					// make "find" string litaral					
					p<<"\\"<<m;	

					// reduce 1 from back ref to be used as Индекс.
					m--;		
					
					// doing actual replacement.
					if(m<match_count) new_rv[i].замени(p, дайТкст(m));																
				}									
			}						
		}
	}
	
	return new_rv;		
}



int RegExp::Replace0(Ткст& t, const Вектор<Ткст>& rv, const int& rv_count, int& offset)
{
	
	int x=0, y=0;
	int count=дайСчёт();

		
	for(int i=0; i<count; i++){
				
		GetMatchPos(i, x, y);
		
		if(i<rv_count) {
			
			ReplacePos(t, x+offset, y+offset, rv[i]);
							
			offset = offset + rv[i].дайДлину() - (y-x);
			
		}
		else{
	
			ReplacePos(t, x+offset, y+offset, "");	
			
			offset = offset - (y-x);				 
		}										
	}
		
	return count;		
}

int RegExp::замени(Ткст& t, const Вектор<Ткст>& rv, bool backref)
{
	Ткст t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=rv.дайСчёт();	
	Вектор<Ткст> new_rv; 
	
	new_rv.сожми();
	first=false;
	
	сверь(t_copy);
	
	if(backref){
		
		 new_rv.очисть();
		 new_rv = ResolveBackRef(rv);	
		 count  = Replace0(t, new_rv, rv_count, pos_offset);				
		 
	}
	else count = Replace0(t, rv, rv_count, pos_offset);	

	return count;
}

int RegExp::замени(Ткст& t, const Ткст& r, bool backref)
{
	Вектор<Ткст> rv(Make_rv(r)); 
	return замени(t, rv, backref);
}


int RegExp::замени(Ткст& t, Событие<Вектор<Ткст>&> cbr)
{
	Ткст t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=0;
	Вектор<Ткст> rv; 

	first=false;
	
	сверь(t_copy);
	
	rv=GetStrings();	
	
	cbr(rv);	
	
	rv_count=rv.дайСчёт();
	
	Replace0(t, rv, rv_count, pos_offset);	

	return count;	
	
}

int RegExp::ReplaceGlobal(Ткст& t, const Вектор<Ткст>& rv, bool backref)
{
	Ткст t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=rv.дайСчёт();	
	Вектор<Ткст> new_rv; 
	
	new_rv.сожми();
	first=false;
	
	while(GlobalMatch(t_copy)){
		
			if(backref){
				
				 new_rv.очисть();
				 new_rv = ResolveBackRef(rv);	
				 count += Replace0(t, new_rv, rv_count, pos_offset);				
				 
			}
			else count += Replace0(t, rv, rv_count, pos_offset);	
			
	}
	
	return count;
	
}

int RegExp::ReplaceGlobal(Ткст& t, const Ткст& r, bool backref)
{
	Вектор<Ткст> rv(Make_rv(r)); 
	return ReplaceGlobal(t, rv, backref);
}

int RegExp::ReplaceGlobal(Ткст& t, Событие<Вектор<Ткст>&> cbr)
{
	Ткст t_copy(t);
	int count=0;
	int pos_offset=0;
	int rv_count=0;
	Вектор<Ткст> rv; 
	
	first = false;
	while(GlobalMatch(t_copy)) {
		rv.очисть();
		rv=GetStrings();
		cbr(rv);
		rv_count=rv.дайСчёт();
		count += Replace0(t, rv, rv_count, pos_offset);	
	}
	
	return count;
	
}

}
