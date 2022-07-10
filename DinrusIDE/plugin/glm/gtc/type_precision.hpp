/// @ref gtc_type_precision
/// @file glm/gtc/type_precision.hpp
///
/// @see core (dependence)
/// @see gtc_quaternion (dependence)
///
/// @defgroup gtc_type_precision GLM_GTC_type_precision
/// @ingroup gtc
///
/// Include <glm/gtc/type_precision.hpp> to use the features of this extension.
///
/// Defines specific C++-based qualifier types.

#pragma once

// Dependency:
#include "../gtc/quaternion.hpp"
#include "../gtc/vec1.hpp"
#include "../ext/scalar_int_sized.hpp"
#include "../ext/scalar_uint_sized.hpp"
#include "../detail/type_vec2.hpp"
#include "../detail/type_vec3.hpp"
#include "../detail/type_vec4.hpp"
#include "../detail/type_mat2x2.hpp"
#include "../detail/type_mat2x3.hpp"
#include "../detail/type_mat2x4.hpp"
#include "../detail/type_mat3x2.hpp"
#include "../detail/type_mat3x3.hpp"
#include "../detail/type_mat3x4.hpp"
#include "../detail/type_mat4x2.hpp"
#include "../detail/type_mat4x3.hpp"
#include "../detail/type_mat4x4.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_type_precision extension included")
#endif

namespace glm
{
	///////////////////////////
	// Signed int vector types

	/// @addtogroup gtc_type_precision
	/// @{

	/// наименьш qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 lowp_int8;

	/// наименьш qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 lowp_int16;

	/// наименьш qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 lowp_int32;

	/// наименьш qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 lowp_int64;

	/// наименьш qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 lowp_int8_t;

	/// наименьш qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 lowp_int16_t;

	/// наименьш qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 lowp_int32_t;

	/// наименьш qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 lowp_int64_t;

	/// наименьш qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 lowp_i8;

	/// наименьш qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 lowp_i16;

	/// наименьш qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 lowp_i32;

	/// наименьш qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 lowp_i64;

	/// Medium qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 mediump_int8;

	/// Medium qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 mediump_int16;

	/// Medium qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 mediump_int32;

	/// Medium qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 mediump_int64;

	/// Medium qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 mediump_int8_t;

	/// Medium qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 mediump_int16_t;

	/// Medium qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 mediump_int32_t;

	/// Medium qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 mediump_int64_t;

	/// Medium qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 mediump_i8;

	/// Medium qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 mediump_i16;

	/// Medium qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 mediump_i32;

	/// Medium qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 mediump_i64;

	/// наибольш qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 highp_int8;

	/// наибольш qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 highp_int16;

	/// наибольш qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 highp_int32;

	/// наибольш qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 highp_int64;

	/// наибольш qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 highp_int8_t;

	/// наибольш qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 highp_int16_t;

	/// 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 highp_int32_t;

	/// наибольш qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 highp_int64_t;

	/// наибольш qualifier 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 highp_i8;

	/// наибольш qualifier 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 highp_i16;

	/// наибольш qualifier 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 highp_i32;

	/// наибольш qualifier 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 highp_i64;


#if GLM_HAS_EXTENDED_INTEGER_TYPE
	using std::int8_t;
	using std::int16_t;
	using std::int32_t;
	using std::int64_t;
#else
	/// 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 int8_t;

	/// 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 int16_t;

	/// 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 int32_t;

	/// 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 int64_t;
#endif

	/// 8 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int8 i8;

	/// 16 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int16 i16;

	/// 32 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int32 i32;

	/// 64 bit signed integer тип.
	/// @see gtc_type_precision
	typedef detail::int64 i64;



	/// наименьш qualifier 8 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i8, lowp> lowp_i8vec1;

	/// наименьш qualifier 8 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i8, lowp> lowp_i8vec2;

	/// наименьш qualifier 8 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i8, lowp> lowp_i8vec3;

	/// наименьш qualifier 8 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i8, lowp> lowp_i8vec4;


	/// Medium qualifier 8 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i8, mediump> mediump_i8vec1;

	/// Medium qualifier 8 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i8, mediump> mediump_i8vec2;

	/// Medium qualifier 8 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i8, mediump> mediump_i8vec3;

	/// Medium qualifier 8 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i8, mediump> mediump_i8vec4;


	/// наибольш qualifier 8 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i8, highp> highp_i8vec1;

	/// наибольш qualifier 8 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i8, highp> highp_i8vec2;

	/// наибольш qualifier 8 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i8, highp> highp_i8vec3;

	/// наибольш qualifier 8 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i8, highp> highp_i8vec4;



	/// 8 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i8, defaultp> i8vec1;

	/// 8 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i8, defaultp> i8vec2;

	/// 8 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i8, defaultp> i8vec3;

	/// 8 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i8, defaultp> i8vec4;





	/// наименьш qualifier 16 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i16, lowp>		lowp_i16vec1;

	/// наименьш qualifier 16 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i16, lowp>		lowp_i16vec2;

	/// наименьш qualifier 16 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i16, lowp>		lowp_i16vec3;

	/// наименьш qualifier 16 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i16, lowp>		lowp_i16vec4;


	/// Medium qualifier 16 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i16, mediump>		mediump_i16vec1;

	/// Medium qualifier 16 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i16, mediump>		mediump_i16vec2;

	/// Medium qualifier 16 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i16, mediump>		mediump_i16vec3;

	/// Medium qualifier 16 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i16, mediump>		mediump_i16vec4;


	/// наибольш qualifier 16 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i16, highp>		highp_i16vec1;

	/// наибольш qualifier 16 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i16, highp>		highp_i16vec2;

	/// наибольш qualifier 16 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i16, highp>		highp_i16vec3;

	/// наибольш qualifier 16 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i16, highp>		highp_i16vec4;




	/// 16 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i16, defaultp> i16vec1;

	/// 16 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i16, defaultp> i16vec2;

	/// 16 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i16, defaultp> i16vec3;

	/// 16 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i16, defaultp> i16vec4;



	/// наименьш qualifier 32 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i32, lowp>		lowp_i32vec1;

	/// наименьш qualifier 32 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i32, lowp>		lowp_i32vec2;

	/// наименьш qualifier 32 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i32, lowp>		lowp_i32vec3;

	/// наименьш qualifier 32 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i32, lowp>		lowp_i32vec4;


	/// Medium qualifier 32 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i32, mediump>		mediump_i32vec1;

	/// Medium qualifier 32 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i32, mediump>		mediump_i32vec2;

	/// Medium qualifier 32 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i32, mediump>		mediump_i32vec3;

	/// Medium qualifier 32 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i32, mediump>		mediump_i32vec4;


	/// наибольш qualifier 32 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i32, highp>		highp_i32vec1;

	/// наибольш qualifier 32 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i32, highp>		highp_i32vec2;

	/// наибольш qualifier 32 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i32, highp>		highp_i32vec3;

	/// наибольш qualifier 32 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i32, highp>		highp_i32vec4;


	/// 32 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i32, defaultp> i32vec1;

	/// 32 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i32, defaultp> i32vec2;

	/// 32 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i32, defaultp> i32vec3;

	/// 32 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i32, defaultp> i32vec4;




	/// наименьш qualifier 64 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i64, lowp>		lowp_i64vec1;

	/// наименьш qualifier 64 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i64, lowp>		lowp_i64vec2;

	/// наименьш qualifier 64 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i64, lowp>		lowp_i64vec3;

	/// наименьш qualifier 64 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i64, lowp>		lowp_i64vec4;


	/// Medium qualifier 64 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i64, mediump>		mediump_i64vec1;

	/// Medium qualifier 64 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i64, mediump>		mediump_i64vec2;

	/// Medium qualifier 64 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i64, mediump>		mediump_i64vec3;

	/// Medium qualifier 64 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i64, mediump>		mediump_i64vec4;


	/// наибольш qualifier 64 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i64, highp>		highp_i64vec1;

	/// наибольш qualifier 64 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i64, highp>		highp_i64vec2;

	/// наибольш qualifier 64 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i64, highp>		highp_i64vec3;

	/// наибольш qualifier 64 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i64, highp>		highp_i64vec4;


	/// 64 bit signed integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, i64, defaultp> i64vec1;

	/// 64 bit signed integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, i64, defaultp> i64vec2;

	/// 64 bit signed integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, i64, defaultp> i64vec3;

	/// 64 bit signed integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, i64, defaultp> i64vec4;


	/////////////////////////////
	// Unsigned int vector types

	/// наименьш qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 lowp_uint8;

	/// наименьш qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 lowp_uint16;

	/// наименьш qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 lowp_uint32;

	/// наименьш qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 lowp_uint64;

	/// наименьш qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 lowp_uint8_t;

	/// наименьш qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 lowp_uint16_t;

	/// наименьш qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 lowp_uint32_t;

	/// наименьш qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 lowp_uint64_t;

	/// наименьш qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 lowp_u8;

	/// наименьш qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 lowp_u16;

	/// наименьш qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 lowp_u32;

	/// наименьш qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 lowp_u64;

	/// Medium qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 mediump_uint8;

	/// Medium qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 mediump_uint16;

	/// Medium qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 mediump_uint32;

	/// Medium qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 mediump_uint64;

	/// Medium qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 mediump_uint8_t;

	/// Medium qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 mediump_uint16_t;

	/// Medium qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 mediump_uint32_t;

	/// Medium qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 mediump_uint64_t;

	/// Medium qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 mediump_u8;

	/// Medium qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 mediump_u16;

	/// Medium qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 mediump_u32;

	/// Medium qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 mediump_u64;

	/// наибольш qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 highp_uint8;

	/// наибольш qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 highp_uint16;

	/// наибольш qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 highp_uint32;

	/// наибольш qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 highp_uint64;

	/// наибольш qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 highp_uint8_t;

	/// наибольш qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 highp_uint16_t;

	/// наибольш qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 highp_uint32_t;

	/// наибольш qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 highp_uint64_t;

	/// наибольш qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 highp_u8;

	/// наибольш qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 highp_u16;

	/// наибольш qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 highp_u32;

	/// наибольш qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 highp_u64;

#if GLM_HAS_EXTENDED_INTEGER_TYPE
	using std::uint8_t;
	using std::uint16_t;
	using std::uint32_t;
	using std::uint64_t;
#else
	/// дефолт qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 uint8_t;

	/// дефолт qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 uint16_t;

	/// дефолт qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 uint32_t;

	/// дефолт qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 uint64_t;
#endif

	/// дефолт qualifier 8 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint8 u8;

	/// дефолт qualifier 16 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint16 u16;

	/// дефолт qualifier 32 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint32 u32;

	/// дефолт qualifier 64 bit unsigned integer тип.
	/// @see gtc_type_precision
	typedef detail::uint64 u64;





	//////////////////////
	// Float vector types

	/// Single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float float32;

	/// Дво-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef double float64;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_float32;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_float64;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_float32_t;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_float64_t;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_f32;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_f64;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_float32;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_float64;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_float32_t;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_float64_t;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_f32;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_f64;


	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_float32;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_float64;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_float32_t;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_float64_t;

	/// наименьш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 lowp_f32;

	/// наименьш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 lowp_f64;


	/// Medium 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 mediump_float32;

	/// Medium 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 mediump_float64;

	/// Medium 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 mediump_float32_t;

	/// Medium 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 mediump_float64_t;

	/// Medium 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 mediump_f32;

	/// Medium 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 mediump_f64;


	/// наибольш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 highp_float32;

	/// наибольш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 highp_float64;

	/// наибольш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 highp_float32_t;

	/// наибольш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 highp_float64_t;

	/// наибольш 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 highp_f32;

	/// наибольш 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float64 highp_f64;


#if(defined(GLM_PRECISION_LOWP_FLOAT))
	/// дефолт 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef lowp_float32_t float32_t;

	/// дефолт 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef lowp_float64_t float64_t;

	/// дефолт 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef lowp_f32 f32;

	/// дефолт 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef lowp_f64 f64;

#elif(defined(GLM_PRECISION_MEDIUMP_FLOAT))
	/// дефолт 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef mediump_float32 float32_t;

	/// дефолт 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef mediump_float64 float64_t;

	/// дефолт 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef mediump_float32 f32;

	/// дефолт 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef mediump_float64 f64;

#else//(defined(GLM_PRECISION_HIGHP_FLOAT))

	/// дефолт 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef highp_float32_t float32_t;

	/// дефолт 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef highp_float64_t float64_t;

	/// дефолт 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef highp_float32_t f32;

	/// дефолт 64 bit double-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef highp_float64_t f64;
#endif


	/// наименьш single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, float, lowp> lowp_fvec1;

	/// наименьш single-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, float, lowp> lowp_fvec2;

	/// наименьш single-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, float, lowp> lowp_fvec3;

	/// наименьш single-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, float, lowp> lowp_fvec4;


	/// Medium single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, float, mediump> mediump_fvec1;

	/// Medium Single-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, float, mediump> mediump_fvec2;

	/// Medium Single-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, float, mediump> mediump_fvec3;

	/// Medium Single-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, float, mediump> mediump_fvec4;


	/// наибольш single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, float, highp> highp_fvec1;

	/// наибольш Single-qualifier floating-point vector of 2 components.
	/// @see core_precision
	typedef vec<2, float, highp> highp_fvec2;

	/// наибольш Single-qualifier floating-point vector of 3 components.
	/// @see core_precision
	typedef vec<3, float, highp> highp_fvec3;

	/// наибольш Single-qualifier floating-point vector of 4 components.
	/// @see core_precision
	typedef vec<4, float, highp> highp_fvec4;


	/// наименьш single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f32, lowp> lowp_f32vec1;

	/// наименьш single-qualifier floating-point vector of 2 components.
	/// @see core_precision
	typedef vec<2, f32, lowp> lowp_f32vec2;

	/// наименьш single-qualifier floating-point vector of 3 components.
	/// @see core_precision
	typedef vec<3, f32, lowp> lowp_f32vec3;

	/// наименьш single-qualifier floating-point vector of 4 components.
	/// @see core_precision
	typedef vec<4, f32, lowp> lowp_f32vec4;

	/// Medium single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f32, mediump> mediump_f32vec1;

	/// Medium single-qualifier floating-point vector of 2 components.
	/// @see core_precision
	typedef vec<2, f32, mediump> mediump_f32vec2;

	/// Medium single-qualifier floating-point vector of 3 components.
	/// @see core_precision
	typedef vec<3, f32, mediump> mediump_f32vec3;

	/// Medium single-qualifier floating-point vector of 4 components.
	/// @see core_precision
	typedef vec<4, f32, mediump> mediump_f32vec4;

	/// наибольш single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f32, highp> highp_f32vec1;

	/// наибольш single-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, f32, highp> highp_f32vec2;

	/// наибольш single-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, f32, highp> highp_f32vec3;

	/// наибольш single-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, f32, highp> highp_f32vec4;


	/// наименьш double-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f64, lowp> lowp_f64vec1;

	/// наименьш double-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, f64, lowp> lowp_f64vec2;

	/// наименьш double-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, f64, lowp> lowp_f64vec3;

	/// наименьш double-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, f64, lowp> lowp_f64vec4;

	/// Medium double-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f64, mediump> mediump_f64vec1;

	/// Medium double-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, f64, mediump> mediump_f64vec2;

	/// Medium double-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, f64, mediump> mediump_f64vec3;

	/// Medium double-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, f64, mediump> mediump_f64vec4;

	/// наибольш double-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f64, highp> highp_f64vec1;

	/// наибольш double-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, f64, highp> highp_f64vec2;

	/// наибольш double-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, f64, highp> highp_f64vec3;

	/// наибольш double-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, f64, highp> highp_f64vec4;



	//////////////////////
	// Float matrix types

	/// наименьш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef lowp_f32 lowp_fmat1x1;

	/// наименьш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, lowp> lowp_fmat2x2;

	/// наименьш single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, lowp> lowp_fmat2x3;

	/// наименьш single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, lowp> lowp_fmat2x4;

	/// наименьш single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, lowp> lowp_fmat3x2;

	/// наименьш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, lowp> lowp_fmat3x3;

	/// наименьш single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, lowp> lowp_fmat3x4;

	/// наименьш single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, lowp> lowp_fmat4x2;

	/// наименьш single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, lowp> lowp_fmat4x3;

	/// наименьш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, lowp> lowp_fmat4x4;

	/// наименьш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef lowp_fmat1x1 lowp_fmat1;

	/// наименьш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef lowp_fmat2x2 lowp_fmat2;

	/// наименьш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef lowp_fmat3x3 lowp_fmat3;

	/// наименьш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef lowp_fmat4x4 lowp_fmat4;


	/// Medium single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef mediump_f32 mediump_fmat1x1;

	/// Medium single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, mediump> mediump_fmat2x2;

	/// Medium single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, mediump> mediump_fmat2x3;

	/// Medium single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, mediump> mediump_fmat2x4;

	/// Medium single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, mediump> mediump_fmat3x2;

	/// Medium single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, mediump> mediump_fmat3x3;

	/// Medium single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, mediump> mediump_fmat3x4;

	/// Medium single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, mediump> mediump_fmat4x2;

	/// Medium single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, mediump> mediump_fmat4x3;

	/// Medium single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, mediump> mediump_fmat4x4;

	/// Medium single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef mediump_fmat1x1 mediump_fmat1;

	/// Medium single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mediump_fmat2x2 mediump_fmat2;

	/// Medium single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mediump_fmat3x3 mediump_fmat3;

	/// Medium single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mediump_fmat4x4 mediump_fmat4;


	/// наибольш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef highp_f32 highp_fmat1x1;

	/// наибольш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, highp> highp_fmat2x2;

	/// наибольш single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, highp> highp_fmat2x3;

	/// наибольш single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, highp> highp_fmat2x4;

	/// наибольш single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, highp> highp_fmat3x2;

	/// наибольш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, highp> highp_fmat3x3;

	/// наибольш single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, highp> highp_fmat3x4;

	/// наибольш single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, highp> highp_fmat4x2;

	/// наибольш single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, highp> highp_fmat4x3;

	/// наибольш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, highp> highp_fmat4x4;

	/// наибольш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef highp_fmat1x1 highp_fmat1;

	/// наибольш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef highp_fmat2x2 highp_fmat2;

	/// наибольш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef highp_fmat3x3 highp_fmat3;

	/// наибольш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef highp_fmat4x4 highp_fmat4;


	/// наименьш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f32 lowp_f32mat1x1;

	/// наименьш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, lowp> lowp_f32mat2x2;

	/// наименьш single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, lowp> lowp_f32mat2x3;

	/// наименьш single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, lowp> lowp_f32mat2x4;

	/// наименьш single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, lowp> lowp_f32mat3x2;

	/// наименьш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, lowp> lowp_f32mat3x3;

	/// наименьш single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, lowp> lowp_f32mat3x4;

	/// наименьш single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, lowp> lowp_f32mat4x2;

	/// наименьш single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, lowp> lowp_f32mat4x3;

	/// наименьш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, lowp> lowp_f32mat4x4;

	/// наименьш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef detail::tmat1x1<f32, lowp> lowp_f32mat1;

	/// наименьш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef lowp_f32mat2x2 lowp_f32mat2;

	/// наименьш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef lowp_f32mat3x3 lowp_f32mat3;

	/// наименьш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef lowp_f32mat4x4 lowp_f32mat4;


	/// наибольш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f32 mediump_f32mat1x1;

	/// наименьш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, mediump> mediump_f32mat2x2;

	/// Medium single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, mediump> mediump_f32mat2x3;

	/// Medium single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, mediump> mediump_f32mat2x4;

	/// Medium single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, mediump> mediump_f32mat3x2;

	/// Medium single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, mediump> mediump_f32mat3x3;

	/// Medium single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, mediump> mediump_f32mat3x4;

	/// Medium single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, mediump> mediump_f32mat4x2;

	/// Medium single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, mediump> mediump_f32mat4x3;

	/// Medium single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, mediump> mediump_f32mat4x4;

	/// Medium single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef detail::tmat1x1<f32, mediump> f32mat1;

	/// Medium single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mediump_f32mat2x2 mediump_f32mat2;

	/// Medium single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mediump_f32mat3x3 mediump_f32mat3;

	/// Medium single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mediump_f32mat4x4 mediump_f32mat4;


	/// наибольш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f32 highp_f32mat1x1;

	/// наибольш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, highp> highp_f32mat2x2;

	/// наибольш single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, highp> highp_f32mat2x3;

	/// наибольш single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, highp> highp_f32mat2x4;

	/// наибольш single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, highp> highp_f32mat3x2;

	/// наибольш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, highp> highp_f32mat3x3;

	/// наибольш single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, highp> highp_f32mat3x4;

	/// наибольш single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, highp> highp_f32mat4x2;

	/// наибольш single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, highp> highp_f32mat4x3;

	/// наибольш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, highp> highp_f32mat4x4;

	/// наибольш single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef detail::tmat1x1<f32, highp> f32mat1;

	/// наибольш single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef highp_f32mat2x2 highp_f32mat2;

	/// наибольш single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef highp_f32mat3x3 highp_f32mat3;

	/// наибольш single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef highp_f32mat4x4 highp_f32mat4;


	/// наименьш double-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f64 lowp_f64mat1x1;

	/// наименьш double-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f64, lowp> lowp_f64mat2x2;

	/// наименьш double-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f64, lowp> lowp_f64mat2x3;

	/// наименьш double-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f64, lowp> lowp_f64mat2x4;

	/// наименьш double-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f64, lowp> lowp_f64mat3x2;

	/// наименьш double-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f64, lowp> lowp_f64mat3x3;

	/// наименьш double-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f64, lowp> lowp_f64mat3x4;

	/// наименьш double-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f64, lowp> lowp_f64mat4x2;

	/// наименьш double-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f64, lowp> lowp_f64mat4x3;

	/// наименьш double-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f64, lowp> lowp_f64mat4x4;

	/// наименьш double-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef lowp_f64mat1x1 lowp_f64mat1;

	/// наименьш double-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef lowp_f64mat2x2 lowp_f64mat2;

	/// наименьш double-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef lowp_f64mat3x3 lowp_f64mat3;

	/// наименьш double-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef lowp_f64mat4x4 lowp_f64mat4;


	/// Medium double-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f64 Highp_f64mat1x1;

	/// Medium double-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f64, mediump> mediump_f64mat2x2;

	/// Medium double-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f64, mediump> mediump_f64mat2x3;

	/// Medium double-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f64, mediump> mediump_f64mat2x4;

	/// Medium double-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f64, mediump> mediump_f64mat3x2;

	/// Medium double-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f64, mediump> mediump_f64mat3x3;

	/// Medium double-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f64, mediump> mediump_f64mat3x4;

	/// Medium double-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f64, mediump> mediump_f64mat4x2;

	/// Medium double-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f64, mediump> mediump_f64mat4x3;

	/// Medium double-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f64, mediump> mediump_f64mat4x4;

	/// Medium double-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef mediump_f64mat1x1 mediump_f64mat1;

	/// Medium double-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mediump_f64mat2x2 mediump_f64mat2;

	/// Medium double-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mediump_f64mat3x3 mediump_f64mat3;

	/// Medium double-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mediump_f64mat4x4 mediump_f64mat4;

	/// наибольш double-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f64 highp_f64mat1x1;

	/// наибольш double-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f64, highp> highp_f64mat2x2;

	/// наибольш double-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f64, highp> highp_f64mat2x3;

	/// наибольш double-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f64, highp> highp_f64mat2x4;

	/// наибольш double-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f64, highp> highp_f64mat3x2;

	/// наибольш double-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f64, highp> highp_f64mat3x3;

	/// наибольш double-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f64, highp> highp_f64mat3x4;

	/// наибольш double-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f64, highp> highp_f64mat4x2;

	/// наибольш double-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f64, highp> highp_f64mat4x3;

	/// наибольш double-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f64, highp> highp_f64mat4x4;

	/// наибольш double-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef highp_f64mat1x1 highp_f64mat1;

	/// наибольш double-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef highp_f64mat2x2 highp_f64mat2;

	/// наибольш double-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef highp_f64mat3x3 highp_f64mat3;

	/// наибольш double-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef highp_f64mat4x4 highp_f64mat4;




	/// наименьш qualifier 8 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u8, lowp> lowp_u8vec1;

	/// наименьш qualifier 8 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u8, lowp> lowp_u8vec2;

	/// наименьш qualifier 8 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u8, lowp> lowp_u8vec3;

	/// наименьш qualifier 8 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u8, lowp> lowp_u8vec4;


	/// Medium qualifier 8 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u8, mediump> mediump_u8vec1;

	/// Medium qualifier 8 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u8, mediump> mediump_u8vec2;

	/// Medium qualifier 8 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u8, mediump> mediump_u8vec3;

	/// Medium qualifier 8 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u8, mediump> mediump_u8vec4;


	/// наибольш qualifier 8 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u8, highp> highp_u8vec1;

	/// наибольш qualifier 8 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u8, highp> highp_u8vec2;

	/// наибольш qualifier 8 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u8, highp> highp_u8vec3;

	/// наибольш qualifier 8 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u8, highp> highp_u8vec4;



	/// дефолт qualifier 8 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u8, defaultp> u8vec1;

	/// дефолт qualifier 8 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u8, defaultp> u8vec2;

	/// дефолт qualifier 8 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u8, defaultp> u8vec3;

	/// дефолт qualifier 8 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u8, defaultp> u8vec4;




	/// наименьш qualifier 16 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u16, lowp>		lowp_u16vec1;

	/// наименьш qualifier 16 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u16, lowp>		lowp_u16vec2;

	/// наименьш qualifier 16 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u16, lowp>		lowp_u16vec3;

	/// наименьш qualifier 16 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u16, lowp>		lowp_u16vec4;


	/// Medium qualifier 16 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u16, mediump>		mediump_u16vec1;

	/// Medium qualifier 16 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u16, mediump>		mediump_u16vec2;

	/// Medium qualifier 16 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u16, mediump>		mediump_u16vec3;

	/// Medium qualifier 16 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u16, mediump>		mediump_u16vec4;


	/// наибольш qualifier 16 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u16, highp>		highp_u16vec1;

	/// наибольш qualifier 16 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u16, highp>		highp_u16vec2;

	/// наибольш qualifier 16 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u16, highp>		highp_u16vec3;

	/// наибольш qualifier 16 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u16, highp>		highp_u16vec4;




	/// дефолт qualifier 16 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u16, defaultp> u16vec1;

	/// дефолт qualifier 16 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u16, defaultp> u16vec2;

	/// дефолт qualifier 16 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u16, defaultp> u16vec3;

	/// дефолт qualifier 16 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u16, defaultp> u16vec4;



	/// наименьш qualifier 32 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u32, lowp>		lowp_u32vec1;

	/// наименьш qualifier 32 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u32, lowp>		lowp_u32vec2;

	/// наименьш qualifier 32 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u32, lowp>		lowp_u32vec3;

	/// наименьш qualifier 32 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u32, lowp>		lowp_u32vec4;


	/// Medium qualifier 32 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u32, mediump>		mediump_u32vec1;

	/// Medium qualifier 32 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u32, mediump>		mediump_u32vec2;

	/// Medium qualifier 32 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u32, mediump>		mediump_u32vec3;

	/// Medium qualifier 32 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u32, mediump>		mediump_u32vec4;


	/// наибольш qualifier 32 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u32, highp>		highp_u32vec1;

	/// наибольш qualifier 32 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u32, highp>		highp_u32vec2;

	/// наибольш qualifier 32 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u32, highp>		highp_u32vec3;

	/// наибольш qualifier 32 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u32, highp>		highp_u32vec4;



	/// дефолт qualifier 32 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u32, defaultp> u32vec1;

	/// дефолт qualifier 32 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u32, defaultp> u32vec2;

	/// дефолт qualifier 32 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u32, defaultp> u32vec3;

	/// дефолт qualifier 32 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u32, defaultp> u32vec4;




	/// наименьш qualifier 64 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u64, lowp>		lowp_u64vec1;

	/// наименьш qualifier 64 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u64, lowp>		lowp_u64vec2;

	/// наименьш qualifier 64 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u64, lowp>		lowp_u64vec3;

	/// наименьш qualifier 64 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u64, lowp>		lowp_u64vec4;


	/// Medium qualifier 64 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u64, mediump>		mediump_u64vec1;

	/// Medium qualifier 64 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u64, mediump>		mediump_u64vec2;

	/// Medium qualifier 64 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u64, mediump>		mediump_u64vec3;

	/// Medium qualifier 64 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u64, mediump>		mediump_u64vec4;


	/// наибольш qualifier 64 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u64, highp>		highp_u64vec1;

	/// наибольш qualifier 64 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u64, highp>		highp_u64vec2;

	/// наибольш qualifier 64 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u64, highp>		highp_u64vec3;

	/// наибольш qualifier 64 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u64, highp>		highp_u64vec4;




	/// дефолт qualifier 64 bit unsigned integer scalar тип.
	/// @see gtc_type_precision
	typedef vec<1, u64, defaultp> u64vec1;

	/// дефолт qualifier 64 bit unsigned integer vector of 2 components тип.
	/// @see gtc_type_precision
	typedef vec<2, u64, defaultp> u64vec2;

	/// дефолт qualifier 64 bit unsigned integer vector of 3 components тип.
	/// @see gtc_type_precision
	typedef vec<3, u64, defaultp> u64vec3;

	/// дефолт qualifier 64 bit unsigned integer vector of 4 components тип.
	/// @see gtc_type_precision
	typedef vec<4, u64, defaultp> u64vec4;


	//////////////////////
	// Float vector types

	/// 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 float32_t;

	/// 32 bit single-qualifier floating-point scalar.
	/// @see gtc_type_precision
	typedef float32 f32;

#	ifndef GLM_FORCE_SINGLE_ONLY

		/// 64 bit double-qualifier floating-point scalar.
		/// @see gtc_type_precision
		typedef float64 float64_t;

		/// 64 bit double-qualifier floating-point scalar.
		/// @see gtc_type_precision
		typedef float64 f64;
#	endif//GLM_FORCE_SINGLE_ONLY

	/// Single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, float, defaultp> fvec1;

	/// Single-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, float, defaultp> fvec2;

	/// Single-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, float, defaultp> fvec3;

	/// Single-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, float, defaultp> fvec4;


	/// Single-qualifier floating-point vector of 1 component.
	/// @see gtc_type_precision
	typedef vec<1, f32, defaultp> f32vec1;

	/// Single-qualifier floating-point vector of 2 components.
	/// @see gtc_type_precision
	typedef vec<2, f32, defaultp> f32vec2;

	/// Single-qualifier floating-point vector of 3 components.
	/// @see gtc_type_precision
	typedef vec<3, f32, defaultp> f32vec3;

	/// Single-qualifier floating-point vector of 4 components.
	/// @see gtc_type_precision
	typedef vec<4, f32, defaultp> f32vec4;

#	ifndef GLM_FORCE_SINGLE_ONLY
		/// Дво-qualifier floating-point vector of 1 component.
		/// @see gtc_type_precision
		typedef vec<1, f64, defaultp> f64vec1;

		/// Дво-qualifier floating-point vector of 2 components.
		/// @see gtc_type_precision
		typedef vec<2, f64, defaultp> f64vec2;

		/// Дво-qualifier floating-point vector of 3 components.
		/// @see gtc_type_precision
		typedef vec<3, f64, defaultp> f64vec3;

		/// Дво-qualifier floating-point vector of 4 components.
		/// @see gtc_type_precision
		typedef vec<4, f64, defaultp> f64vec4;
#	endif//GLM_FORCE_SINGLE_ONLY


	//////////////////////
	// Float matrix types

	/// Single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef detail::tmat1x1<f32> fmat1;

	/// Single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, defaultp> fmat2;

	/// Single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, defaultp> fmat3;

	/// Single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, defaultp> fmat4;


	/// Single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f32 fmat1x1;

	/// Single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, defaultp> fmat2x2;

	/// Single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, defaultp> fmat2x3;

	/// Single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, defaultp> fmat2x4;

	/// Single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, defaultp> fmat3x2;

	/// Single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, defaultp> fmat3x3;

	/// Single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, defaultp> fmat3x4;

	/// Single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, defaultp> fmat4x2;

	/// Single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, defaultp> fmat4x3;

	/// Single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, defaultp> fmat4x4;


	/// Single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef detail::tmat1x1<f32, defaultp> f32mat1;

	/// Single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, defaultp> f32mat2;

	/// Single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, defaultp> f32mat3;

	/// Single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, defaultp> f32mat4;


	/// Single-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f32 f32mat1x1;

	/// Single-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f32, defaultp> f32mat2x2;

	/// Single-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f32, defaultp> f32mat2x3;

	/// Single-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f32, defaultp> f32mat2x4;

	/// Single-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f32, defaultp> f32mat3x2;

	/// Single-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f32, defaultp> f32mat3x3;

	/// Single-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f32, defaultp> f32mat3x4;

	/// Single-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f32, defaultp> f32mat4x2;

	/// Single-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f32, defaultp> f32mat4x3;

	/// Single-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f32, defaultp> f32mat4x4;


#	ifndef GLM_FORCE_SINGLE_ONLY

	/// Дво-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef detail::tmat1x1<f64, defaultp> f64mat1;

	/// Дво-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f64, defaultp> f64mat2;

	/// Дво-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f64, defaultp> f64mat3;

	/// Дво-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f64, defaultp> f64mat4;


	/// Дво-qualifier floating-point 1x1 matrix.
	/// @see gtc_type_precision
	//typedef f64 f64mat1x1;

	/// Дво-qualifier floating-point 2x2 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 2, f64, defaultp> f64mat2x2;

	/// Дво-qualifier floating-point 2x3 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 3, f64, defaultp> f64mat2x3;

	/// Дво-qualifier floating-point 2x4 matrix.
	/// @see gtc_type_precision
	typedef mat<2, 4, f64, defaultp> f64mat2x4;

	/// Дво-qualifier floating-point 3x2 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 2, f64, defaultp> f64mat3x2;

	/// Дво-qualifier floating-point 3x3 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 3, f64, defaultp> f64mat3x3;

	/// Дво-qualifier floating-point 3x4 matrix.
	/// @see gtc_type_precision
	typedef mat<3, 4, f64, defaultp> f64mat3x4;

	/// Дво-qualifier floating-point 4x2 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 2, f64, defaultp> f64mat4x2;

	/// Дво-qualifier floating-point 4x3 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 3, f64, defaultp> f64mat4x3;

	/// Дво-qualifier floating-point 4x4 matrix.
	/// @see gtc_type_precision
	typedef mat<4, 4, f64, defaultp> f64mat4x4;

#	endif//GLM_FORCE_SINGLE_ONLY

	//////////////////////////
	// Quaternion types

	/// Single-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f32, defaultp> f32quat;

	/// наименьш single-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f32, lowp> lowp_f32quat;

	/// наименьш double-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f64, lowp> lowp_f64quat;

	/// Medium single-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f32, mediump> mediump_f32quat;

#	ifndef GLM_FORCE_SINGLE_ONLY

	/// Medium double-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f64, mediump> mediump_f64quat;

	/// наибольш single-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f32, highp> highp_f32quat;

	/// наибольш double-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f64, highp> highp_f64quat;

	/// Дво-qualifier floating-point quaternion.
	/// @see gtc_type_precision
	typedef qua<f64, defaultp> f64quat;

#	endif//GLM_FORCE_SINGLE_ONLY

	/// @}
}//namespace glm

#include "type_precision.inl"
