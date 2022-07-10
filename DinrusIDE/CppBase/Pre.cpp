#include "CppBase.h"
#include "Internal.h"

#define LTIMING(x) // DTIMING(x)

namespace РНЦП {

void SLPos(SrcFile& res)
{
	res.linepos.добавь(res.text.дайДлину());
}

SrcFile::SrcFile() :
	preprocessorLinesRemoved(0),
	blankLinesRemoved(0),
	commentLinesRemoved(0)
{
}

SrcFile PreProcess(Поток& in, Parser& parser) // This is not really C preprocess, only removes (or processes) comments and directives
{
	SrcFile res;
	bool include = true;
	int lineno = 0;
	while(!in.кф_ли()) {
		Ткст ln = in.дайСтроку();
		lineno++;
		SLPos(res);
		while(*ln.последний() == '\\') {
			ln.обрежь(ln.дайДлину() - 1);
			ln.конкат(in.дайСтроку());
			SLPos(res);
		}
		const char *rm = ln;
		if(IsAlNum(*rm)) {
			const char *s = ln;
			bool islbl = false;
			bool wassemi = false;
			while(*s && iscid(*s) || findarg(*s, '\t', ' ', ':') >= 0) { // check for label, labeled lines are not grounded
				if(*s == ':' && !wassemi) {
					islbl = true;
					wassemi = true; // second ':' cancels label
				}
				else
				if(*s != '\t' && *s != ' ')
					islbl = false; // something was after the label, e.g. void Foo::Бар()
				s++;
			}
			if(!islbl)
				res.text << '\2';
		}
		else
		if(*rm == '\x1f') // line started with macro
			res.text << '\2';
		while(*rm == ' ' || *rm == '\t') rm++;
		if(*rm == '\0')
			res.blankLinesRemoved++;
		else
		if(*rm == '#') {
			const char *s = rm + 1;
			while(*s == ' ' || *s == '\t')
				s++;
			if(s[0] == 'd' && s[1] == 'e' && s[2] == 'f' &&
			   s[3] == 'i' && s[4] == 'n' && s[5] == 'e' && !iscid(s[6])) {
				s += 6;
				while(*s == ' ' || *s == '\t') s++;
				const char *b = s;
				while(iscid(*s))
					s++;
				Ткст macro(b, s);
				if(*s == '(') {
					b = s;
					while(*s != ')' && *s)
						s++;
					macro.конкат(b, s);
					macro << ')';
				}
				if(include)
					parser.AddMacro(lineno, macro);
			}
			else
			if(s[0] == 'i' && s[1] == 'f') {
				СиПарсер p(s + 2);
				try {
					while(!p.кф_ли()) {
						if(p.ид_ли()) {
							Ткст id = p.читайИд();
							if(id.начинаетсяС("flag"))
								parser.AddMacro(lineno, id, FLAGTEST);
						}
						else
							p.пропустиТерм();
					}
				}
				catch(СиПарсер::Ошибка) {}
			}
			res.preprocessorLinesRemoved++;
		}
		else {
			bool lineContainsComment = false;
			bool lineContainsNonComment = false;
			Ткст cmd;
			while(*rm) {
				if(*rm == '\"') {
					lineContainsNonComment = true;
					res.text << '\"';
					rm++;
					while((byte)*rm && *rm != '\r' && *rm != '\n') {
						if(*rm == '\"') {
							res.text << '\"';
							rm++;
							break;
						}
						if(*rm == '\\' && rm[1]) {
							if(include)
								res.text.конкат(*rm);
							rm++;
						}
						if(include)
							res.text.конкат(*rm);
						rm++;
					}
				}
				else
				if(*rm == '\\' && rm[1]) {
					lineContainsNonComment = true;
					if(include) {
						res.text.конкат(*rm++);
						res.text.конкат(*rm++);
					}
					else
						rm += 2;
				}
				else
				if(rm[0] == '/' && rm[1] == '/') {
					cmd = rm + 2;
					if(!lineContainsNonComment)
						res.commentLinesRemoved++;
					break;
				}
				else
				if(rm[0] == '/' && rm[1] == '*') {
					lineContainsComment = true;
					rm += 2;
					for(;;) {
						if(*rm == '\0') {
							if(!lineContainsNonComment)
								res.commentLinesRemoved++;
							if(in.кф_ли()) break;
							SLPos(res);
							ln = in.дайСтроку();
							rm = ~ln;
							lineContainsNonComment = false;
						}
						if(rm[0] == '*' && rm[1] == '/') {
							rm += 2;
							break;
						}
						rm++;
					}
					if(include)
						res.text.конкат(' ');
				}
				else {
					lineContainsNonComment = true;
					if(include)
						res.text.конкат(*rm);
					rm++;
				}
			}
			if(include)
				res.text << ' ';
			if(cmd[0] == '$') {
				if(cmd[1] == '-') include = false;
				if(cmd[1] == '+') include = true;
				if(cmd[1]) {
					res.text.конкат(~cmd + 2);
					res.text.конкат(' ');
				}
			}
			if(lineContainsComment && !lineContainsNonComment)
				res.commentLinesRemoved++;
		}
	}
	return pick(res);
}

}
