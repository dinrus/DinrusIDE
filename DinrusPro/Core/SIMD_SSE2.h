struct f32x4 {
	__m128 данные;

	f32x4& грузи(кук укз)   { данные = _mm_loadu_ps((плав *)укз); return *this; }
	f32x4& Load64(кук укз) { данные = _mm_castpd_ps(_mm_load_sd((дво *)укз)); return *this; }
	f32x4& Load32(кук укз) { данные = _mm_load_ss((плав *)укз); return *this; }

	проц   сохрани(ук укз)        { _mm_storeu_ps((плав *)укз, данные); }
	проц   Store64(ук укз)      { _mm_store_sd((дво *)укз, _mm_castps_pd(данные)); }
	проц   Store32(ук укз)      { _mm_store_ss((плав *)укз, данные); }
	
	f32x4()                      {}
	f32x4(кук укз)       { грузи(укз); }
	f32x4(__m128 d)              { данные = d; }
	f32x4(дво f)              { данные = _mm_set_ss((плав)f); }
	f32x4(плав f)               { данные = _mm_set_ss(f); }
	f32x4(цел f)                 { данные = _mm_set_ss((плав)f); }
	f32x4(дво a, дво b, дво c, дво d) { данные = _mm_set_ps((плав)a, (плав)b, (плав)c, (плав)d); }
	
	operator __m128()            { return данные; }
};

форс_инлайн f32x4  f32all(дво f)              { return _mm_set1_ps((плав)f); }

форс_инлайн f32x4  operator+(f32x4 a, f32x4 b)   { return _mm_add_ps(a.данные, b.данные); }
форс_инлайн f32x4& operator+=(f32x4& a, f32x4 b) { return a = a + b; }
форс_инлайн f32x4  operator-(f32x4 a, f32x4 b)   { return _mm_sub_ps(a.данные, b.данные); }
форс_инлайн f32x4& operator-=(f32x4& a, f32x4 b) { return a = a - b; }
форс_инлайн f32x4  operator*(f32x4 a, f32x4 b)   { return _mm_mul_ps(a.данные, b.данные); }
форс_инлайн f32x4& operator*=(f32x4& a, f32x4 b) { return a = a * b; }
форс_инлайн f32x4  operator/(f32x4 a, f32x4 b)   { return _mm_div_ps(a.данные, b.данные); }
форс_инлайн f32x4& operator/=(f32x4& a, f32x4 b) { return a = a / b; }

форс_инлайн f32x4  operator==(f32x4 a, f32x4 b)  { return _mm_cmpeq_ps(a.данные, b.данные); }
форс_инлайн f32x4  operator!=(f32x4 a, f32x4 b)  { return _mm_cmpneq_ps(a.данные, b.данные); }
форс_инлайн f32x4  operator<(f32x4 a, f32x4 b)   { return _mm_cmplt_ps(a.данные, b.данные); }
форс_инлайн f32x4  operator>(f32x4 a, f32x4 b)   { return _mm_cmpgt_ps(a.данные, b.данные); }
форс_инлайн f32x4  operator<=(f32x4 a, f32x4 b)  { return _mm_cmple_ps(a.данные, b.данные); }
форс_инлайн f32x4  operator>=(f32x4 a, f32x4 b)  { return _mm_cmpge_ps(a.данные, b.данные); }
форс_инлайн бул   AllTrue(f32x4 a)              { return _mm_movemask_ps(a.данные) == 0xf; }

форс_инлайн f32x4 мин(f32x4 a, f32x4 b)          { return _mm_min_ps(a.данные, b.данные); }
форс_инлайн f32x4 макс(f32x4 a, f32x4 b)          { return _mm_max_ps(a.данные, b.данные); }

#define _MM_BCAST(a)                              _MM_SHUFFLE(a,a,a,a)

форс_инлайн f32x4 Broadcast0(f32x4 a)            { return _mm_shuffle_ps(a.данные, a.данные, _MM_BCAST(0)); }
форс_инлайн f32x4 Broadcast1(f32x4 a)            { return _mm_shuffle_ps(a.данные, a.данные, _MM_BCAST(1)); }
форс_инлайн f32x4 Broadcast2(f32x4 a)            { return _mm_shuffle_ps(a.данные, a.данные, _MM_BCAST(2)); }
форс_инлайн f32x4 Broadcast3(f32x4 a)            { return _mm_shuffle_ps(a.данные, a.данные, _MM_BCAST(3)); }

struct i16x8 { // 8xint16
	__m128i данные;

	i16x8& грузи(кук укз)       { данные = _mm_loadu_si128((__m128i *)укз); return *this; }
	i16x8& Load64(кук укз)     { данные = _mm_castpd_si128(_mm_load_sd((дво *)укз)); return *this; }
	i16x8& Load32(кук укз)     { данные = _mm_castps_si128(_mm_load_ss((плав *)укз)); return *this; }

	проц   сохрани(ук укз)      { _mm_storeu_si128((__m128i *)укз, данные); }
	проц   Store64(ук укз)    { _mm_store_sd((дво *)укз, _mm_castsi128_pd(данные)); }
	проц   Store32(ук укз)    { _mm_store_ss((плав *)укз, _mm_castsi128_ps(данные)); }
	проц   Поток(ук укз)     { _mm_stream_si128((__m128i *)укз, данные); };
	
	i16x8()                      {}
	i16x8(кук укз)       { грузи(укз); }
	i16x8(__m128i d)             { данные = d; }
	i16x8(цел v)                 { данные = _mm_set_epi16(0, 0, 0, 0, 0, 0, 0, v); }
	i16x8(цел a, цел b, цел c, цел d, цел e, цел f, цел g, цел h)  { данные = _mm_set_epi16(a, b, c, d, e, f, g, h); }
	operator __m128i()           { return данные; }
};

форс_инлайн i16x8  i16all(цел v)                  { return _mm_set1_epi16(v); }

форс_инлайн i16x8  operator+(i16x8 a, i16x8 b)    { return _mm_add_epi16(a.данные, b.данные); }
форс_инлайн i16x8& operator+=(i16x8& a, i16x8 b)  { return a = a + b; }
форс_инлайн i16x8  operator-(i16x8 a, i16x8 b)    { return _mm_sub_epi16(a.данные, b.данные); }
форс_инлайн i16x8& operator-=(i16x8& a, i16x8 b)  { return a = a - b; }
форс_инлайн i16x8  operator*(i16x8 a, i16x8 b)    { return _mm_mullo_epi16(a.данные, b.данные); }
форс_инлайн i16x8& operator*=(i16x8& a, i16x8 b)  { return a = a * b; }

форс_инлайн i16x8  operator&(i16x8 a, i16x8 b)    { return _mm_and_si128(a.данные, b.данные); }
форс_инлайн i16x8& operator&=(i16x8& a, i16x8 b)  { return a = a & b; }
форс_инлайн i16x8  operator|(i16x8 a, i16x8 b)    { return _mm_or_si128(a.данные, b.данные); }
форс_инлайн i16x8& operator|=(i16x8& a, i16x8 b)  { return a = a | b; }
форс_инлайн i16x8  operator^(i16x8 a, i16x8 b)    { return _mm_xor_si128(a.данные, b.данные); }
форс_инлайн i16x8& operator^=(i16x8& a, i16x8 b)  { return a = a ^ b; }
форс_инлайн i16x8  operator~(i16x8 a)             { return _mm_xor_si128(a.данные, i16all(0xffff).данные); }

форс_инлайн i16x8  operator>>(i16x8 a, цел b)     { return _mm_srli_epi16(a.данные, b); }
форс_инлайн i16x8& operator>>=(i16x8& a, цел b)   { return a = a >> b; }
форс_инлайн i16x8  operator<<(i16x8 a, цел b)     { return _mm_slli_epi16(a.данные, b); }
форс_инлайн i16x8& operator<<=(i16x8& a, цел b)   { return a = a << b; }

форс_инлайн i16x8  operator==(i16x8 a, i16x8 b)   { return _mm_cmpeq_epi16(a.данные, b.данные); }
форс_инлайн i16x8  operator<(i16x8 a, i16x8 b)    { return _mm_cmplt_epi16(a.данные, b.данные); }
форс_инлайн i16x8  operator>(i16x8 a, i16x8 b)    { return _mm_cmpgt_epi16(a.данные, b.данные); }
форс_инлайн бул   AllTrue(i16x8 a)               { return _mm_movemask_epi8(a.данные) == 0xffff; }

struct i32x4 : i16x8 { // 4xint32
	i32x4()                      {}
	i32x4(кук укз)       { грузи(укз); }
	i32x4(__m128i d)             { данные = d; }
	i32x4(цел v)                 { данные = _mm_set_epi32(0, 0, 0, v); }
	i32x4(цел a, цел b, цел c, цел d)  { данные = _mm_set_epi32(a, b, c, d); }
	operator цел()               { return _mm_cvtsi128_si32(данные); }
};

форс_инлайн i32x4  i32all(цел v)                 { return _mm_set1_epi32(v); }

форс_инлайн i32x4  operator+(i32x4 a, i32x4 b)   { return _mm_add_epi32(a.данные, b.данные); }
форс_инлайн i32x4& operator+=(i32x4& a, i32x4 b) { return a = a + b; }
форс_инлайн i32x4  operator-(i32x4 a, i32x4 b)   { return _mm_sub_epi32(a.данные, b.данные); }
форс_инлайн i32x4& operator-=(i32x4& a, i32x4 b) { return a = a - b; }

форс_инлайн i32x4  operator&(i32x4 a, i32x4 b)   { return _mm_and_si128(a.данные, b.данные); }
форс_инлайн i32x4& operator&=(i32x4& a, i32x4 b) { return a = a & b; }
форс_инлайн i32x4  operator|(i32x4 a, i32x4 b)   { return _mm_or_si128(a.данные, b.данные); }
форс_инлайн i32x4& operator|=(i32x4& a, i32x4 b) { return a = a | b; }
форс_инлайн i32x4  operator^(i32x4 a, i32x4 b)   { return _mm_xor_si128(a.данные, b.данные); }
форс_инлайн i32x4& operator^=(i32x4& a, i32x4 b) { return a = a ^ b; }
форс_инлайн i32x4  operator~(i32x4 a)            { return _mm_xor_si128(a.данные, i32all(0xffffffff).данные); }

форс_инлайн i32x4  operator>>(i32x4 a, цел b)    { return _mm_srli_epi32(a.данные, b); }
форс_инлайн i32x4& operator>>=(i32x4& a, цел b)  { return a = a >> b; }
форс_инлайн i32x4  operator<<(i32x4 a, цел b)    { return _mm_slli_epi32(a.данные, b); }
форс_инлайн i32x4& operator<<=(i32x4& a, цел b)  { return a = a << b; }

форс_инлайн i32x4  operator==(i32x4 a, i32x4 b)   { return _mm_cmpeq_epi32(a.данные, b.данные); }
форс_инлайн i32x4  operator<(i32x4 a, i32x4 b)    { return _mm_cmplt_epi32(a.данные, b.данные); }
форс_инлайн i32x4  operator>(i32x4 a, i32x4 b)    { return _mm_cmpgt_epi32(a.данные, b.данные); }
форс_инлайн бул   AllTrue(i32x4 a)               { return _mm_movemask_epi8(a.данные) == 0xffff; }

struct i8x16 : i16x8 { // 16xint8
	i8x16()                      {}
	i8x16(кук укз)       { грузи(укз); }
	i8x16(__m128i d)             { данные = d; }
	i8x16(цел v)                 { данные = _mm_set_epi8(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,v); }
	i8x16(цел a, цел b, цел c, цел d, цел e, цел f, цел g, цел h, цел i, цел j, цел k, цел l, цел m, цел n, цел o, цел p)
	                             { данные = _mm_set_epi8(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p); }
};

форс_инлайн i8x16  i8all(цел v)                  { return _mm_set1_epi8(v); }

форс_инлайн i8x16  operator+(i8x16 a, i8x16 b)   { return _mm_add_epi8(a.данные, b.данные); }
форс_инлайн i8x16& operator+=(i8x16& a, i8x16 b) { return a = a + b; }
форс_инлайн i8x16  operator-(i8x16 a, i8x16 b)   { return _mm_sub_epi8(a.данные, b.данные); }
форс_инлайн i8x16& operator-=(i8x16& a, i8x16 b) { return a = a - b; }

форс_инлайн i8x16  operator&(i8x16 a, i8x16 b)   { return _mm_and_si128(a.данные, b.данные); }
форс_инлайн i8x16& operator&=(i8x16& a, i8x16 b) { return a = a & b; }
форс_инлайн i8x16  operator|(i8x16 a, i8x16 b)   { return _mm_or_si128(a.данные, b.данные); }
форс_инлайн i8x16& operator|=(i8x16& a, i8x16 b) { return a = a | b; }
форс_инлайн i8x16  operator^(i8x16 a, i8x16 b)   { return _mm_xor_si128(a.данные, b.данные); }
форс_инлайн i8x16& operator^=(i8x16& a, i8x16 b) { return a = a ^ b; }
форс_инлайн i8x16  operator~(i8x16 a)            { return _mm_xor_si128(a.данные, i8all(0xff).данные); }

форс_инлайн f32x4 ToFloat(i32x4 a)               { return _mm_cvtepi32_ps(a.данные); }
форс_инлайн i32x4 Truncate(f32x4 a)              { return _mm_cvttps_epi32(a.данные); }
// форс_инлайн i32x4 Round(f32x4 a)                 { return _mm_cvtps_epi32(a.данные); }

форс_инлайн i16x8 Unpack8L(i16x8 a)              { return _mm_unpacklo_epi8(a.данные, _mm_setzero_si128()); }
форс_инлайн i16x8 Unpack8H(i16x8 a)              { return _mm_unpackhi_epi8(a.данные, _mm_setzero_si128()); }
форс_инлайн i32x4 Unpack16L(i16x8 a)             { return _mm_unpacklo_epi16(a.данные, _mm_setzero_si128()); }
форс_инлайн i32x4 Unpack16H(i16x8 a)             { return _mm_unpackhi_epi16(a.данные, _mm_setzero_si128()); }

форс_инлайн i8x16 пакуй16(i16x8 l, i16x8 h)       { return _mm_packus_epi16(l.данные, h.данные); }
форс_инлайн i8x16 пакуй16(i16x8 l)                { return _mm_packus_epi16(l.данные, _mm_setzero_si128()); }
форс_инлайн i16x8 Pack32(i32x4 a)                { return _mm_packs_epi32(a.данные, _mm_setzero_si128()); }

форс_инлайн i16x8 BroadcastLH0(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.данные, _MM_BCAST(0)), _MM_BCAST(0)); }
форс_инлайн i16x8 BroadcastLH1(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.данные, _MM_BCAST(1)), _MM_BCAST(1)); }
форс_инлайн i16x8 BroadcastLH2(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.данные, _MM_BCAST(2)), _MM_BCAST(2)); }
форс_инлайн i16x8 BroadcastLH3(i16x8 a)          { return _mm_shufflelo_epi16(_mm_shufflehi_epi16(a.данные, _MM_BCAST(3)), _MM_BCAST(3)); }

форс_инлайн i16x8 i64all(дим данные)             { return _mm_set1_epi64x(данные); }
