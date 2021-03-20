#ifndef ASSERT_H_
#define ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#define assert(x) do { \
	if(!(x)) { \
		__builtin_trap(); \
	}\
} while(0)

#define ASSERT assert

#if __STDC_VERSION__ >= 201112L && !defined(__cplusplus)
#define static_assert _Static_assert
#endif
												  
#ifdef __cplusplus
}
#endif

#endif // ASSERT_H_
