#ifndef MATH_H__
#define MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef FLT_EVAL_METHOD
#if FLT_EVAL_METHOD == 0
typedef float float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
typedef float float_t;
typedef double double_t;
#endif // FLT_EVAL_METHOD value
#else
typedef float float_t;
typedef double double_t;
#endif // ifdef FLT_EVAL_METHOD

#ifdef __cplusplus
}
#endif

#endif // MATH_H__
