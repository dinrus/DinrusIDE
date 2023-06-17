#include <arm_neon.h>

struct f32x4 {
	float32x4_t данные;

	f32x4& грузи(кук укз)   { данные = vld1q_f32((плав *)укз); return *this; }
	f32x4& Load64(кук укз) { данные = vcombine_f32(vld1_f32((плав *)укз), vdup_n_f32(0)); return *this; }
	f32x4& Load32(кук укз) { данные = vsetq_lane_f32(*(плав *)укз, vdupq_n_f32(0), 0); return *this; }

	проц   сохрани(ук укз)        { vst1q_f32((float32_t *)укз, данные); }
	проц   Store64(ук укз)      { vst1_f32((float32_t *)укз, vget_low_f32(данные)); }
	проц   Store32(ук укз)      { *(float32_t *)укз = vgetq_lane_f32(данные, 0); }
	
	f32x4()                      {}
	f32x4(кук укз)       { грузи(укз); }
	f32x4(float32x4_t d)         { данные = d; }

	f32x4(дво f)              { данные = vsetq_lane_f32((плав)f, vdupq_n_f32(0), 0); }
	f32x4(плав f)               { данные = vsetq_lane_f32((плав)f, vdupq_n_f32(0), 0); }
	f32x4(цел f)                 { данные = vsetq_lane_f32((плав)f, vdupq_n_f32(0), 0); }
	f32x4(дво a, дво b, дво c, дво d) {
		плав __attribute__((aligned(16))) w[4] = { (плав)d, (плав)c, (плав)b, (плав)a };
		данные = vld1q_f32(w);
	}
	
	operator float32x4_t()       { return данные; }
};

форс_инлайн f32x4  f32all(дво f) { return vdupq_n_f32((плав)f); }

форс_инлайн f32x4  operator+(f32x4 a, f32x4 b)   { return vaddq_f32(a, b); }
форс_инлайн f32x4& operator+=(f32x4& a, f32x4 b) { return a = a + b; }
форс_инлайн f32x4  operator-(f32x4 a, f32x4 b)   { return vsubq_f32(a, b); }
форс_инлайн f32x4& operator-=(f32x4& a, f32x4 b) { return a = a - b; }
форс_инлайн f32x4  operator*(f32x4 a, f32x4 b)   { return vmulq_f32(a, b); }
форс_инлайн f32x4& operator*=(f32x4& a, f32x4 b) { return a = a * b; }

форс_инлайн f32x4  operator/(f32x4 a, f32x4 b)   {
	float32x4_t reciprocal = vrecpeq_f32(b);
	reciprocal = vmulq_f32(vrecpsq_f32(b, reciprocal), reciprocal);
	reciprocal = vmulq_f32(vrecpsq_f32(b, reciprocal), reciprocal);
	return vmulq_f32(a,reciprocal);
}

форс_инлайн f32x4& operator/=(f32x4& a, f32x4 b) { return a = a / b; }

форс_инлайн f32x4  operator==(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vceqq_f32(a, b)); }
форс_инлайн f32x4  operator!=(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vmvnq_u32(vceqq_f32(a, b))); }
форс_инлайн f32x4  operator<(f32x4 a, f32x4 b)   { return vreinterpretq_f32_u32(vcltq_f32(a, b)); }
форс_инлайн f32x4  operator>(f32x4 a, f32x4 b)   { return vreinterpretq_f32_u32(vcgtq_f32(a, b)); }
форс_инлайн f32x4  operator<=(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vcleq_f32(a, b)); }
форс_инлайн f32x4  operator>=(f32x4 a, f32x4 b)  { return vreinterpretq_f32_u32(vcgeq_f32(a, b)); }

форс_инлайн бул   AllTrue(uint32x4_t v) {
    uint32x2_t tmp = vand_u32(vget_low_u32(v), vget_high_u32(v));
    return vget_lane_u32(vpmin_u32(tmp, tmp), 0) == 0xffffffff;;
}

форс_инлайн бул   AllTrue(f32x4 a) {
	return AllTrue(vreinterpretq_u32_f32(a));
}

форс_инлайн f32x4 мин(f32x4 a, f32x4 b)          { return vminq_f32(a, b); }
форс_инлайн f32x4 макс(f32x4 a, f32x4 b)          { return vmaxq_f32(a, b); }

форс_инлайн f32x4 Broadcast0(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 0)); }
форс_инлайн f32x4 Broadcast1(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 1)); }
форс_инлайн f32x4 Broadcast2(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 2)); }
форс_инлайн f32x4 Broadcast3(f32x4 a)            { return vdupq_n_f32(vgetq_lane_f32(a, 3)); }

struct i16x8 { // 8xint16
	int16x8_t данные;

	i16x8& грузи(кук укз)   { данные = vld1q_s16((int16_t *)укз); return *this; }
	i16x8& Load64(кук укз) { данные = vcombine_s16(vld1_s16((int16_t *)укз), vdup_n_s16(0)); return *this; }
	i16x8& Load32(кук укз) { данные = vreinterpretq_s16_s32(vsetq_lane_s32(*(int32_t *)укз, vdupq_n_s32(0), 0)); return *this; }

	проц   сохрани(ук укз)      { vst1q_s16((int16_t *)укз, данные); }
	проц   Store64(ук укз)    { vst1_s16((int16_t *)укз, vget_low_s16(данные)); }
	проц   Store32(ук укз)    { *(int32_t *)укз = vgetq_lane_s32(vreinterpretq_s32_s16(данные), 0); }

	i16x8()                      {}
	i16x8(кук укз)       { грузи(укз); }
	i16x8(int16x8_t d)           { данные = d; }
	i16x8(int8x16_t d)           { данные = vreinterpretq_s16_s8(d); }
	i16x8(int32x4_t d)           { данные = vreinterpretq_s16_s32(d); }
	i16x8(цел v)                 { данные = vsetq_lane_s16(v, vdupq_n_s16(0), 0); }
	i16x8(цел a, цел b, цел c, цел d, цел e, цел f, цел g, цел h) {
		int16_t __attribute__((aligned(16))) знач[8] = { (int16_t)h, (int16_t)g, (int16_t)f, (int16_t)e, (int16_t)d, (int16_t)c, (int16_t)b, (int16_t)a };
		данные = vld1q_s16(знач);
	}

	operator int16x8_t()         { return данные; }
};


форс_инлайн i16x8  i16all(цел v)                  { return vdupq_n_s16(v); }

форс_инлайн i16x8  operator+(i16x8 a, i16x8 b)    { return vaddq_s16(a, b); }
форс_инлайн i16x8& operator+=(i16x8& a, i16x8 b)  { return a = a + b; }
форс_инлайн i16x8  operator-(i16x8 a, i16x8 b)    { return vsubq_s16(a, b); }
форс_инлайн i16x8& operator-=(i16x8& a, i16x8 b)  { return a = a - b; }
форс_инлайн i16x8  operator*(i16x8 a, i16x8 b)    { return vmulq_s16(a, b); }
форс_инлайн i16x8& operator*=(i16x8& a, i16x8 b)  { return a = a * b; }

форс_инлайн i16x8  operator&(i16x8 a, i16x8 b)    { return vandq_s16(a, b); }
форс_инлайн i16x8& operator&=(i16x8& a, i16x8 b)  { return a = a & b; }
форс_инлайн i16x8  operator|(i16x8 a, i16x8 b)    { return vorrq_s16(a, b); }
форс_инлайн i16x8& operator|=(i16x8& a, i16x8 b)  { return a = a | b; }
форс_инлайн i16x8  operator^(i16x8 a, i16x8 b)    { return veorq_s16(a, b); }
форс_инлайн i16x8& operator^=(i16x8& a, i16x8 b)  { return a = a ^ b; }
форс_инлайн i16x8  operator~(i16x8 a)             { return vmvnq_s16(a); }

форс_инлайн i16x8  operator>>(i16x8 a, цел b)     { return vreinterpretq_s16_u16(vshlq_u16(vreinterpretq_u16_s16(a), vdupq_n_s16(-b))); }
форс_инлайн i16x8& operator>>=(i16x8& a, цел b)   { return a = a >> b; }
форс_инлайн i16x8  operator<<(i16x8 a, цел b)     { return vshlq_s16(a, vdupq_n_s16(b)); }
форс_инлайн i16x8& operator<<=(i16x8& a, цел b)   { return a = a << b; }

форс_инлайн i16x8  operator==(i16x8 a, i16x8 b)  { return vreinterpretq_s16_u16(vceqq_s16(a, b)); }
форс_инлайн i16x8  operator<(i16x8 a, i16x8 b)   { return vreinterpretq_s16_u16(vcltq_s16(a, b)); }
форс_инлайн i16x8  operator>(i16x8 a, i16x8 b)   { return vreinterpretq_s16_u16(vcgtq_s16(a, b)); }

форс_инлайн бул   AllTrue(i16x8 a) {
	return AllTrue(vreinterpretq_u32_s16(a));
}

struct i32x4 { // 4xint32
	int32x4_t данные;

	i32x4& грузи(кук укз)   { данные = vld1q_s32((int32_t *)укз); return *this; }
	i32x4& Load64(кук укз) { данные = vcombine_s32(vld1_s32((int32_t *)укз), vdup_n_s32(0)); return *this; }
	i32x4& Load32(кук укз) { данные = vsetq_lane_s32(*(int32_t *)укз, vdupq_n_s32(0), 0); return *this; }

	проц   сохрани(ук укз)      { vst1q_s32((int32_t *)укз, данные); }
	проц   Store64(ук укз)    { vst1_s32((int32_t *)укз, vget_low_s32(данные)); }
	проц   Store32(ук укз)    { *(int32_t *)укз = vgetq_lane_s32(данные, 0); }

	i32x4()                      {}
	i32x4(кук укз)       { грузи(укз); }
	i32x4(int32x4_t d)           { данные = d; }
	i32x4(int8x16_t d)           { данные = vreinterpretq_s32_s8(d); }
	i32x4(int16x8_t d)           { данные = vreinterpretq_s32_s16(d); }
	i32x4(цел v)                 { данные = vsetq_lane_s32(v, vdupq_n_s32(0), 0); }
	i32x4(цел a, цел b, цел c, цел d)  {
		int32_t __attribute__((aligned(16))) знач[4] = { (int32_t)d, (int32_t)c, (int32_t)b, (int32_t)a };
		данные = vld1q_s32(знач);
	}
	operator int32x4_t()         { return данные; }
	operator цел()               { return vgetq_lane_s32(данные, 0); }
	operator i16x8() const       { return i16x8(данные); }
};

форс_инлайн i32x4  i32all(цел v)                 { return vdupq_n_s32(v); }

форс_инлайн i32x4  operator+(i32x4 a, i32x4 b)   { return vaddq_s32(a, b); }
форс_инлайн i32x4& operator+=(i32x4& a, i32x4 b) { return a = a + b; }
форс_инлайн i32x4  operator-(i32x4 a, i32x4 b)   { return vsubq_s32(a, b); }
форс_инлайн i32x4& operator-=(i32x4& a, i32x4 b) { return a = a - b; }

форс_инлайн i32x4  operator&(i32x4 a, i32x4 b)    { return vandq_s32(a, b); }
форс_инлайн i32x4& operator&=(i32x4& a, i32x4 b)  { return a = a & b; }
форс_инлайн i32x4  operator|(i32x4 a, i32x4 b)    { return vorrq_s32(a, b); }
форс_инлайн i32x4& operator|=(i32x4& a, i32x4 b)  { return a = a | b; }
форс_инлайн i32x4  operator^(i32x4 a, i32x4 b)    { return veorq_s32(a, b); }
форс_инлайн i32x4& operator^=(i32x4& a, i32x4 b)  { return a = a ^ b; }
форс_инлайн i32x4  operator~(i32x4 a)             { return vmvnq_s32(a); }

форс_инлайн i32x4  operator>>(i32x4 a, цел b)     { return vshlq_s32(a, vdupq_n_s32(-b)); }
форс_инлайн i32x4& operator>>=(i32x4& a, цел b)   { return a = a >> b; }
форс_инлайн i32x4  operator<<(i32x4 a, цел b)     { return vshlq_s32(a, vdupq_n_s32(b)); }
форс_инлайн i32x4& operator<<=(i32x4& a, цел b)   { return a = a << b; }

форс_инлайн i32x4  operator==(i32x4 a, i32x4 b)  { return vreinterpretq_s32_u32(vceqq_s32(a, b)); }
форс_инлайн i32x4  operator<(i32x4 a, i32x4 b)   { return vreinterpretq_s32_u32(vcltq_s32(a, b)); }
форс_инлайн i32x4  operator>(i32x4 a, i32x4 b)   { return vreinterpretq_s32_u32(vcgtq_s32(a, b)); }

форс_инлайн бул   AllTrue(i32x4 a) {
	return AllTrue(vreinterpretq_u32_s32(a));
}

struct i8x16 { // 16*байт
	int8x16_t данные;

	i8x16& грузи(кук укз)   { данные = vld1q_s8((int8_t *)укз); return *this; }
	i8x16& Load64(кук укз) { данные = vcombine_s8(vld1_s8((int8_t *)укз), vdup_n_s8(0)); return *this; }
	i8x16& Load32(кук укз) { данные = vreinterpretq_s8_s32(vsetq_lane_s32(*(int32_t *)укз, vdupq_n_s32(0), 0)); return *this; }

	проц   сохрани(ук укз)      { vst1q_s8((int8_t *)укз, данные); }
	проц   Store64(ук укз)    { vst1_s32((int32_t *)укз, vget_low_s32(vreinterpretq_s32_s8(данные))); }
	проц   Store32(ук укз)    { *(int32_t *)укз = vgetq_lane_s32(vreinterpretq_s32_s8(данные), 0); }

	i8x16()                      {}
	i8x16(кук укз)       { грузи(укз); }
	i8x16(int8x16_t d)           { данные = d; }
	i8x16(int16x8_t d)           { данные = vreinterpretq_s8_s16(d); }
	i8x16(int32x4_t d)           { данные = vreinterpretq_s8_s32(d); }
	i8x16(цел v)                 { данные = vsetq_lane_s8(v, vdupq_n_s8(0), 0); }
	i8x16(цел a, цел b, цел c, цел d, цел e, цел f, цел g, цел h, цел i, цел j, цел k, цел l, цел m, цел n, цел o, цел p)
	{
		int8_t __attribute__((aligned(16))) знач[16] = {
			(int8_t)p, (int8_t)o, (int8_t)n, (int8_t)m,
			(int8_t)l, (int8_t)k, (int8_t)j, (int8_t)i,
			(int8_t)h, (int8_t)g, (int8_t)f, (int8_t)e,
			(int8_t)d, (int8_t)c, (int8_t)b, (int8_t)a,
		};
		данные = vld1q_s8(знач);
	}
	operator int8x16_t() const   { return данные; }
	operator i16x8() const       { return i16x8(данные); }
};

форс_инлайн i8x16  i8all(цел v)                  { return vdupq_n_s8(v); }

форс_инлайн i8x16  operator+(i8x16 a, i8x16 b)   { return vaddq_s8(a, b); }
форс_инлайн i8x16& operator+=(i8x16& a, i8x16 b) { return a = a + b; }
форс_инлайн i8x16  operator-(i8x16 a, i8x16 b)   { return vsubq_s8(a, b); }
форс_инлайн i8x16& operator-=(i8x16& a, i8x16 b) { return a = a - b; }

форс_инлайн i8x16  operator&(i8x16 a, i8x16 b)    { return vandq_s8(a, b); }
форс_инлайн i8x16& operator&=(i8x16& a, i8x16 b)  { return a = a & b; }
форс_инлайн i8x16  operator|(i8x16 a, i8x16 b)    { return vorrq_s8(a, b); }
форс_инлайн i8x16& operator|=(i8x16& a, i8x16 b)  { return a = a | b; }
форс_инлайн i8x16  operator^(i8x16 a, i8x16 b)    { return veorq_s8(a, b); }
форс_инлайн i8x16& operator^=(i8x16& a, i8x16 b)  { return a = a ^ b; }
форс_инлайн i8x16  operator~(i8x16 a)             { return vmvnq_s8(a); }

форс_инлайн f32x4 ToFloat(i32x4 a)               { return vcvtq_f32_s32(a); }
форс_инлайн i32x4 Truncate(f32x4 a)              { return vcvtq_s32_f32(a); }

форс_инлайн i16x8 Unpack8L(i16x8 a)              { return vzipq_s8(vreinterpretq_s8_s16(a), vdupq_n_s8(0)).знач[0]; }
форс_инлайн i16x8 Unpack8H(i16x8 a)              { return vzipq_s8(vreinterpretq_s8_s16(a), vdupq_n_s8(0)).знач[1]; }

форс_инлайн i32x4 Unpack16L(i16x8 a)             { return vzipq_s16(a, vdupq_n_s16(0)).знач[0]; }
форс_инлайн i32x4 Unpack16H(i16x8 a)             { return vzipq_s16(a, vdupq_n_s16(0)).знач[1]; }

форс_инлайн i8x16 пакуй16(i16x8 l, i16x8 h)       { return vreinterpretq_s8_u8(vcombine_u8(vqmovun_s16(l), vqmovun_s16(h))); }
форс_инлайн i8x16 пакуй16(i16x8 l)                { return vreinterpretq_s8_u8(vcombine_u8(vqmovun_s16(l), vdup_n_u8(0))); }

форс_инлайн i16x8 Pack32(i32x4 a)                { return vcombine_s16(vqmovn_s32(a), vdup_n_s16(0)); }

форс_инлайн i16x8 BroadcastLH0(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 0)), vdup_n_s16(vgetq_lane_s16(a, 4)));
}

форс_инлайн i16x8 BroadcastLH1(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 1)), vdup_n_s16(vgetq_lane_s16(a, 5)));
}

форс_инлайн i16x8 BroadcastLH2(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 2)), vdup_n_s16(vgetq_lane_s16(a, 6)));
}

форс_инлайн i16x8 BroadcastLH3(i16x8 a)          {
	return vcombine_s16(vdup_n_s16(vgetq_lane_s16(a, 3)), vdup_n_s16(vgetq_lane_s16(a, 7)));
}

форс_инлайн i16x8 i64all(дим данные)             { return vreinterpretq_s16_u64(vdupq_n_u64(данные)); }
