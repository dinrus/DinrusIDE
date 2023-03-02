template <class T, class X> РярВВ РярВВ::Var(const char *tag, T& var, X item_xmlize)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		return *this;
	item_xmlize(n, var);
	return *this;
}

template <class T> РярВВ РярВВ::operator()(const char *tag, T& var) {
	return Var(tag, var, [](РярВВ& io, T& var) { вРяр(io, var); });
}

template <class T, class X> РярВВ РярВВ::Массив(const char *tag, T& var, X item_xmlize, const char *itemtag)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		return *this;
	контейнерВРяр(n, itemtag, var, item_xmlize);
	return *this;
}

template <class T> РярВВ РярВВ::Список(const char *tag, const char *itemtag, T& var) {
	return Массив(tag, var, [](РярВВ& io, типЗначУ<T>& данные) { вРяр(io, данные); }, itemtag);
}

template <class T, class D> РярВВ РярВВ::operator()(const char *tag, T& var, const D& опр)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		var = опр;
	else
		вРяр(n, var);
	return *this;
}

template <class T, class D> РярВВ РярВВ::Список(const char *tag, const char *itemtag, T& var, const D& опр)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		var = опр;
	else
		вРяр(n, itemtag, var);
	return *this;
}

template <class T, class X>
void контейнерВРяр(РярВВ& xml, const char *tag, T& данные, X item_xmlize)
{
	if(xml.сохраняется())
		for(int i = 0; i < данные.дайСчёт(); i++) {
			РярВВ io = xml.добавь(tag);
			item_xmlize(io, данные[i]);
		}
	else {
		данные.очисть();
		for(int i = 0; i < xml->дайСчёт(); i++)
			if(xml->Узел(i).тэг_ли(tag)) {
				РярВВ io = xml.по(i);
				item_xmlize(io, данные.добавь());
			}
	}
}

template<class T>
void контейнерВРяр(РярВВ& xml, const char *tag, T& данные)
{
	контейнерВРяр(xml, tag, данные, [](РярВВ& xml, типЗначУ<T>& данные) { вРяр(xml, данные); });
}

template<class K, class V, class T>
void мапВРяр(РярВВ& xml, const char *keytag, const char *valuetag, T& данные)
{
	if(xml.сохраняется()) {
		for(int i = 0; i < данные.дайСчёт(); i++)
			if(!данные.отлинкован(i)) {
				РярВВ k = xml.добавь(keytag);
				XmlizeStore(k, данные.дайКлюч(i));
				РярВВ v = xml.добавь(valuetag);
				XmlizeStore(v, данные[i]);
			}
	}
	else {
		данные.очисть();
		int i = 0;
		while(i < xml->дайСчёт() - 1 && xml->Узел(i).тэг_ли(keytag) && xml->Узел(i + 1).тэг_ли(valuetag)) {
			K ключ;
			РярВВ k = xml.по(i++);
			вРяр(k, ключ);
			РярВВ v = xml.по(i++);
			вРяр(v, данные.добавь(ключ));
		}
	}
}

template<class K, class V, class T>
void XmlizeSortedMap(РярВВ& xml, const char *keytag, const char *valuetag, T& данные)
{
	if(xml.сохраняется()) {
		for(int i = 0; i < данные.дайСчёт(); i++) {
			РярВВ k = xml.добавь(keytag);
			XmlizeStore(k, данные.дайКлюч(i));
			РярВВ v = xml.добавь(valuetag);
			XmlizeStore(v, данные[i]);
		}
	}
	else {
		данные.очисть();
		int i = 0;
		while(i < xml->дайСчёт() - 1 && xml->Узел(i).тэг_ли(keytag) && xml->Узел(i + 1).тэг_ли(valuetag)) {
			K ключ;
			РярВВ k = xml.по(i++);
			вРяр(k, ключ);
			РярВВ v = xml.по(i++);
			вРяр(v, данные.добавь(ключ));
		}
	}
}

template<class K, class T>
void индексВРяр(РярВВ& xml, const char *keytag, T& данные)
{
	if(xml.сохраняется()) {
		for(int i = 0; i < данные.дайСчёт(); i++)
			if(!данные.отлинкован(i)) {
				РярВВ io = xml.добавь(keytag);
				XmlizeStore(io, данные[i]);
			}
	}
	else {
		данные.очисть();
		int i = 0;
		while(i < xml->дайСчёт() && xml->Узел(i).тэг_ли(keytag)) {
			K k;
			РярВВ io = xml.по(i++);
			вРяр(io, k);
			данные.добавь(k);
		}
	}
}
