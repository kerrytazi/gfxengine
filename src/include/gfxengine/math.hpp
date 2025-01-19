#pragma once

#include <cmath>
#include <limits>
#include <array>

/*
#define GLM_FORCE_XYZW_ONLY
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
*/


#include <bit>

// https://github.com/a2flo/floor/blob/master/constexpr/const_math.hpp
namespace math
{

typedef long double max_fp_type;

//! pi
template <typename fp_type = max_fp_type>
constexpr fp_type PI = fp_type(3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825L);
//! pi/2
template <typename fp_type = max_fp_type>
constexpr fp_type PI_DIV_2 = fp_type(1.570796326794896619231321691639751442098584699687552910487472296153908203143104499314L);
//! pi/4
template <typename fp_type = max_fp_type>
constexpr fp_type PI_DIV_4 = fp_type(0.785398163397448309615660845819875721049292349843776455243736148076954101571552249657L);
//! pi/180
template <typename fp_type = max_fp_type>
constexpr fp_type PI_DIV_180 = fp_type(0.0174532925199432957692369076848861271344287188854172545609719144017100911460344944L);
//! pi/360
template <typename fp_type = max_fp_type>
constexpr fp_type PI_DIV_360 = fp_type(0.0087266462599716478846184538424430635672143594427086272804859572008550455730172472L);
//! 2*pi
template <typename fp_type = max_fp_type>
constexpr fp_type PI_MUL_2 = fp_type(6.28318530717958647692528676655900576839433879875021164194988918461563281257241799725607L);
//! 4*pi
template <typename fp_type = max_fp_type>
constexpr fp_type PI_MUL_4 = fp_type(12.5663706143591729538505735331180115367886775975004232838997783692312656251448359945121L);
//! 1/pi
template <typename fp_type = max_fp_type>
constexpr fp_type _1_DIV_PI = fp_type(0.31830988618379067153776752674502872406891929148091289749533468811779359526845307018L);
//! 1/(2*pi)
template <typename fp_type = max_fp_type>
constexpr fp_type _1_DIV_2PI = fp_type(0.1591549430918953357688837633725143620344596457404564487476673440588967976342265351L);
//! 2/pi
template <typename fp_type = max_fp_type>
constexpr fp_type _2_DIV_PI = fp_type(0.63661977236758134307553505349005744813783858296182579499066937623558719053690614036L);
//! 180/pi
template <typename fp_type = max_fp_type>
constexpr fp_type _180_DIV_PI = fp_type(57.29577951308232087679815481410517033240547246656432154916024386120284714832155263L);
//! 360/pi
template <typename fp_type = max_fp_type>
constexpr fp_type _360_DIV_PI = fp_type(114.5915590261646417535963096282103406648109449331286430983204877224056942966431053L);
//! 2/sqrt(pi)
template <typename fp_type = max_fp_type>
constexpr fp_type _2_DIV_SQRT_PI = fp_type(1.1283791670955125738961589031215451716881012586579977136881714434212849368829868L);
//! sqrt(2)
template <typename fp_type = max_fp_type>
constexpr fp_type SQRT_2 = fp_type(1.4142135623730950488016887242096980785696718753769480731766797379907324784621070388503875L);
//! 1/sqrt(2)
template <typename fp_type = max_fp_type>
constexpr fp_type _1_DIV_SQRT_2 = fp_type(0.707106781186547524400844362104849039284835937688474036588339868995366239231053519L);
//! ln(2)
template <typename fp_type = max_fp_type>
constexpr fp_type LN_2 = fp_type(0.693147180559945309417232121458176568075500134360255254120680009493393621969694715605863327L);
//! ln(10)
template <typename fp_type = max_fp_type>
constexpr fp_type LN_10 = fp_type(2.30258509299404568401799145468436420760110148862877297603332790096757260967735248023599721L);
//! 1/ln(2)
template <typename fp_type = max_fp_type>
constexpr fp_type _1_DIV_LN_2 = fp_type(1.44269504088896340735992468100189213742664595415298593413544940693110921918118507989L);
//! e
template <typename fp_type = max_fp_type>
constexpr fp_type E = fp_type(2.71828182845904523536028747135266249775724709369995957496696762772407663035354759457138217853L);
//! ld(e)
template <typename fp_type = max_fp_type>
constexpr fp_type LD_E = _1_DIV_LN_2<fp_type>;
//! log(e)
template <typename fp_type = max_fp_type>
constexpr fp_type LOG_E = fp_type(0.4342944819032518276511289189166050822943970058036665661144537831658646492088707747292249L);
//! 1/ld(e)
template <typename fp_type = max_fp_type>
constexpr fp_type _1_DIV_LD_E = fp_type(0.693147180559945309417232121458176568075500134360255254120680009493393621969694716L);
//! epsilon (for general use)
template <typename fp_type = max_fp_type>
constexpr fp_type EPSILON = fp_type(0.00001L);


// https://github.com/fmtlib/fmt/blob/master/include/fmt/format.h
template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr bool is_nan(fp_type value)
{
	if (!std::is_constant_evaluated()) return std::isnan(value);
	return value != value;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr bool is_infinite(fp_type value)
{
	if (!std::is_constant_evaluated()) return std::isinf(value);
	constexpr fp_type inf = std::numeric_limits<fp_type>::infinity();
	return value == inf || value == -inf;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr bool is_finite(fp_type value)
{
	if (!std::is_constant_evaluated()) return std::isfinite(value);
	return !is_nan(value) && !is_infinite(value);
}


template <typename fp_type>
struct _s_decompose_fp
{
	fp_type first;
	int32_t second;
};

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr _s_decompose_fp<fp_type> _decompose_fp(const fp_type in_val)
{
	constexpr const auto max_exp = std::numeric_limits<fp_type>::max_exponent - 1;
	constexpr const auto min_exp = std::numeric_limits<fp_type>::min_exponent - 1;
	constexpr const auto abs_min_exp = -min_exp;

	const fp_type sign = in_val < (fp_type)0 ? -(fp_type)1 : (fp_type)1;
	fp_type val = sign < (fp_type)0 ? -in_val : in_val;

	int32_t exp = 0;
	fp_type fexp = (fp_type)1;
	if (val > (fp_type)2)
	{
		++exp;
		fexp *= (fp_type)2;
		for (; exp <= max_exp; ++exp)
		{
			const fp_type div_by_exp = (val / fexp);
			if (div_by_exp >= (fp_type)1 && div_by_exp < (fp_type)2)
			{
				val = div_by_exp;
				break;
			}
			fexp *= (fp_type)2;
		}
	}
	else if (val < (fp_type)1)
	{
		++exp;
		fexp *= (fp_type)2;
		for (; exp <= abs_min_exp; ++exp)
		{
			const fp_type mul_by_exp = (val * fexp);
			if (mul_by_exp >= (fp_type)1 && mul_by_exp < (fp_type)2)
			{
				val = mul_by_exp;
				break;
			}
			fexp *= (fp_type)2;
		}
		exp = -exp;
	}

	return { val * sign, exp };
}


template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type rad_to_deg(const fp_type val)
{
	return _180_DIV_PI<fp_type> * val;
}

template <typename any_type> requires(!std::is_floating_point_v<any_type>)
constexpr any_type rad_to_deg(const any_type val)
{
	return any_type(_180_DIV_PI<> * (max_fp_type)val);
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type deg_to_rad(const fp_type val)
{
	return PI_DIV_180<fp_type> * val;
}

template <typename any_type> requires(!std::is_floating_point_v<any_type>)
constexpr any_type deg_to_rad(const any_type val)
{
	return any_type(PI_DIV_180<> * (max_fp_type)val);
}


template <typename arithmetic_type> requires(std::is_arithmetic_v<arithmetic_type>)
constexpr arithmetic_type abs(const arithmetic_type val)
{
	if (!std::is_constant_evaluated()) return std::abs(val);
	return (val < (arithmetic_type)0 ? -val : val);
}

template <typename arithmetic_type> requires(std::is_arithmetic_v<arithmetic_type>)
constexpr arithmetic_type min(const arithmetic_type x, const arithmetic_type y)
{
	return (x <= y ? x : y);
}

template <typename arithmetic_type> requires(std::is_arithmetic_v<arithmetic_type>)
constexpr arithmetic_type max(const arithmetic_type x, const arithmetic_type y)
{
	return (x >= y ? x : y);
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type round(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::round(val);
	return (fp_type)(int64_t)(val + (val >= (fp_type)0 ? (fp_type)0.5 : (fp_type)-0.5));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type floor(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::floor(val);
	const auto val_int = (int64_t)val;
	const fp_type fval_int = (fp_type)val_int;
	return (val >= (fp_type)0 ? fval_int : (val == fval_int ? val : fval_int - (fp_type)1));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type ceil(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::ceil(val);
	const auto val_int = (int64_t)val;
	const fp_type fval_int = (fp_type)val_int;
	return (val < (fp_type)0 ? fval_int : (val == fval_int ? val : fval_int + (fp_type)1));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type trunc(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::trunc(val);
	return (fp_type)(int64_t)val;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type fmod(const fp_type x, const fp_type y)
{
	if (!std::is_constant_evaluated()) return std::fmod(x, y);
	return x - y * trunc(x / y);
}

template <typename arithmetic_type> requires(std::is_arithmetic_v<arithmetic_type>)
constexpr arithmetic_type pow(const arithmetic_type base, const int32_t exponent)
{
	arithmetic_type ret = (arithmetic_type)1;
	for(int32_t i = 0; i < exponent; ++i)
	{
		ret *= base;
	}
	return ret;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type copysign(const fp_type x, const fp_type y)
{
	if (!std::is_constant_evaluated()) return std::copysign(x, y);
	uint64_t ix = std::bit_cast<uint64_t, double>((double)x) >> 63;
	uint64_t iy = std::bit_cast<uint64_t, double>((double)y) >> 63;
	if (ix == iy) return y;
	return -y;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type exp(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::exp(val);
	const auto abs_val = abs(val);
	const auto exponent = _1_DIV_LN_2<> * (max_fp_type)abs_val;
	auto pot_factors = 1.0;

	auto rem = exponent;
	int32_t pot_bits = 0;
	for (int32_t pot = 14; pot >= 0; --pot)
	{
		const auto ldbl_pot = (max_fp_type)(1 << pot);
		if (rem >= ldbl_pot)
		{
			pot_bits |= (1 << pot);
			rem -= ldbl_pot;
			for(int32_t i = 0; i < (1 << pot); ++i)
				pot_factors *= 2;
		}

		if (rem < 1.0)
			break;
	}

	const auto exp_val = ((max_fp_type)abs_val) - (LN_2<> * (max_fp_type)pot_bits);
	constexpr const uint32_t pade_deg = 10 + 1;
	constexpr const max_fp_type pade[pade_deg]
	{
		1.0,
		0.5,
		9.0 / 76.0,
		1.0 / 57.0,
		7.0 / 3876.0,
		7.0 / 51680.0,
		7.0 / 930240.0,
		1.0 / 3255840.0,
		1.0 / 112869120.0,
		1.0 / 6094932480.0,
		1.0 / 670442572800.0,
	};
	auto exp_num = 0.0;
	auto exp_denom = 0.0;
	auto exp_pow = 1.0;
	for (uint32_t i = 0; i < pade_deg; ++i)
	{
		exp_num += pade[i] * exp_pow;
		exp_denom += pade[i] * exp_pow * (i % 2 == 1 ? -1.0 : 1.0);
		exp_pow *= exp_val;
	}
	const auto exp_approx = exp_num / exp_denom;

	auto ret = pot_factors * exp_approx;
	if (val < fp_type(0.0))
	{
		ret = 1.0 / ret;
	}
	return fp_type(ret);
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type exp2(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::exp2(val);
	return fp_type(exp(max_fp_type(val) * LN_2<>));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr auto _partial_ln_and_log2(const fp_type val)
{
	struct ln_ret
	{
		bool valid;
		fp_type invalid_ret;
		max_fp_type decomp_base;
		max_fp_type decomp_exp;
	};
	if (val == (fp_type)0 || val == -(fp_type)0) return ln_ret{ false, -std::numeric_limits<fp_type>::infinity(), 0.0, 0.0 };
	if (val == (fp_type)1) return ln_ret{ false, (fp_type)0, 0.0, 0.0 };
	if (val < (fp_type)0) return ln_ret{ false, std::numeric_limits<fp_type>::quiet_NaN(), 0.0, 0.0 };
	if (is_infinite(val)) return ln_ret{ false, std::numeric_limits<fp_type>::infinity(), 0.0, 0.0 };
	if (is_nan(val)) return ln_ret{ false, std::numeric_limits<fp_type>::quiet_NaN(), 0.0, 0.0 };

	const auto decomp = _decompose_fp(val);

	const auto ldbl_val = (long double)decomp.first;
	const auto frac = (ldbl_val - 1.0) / (ldbl_val + 1.0);
	const auto frac_sq = frac * frac;
	auto frac_exp = frac;
	auto res = frac;
	for (uint32_t i = 1; i < 32; ++i)
	{
		frac_exp *= frac_sq;
		res += frac_exp / (long double)(i * 2 + 1);
	}
	const auto decomp_res = max_fp_type(res * 2.0);

	const auto exp_log2_val = (max_fp_type)decomp.second;

	return ln_ret{ true, (fp_type)0, decomp_res, exp_log2_val };
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type log(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::log(val);
	const auto ret = _partial_ln_and_log2(val);
	if(!ret.valid)
		return ret.invalid_ret;

	return fp_type(ret.decomp_base + (ret.decomp_exp * _1_DIV_LD_E<max_fp_type>));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type log2(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::log2(val);
	const auto ret = partial_ln_and_log2(val);
	if (!ret.valid)
		return ret.invalid_ret;

	return fp_type((ret.decomp_base * _1_DIV_LN_2<max_fp_type>) + ret.decomp_exp);
}


// https://github.com/bolero-MURAKAMI/Sprout/blob/master/sprout/math/pow.hpp
template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type pow(const fp_type x, const fp_type y)
{
	if (!std::is_constant_evaluated()) return std::pow(x, y);
	return x == 1 ? fp_type(1)
		: y == 0 ? fp_type(1)
		: is_nan(y) ? y
		: is_nan(x) ? x
		: x == 0
			? y < 0
				? (fmod(y, fp_type(2)) == 1) ? copysign(std::numeric_limits<fp_type>::infinity(), x)
				: std::numeric_limits<fp_type>::infinity()
			: (fmod(y, fp_type(2)) == 1) ? x
			: fp_type(0)
		: x == -1 && (y == std::numeric_limits<fp_type>::infinity() || y == -std::numeric_limits<fp_type>::infinity()) ? fp_type(1)
		: y == -std::numeric_limits<fp_type>::infinity()
			? abs(x) < 1 ? std::numeric_limits<fp_type>::infinity()
			: fp_type(0)
		: y == std::numeric_limits<fp_type>::infinity()
			? abs(x) < 1 ? fp_type(0)
			: std::numeric_limits<fp_type>::infinity()
		: x == -std::numeric_limits<fp_type>::infinity()
			? y < 0
				? (fmod(y, fp_type(2)) == 1) ? -fp_type(0)
				: fp_type(0)
			: (fmod(y, fp_type(2)) == 1) ? -std::numeric_limits<fp_type>::infinity()
			: std::numeric_limits<fp_type>::infinity()
		: x == std::numeric_limits<fp_type>::infinity()
			? y < 0 ? fp_type(0)
			: std::numeric_limits<fp_type>::infinity()
		: x < 0 && !(y == trunc(y)) ? std::numeric_limits<fp_type>::quiet_NaN()
		: x < 0 ? (fmod(y, fp_type(2)) == 1)
			? -exp(y * log(-x))
				: exp(y * log(-x))
			: exp(y * log(x))
		;
}

template <typename T> constexpr int _select_halley_iters()
{
	switch(sizeof(T))
	{
		case 4:
			return 3;
		case 8:
			return 4;
		case 10:
		case 12:
		case 16:
		default:
			return 5;
	}
}

template <typename fp_type>
struct _s_sqrt_and_rsqrt
{
	fp_type sqrt;
	fp_type rsqrt;
};

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr _s_sqrt_and_rsqrt<fp_type> _sqrt_and_rsqrt(const fp_type val)
{
	static_assert(std::numeric_limits<fp_type>::is_iec559, "compiler or target is not IEC559/IEEE-754 compliant!");

	if (val == (fp_type)0 || val == -(fp_type)0)
	{
		return { val, std::numeric_limits<fp_type>::quiet_NaN() };
	}

	if (is_infinite(val) && val > (fp_type)0)
	{
		return { val, (fp_type)0 };
	}
	if (is_nan(val) || is_infinite(val) || val < -(fp_type)0)
	{
		return { std::numeric_limits<fp_type>::quiet_NaN(), std::numeric_limits<fp_type>::quiet_NaN() };
	}
	if (!is_finite(val) || val == 0.0)
	{
		return { (fp_type)0, (fp_type)0 };
	}

	const auto decomp = _decompose_fp(val);
	auto ldbl_val = (max_fp_type)decomp.first;
	const bool is_neg_exp = (decomp.second < 0);
	int32_t abs_exp = (is_neg_exp ? -decomp.second : decomp.second);
	if (abs_exp % 2 == 1)
	{
		if (!is_neg_exp) --abs_exp;
		else ++abs_exp;
		ldbl_val *= 2.0;
	}

	const auto estimate = 0.546702 + 0.502315 * ldbl_val - 0.0352763 * ldbl_val * ldbl_val;

	constexpr const int halley_iters = _select_halley_iters<fp_type>();
	auto x = 1.0 / estimate;
	for (int i = 0; i < halley_iters; ++i)
	{
		const auto y = ldbl_val * x * x;
		x = (x * 0.125) * (15.0 - y * (10.0 - 3.0 * y));
	}
	auto rcp_x = x;

	x *= ldbl_val;

	const max_fp_type sqrt_two_exp = pow(2.0, (abs_exp >> 1));
	if (!is_neg_exp)
	{
		x *= sqrt_two_exp;
		rcp_x *= 1.0 / sqrt_two_exp;
	}
	else
	{
		x *= 1.0 / sqrt_two_exp;
		rcp_x *= sqrt_two_exp;
	}

	return { fp_type(x), fp_type(rcp_x) };
}
	
template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type sqrt(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::sqrt(val);
	return _sqrt_and_rsqrt(val).sqrt;
}
	
template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type rsqrt(const fp_type val)
{
	if (!std::is_constant_evaluated()) return fp_type(1) / std::sqrt(val);
	return _sqrt_and_rsqrt(val).rsqrt;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type cos(const fp_type rad_angle)
{
	if (!std::is_constant_evaluated()) return std::cos(rad_angle);
	const max_fp_type lrad_angle = fmod((max_fp_type)rad_angle, PI_MUL_2<>);
	const max_fp_type ldbl_val = lrad_angle + (lrad_angle > PI<> ? -PI_MUL_2<> : (lrad_angle < -PI<> ? PI_MUL_2<> : 0.0));

	max_fp_type cos_x = 1.0;
	uint64_t factorial_2k = 1ull;
	max_fp_type pow_x_2k = 1.0;
	const max_fp_type x_2 = ldbl_val * ldbl_val;
	for(int k = 1; k <= 10; ++k)
	{
		pow_x_2k *= x_2;
		factorial_2k *= (uint64_t)(k * 2 - 1) * (uint64_t)(k * 2);
		cos_x += ((k % 2 == 1 ? -1.0 : 1.0) * pow_x_2k) / (max_fp_type)(factorial_2k);
	}
	return (fp_type)cos_x;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type sin(const fp_type rad_angle)
{
	if (!std::is_constant_evaluated()) return std::sin(rad_angle);
	return (fp_type)cos(PI_DIV_2<> - (max_fp_type)rad_angle);
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type tan(const fp_type rad_angle)
{
	if (!std::is_constant_evaluated()) return std::tan(rad_angle);
	return (fp_type)(sin((max_fp_type)rad_angle) / cos((max_fp_type)rad_angle));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type asin(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::asin(val);
	if (val < (fp_type)-1 || val >(fp_type)1)
	{
		return std::numeric_limits<fp_type>::quiet_NaN();
	}
	if (is_nan(val))
	{
		return val;
	}

	const max_fp_type ldbl_val = (max_fp_type)val;

	if (abs(ldbl_val) > 0.5)
	{
		return (fp_type)(PI_DIV_2<> -2.0 * asin(sqrt((1.0 - ldbl_val) * 0.5)));
	}

	max_fp_type asin_x = ldbl_val;
	max_fp_type binom_2k_k = 1.0;
	max_fp_type pow_x_1_2k = ldbl_val;
	const max_fp_type x_2 = ldbl_val * ldbl_val;
	max_fp_type pow_4_k = 1.0;
	for (int k = 1; k <= 9; ++k)
	{
		const auto fp_k = max_fp_type(k);
		binom_2k_k *= 4.0 - 2.0 / fp_k;
		pow_x_1_2k *= x_2;
		pow_4_k *= 4.0;
		asin_x += (binom_2k_k * pow_x_1_2k) / (pow_4_k * (1.0 + 2.0 * fp_k));
	}

	return (fp_type)asin_x;
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type acos(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::acos(val);
	return (fp_type)(PI_DIV_2<> - asin((max_fp_type)val));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type atan(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::atan(val);
	const max_fp_type ldbl_val = (max_fp_type)val;
	return (fp_type)asin(ldbl_val / sqrt(ldbl_val * ldbl_val + 1.0));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type atan2(const fp_type y, const fp_type x)
{
	if (!std::is_constant_evaluated()) return std::atan(y, x);
	const max_fp_type ldbl_x = (max_fp_type)x;
	const max_fp_type ldbl_y = (max_fp_type)y;
	if (x > (fp_type)0)
	{
		return (fp_type)atan(ldbl_y / ldbl_x);
	}
	else if (x < (fp_type)0)
	{
		if (y >= (fp_type)0)
		{
			return (fp_type)(atan(ldbl_y / ldbl_x) + PI<>);
		}
		else
		{
			return (fp_type)(atan(ldbl_y / ldbl_x) - PI<>);
		}
	}
	else
	{
		if (y > (fp_type)0)
		{
			return (fp_type)PI_DIV_2<>;
		}
		else if (y < (fp_type)0)
		{
			return (fp_type)-PI_DIV_2<>;
		}
		else
		{
			return std::numeric_limits<fp_type>::quiet_NaN();
		}
	}
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type sinh(const fp_type rad_angle)
{
	if (!std::is_constant_evaluated()) return std::sinh(rad_angle);
	const auto ldbl_val = (max_fp_type)rad_angle;
	return fp_type(0.5 * (exp(ldbl_val) - exp(-ldbl_val)));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type cosh(const fp_type rad_angle)
{
	if (!std::is_constant_evaluated()) return std::cosh(rad_angle);
	const auto ldbl_val = (max_fp_type)rad_angle;
	return fp_type(0.5 * (exp(ldbl_val) + exp(-ldbl_val)));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type tanh(const fp_type rad_angle)
{
	if (!std::is_constant_evaluated()) return std::tanh(rad_angle);
	const auto ldbl_val = (max_fp_type)rad_angle;
	const auto exp_pos = exp(ldbl_val);
	const auto exp_neg = exp(-ldbl_val);
	return fp_type((exp_pos - exp_neg) / (exp_pos + exp_neg));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type asinh(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::asinh(val);
	const auto ldbl_val = (max_fp_type)val;
	return (fp_type)log(ldbl_val + sqrt(ldbl_val * ldbl_val + 1.0));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type acosh(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::acosh(val);
	const auto ldbl_val = (max_fp_type)val;
	return (fp_type)log(ldbl_val + sqrt(ldbl_val * ldbl_val - 1.0));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type atanh(const fp_type val)
{
	if (!std::is_constant_evaluated()) return std::atanh(val);
	const auto ldbl_val = (max_fp_type)val;
	return (fp_type)(0.5 * log((1.0 + ldbl_val) / (1.0 - ldbl_val)));
}

template <typename arithmetic_type> requires(std::is_arithmetic_v<arithmetic_type>)
constexpr arithmetic_type clamp(const arithmetic_type val, const arithmetic_type min, const arithmetic_type max)
{
	return (val > max ? max : (val < min ? min : val));
}

template <typename fp_type> requires(std::is_floating_point_v<fp_type>)
constexpr fp_type lerp(const fp_type x, const fp_type y, const fp_type t)
{
	return (y - x) * t + x;
}

template <typename T> requires(std::is_arithmetic_v<T>) struct vec1_base;
template <typename T> requires(std::is_arithmetic_v<T>) struct vec2_base;
template <typename T> requires(std::is_arithmetic_v<T>) struct vec3_base;
template <typename T> requires(std::is_arithmetic_v<T>) struct vec4_base;

template <typename T> requires(std::is_arithmetic_v<T>)
struct vec1_base
{
	using value_type = T;
	static constexpr inline size_t SIZE = 1;

	T x;

	constexpr vec1_base() = default;

	template <typename T1> requires(std::is_arithmetic_v<T1>)
	constexpr explicit vec1_base(T1 _x)
		: x{ T(_x) }
	{
	}

	template <typename T1> requires(std::is_arithmetic_v<T1>)
	constexpr vec1_base(vec1_base<T1> _x)
		: x{ T(_x.x) }
	{
	}

#if 1
	constexpr vec1_base<T> _x() const { return x; }
	constexpr vec2_base<T> _xx() const;
	constexpr vec3_base<T> _xxx() const;
	constexpr vec4_base<T> _xxxx() const;
#endif // 1

	static constexpr vec1_base unit_x() { return vec1_base{ 1 }; };

	constexpr bool operator == (vec1_base other) const { return x == other.x; }

	constexpr T &operator [] (size_t index)
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		throw 1;
	}
	constexpr T const &operator [] (size_t index) const
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		throw 1;
	}

	constexpr vec1_base &operator += (T val)                 { x += val; return *this; }
	constexpr vec1_base  operator +  (T val) const           { return vec1_base(*this) += val; }
	constexpr vec1_base &operator += (vec1_base other)       { x += other.x; return *this; }
	constexpr vec1_base  operator +  (vec1_base other) const { return vec1_base(*this) += other; }

	constexpr vec1_base &operator -= (T val)                 { x -= val; return *this; }
	constexpr vec1_base  operator -  (T val) const           { return vec1_base(*this) -= val; }
	constexpr vec1_base &operator -= (vec1_base other)       { x -= other.x; return *this; }
	constexpr vec1_base  operator -  (vec1_base other) const { return vec1_base(*this) -= other; }

	constexpr vec1_base &operator *= (T val)                 { x *= val; return *this; }
	constexpr vec1_base  operator *  (T val) const           { return vec1_base(*this) *= val; }
	constexpr vec1_base &operator *= (vec1_base other)       { x *= other.x; return *this; }
	constexpr vec1_base  operator *  (vec1_base other) const { return vec1_base(*this) *= other; }

	constexpr vec1_base &operator /= (T val)                 { x /= val; return *this; }
	constexpr vec1_base  operator /  (T val) const           { return vec1_base(*this) /= val; }
	constexpr vec1_base &operator /= (vec1_base other)       { x /= other.x; return *this; }
	constexpr vec1_base  operator /  (vec1_base other) const { return vec1_base(*this) /= other; }

	constexpr vec1_base &operator %= (T val)                 { x = fmod(x, val); return *this; }
	constexpr vec1_base  operator %  (T val) const           { return vec1_base(*this) %= val; }
	constexpr vec1_base &operator %= (vec1_base other)       { x = fmod(x, other.x); return *this; }
	constexpr vec1_base  operator %  (vec1_base other) const { return vec1_base(*this) %= other; }

	constexpr vec1_base operator - () const { return vec1_base{ -x }; }

	constexpr T magnitude_squared() const
	{
		return x*x;
	}

	constexpr T magnitude() const
	{
		return x;
	}

	constexpr T length() const
	{
		return magnitude();
	}

	constexpr vec1_base lerp(vec1_base other, T t) const
	{
		return vec1_base{
			lerp(x, other.x, t),
		};
	}
};

template <typename T> requires(std::is_arithmetic_v<T>)
struct vec2_base
{
	using value_type = T;
	static constexpr inline size_t SIZE = 2;

	T x, y;

	constexpr vec2_base() = default;

	template <typename T1, typename T2 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
	constexpr vec2_base(T1 _x, T2 _y = T2(0))
		: x{ T(_x) }, y{ T(_y) }
	{
	}

	template <typename T1> requires(std::is_arithmetic_v<T1>)
	constexpr vec2_base(vec2_base<T1> _xy)
		: x{ T(_xy.x) }, y{ T(_xy.y) }
	{
	}

#if 1
	constexpr vec1_base<T> _x() const { return x; }
	constexpr vec1_base<T> _y() const { return y; }

	constexpr vec2_base<T> _xx() const { return { x, x }; }
	constexpr vec2_base<T> _xy() const { return { x, y }; }
	constexpr vec2_base<T> _yx() const { return { y, x }; }
	constexpr vec2_base<T> _yy() const { return { y, y }; }

	constexpr vec3_base<T> _xxx() const;
	constexpr vec3_base<T> _xxy() const;
	constexpr vec3_base<T> _xyx() const;
	constexpr vec3_base<T> _xyy() const;

	constexpr vec3_base<T> _yxx() const;
	constexpr vec3_base<T> _yxy() const;
	constexpr vec3_base<T> _yyx() const;
	constexpr vec3_base<T> _yyy() const;

	constexpr vec4_base<T> _xxxx() const;
	constexpr vec4_base<T> _xxxy() const;
	constexpr vec4_base<T> _xxyx() const;
	constexpr vec4_base<T> _xxyy() const;
	constexpr vec4_base<T> _xyxx() const;
	constexpr vec4_base<T> _xyxy() const;
	constexpr vec4_base<T> _xyyx() const;
	constexpr vec4_base<T> _xyyy() const;

	constexpr vec4_base<T> _yxxx() const;
	constexpr vec4_base<T> _yxxy() const;
	constexpr vec4_base<T> _yxyx() const;
	constexpr vec4_base<T> _yxyy() const;
	constexpr vec4_base<T> _yyxx() const;
	constexpr vec4_base<T> _yyxy() const;
	constexpr vec4_base<T> _yyyx() const;
	constexpr vec4_base<T> _yyyy() const;
#endif // 1

	static constexpr vec2_base unit_x() { return vec2_base{ 1, 0 }; };
	static constexpr vec2_base unit_y() { return vec2_base{ 0, 1 }; };

	constexpr bool operator == (vec2_base other) const { return x == other.x && y == other.y; }

	constexpr T &operator [] (size_t index)
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		if (index == 1) return y;
		throw 1;
	}
	constexpr T const &operator [] (size_t index) const
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		if (index == 1) return y;
		throw 1;
	}

	constexpr vec2_base &operator += (T val)        { x += val; y += val; return *this; }
	constexpr vec2_base  operator +  (T val) const  { return vec2_base(*this) += val; }
	constexpr vec2_base &operator += (vec2_base other)       { x += other.x; y += other.y; return *this; }
	constexpr vec2_base  operator +  (vec2_base other) const { return vec2_base(*this) += other; }

	constexpr vec2_base &operator -= (T val)        { x -= val; y -= val; return *this; }
	constexpr vec2_base  operator -  (T val) const  { return vec2_base(*this) -= val; }
	constexpr vec2_base &operator -= (vec2_base other)       { x -= other.x; y -= other.y; return *this; }
	constexpr vec2_base  operator -  (vec2_base other) const { return vec2_base(*this) -= other; }

	constexpr vec2_base &operator *= (T val)        { x *= val; y *= val; return *this; }
	constexpr vec2_base  operator *  (T val) const  { return vec2_base(*this) *= val; }
	constexpr vec2_base &operator *= (vec2_base other)       { x *= other.x; y *= other.y; return *this; }
	constexpr vec2_base  operator *  (vec2_base other) const { return vec2_base(*this) *= other; }

	constexpr vec2_base &operator /= (T val)        { x /= val; y /= val; return *this; }
	constexpr vec2_base  operator /  (T val) const  { return vec2_base(*this) /= val; }
	constexpr vec2_base &operator /= (vec2_base other)       { x /= other.x; y /= other.y; return *this; }
	constexpr vec2_base  operator /  (vec2_base other) const { return vec2_base(*this) /= other; }

	constexpr vec2_base &operator %= (T val)        { x = fmod(x, val); y = fmod(y, val); return *this; }
	constexpr vec2_base  operator %  (T val) const  { return vec2_base(*this) %= val; }
	constexpr vec2_base &operator %= (vec2_base other)       { x = fmod(x, other.x); y = fmod(y, other.y); return *this; }
	constexpr vec2_base  operator %  (vec2_base other) const { return vec2_base(*this) %= other; }

	constexpr vec2_base operator - () const { return vec2_base{ -x, -y }; }

	constexpr T magnitude_squared() const
	{
		return x*x + y*y;
	}

	constexpr T magnitude() const
	{
		return sqrt(magnitude_squared());
	}

	constexpr T length() const
	{
		return magnitude();
	}

	constexpr vec2_base normalize() const
	{
		return vec2_base(*this) / length();
	}

	constexpr vec2_base rotate_z(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec2_base{ x*c - y*s, x*s + y*c };
	}

	constexpr T dot(vec2_base other) const
	{
		return x*other.x + y*other.y;
	}
};

template <typename T> requires(std::is_arithmetic_v<T>)
struct vec3_base
{
	using value_type = T;
	static constexpr inline size_t SIZE = 3;

	T x, y, z;

	constexpr vec3_base() = default;

	template <typename T1, typename T2 = T, typename T3 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>)
	constexpr vec3_base(T1 _x, T2 _y = T2(0), T3 _z = T3(0))
		: x{ T(_x) }, y{ T(_y) }, z{ T(_z) }
	{
	}

	template <typename T1, typename T2 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
	constexpr vec3_base(vec2_base<T1> _xy, T2 _z = T2(0))
		: x{ T(_xy.x) }, y{ T(_xy.y) }, z{ T(_z) }
	{
	}

	template <typename T1, typename T2> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
	constexpr vec3_base(T1 _x, vec2_base<T2> _yz)
		: x{ T(_x) }, y{ T(_yz.x) }, z{ T(_yz.z) }
	{
	}

	template <typename T1> requires(std::is_arithmetic_v<T1>)
	constexpr vec3_base(vec3_base<T1> _xyz)
		: x{ T(_xyz.x) }, y{ T(_xyz.y) }, z{ T(_xyz.z) }
	{
	}

#if 1
	constexpr vec1_base<T> _x() const { return x; }
	constexpr vec1_base<T> _y() const { return y; }
	constexpr vec1_base<T> _z() const { return z; }

	constexpr vec2_base<T> _xx() const { return { x, x }; }
	constexpr vec2_base<T> _xy() const { return { x, y }; }
	constexpr vec2_base<T> _xz() const { return { x, z }; }
	constexpr vec2_base<T> _yx() const { return { y, x }; }
	constexpr vec2_base<T> _yy() const { return { y, y }; }
	constexpr vec2_base<T> _yz() const { return { y, z }; }
	constexpr vec2_base<T> _zx() const { return { z, x }; }
	constexpr vec2_base<T> _zy() const { return { z, y }; }
	constexpr vec2_base<T> _zz() const { return { z, z }; }

	constexpr vec3_base<T> _xxx() const { return { x, x, x }; }
	constexpr vec3_base<T> _xxy() const { return { x, x, y }; }
	constexpr vec3_base<T> _xxz() const { return { x, x, z }; }
	constexpr vec3_base<T> _xyx() const { return { x, y, x }; }
	constexpr vec3_base<T> _xyy() const { return { x, y, y }; }
	constexpr vec3_base<T> _xyz() const { return { x, y, z }; }
	constexpr vec3_base<T> _xzx() const { return { x, z, x }; }
	constexpr vec3_base<T> _xzy() const { return { x, z, y }; }
	constexpr vec3_base<T> _xzz() const { return { x, z, z }; }

	constexpr vec3_base<T> _yxx() const { return { y, x, x }; }
	constexpr vec3_base<T> _yxy() const { return { y, x, y }; }
	constexpr vec3_base<T> _yxz() const { return { y, x, z }; }
	constexpr vec3_base<T> _yyx() const { return { y, y, x }; }
	constexpr vec3_base<T> _yyy() const { return { y, y, y }; }
	constexpr vec3_base<T> _yyz() const { return { y, y, z }; }
	constexpr vec3_base<T> _yzx() const { return { y, z, x }; }
	constexpr vec3_base<T> _yzy() const { return { y, z, y }; }
	constexpr vec3_base<T> _yzz() const { return { y, z, z }; }

	constexpr vec3_base<T> _zxx() const { return { z, x, x }; }
	constexpr vec3_base<T> _zxy() const { return { z, x, y }; }
	constexpr vec3_base<T> _zxz() const { return { z, x, z }; }
	constexpr vec3_base<T> _zyx() const { return { z, y, x }; }
	constexpr vec3_base<T> _zyy() const { return { z, y, y }; }
	constexpr vec3_base<T> _zyz() const { return { z, y, z }; }
	constexpr vec3_base<T> _zzx() const { return { z, z, x }; }
	constexpr vec3_base<T> _zzy() const { return { z, z, y }; }
	constexpr vec3_base<T> _zzz() const { return { z, z, z }; }

	constexpr vec4_base<T> _xxxx() const;
	constexpr vec4_base<T> _xxxy() const;
	constexpr vec4_base<T> _xxxz() const;
	constexpr vec4_base<T> _xxyx() const;
	constexpr vec4_base<T> _xxyy() const;
	constexpr vec4_base<T> _xxyz() const;
	constexpr vec4_base<T> _xxzx() const;
	constexpr vec4_base<T> _xxzy() const;
	constexpr vec4_base<T> _xxzz() const;
	constexpr vec4_base<T> _xyxx() const;
	constexpr vec4_base<T> _xyxy() const;
	constexpr vec4_base<T> _xyxz() const;
	constexpr vec4_base<T> _xyyx() const;
	constexpr vec4_base<T> _xyyy() const;
	constexpr vec4_base<T> _xyyz() const;
	constexpr vec4_base<T> _xyzx() const;
	constexpr vec4_base<T> _xyzy() const;
	constexpr vec4_base<T> _xyzz() const;
	constexpr vec4_base<T> _xzxx() const;
	constexpr vec4_base<T> _xzxy() const;
	constexpr vec4_base<T> _xzxz() const;
	constexpr vec4_base<T> _xzyx() const;
	constexpr vec4_base<T> _xzyy() const;
	constexpr vec4_base<T> _xzyz() const;
	constexpr vec4_base<T> _xzzx() const;
	constexpr vec4_base<T> _xzzy() const;
	constexpr vec4_base<T> _xzzz() const;

	constexpr vec4_base<T> _yxxx() const;
	constexpr vec4_base<T> _yxxy() const;
	constexpr vec4_base<T> _yxxz() const;
	constexpr vec4_base<T> _yxyx() const;
	constexpr vec4_base<T> _yxyy() const;
	constexpr vec4_base<T> _yxyz() const;
	constexpr vec4_base<T> _yxzx() const;
	constexpr vec4_base<T> _yxzy() const;
	constexpr vec4_base<T> _yxzz() const;
	constexpr vec4_base<T> _yyxx() const;
	constexpr vec4_base<T> _yyxy() const;
	constexpr vec4_base<T> _yyxz() const;
	constexpr vec4_base<T> _yyyx() const;
	constexpr vec4_base<T> _yyyy() const;
	constexpr vec4_base<T> _yyyz() const;
	constexpr vec4_base<T> _yyzx() const;
	constexpr vec4_base<T> _yyzy() const;
	constexpr vec4_base<T> _yyzz() const;
	constexpr vec4_base<T> _yzxx() const;
	constexpr vec4_base<T> _yzxy() const;
	constexpr vec4_base<T> _yzxz() const;
	constexpr vec4_base<T> _yzyx() const;
	constexpr vec4_base<T> _yzyy() const;
	constexpr vec4_base<T> _yzyz() const;
	constexpr vec4_base<T> _yzzx() const;
	constexpr vec4_base<T> _yzzy() const;
	constexpr vec4_base<T> _yzzz() const;

	constexpr vec4_base<T> _zxxx() const;
	constexpr vec4_base<T> _zxxy() const;
	constexpr vec4_base<T> _zxxz() const;
	constexpr vec4_base<T> _zxyx() const;
	constexpr vec4_base<T> _zxyy() const;
	constexpr vec4_base<T> _zxyz() const;
	constexpr vec4_base<T> _zxzx() const;
	constexpr vec4_base<T> _zxzy() const;
	constexpr vec4_base<T> _zxzz() const;
	constexpr vec4_base<T> _zyxx() const;
	constexpr vec4_base<T> _zyxy() const;
	constexpr vec4_base<T> _zyxz() const;
	constexpr vec4_base<T> _zyyx() const;
	constexpr vec4_base<T> _zyyy() const;
	constexpr vec4_base<T> _zyyz() const;
	constexpr vec4_base<T> _zyzx() const;
	constexpr vec4_base<T> _zyzy() const;
	constexpr vec4_base<T> _zyzz() const;
	constexpr vec4_base<T> _zzxx() const;
	constexpr vec4_base<T> _zzxy() const;
	constexpr vec4_base<T> _zzxz() const;
	constexpr vec4_base<T> _zzyx() const;
	constexpr vec4_base<T> _zzyy() const;
	constexpr vec4_base<T> _zzyz() const;
	constexpr vec4_base<T> _zzzx() const;
	constexpr vec4_base<T> _zzzy() const;
	constexpr vec4_base<T> _zzzz() const;
#endif // 1

	static constexpr vec3_base unit_x() { return vec3_base{ 1, 0, 0 }; };
	static constexpr vec3_base unit_y() { return vec3_base{ 0, 1, 0 }; };
	static constexpr vec3_base unit_z() { return vec3_base{ 0, 0, 1 }; };

	constexpr bool operator == (vec3_base other) const { return x == other.x && y == other.y && z == other.z; }

	constexpr T &operator [] (size_t index)
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		throw 1;
	}
	constexpr T const &operator [] (size_t index) const
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		throw 1;
	}

	constexpr vec3_base &operator += (T val)        { x += val; y += val; z += val; return *this; }
	constexpr vec3_base  operator +  (T val) const  { return vec3_base(*this) += val; }
	constexpr vec3_base &operator += (vec3_base other)       { x += other.x; y += other.y; z += other.z; return *this; }
	constexpr vec3_base  operator +  (vec3_base other) const { return vec3_base(*this) += other; }

	constexpr vec3_base &operator -= (T val)        { x -= val; y -= val; z -= val; return *this; }
	constexpr vec3_base  operator -  (T val) const  { return vec3_base(*this) -= val; }
	constexpr vec3_base &operator -= (vec3_base other)       { x -= other.x; y -= other.y; z -= other.z; return *this; }
	constexpr vec3_base  operator -  (vec3_base other) const { return vec3_base(*this) -= other; }

	constexpr vec3_base &operator *= (T val)        { x *= val; y *= val; z *= val; return *this; }
	constexpr vec3_base  operator *  (T val) const  { return vec3_base(*this) *= val; }
	constexpr vec3_base &operator *= (vec3_base other)       { x *= other.x; y *= other.y; z *= other.z; return *this; }
	constexpr vec3_base  operator *  (vec3_base other) const { return vec3_base(*this) *= other; }

	constexpr vec3_base &operator /= (T val)        { x /= val; y /= val; z /= val; return *this; }
	constexpr vec3_base  operator /  (T val) const  { return vec3_base(*this) /= val; }
	constexpr vec3_base &operator /= (vec3_base other)       { x /= other.x; y /= other.y; z /= other.z; return *this; }
	constexpr vec3_base  operator /  (vec3_base other) const { return vec3_base(*this) /= other; }

	constexpr vec3_base &operator %= (T val)        { x = fmod(x, val); y = fmod(y, val); z = fmod(z, val); return *this; }
	constexpr vec3_base  operator %  (T val) const  { return vec3_base(*this) %= val; }
	constexpr vec3_base &operator %= (vec3_base other)       { x = fmod(x, other.x); y = fmod(y, other.y); z = fmod(z, other.z); return *this; }
	constexpr vec3_base  operator %  (vec3_base other) const { return vec3_base(*this) %= other; }

	constexpr vec3_base operator - () const { return vec3_base{ -x, -y, -z }; }

	constexpr T magnitude_squared() const
	{
		return x*x + y*y + z*z;
	}

	constexpr T magnitude() const
	{
		return sqrt(magnitude_squared());
	}

	constexpr T length() const
	{
		return magnitude();
	}

	constexpr vec3_base normalize() const
	{
		return vec3_base(*this) / length();
	}

	constexpr vec3_base rotate_x(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec3_base{ x, y*c - z*s, y*s + z*c };
	}

	constexpr vec3_base rotate_y(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec3_base{ x*c - z*s, y, x*s + z*c };
	}

	constexpr vec3_base rotate_z(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec3_base{ x*c - y*s, x*s + y*c, z };
	}

	constexpr T dot(vec3_base other) const
	{
		return x*other.x + y*other.y + z*other.z;
	}

	constexpr vec3_base cross(vec3_base other) const
	{
		return vec3_base{
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x,
		};
	}
};

template <typename T> requires(std::is_arithmetic_v<T>)
struct vec4_base
{
	using value_type = T;
	static constexpr inline size_t SIZE = 4;

	T x, y, z, w;

	constexpr vec4_base() = default;

	template <typename T1, typename T2 = T, typename T3 = T, typename T4 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3> && std::is_arithmetic_v<T4>)
	constexpr vec4_base(T1 _x, T2 _y = T2(0), T3 _z = T3(0), T4 _w = T4(0))
		: x{ T(_x) }, y{ T(_y) }, z{ T(_z) }, w{ T(_w) }
	{
	}

	template <typename T1, typename T2 = T, typename T3 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>)
	constexpr vec4_base(vec2_base<T1> _xy, T2 _z = T2(0), T3 _w = T3(0))
		: x{ T(_xy.x) }, y{ T(_xy.y) }, z{ T(_z) }, w{ T(_w) }
	{
	}

	template <typename T1, typename T2, typename T3 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>)
	constexpr vec4_base(T1 _x, vec2_base<T2> _yz, T3 _w = T3(0))
		: x{ T(_x) }, y{ T(_yz.x) }, z{ T(_yz.z) }, w{ T(_w) }
	{
	}

	template <typename T1, typename T2, typename T3> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_arithmetic_v<T3>)
	constexpr vec4_base(T1 _x, T2 _y, vec2_base<T3> _zw)
		: x{ T(_x) }, y{ T(_y) }, z{ T(_zw.x) }, w{ T(_zw.y) }
	{
	}

	template <typename T1, typename T2> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
	constexpr vec4_base(vec2_base<T1> _xy, vec2_base<T2> _zw)
		: x{ T(_xy.x) }, y{ T(_xy.y) }, z{ T(_zw.x) }, w{ T(_zw.y) }
	{
	}

	template <typename T1, typename T2 = T> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
	constexpr vec4_base(vec3_base<T1> _xyz, T2 _z = T2(0))
		: x{ T(_xyz.x) }, y{ T(_xyz.y) }, z{ T(_xyz.z) }, w{ T(_z) }
	{
	}

	template <typename T1, typename T2> requires(std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2>)
	constexpr vec4_base(T1 _x, vec3_base<T2> _yzw)
		: x{ T(_x) }, y{ T(_yzw.x) }, z{ T(_yzw.y) }, w{ T(_yzw.z) }
	{
	}

	template <typename T1> requires(std::is_arithmetic_v<T1>)
	constexpr vec4_base(vec4_base<T1> _xyzw)
		: x{ T(_xyzw.x) }, y{ T(_xyzw.y) }, z{ T(_xyzw.z) }, w{ T(_xyzw.w) }
	{
	}

#if 1
	constexpr vec1_base<T> _x() const { return x; }
	constexpr vec1_base<T> _y() const { return y; }
	constexpr vec1_base<T> _z() const { return z; }
	constexpr vec1_base<T> _w() const { return w; }

	constexpr vec2_base<T> _xx() const { return { x, x }; }
	constexpr vec2_base<T> _xy() const { return { x, y }; }
	constexpr vec2_base<T> _xz() const { return { x, z }; }
	constexpr vec2_base<T> _xw() const { return { x, w }; }
	constexpr vec2_base<T> _yx() const { return { y, x }; }
	constexpr vec2_base<T> _yy() const { return { y, y }; }
	constexpr vec2_base<T> _yz() const { return { y, z }; }
	constexpr vec2_base<T> _yw() const { return { y, w }; }
	constexpr vec2_base<T> _zx() const { return { z, x }; }
	constexpr vec2_base<T> _zy() const { return { z, y }; }
	constexpr vec2_base<T> _zz() const { return { z, z }; }
	constexpr vec2_base<T> _zw() const { return { z, w }; }
	constexpr vec2_base<T> _wx() const { return { w, x }; }
	constexpr vec2_base<T> _wy() const { return { w, y }; }
	constexpr vec2_base<T> _wz() const { return { w, z }; }
	constexpr vec2_base<T> _ww() const { return { w, w }; }

	constexpr vec3_base<T> _xxx() const { return { x, x, x }; }
	constexpr vec3_base<T> _xxy() const { return { x, x, y }; }
	constexpr vec3_base<T> _xxz() const { return { x, x, z }; }
	constexpr vec3_base<T> _xxw() const { return { x, x, w }; }
	constexpr vec3_base<T> _xyx() const { return { x, y, x }; }
	constexpr vec3_base<T> _xyy() const { return { x, y, y }; }
	constexpr vec3_base<T> _xyz() const { return { x, y, z }; }
	constexpr vec3_base<T> _xyw() const { return { x, y, w }; }
	constexpr vec3_base<T> _xzx() const { return { x, z, x }; }
	constexpr vec3_base<T> _xzy() const { return { x, z, y }; }
	constexpr vec3_base<T> _xzz() const { return { x, z, z }; }
	constexpr vec3_base<T> _xzw() const { return { x, z, w }; }
	constexpr vec3_base<T> _xwx() const { return { x, w, x }; }
	constexpr vec3_base<T> _xwy() const { return { x, w, y }; }
	constexpr vec3_base<T> _xwz() const { return { x, w, z }; }
	constexpr vec3_base<T> _xww() const { return { x, w, w }; }

	constexpr vec3_base<T> _yxx() const { return { y, x, x }; }
	constexpr vec3_base<T> _yxy() const { return { y, x, y }; }
	constexpr vec3_base<T> _yxz() const { return { y, x, z }; }
	constexpr vec3_base<T> _yxw() const { return { y, x, w }; }
	constexpr vec3_base<T> _yyx() const { return { y, y, x }; }
	constexpr vec3_base<T> _yyy() const { return { y, y, y }; }
	constexpr vec3_base<T> _yyz() const { return { y, y, z }; }
	constexpr vec3_base<T> _yyw() const { return { y, y, w }; }
	constexpr vec3_base<T> _yzx() const { return { y, z, x }; }
	constexpr vec3_base<T> _yzy() const { return { y, z, y }; }
	constexpr vec3_base<T> _yzz() const { return { y, z, z }; }
	constexpr vec3_base<T> _yzw() const { return { y, z, w }; }
	constexpr vec3_base<T> _ywx() const { return { y, w, x }; }
	constexpr vec3_base<T> _ywy() const { return { y, w, y }; }
	constexpr vec3_base<T> _ywz() const { return { y, w, z }; }
	constexpr vec3_base<T> _yww() const { return { y, w, w }; }

	constexpr vec3_base<T> _zxx() const { return { z, x, x }; }
	constexpr vec3_base<T> _zxy() const { return { z, x, y }; }
	constexpr vec3_base<T> _zxz() const { return { z, x, z }; }
	constexpr vec3_base<T> _zxw() const { return { z, x, w }; }
	constexpr vec3_base<T> _zyx() const { return { z, y, x }; }
	constexpr vec3_base<T> _zyy() const { return { z, y, y }; }
	constexpr vec3_base<T> _zyz() const { return { z, y, z }; }
	constexpr vec3_base<T> _zyw() const { return { z, y, w }; }
	constexpr vec3_base<T> _zzx() const { return { z, z, x }; }
	constexpr vec3_base<T> _zzy() const { return { z, z, y }; }
	constexpr vec3_base<T> _zzz() const { return { z, z, z }; }
	constexpr vec3_base<T> _zzw() const { return { z, z, w }; }
	constexpr vec3_base<T> _zwx() const { return { z, w, x }; }
	constexpr vec3_base<T> _zwy() const { return { z, w, y }; }
	constexpr vec3_base<T> _zwz() const { return { z, w, z }; }
	constexpr vec3_base<T> _zww() const { return { z, w, w }; }

	constexpr vec3_base<T> _wxx() const { return { w, x, x }; }
	constexpr vec3_base<T> _wxy() const { return { w, x, y }; }
	constexpr vec3_base<T> _wxz() const { return { w, x, z }; }
	constexpr vec3_base<T> _wxw() const { return { w, x, w }; }
	constexpr vec3_base<T> _wyx() const { return { w, y, x }; }
	constexpr vec3_base<T> _wyy() const { return { w, y, y }; }
	constexpr vec3_base<T> _wyz() const { return { w, y, z }; }
	constexpr vec3_base<T> _wyw() const { return { w, y, w }; }
	constexpr vec3_base<T> _wzx() const { return { w, z, x }; }
	constexpr vec3_base<T> _wzy() const { return { w, z, y }; }
	constexpr vec3_base<T> _wzz() const { return { w, z, z }; }
	constexpr vec3_base<T> _wzw() const { return { w, z, w }; }
	constexpr vec3_base<T> _wwx() const { return { w, w, x }; }
	constexpr vec3_base<T> _wwy() const { return { w, w, y }; }
	constexpr vec3_base<T> _wwz() const { return { w, w, z }; }
	constexpr vec3_base<T> _www() const { return { w, w, w }; }

	constexpr vec4_base<T> _xxxx() const { return { x, x, x, x }; }
	constexpr vec4_base<T> _xxxy() const { return { x, x, x, y }; }
	constexpr vec4_base<T> _xxxz() const { return { x, x, x, z }; }
	constexpr vec4_base<T> _xxxw() const { return { x, x, x, w }; }
	constexpr vec4_base<T> _xxyx() const { return { x, x, y, x }; }
	constexpr vec4_base<T> _xxyy() const { return { x, x, y, y }; }
	constexpr vec4_base<T> _xxyz() const { return { x, x, y, z }; }
	constexpr vec4_base<T> _xxyw() const { return { x, x, y, w }; }
	constexpr vec4_base<T> _xxzx() const { return { x, x, z, x }; }
	constexpr vec4_base<T> _xxzy() const { return { x, x, z, y }; }
	constexpr vec4_base<T> _xxzz() const { return { x, x, z, z }; }
	constexpr vec4_base<T> _xxzw() const { return { x, x, z, w }; }
	constexpr vec4_base<T> _xxwx() const { return { x, x, w, x }; }
	constexpr vec4_base<T> _xxwy() const { return { x, x, w, y }; }
	constexpr vec4_base<T> _xxwz() const { return { x, x, w, z }; }
	constexpr vec4_base<T> _xxww() const { return { x, x, w, w }; }
	constexpr vec4_base<T> _xyxx() const { return { x, y, x, x }; }
	constexpr vec4_base<T> _xyxy() const { return { x, y, x, y }; }
	constexpr vec4_base<T> _xyxz() const { return { x, y, x, z }; }
	constexpr vec4_base<T> _xyxw() const { return { x, y, x, w }; }
	constexpr vec4_base<T> _xyyx() const { return { x, y, y, x }; }
	constexpr vec4_base<T> _xyyy() const { return { x, y, y, y }; }
	constexpr vec4_base<T> _xyyz() const { return { x, y, y, z }; }
	constexpr vec4_base<T> _xyyw() const { return { x, y, y, w }; }
	constexpr vec4_base<T> _xyzx() const { return { x, y, z, x }; }
	constexpr vec4_base<T> _xyzy() const { return { x, y, z, y }; }
	constexpr vec4_base<T> _xyzz() const { return { x, y, z, z }; }
	constexpr vec4_base<T> _xyzw() const { return { x, y, z, w }; }
	constexpr vec4_base<T> _xywx() const { return { x, y, w, x }; }
	constexpr vec4_base<T> _xywy() const { return { x, y, w, y }; }
	constexpr vec4_base<T> _xywz() const { return { x, y, w, z }; }
	constexpr vec4_base<T> _xyww() const { return { x, y, w, w }; }
	constexpr vec4_base<T> _xzxx() const { return { x, z, x, x }; }
	constexpr vec4_base<T> _xzxy() const { return { x, z, x, y }; }
	constexpr vec4_base<T> _xzxz() const { return { x, z, x, z }; }
	constexpr vec4_base<T> _xzxw() const { return { x, z, x, w }; }
	constexpr vec4_base<T> _xzyx() const { return { x, z, y, x }; }
	constexpr vec4_base<T> _xzyy() const { return { x, z, y, y }; }
	constexpr vec4_base<T> _xzyz() const { return { x, z, y, z }; }
	constexpr vec4_base<T> _xzyw() const { return { x, z, y, w }; }
	constexpr vec4_base<T> _xzzx() const { return { x, z, z, x }; }
	constexpr vec4_base<T> _xzzy() const { return { x, z, z, y }; }
	constexpr vec4_base<T> _xzzz() const { return { x, z, z, z }; }
	constexpr vec4_base<T> _xzzw() const { return { x, z, z, w }; }
	constexpr vec4_base<T> _xzwx() const { return { x, z, w, x }; }
	constexpr vec4_base<T> _xzwy() const { return { x, z, w, y }; }
	constexpr vec4_base<T> _xzwz() const { return { x, z, w, z }; }
	constexpr vec4_base<T> _xzww() const { return { x, z, w, w }; }
	constexpr vec4_base<T> _xwxx() const { return { x, w, x, x }; }
	constexpr vec4_base<T> _xwxy() const { return { x, w, x, y }; }
	constexpr vec4_base<T> _xwxz() const { return { x, w, x, z }; }
	constexpr vec4_base<T> _xwxw() const { return { x, w, x, w }; }
	constexpr vec4_base<T> _xwyx() const { return { x, w, y, x }; }
	constexpr vec4_base<T> _xwyy() const { return { x, w, y, y }; }
	constexpr vec4_base<T> _xwyz() const { return { x, w, y, z }; }
	constexpr vec4_base<T> _xwyw() const { return { x, w, y, w }; }
	constexpr vec4_base<T> _xwzx() const { return { x, w, z, x }; }
	constexpr vec4_base<T> _xwzy() const { return { x, w, z, y }; }
	constexpr vec4_base<T> _xwzz() const { return { x, w, z, z }; }
	constexpr vec4_base<T> _xwzw() const { return { x, w, z, w }; }
	constexpr vec4_base<T> _xwwx() const { return { x, w, w, x }; }
	constexpr vec4_base<T> _xwwy() const { return { x, w, w, y }; }
	constexpr vec4_base<T> _xwwz() const { return { x, w, w, z }; }
	constexpr vec4_base<T> _xwww() const { return { x, w, w, w }; }

	constexpr vec4_base<T> _yxxx() const { return { y, x, x, x }; }
	constexpr vec4_base<T> _yxxy() const { return { y, x, x, y }; }
	constexpr vec4_base<T> _yxxz() const { return { y, x, x, z }; }
	constexpr vec4_base<T> _yxxw() const { return { y, x, x, w }; }
	constexpr vec4_base<T> _yxyx() const { return { y, x, y, x }; }
	constexpr vec4_base<T> _yxyy() const { return { y, x, y, y }; }
	constexpr vec4_base<T> _yxyz() const { return { y, x, y, z }; }
	constexpr vec4_base<T> _yxyw() const { return { y, x, y, w }; }
	constexpr vec4_base<T> _yxzx() const { return { y, x, z, x }; }
	constexpr vec4_base<T> _yxzy() const { return { y, x, z, y }; }
	constexpr vec4_base<T> _yxzz() const { return { y, x, z, z }; }
	constexpr vec4_base<T> _yxzw() const { return { y, x, z, w }; }
	constexpr vec4_base<T> _yxwx() const { return { y, x, w, x }; }
	constexpr vec4_base<T> _yxwy() const { return { y, x, w, y }; }
	constexpr vec4_base<T> _yxwz() const { return { y, x, w, z }; }
	constexpr vec4_base<T> _yxww() const { return { y, x, w, w }; }
	constexpr vec4_base<T> _yyxx() const { return { y, y, x, x }; }
	constexpr vec4_base<T> _yyxy() const { return { y, y, x, y }; }
	constexpr vec4_base<T> _yyxz() const { return { y, y, x, z }; }
	constexpr vec4_base<T> _yyxw() const { return { y, y, x, w }; }
	constexpr vec4_base<T> _yyyx() const { return { y, y, y, x }; }
	constexpr vec4_base<T> _yyyy() const { return { y, y, y, y }; }
	constexpr vec4_base<T> _yyyz() const { return { y, y, y, z }; }
	constexpr vec4_base<T> _yyyw() const { return { y, y, y, w }; }
	constexpr vec4_base<T> _yyzx() const { return { y, y, z, x }; }
	constexpr vec4_base<T> _yyzy() const { return { y, y, z, y }; }
	constexpr vec4_base<T> _yyzz() const { return { y, y, z, z }; }
	constexpr vec4_base<T> _yyzw() const { return { y, y, z, w }; }
	constexpr vec4_base<T> _yywx() const { return { y, y, w, x }; }
	constexpr vec4_base<T> _yywy() const { return { y, y, w, y }; }
	constexpr vec4_base<T> _yywz() const { return { y, y, w, z }; }
	constexpr vec4_base<T> _yyww() const { return { y, y, w, w }; }
	constexpr vec4_base<T> _yzxx() const { return { y, z, x, x }; }
	constexpr vec4_base<T> _yzxy() const { return { y, z, x, y }; }
	constexpr vec4_base<T> _yzxz() const { return { y, z, x, z }; }
	constexpr vec4_base<T> _yzxw() const { return { y, z, x, w }; }
	constexpr vec4_base<T> _yzyx() const { return { y, z, y, x }; }
	constexpr vec4_base<T> _yzyy() const { return { y, z, y, y }; }
	constexpr vec4_base<T> _yzyz() const { return { y, z, y, z }; }
	constexpr vec4_base<T> _yzyw() const { return { y, z, y, w }; }
	constexpr vec4_base<T> _yzzx() const { return { y, z, z, x }; }
	constexpr vec4_base<T> _yzzy() const { return { y, z, z, y }; }
	constexpr vec4_base<T> _yzzz() const { return { y, z, z, z }; }
	constexpr vec4_base<T> _yzzw() const { return { y, z, z, w }; }
	constexpr vec4_base<T> _yzwx() const { return { y, z, w, x }; }
	constexpr vec4_base<T> _yzwy() const { return { y, z, w, y }; }
	constexpr vec4_base<T> _yzwz() const { return { y, z, w, z }; }
	constexpr vec4_base<T> _yzww() const { return { y, z, w, w }; }
	constexpr vec4_base<T> _ywxx() const { return { y, w, x, x }; }
	constexpr vec4_base<T> _ywxy() const { return { y, w, x, y }; }
	constexpr vec4_base<T> _ywxz() const { return { y, w, x, z }; }
	constexpr vec4_base<T> _ywxw() const { return { y, w, x, w }; }
	constexpr vec4_base<T> _ywyx() const { return { y, w, y, x }; }
	constexpr vec4_base<T> _ywyy() const { return { y, w, y, y }; }
	constexpr vec4_base<T> _ywyz() const { return { y, w, y, z }; }
	constexpr vec4_base<T> _ywyw() const { return { y, w, y, w }; }
	constexpr vec4_base<T> _ywzx() const { return { y, w, z, x }; }
	constexpr vec4_base<T> _ywzy() const { return { y, w, z, y }; }
	constexpr vec4_base<T> _ywzz() const { return { y, w, z, z }; }
	constexpr vec4_base<T> _ywzw() const { return { y, w, z, w }; }
	constexpr vec4_base<T> _ywwx() const { return { y, w, w, x }; }
	constexpr vec4_base<T> _ywwy() const { return { y, w, w, y }; }
	constexpr vec4_base<T> _ywwz() const { return { y, w, w, z }; }
	constexpr vec4_base<T> _ywww() const { return { y, w, w, w }; }

	constexpr vec4_base<T> _zxxx() const { return { z, x, x, x }; }
	constexpr vec4_base<T> _zxxy() const { return { z, x, x, y }; }
	constexpr vec4_base<T> _zxxz() const { return { z, x, x, z }; }
	constexpr vec4_base<T> _zxxw() const { return { z, x, x, w }; }
	constexpr vec4_base<T> _zxyx() const { return { z, x, y, x }; }
	constexpr vec4_base<T> _zxyy() const { return { z, x, y, y }; }
	constexpr vec4_base<T> _zxyz() const { return { z, x, y, z }; }
	constexpr vec4_base<T> _zxyw() const { return { z, x, y, w }; }
	constexpr vec4_base<T> _zxzx() const { return { z, x, z, x }; }
	constexpr vec4_base<T> _zxzy() const { return { z, x, z, y }; }
	constexpr vec4_base<T> _zxzz() const { return { z, x, z, z }; }
	constexpr vec4_base<T> _zxzw() const { return { z, x, z, w }; }
	constexpr vec4_base<T> _zxwx() const { return { z, x, w, x }; }
	constexpr vec4_base<T> _zxwy() const { return { z, x, w, y }; }
	constexpr vec4_base<T> _zxwz() const { return { z, x, w, z }; }
	constexpr vec4_base<T> _zxww() const { return { z, x, w, w }; }
	constexpr vec4_base<T> _zyxx() const { return { z, y, x, x }; }
	constexpr vec4_base<T> _zyxy() const { return { z, y, x, y }; }
	constexpr vec4_base<T> _zyxz() const { return { z, y, x, z }; }
	constexpr vec4_base<T> _zyxw() const { return { z, y, x, w }; }
	constexpr vec4_base<T> _zyyx() const { return { z, y, y, x }; }
	constexpr vec4_base<T> _zyyy() const { return { z, y, y, y }; }
	constexpr vec4_base<T> _zyyz() const { return { z, y, y, z }; }
	constexpr vec4_base<T> _zyyw() const { return { z, y, y, w }; }
	constexpr vec4_base<T> _zyzx() const { return { z, y, z, x }; }
	constexpr vec4_base<T> _zyzy() const { return { z, y, z, y }; }
	constexpr vec4_base<T> _zyzz() const { return { z, y, z, z }; }
	constexpr vec4_base<T> _zyzw() const { return { z, y, z, w }; }
	constexpr vec4_base<T> _zywx() const { return { z, y, w, x }; }
	constexpr vec4_base<T> _zywy() const { return { z, y, w, y }; }
	constexpr vec4_base<T> _zywz() const { return { z, y, w, z }; }
	constexpr vec4_base<T> _zyww() const { return { z, y, w, w }; }
	constexpr vec4_base<T> _zzxx() const { return { z, z, x, x }; }
	constexpr vec4_base<T> _zzxy() const { return { z, z, x, y }; }
	constexpr vec4_base<T> _zzxz() const { return { z, z, x, z }; }
	constexpr vec4_base<T> _zzxw() const { return { z, z, x, w }; }
	constexpr vec4_base<T> _zzyx() const { return { z, z, y, x }; }
	constexpr vec4_base<T> _zzyy() const { return { z, z, y, y }; }
	constexpr vec4_base<T> _zzyz() const { return { z, z, y, z }; }
	constexpr vec4_base<T> _zzyw() const { return { z, z, y, w }; }
	constexpr vec4_base<T> _zzzx() const { return { z, z, z, x }; }
	constexpr vec4_base<T> _zzzy() const { return { z, z, z, y }; }
	constexpr vec4_base<T> _zzzz() const { return { z, z, z, z }; }
	constexpr vec4_base<T> _zzzw() const { return { z, z, z, w }; }
	constexpr vec4_base<T> _zzwx() const { return { z, z, w, x }; }
	constexpr vec4_base<T> _zzwy() const { return { z, z, w, y }; }
	constexpr vec4_base<T> _zzwz() const { return { z, z, w, z }; }
	constexpr vec4_base<T> _zzww() const { return { z, z, w, w }; }
	constexpr vec4_base<T> _zwxx() const { return { z, w, x, x }; }
	constexpr vec4_base<T> _zwxy() const { return { z, w, x, y }; }
	constexpr vec4_base<T> _zwxz() const { return { z, w, x, z }; }
	constexpr vec4_base<T> _zwxw() const { return { z, w, x, w }; }
	constexpr vec4_base<T> _zwyx() const { return { z, w, y, x }; }
	constexpr vec4_base<T> _zwyy() const { return { z, w, y, y }; }
	constexpr vec4_base<T> _zwyz() const { return { z, w, y, z }; }
	constexpr vec4_base<T> _zwyw() const { return { z, w, y, w }; }
	constexpr vec4_base<T> _zwzx() const { return { z, w, z, x }; }
	constexpr vec4_base<T> _zwzy() const { return { z, w, z, y }; }
	constexpr vec4_base<T> _zwzz() const { return { z, w, z, z }; }
	constexpr vec4_base<T> _zwzw() const { return { z, w, z, w }; }
	constexpr vec4_base<T> _zwwx() const { return { z, w, w, x }; }
	constexpr vec4_base<T> _zwwy() const { return { z, w, w, y }; }
	constexpr vec4_base<T> _zwwz() const { return { z, w, w, z }; }
	constexpr vec4_base<T> _zwww() const { return { z, w, w, w }; }

	constexpr vec4_base<T> _wxxx() const { return { w, x, x, x }; }
	constexpr vec4_base<T> _wxxy() const { return { w, x, x, y }; }
	constexpr vec4_base<T> _wxxz() const { return { w, x, x, z }; }
	constexpr vec4_base<T> _wxxw() const { return { w, x, x, w }; }
	constexpr vec4_base<T> _wxyx() const { return { w, x, y, x }; }
	constexpr vec4_base<T> _wxyy() const { return { w, x, y, y }; }
	constexpr vec4_base<T> _wxyz() const { return { w, x, y, z }; }
	constexpr vec4_base<T> _wxyw() const { return { w, x, y, w }; }
	constexpr vec4_base<T> _wxzx() const { return { w, x, z, x }; }
	constexpr vec4_base<T> _wxzy() const { return { w, x, z, y }; }
	constexpr vec4_base<T> _wxzz() const { return { w, x, z, z }; }
	constexpr vec4_base<T> _wxzw() const { return { w, x, z, w }; }
	constexpr vec4_base<T> _wxwx() const { return { w, x, w, x }; }
	constexpr vec4_base<T> _wxwy() const { return { w, x, w, y }; }
	constexpr vec4_base<T> _wxwz() const { return { w, x, w, z }; }
	constexpr vec4_base<T> _wxww() const { return { w, x, w, w }; }
	constexpr vec4_base<T> _wyxx() const { return { w, y, x, x }; }
	constexpr vec4_base<T> _wyxy() const { return { w, y, x, y }; }
	constexpr vec4_base<T> _wyxz() const { return { w, y, x, z }; }
	constexpr vec4_base<T> _wyxw() const { return { w, y, x, w }; }
	constexpr vec4_base<T> _wyyx() const { return { w, y, y, x }; }
	constexpr vec4_base<T> _wyyy() const { return { w, y, y, y }; }
	constexpr vec4_base<T> _wyyz() const { return { w, y, y, z }; }
	constexpr vec4_base<T> _wyyw() const { return { w, y, y, w }; }
	constexpr vec4_base<T> _wyzx() const { return { w, y, z, x }; }
	constexpr vec4_base<T> _wyzy() const { return { w, y, z, y }; }
	constexpr vec4_base<T> _wyzz() const { return { w, y, z, z }; }
	constexpr vec4_base<T> _wyzw() const { return { w, y, z, w }; }
	constexpr vec4_base<T> _wywx() const { return { w, y, w, x }; }
	constexpr vec4_base<T> _wywy() const { return { w, y, w, y }; }
	constexpr vec4_base<T> _wywz() const { return { w, y, w, z }; }
	constexpr vec4_base<T> _wyww() const { return { w, y, w, w }; }
	constexpr vec4_base<T> _wzxx() const { return { w, z, x, x }; }
	constexpr vec4_base<T> _wzxy() const { return { w, z, x, y }; }
	constexpr vec4_base<T> _wzxz() const { return { w, z, x, z }; }
	constexpr vec4_base<T> _wzxw() const { return { w, z, x, w }; }
	constexpr vec4_base<T> _wzyx() const { return { w, z, y, x }; }
	constexpr vec4_base<T> _wzyy() const { return { w, z, y, y }; }
	constexpr vec4_base<T> _wzyz() const { return { w, z, y, z }; }
	constexpr vec4_base<T> _wzyw() const { return { w, z, y, w }; }
	constexpr vec4_base<T> _wzzx() const { return { w, z, z, x }; }
	constexpr vec4_base<T> _wzzy() const { return { w, z, z, y }; }
	constexpr vec4_base<T> _wzzz() const { return { w, z, z, z }; }
	constexpr vec4_base<T> _wzzw() const { return { w, z, z, w }; }
	constexpr vec4_base<T> _wzwx() const { return { w, z, w, x }; }
	constexpr vec4_base<T> _wzwy() const { return { w, z, w, y }; }
	constexpr vec4_base<T> _wzwz() const { return { w, z, w, z }; }
	constexpr vec4_base<T> _wzww() const { return { w, z, w, w }; }
	constexpr vec4_base<T> _wwxx() const { return { w, w, x, x }; }
	constexpr vec4_base<T> _wwxy() const { return { w, w, x, y }; }
	constexpr vec4_base<T> _wwxz() const { return { w, w, x, z }; }
	constexpr vec4_base<T> _wwxw() const { return { w, w, x, w }; }
	constexpr vec4_base<T> _wwyx() const { return { w, w, y, x }; }
	constexpr vec4_base<T> _wwyy() const { return { w, w, y, y }; }
	constexpr vec4_base<T> _wwyz() const { return { w, w, y, z }; }
	constexpr vec4_base<T> _wwyw() const { return { w, w, y, w }; }
	constexpr vec4_base<T> _wwzx() const { return { w, w, z, x }; }
	constexpr vec4_base<T> _wwzy() const { return { w, w, z, y }; }
	constexpr vec4_base<T> _wwzz() const { return { w, w, z, z }; }
	constexpr vec4_base<T> _wwzw() const { return { w, w, z, w }; }
	constexpr vec4_base<T> _wwwx() const { return { w, w, w, x }; }
	constexpr vec4_base<T> _wwwy() const { return { w, w, w, y }; }
	constexpr vec4_base<T> _wwwz() const { return { w, w, w, z }; }
	constexpr vec4_base<T> _wwww() const { return { w, w, w, w }; }
#endif // 1

	static constexpr vec4_base unit_x() { return vec4_base{ 1, 0, 0, 0 }; };
	static constexpr vec4_base unit_y() { return vec4_base{ 0, 1, 0, 0 }; };
	static constexpr vec4_base unit_z() { return vec4_base{ 0, 0, 1, 0 }; };
	static constexpr vec4_base unit_w() { return vec4_base{ 0, 0, 0, 1 }; };

	constexpr bool operator == (vec4_base other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }

	constexpr T &operator [] (size_t index)
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		if (index == 3) return w;
		throw 1;
	}
	constexpr T const &operator [] (size_t index) const
	{
		if (!std::is_constant_evaluated()) return (&x)[index];
		if (index == 0) return x;
		if (index == 1) return y;
		if (index == 2) return z;
		if (index == 3) return w;
		throw 1;
	}

	constexpr vec4_base &operator += (T val)        { x += val; y += val; z += val; w += val; return *this; }
	constexpr vec4_base  operator +  (T val) const  { return vec4_base(*this) += val; }
	constexpr vec4_base &operator += (vec4_base other)       { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
	constexpr vec4_base  operator +  (vec4_base other) const { return vec4_base(*this) += other; }

	constexpr vec4_base &operator -= (T val)        { x -= val; y -= val; z -= val; w -= val; return *this; }
	constexpr vec4_base  operator -  (T val) const  { return vec4_base(*this) -= val; }
	constexpr vec4_base &operator -= (vec4_base other)       { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
	constexpr vec4_base  operator -  (vec4_base other) const { return vec4_base(*this) -= other; }

	constexpr vec4_base &operator *= (T val)        { x *= val; y *= val; z *= val; w *= val; return *this; }
	constexpr vec4_base  operator *  (T val) const  { return vec4_base(*this) *= val; }
	constexpr vec4_base &operator *= (vec4_base other)       { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
	constexpr vec4_base  operator *  (vec4_base other) const { return vec4_base(*this) *= other; }

	constexpr vec4_base &operator /= (T val)        { x /= val; y /= val; z /= val; w /= val; return *this; }
	constexpr vec4_base  operator /  (T val) const  { return vec4_base(*this) /= val; }
	constexpr vec4_base &operator /= (vec4_base other)       { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
	constexpr vec4_base  operator /  (vec4_base other) const { return vec4_base(*this) /= other; }

	constexpr vec4_base &operator %= (T val)        { x = fmod(x, val); y = fmod(y, val); z = fmod(z, val); w = fmod(w, val); return *this; }
	constexpr vec4_base  operator %  (T val) const  { return vec4_base(*this) %= val; }
	constexpr vec4_base &operator %= (vec4_base other)       { x = fmod(x, other.x); y = fmod(y, other.y); z = fmod(z, other.z); w = fmod(w, other.w); return *this; }
	constexpr vec4_base  operator %  (vec4_base other) const { return vec4_base(*this) %= other; }

	constexpr vec4_base operator - () const { return vec4_base{ -x, -y, -z, -w }; }

	constexpr T magnitude_squared() const
	{
		return x*x + y*y + z*z + w*w;
	}

	constexpr T magnitude() const
	{
		return sqrt(magnitude_squared());
	}

	constexpr T length() const
	{
		return magnitude();
	}

	constexpr vec4_base normalize() const
	{
		return vec4_base(*this) * (1.0f / length());
	}

	constexpr vec4_base rotate_x(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec4_base{ x, y*c - z*s, y*s + z*c, w };
	}

	constexpr vec4_base rotate_y(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec4_base{ x*c - z*s, y, x*s + z*c, w };
	}

	constexpr vec4_base rotate_z(T angle) const
	{
		T c = cos(angle);
		T s = sin(angle);
		return vec4_base{ x*c - y*s, x*s + y*c, z, w };
	}

	constexpr T dot(vec4_base other) const
	{
		return x*other.x + y*other.y + z*other.z + w*other.w;
	}

	constexpr vec4_base cross(vec4_base other) const
	{
		return vec4_base{
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x,
			w,
		};
	}
};

#if 1
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec2_base<T> vec1_base<T>::_xx() const { return { x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec1_base<T>::_xxx() const { return { x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec1_base<T>::_xxxx() const { return { x, x, x, x }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_xxx() const { return { x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_xxy() const { return { x, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_xyx() const { return { x, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_xyy() const { return { x, y, y }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_yxx() const { return { y, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_yxy() const { return { y, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_yyx() const { return { y, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec3_base<T> vec2_base<T>::_yyy() const { return { y, y, y }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xxxx() const { return { x, x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xxxy() const { return { x, x, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xxyx() const { return { x, x, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xxyy() const { return { x, x, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xyxx() const { return { x, y, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xyxy() const { return { x, y, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xyyx() const { return { x, y, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_xyyy() const { return { x, y, y, y }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yxxx() const { return { y, x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yxxy() const { return { y, x, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yxyx() const { return { y, x, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yxyy() const { return { y, x, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yyxx() const { return { y, y, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yyxy() const { return { y, y, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yyyx() const { return { y, y, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec2_base<T>::_yyyy() const { return { y, y, y, y }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxxx() const { return { x, x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxxy() const { return { x, x, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxxz() const { return { x, x, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxyx() const { return { x, x, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxyy() const { return { x, x, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxyz() const { return { x, x, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxzx() const { return { x, x, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxzy() const { return { x, x, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xxzz() const { return { x, x, z, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyxx() const { return { x, y, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyxy() const { return { x, y, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyxz() const { return { x, y, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyyx() const { return { x, y, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyyy() const { return { x, y, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyyz() const { return { x, y, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyzx() const { return { x, y, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyzy() const { return { x, y, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xyzz() const { return { x, y, z, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzxx() const { return { x, z, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzxy() const { return { x, z, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzxz() const { return { x, z, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzyx() const { return { x, z, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzyy() const { return { x, z, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzyz() const { return { x, z, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzzx() const { return { x, z, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzzy() const { return { x, z, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_xzzz() const { return { x, z, z, z }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxxx() const { return { y, x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxxy() const { return { y, x, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxxz() const { return { y, x, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxyx() const { return { y, x, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxyy() const { return { y, x, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxyz() const { return { y, x, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxzx() const { return { y, x, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxzy() const { return { y, x, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yxzz() const { return { y, x, z, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyxx() const { return { y, y, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyxy() const { return { y, y, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyxz() const { return { y, y, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyyx() const { return { y, y, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyyy() const { return { y, y, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyyz() const { return { y, y, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyzx() const { return { y, y, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyzy() const { return { y, y, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yyzz() const { return { y, y, z, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzxx() const { return { y, z, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzxy() const { return { y, z, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzxz() const { return { y, z, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzyx() const { return { y, z, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzyy() const { return { y, z, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzyz() const { return { y, z, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzzx() const { return { y, z, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzzy() const { return { y, z, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_yzzz() const { return { y, z, z, z }; }

template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxxx() const { return { z, x, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxxy() const { return { z, x, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxxz() const { return { z, x, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxyx() const { return { z, x, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxyy() const { return { z, x, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxyz() const { return { z, x, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxzx() const { return { z, x, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxzy() const { return { z, x, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zxzz() const { return { z, x, z, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyxx() const { return { z, y, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyxy() const { return { z, y, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyxz() const { return { z, y, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyyx() const { return { z, y, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyyy() const { return { z, y, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyyz() const { return { z, y, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyzx() const { return { z, y, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyzy() const { return { z, y, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zyzz() const { return { z, y, z, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzxx() const { return { z, z, x, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzxy() const { return { z, z, x, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzxz() const { return { z, z, x, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzyx() const { return { z, z, y, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzyy() const { return { z, z, y, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzyz() const { return { z, z, y, z }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzzx() const { return { z, z, z, x }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzzy() const { return { z, z, z, y }; }
template<typename T> requires(std::is_arithmetic_v<T>) constexpr vec4_base<T> vec3_base<T>::_zzzz() const { return { z, z, z, z }; }
#endif // 1

template <typename T> requires(std::is_arithmetic_v<T>)
struct mat4x4_base
{
	using value_type = T;
	static constexpr inline size_t COLS = 4;
	static constexpr inline size_t ROWS = 4;

	vec4_base<T> col0, col1, col2, col3;

	static constexpr mat4x4_base identity()
	{
		return mat4x4_base{
			{ 1, 0, 0, 0 },
			{ 0, 1, 0, 0 },
			{ 0, 0, 1, 0 },
			{ 0, 0, 0, 1 },
		};
	};

	constexpr bool operator == (mat4x4_base other) const { return col0 == other.col0 && col1 == other.col1 && col2 == other.col2 && col3 == other.col3; }

	constexpr vec4_base<T> &operator [] (size_t index)
	{
		if (!std::is_constant_evaluated()) return (&col0)[index];
		if (index == 0) return col0;
		if (index == 1) return col1;
		if (index == 2) return col2;
		if (index == 3) return col3;
		throw 1;
	}
	constexpr vec4_base<T> const &operator [] (size_t index) const
	{
		if (!std::is_constant_evaluated()) return (&col0)[index];
		if (index == 0) return col0;
		if (index == 1) return col1;
		if (index == 2) return col2;
		if (index == 3) return col3;
		throw 1;
	}
	
	constexpr mat4x4_base &operator += (T val)        { col0 += val; col1 += val; col2 += val; col3 += val; return *this; }
	constexpr mat4x4_base  operator +  (T val) const  { return mat4x4_base(*this) += val; }

	constexpr mat4x4_base &operator -= (T val)        { col0 -= val; col1 -= val; col2 -= val; col3 -= val; return *this; }
	constexpr mat4x4_base  operator -  (T val) const  { return mat4x4_base(*this) -= val; }

	constexpr mat4x4_base &operator *= (T val)        { col0 *= val; col1 *= val; col2 *= val; col3 *= val; return *this; }
	constexpr mat4x4_base  operator *  (T val) const  { return mat4x4_base(*this) *= val; }

	constexpr mat4x4_base &operator /= (T val)        { return *this *= 1 / val; }
	constexpr mat4x4_base  operator /  (T val) const  { return mat4x4_base(*this) /= val; }

	constexpr mat4x4_base &operator %= (T val)        { col0 %= val; col1 %= val; col2 %= val; col3 %= val; return *this; }
	constexpr mat4x4_base  operator %  (T val) const  { return mat4x4_base(*this) %= val; }

	constexpr mat4x4_base operator * (mat4x4_base other) const
	{
		mat4x4_base result{};

		result.col0 += col0 * other.col0.x;
		result.col0 += col1 * other.col0.y;
		result.col0 += col2 * other.col0.z;
		result.col0 += col3 * other.col0.w;

		result.col1 += col0 * other.col1.x;
		result.col1 += col1 * other.col1.y;
		result.col1 += col2 * other.col1.z;
		result.col1 += col3 * other.col1.w;

		result.col2 += col0 * other.col2.x;
		result.col2 += col1 * other.col2.y;
		result.col2 += col2 * other.col2.z;
		result.col2 += col3 * other.col2.w;

		result.col3 += col0 * other.col3.x;
		result.col3 += col1 * other.col3.y;
		result.col3 += col2 * other.col3.z;
		result.col3 += col3 * other.col3.w;

		return result;
	}

	constexpr mat4x4_base transpose() const
	{
		return mat4x4_base{
			{ col0.x, col1.x, col2.x, col3.x },
			{ col0.y, col1.y, col2.y, col3.y },
			{ col0.z, col1.z, col2.z, col3.z },
			{ col0.w, col1.w, col2.w, col3.w },
		};
	}

	constexpr mat4x4_base inverse() const
	{
		T Coef00 = col2[2] * col3[3] - col3[2] * col2[3];
		T Coef02 = col1[2] * col3[3] - col3[2] * col1[3];
		T Coef03 = col1[2] * col2[3] - col2[2] * col1[3];

		T Coef04 = col2[1] * col3[3] - col3[1] * col2[3];
		T Coef06 = col1[1] * col3[3] - col3[1] * col1[3];
		T Coef07 = col1[1] * col2[3] - col2[1] * col1[3];

		T Coef08 = col2[1] * col3[2] - col3[1] * col2[2];
		T Coef10 = col1[1] * col3[2] - col3[1] * col1[2];
		T Coef11 = col1[1] * col2[2] - col2[1] * col1[2];

		T Coef12 = col2[0] * col3[3] - col3[0] * col2[3];
		T Coef14 = col1[0] * col3[3] - col3[0] * col1[3];
		T Coef15 = col1[0] * col2[3] - col2[0] * col1[3];

		T Coef16 = col2[0] * col3[2] - col3[0] * col2[2];
		T Coef18 = col1[0] * col3[2] - col3[0] * col1[2];
		T Coef19 = col1[0] * col2[2] - col2[0] * col1[2];

		T Coef20 = col2[0] * col3[1] - col3[0] * col2[1];
		T Coef22 = col1[0] * col3[1] - col3[0] * col1[1];
		T Coef23 = col1[0] * col2[1] - col2[0] * col1[1];

		vec4_base<T> Fac0(Coef00, Coef00, Coef02, Coef03);
		vec4_base<T> Fac1(Coef04, Coef04, Coef06, Coef07);
		vec4_base<T> Fac2(Coef08, Coef08, Coef10, Coef11);
		vec4_base<T> Fac3(Coef12, Coef12, Coef14, Coef15);
		vec4_base<T> Fac4(Coef16, Coef16, Coef18, Coef19);
		vec4_base<T> Fac5(Coef20, Coef20, Coef22, Coef23);

		vec4_base<T> Vec0(col1[0], col0[0], col0[0], col0[0]);
		vec4_base<T> Vec1(col1[1], col0[1], col0[1], col0[1]);
		vec4_base<T> Vec2(col1[2], col0[2], col0[2], col0[2]);
		vec4_base<T> Vec3(col1[3], col0[3], col0[3], col0[3]);

		vec4_base<T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		vec4_base<T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		vec4_base<T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		vec4_base<T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		vec4_base<T> SignA(+1, -1, +1, -1);
		vec4_base<T> SignB(-1, +1, -1, +1);
		mat4x4_base Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		vec4_base<T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		vec4_base<T> Dot0(col0 * Row0);
		T Dot1 = (Dot0.x + Dot0.y) + (Dot0.z + Dot0.w);

		T OneOverDeterminant = static_cast<T>(1) / Dot1;

		return Inverse * OneOverDeterminant;
	}
};

template<typename T>
constexpr mat4x4_base<T> perspective(T fovy, T aspect, T zNear, T zFar)
{
	T const tanHalfFovy = tan(fovy / static_cast<T>(2));

	mat4x4_base<T> Result{};
	Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
	Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
	Result[2][2] = - (zFar + zNear) / (zFar - zNear);
	Result[2][3] = - static_cast<T>(1);
	Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
	return Result;
}

template<typename T>
constexpr mat4x4_base<T> look_at(vec3_base<T> const &eye, vec3_base<T> const &center, vec3_base<T> const &up)
{
	vec3_base<T> const f((center - eye).normalize());
	vec3_base<T> const s(f.cross(up).normalize());
	vec3_base<T> const u(s.cross(f));

	mat4x4_base<T> Result = mat4x4_base<T>::identity();
	Result[0][0] =  s.x;
	Result[1][0] =  s.y;
	Result[2][0] =  s.z;
	Result[0][1] =  u.x;
	Result[1][1] =  u.y;
	Result[2][1] =  u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = -s.dot(eye);
	Result[3][1] = -u.dot(eye);
	Result[3][2] =  f.dot(eye);
	return Result;
}


template <typename T> constexpr bool is_vector = false;
template <typename T> constexpr bool is_vector<vec1_base<T>> = true;
template <typename T> constexpr bool is_vector<vec2_base<T>> = true;
template <typename T> constexpr bool is_vector<vec3_base<T>> = true;
template <typename T> constexpr bool is_vector<vec4_base<T>> = true;

template <typename VT, typename F> requires(is_vector<VT>)
constexpr VT _comp(const VT v1, F f)
{
	VT result{};

	for (size_t i = 0; i < VT::SIZE; ++i)
		result[i] = f(v1[i]);

	return result;
}

template <typename VT, typename F> requires(is_vector<VT>)
constexpr VT _comp(const VT v1, const VT v2, F f)
{
	VT result{};

	for (size_t i = 0; i < VT::SIZE; ++i)
		result[i] = f(v1[i], v2[i]);

	return result;
}

template <typename T, typename VT> requires(is_vector<VT> && std::is_floating_point_v<T>)
constexpr VT lerp(const VT x, const VT y, const T t)
{
	return _comp(x, y, [&](auto _x, auto _y){ return lerp<VT::value_type>(_x, _y, t); });
}

template <typename VT> requires(is_vector<VT>)
constexpr VT floor(const VT val)
{
	return _comp(val, floor<VT::template value_type>);
}

template <typename VT> requires(is_vector<VT>)
constexpr VT abs(const VT val)
{
	return _comp(val, abs<VT::template value_type>);
}

} // namespace math

namespace std
{

template <typename T> struct hash<math::vec1_base<T>> { size_t operator()(const math::vec1_base<T> &v) const { return hash<T>{}(v.x); } };
template <typename T> struct hash<math::vec2_base<T>> { size_t operator()(const math::vec2_base<T> &v) const { return hash<T>{}(v.x) ^ hash<T>{}(v.y) << 1; } };
template <typename T> struct hash<math::vec3_base<T>> { size_t operator()(const math::vec3_base<T> &v) const { return hash<T>{}(v.x) ^ hash<T>{}(v.y) << 1 ^ hash<T>{}(v.z) << 2; } };
template <typename T> struct hash<math::vec4_base<T>> { size_t operator()(const math::vec4_base<T> &v) const { return hash<T>{}(v.x) ^ hash<T>{}(v.y) << 1 ^ hash<T>{}(v.z) << 2 ^ hash<T>{}(v.w) >> 1; } };
template <typename T> struct hash<math::mat4x4_base<T>> { size_t operator()(const math::mat4x4_base<T> &v) const { return hash<math::vec4_base<T>>{}(v.col0) ^ hash<math::vec4_base<T>>{}(v.col1) << 1 ^ hash<math::vec4_base<T>>{}(v.col2) << 2 ^ hash<math::vec4_base<T>>{}(v.col3) >> 1; } };

} // namespace std

using vec1 = math::vec1_base<float>;
using vec2 = math::vec2_base<float>;
using vec3 = math::vec3_base<float>;
using vec4 = math::vec4_base<float>;
using mat4 = math::mat4x4_base<float>;

using ivec1 = math::vec1_base<int32_t>;
using ivec2 = math::vec2_base<int32_t>;
using ivec3 = math::vec3_base<int32_t>;
using ivec4 = math::vec4_base<int32_t>;
using imat4 = math::mat4x4_base<int32_t>;

enum class Line2IntersectResultType
{
	SegmentIntersect,
	LineIntersect,
	Collinear,
	Parallel,
};

struct Line2IntersectResult
{
	Line2IntersectResultType type;
	vec2 pos;
	float phase0, phase1;

	constexpr bool intersect() const
	{
		return type == Line2IntersectResultType::SegmentIntersect || type == Line2IntersectResultType::LineIntersect;
	}

	constexpr bool intersect_segment1_forward() const
	{
		return type == Line2IntersectResultType::SegmentIntersect || (type == Line2IntersectResultType::LineIntersect && phase0 >= 0.0f && phase1 >= 0.0f && phase1 <= 1.0f);
	}
};

struct Line2
{
	vec2 p0, p1;

	constexpr Line2IntersectResult intersect(Line2 const &other) const
	{
		// https://youtu.be/bvlIYX9cgls

		const auto x0 = p0.x, x1 = p1.x, x2 = other.p0.x, x3 = other.p1.x;
		const auto y0 = p0.y, y1 = p1.y, y2 = other.p0.y, y3 = other.p1.y;

		const auto a = (x3 - x2)*(y2 - y0) - (y3 - y2)*(x2 - x0);
		const auto b = (x3 - x2)*(y1 - y0) - (y3 - y2)*(x1 - x0);
		const auto c = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);

		if (a == 0.0f && b == 0.0f)
			return { Line2IntersectResultType::Collinear, { std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() }, std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() };

		if (b == 0.0f)
			return { Line2IntersectResultType::Parallel, { std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() }, std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN() };

		const auto alpha = a / b;
		const auto beta = c / b;

		const vec2 crossPos = lerp(alpha);

		if (alpha >= 0.0f && alpha <= 1.0f && beta >= 0.0f && beta <= 1.0f)
			return { Line2IntersectResultType::SegmentIntersect, crossPos, alpha, beta };

		return { Line2IntersectResultType::LineIntersect, crossPos, alpha, beta };
	}

	constexpr vec2 lerp(float t) const
	{
		return math::lerp(p0, p1, t);
	}

	constexpr vec2 center() const
	{
		return (p0 + p1) / 2.0f;
	}

	constexpr bool Empty() const
	{
		return p0 == p1;
	}
};

struct Triangle2
{
	vec2 p0, p1, p2;

	constexpr std::array<Line2, 3> edges() const
	{
		return { Line2{ p0, p1 }, Line2{ p1, p2 }, Line2{ p2, p0 } };
	}

	constexpr vec2 center() const
	{
		return (p0 + p1 + p2) / 3.0f;
	}
};

struct Color
{
	uint8_t r, g, b, a;

	static constinit const Color WHITE;
	static constinit const Color BLACK;
	static constinit const Color TRANSPARENT;

	static constinit const Color RED;
	static constinit const Color GREEN;
	static constinit const Color BLUE;

	static constinit const Color YELLOW;
	static constinit const Color MAGENTA;
	static constinit const Color CYAN;
};

constinit inline const Color Color::WHITE{ 255, 255, 255, 255 }; // r=1, g=1, b=1, a=1
constinit inline const Color Color::BLACK{ 0, 0, 0, 255 }; // r=0, g=0 b=0, a=1
constinit inline const Color Color::TRANSPARENT{ 0, 0, 0, 0 }; // r=0, g=0, b=0, a=0

constinit inline const Color Color::RED{ 255, 0, 0, 255 }; // r=1, g=0, b=0, a=1
constinit inline const Color Color::GREEN{ 0, 255, 0, 255 }; // r=0, g=1, b=0, a=1
constinit inline const Color Color::BLUE{ 0, 0, 255, 255 }; // r=0, g=0, b=1, a=1

constinit inline const Color Color::YELLOW{ 255, 255, 0, 255 }; // r=1, g=1, b=0, a=1
constinit inline const Color Color::MAGENTA{ 255, 0, 255, 255 }; // r=1, g=0, b=1, a=1
constinit inline const Color Color::CYAN{ 0, 255, 255, 255 }; // r=0, g=1, b=1, a=1

struct ColorF
{
	float r, g, b, a;

	static constinit const ColorF WHITE;
	static constinit const ColorF BLACK;
	static constinit const ColorF TRANSPARENT;

	static constinit const ColorF RED;
	static constinit const ColorF GREEN;
	static constinit const ColorF BLUE;

	static constinit const ColorF YELLOW;
	static constinit const ColorF MAGENTA;
	static constinit const ColorF CYAN;
};

constinit inline const ColorF ColorF::WHITE{ 1.0f, 1.0f, 1.0f, 1.0f }; // r=1, g=1, b=1, a=1
constinit inline const ColorF ColorF::BLACK{ 0.0f, 0.0f, 0.0f, 1.0f }; // r=0, g=0 b=0, a=1
constinit inline const ColorF ColorF::TRANSPARENT{ 0.0f, 0.0f, 0.0f, 0.0f }; // r=0, g=0, b=0, a=0

constinit inline const ColorF ColorF::RED{ 1.0f, 0.0f, 0.0f, 1.0f }; // r=1, g=0, b=0, a=1
constinit inline const ColorF ColorF::GREEN{ 0.0f, 1.0f, 0.0f, 1.0f }; // r=0, g=1, b=0, a=1
constinit inline const ColorF ColorF::BLUE{ 0.0f, 0.0f, 1.0f, 1.0f }; // r=0, g=0, b=1, a=1

constinit inline const ColorF ColorF::YELLOW{ 1.0f, 1.0f, 0.0f, 1.0f }; // r=1, g=1, b=0, a=1
constinit inline const ColorF ColorF::MAGENTA{ 1.0f, 0.0f, 1.0f, 1.0f }; // r=1, g=0, b=1, a=1
constinit inline const ColorF ColorF::CYAN{ 0.0f, 1.0f, 1.0f, 1.0f }; // r=0, g=1, b=1, a=1

