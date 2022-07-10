/// @ref gtx_color_space_YCoCg
/// @file glm/gtx/color_space_YCoCg.hpp
///
/// @see core (dependence)
///
/// @defgroup gtx_color_space_YCoCg GLM_GTX_color_space_YCoCg
/// @ingroup gtx
///
/// Include <glm/gtx/color_space_YCoCg.hpp> to use the features of this extension.
///
/// дайКЗС to YCoCg conversions and operations

#pragma once

// Dependency:
#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	Ошибка "GLM: GLM_GTX_color_space_YCoCg is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTX_color_space_YCoCg extension included")
#endif

namespace glm
{
	/// @addtogroup gtx_color_space_YCoCg
	/// @{

	/// Преобр a color from дайКЗС color space to YCoCg color space.
	/// @see gtx_color_space_YCoCg
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rgb2YCoCg(
		vec<3, T, Q> const& rgbColor);

	/// Преобр a color from YCoCg color space to дайКЗС color space.
	/// @see gtx_color_space_YCoCg
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> YCoCg2rgb(
		vec<3, T, Q> const& YCoCgColor);

	/// Преобр a color from дайКЗС color space to YCoCgR color space.
	/// @see "YCoCg-R: A Цвет Space with дайКЗС Reversibility and наименьш Dynamic Диапазон"
	/// @see gtx_color_space_YCoCg
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> rgb2YCoCgR(
		vec<3, T, Q> const& rgbColor);

	/// Преобр a color from YCoCgR color space to дайКЗС color space.
	/// @see "YCoCg-R: A Цвет Space with дайКЗС Reversibility and наименьш Dynamic Диапазон"
	/// @see gtx_color_space_YCoCg
	template<typename T, qualifier Q>
	GLM_FUNC_DECL vec<3, T, Q> YCoCgR2rgb(
		vec<3, T, Q> const& YCoCgColor);

	/// @}
}//namespace glm

#include "color_space_YCoCg.inl"
