template <class T, class X> РярВВ РярВВ::Var(кткст0 tag, T& var, X item_xmlize)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		return *this;
	item_xmlize(n, var);
	return *this;
}

template <class T> РярВВ РярВВ::operator()(кткст0 tag, T& var) {
	return Var(tag, var, [](РярВВ& io, T& var) { вРяр(io, var); });
}

template <class T, class X> РярВВ РярВВ::Массив(кткст0 tag, T& var, X item_xmlize, кткст0 itemtag)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		return *this;
	контейнерВРяр(n, itemtag, var, item_xmlize);
	return *this;
}

template <class T> РярВВ РярВВ::Список(кткст0 tag, кткст0 itemtag, T& var) {
	return Массив(tag, var, [](РярВВ& io, типЗначУ<T>& данные) { вРяр(io, данные); }, itemtag);
}

template <class T, class D> РярВВ РярВВ::operator()(кткст0 tag, T& var, const D& опр)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		var = опр;
	else
		вРяр(n, var);
	return *this;
}

template <class T, class D> РярВВ РярВВ::Список(кткст0 tag, кткст0 itemtag, T& var, const D& опр)
{
	РярВВ n(*this, tag);
	if(грузится() && n.Узел().дайСчёт() == 0 && n.Узел().дайСчётАтров() == 0)
		var = опр;
	else
		вРяр(n, itemtag, var);
	return *this;
}

template <class T, class X>
проц контейнерВРяр(РярВВ& xml, кткст0 tag, T& данные, X item_xmlize)
{
	if(xml.сохраняется())
		for(цел i = 0; i < данные.дайСчёт(); i++) {
			РярВВ io = xml.добавь(tag);
			item_xmlize(io, данные[i]);
		}
	else {
		данные.очисть();
		for(цел i = 0; i < xml->дайСчёт(); i++)
			if(xml->Узел(i).тэг_ли(tag)) {
				РярВВ io = xml.по(i);
				item_xmlize(io, данные.добавь());
			}
	}
}

template<class T>
проц контейнерВРяр(РярВВ& xml, кткст0 tag, T& данные)
{
	контейнерВРяр(xml, tag, данные, [](РярВВ& xml, типЗначУ<T>& данные) { вРяр(xml, данные); });
}

template<class K, class V, class T>
проц мапВРяр(РярВВ& xml, кткст0 keytag, кткст0 valuetag, T& данные)
{
	if(xml.сохраняется()) {
		for(цел i = 0; i < данные.дайСчёт(); i++)
			if(!данные.отлинкован(i)) {
				РярВВ k = xml.добавь(keytag);
				XmlizeStore(k, данные.дайКлюч(i));
				РярВВ v = xml.добавь(valuetag);
				XmlizeStore(v, данные[i]);
			}
	}
	else {
		данные.очисть();
		цел i = 0;
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
проц XmlizeSortedMap(РярВВ& xml, кткст0 keytag, кткст0 valuetag, T& данные)
{
	if(xml.сохраняется()) {
		for(цел i = 0; i < данные.дайСчёт(); i++) {
			РярВВ k = xml.добавь(keytag);
			XmlizeStore(k, данные.дайКлюч(i));
			РярВВ v = xml.добавь(valuetag);
			XmlizeStore(v, данные[i]);
		}
	}
	else {
		данные.очисть();
		цел i = 0;
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
проц индексВРяр(РярВВ& xml, кткст0 keytag, T& данные)
{
	if(xml.сохраняется()) {
		for(цел i = 0; i < данные.дайСчёт(); i++)
			if(!данные.отлинкован(i)) {
				РярВВ io = xml.добавь(keytag);
				XmlizeStore(io, данные[i]);
			}
	}
	else {
		данные.очисть();
		цел i = 0;
		while(i < xml->дайСчёт() && xml->Узел(i).тэг_ли(keytag)) {
			K k;
			РярВВ io = xml.по(i++);
			вРяр(io, k);
			данные.добавь(k);
		}
	}
}
