#ifndef KERNEL_GTHR_BTHREAD_H
#define KERNEL_GTHR_BTHREAD_H

typedef int __gthread_key_t;
typedef int __gthread_once_t;
typedef int __gthread_mutex_t;
typedef int __gthread_recursive_mutex_t;

#define __GTHREAD_ONCE_INIT 0
#define __GTHREAD_MUTEX_INIT 0
#define __GTHREAD_MUTEX_INIT_FUNCTION(mx) do {} while (0)
#define __GTHREAD_RECURSIVE_MUTEX_INIT 0

#ifdef __cplusplus
extern "C"
{
#endif

int __gthread_active_p (void);
int __gthread_once (__gthread_once_t *__once , void (*__func) (void));
int __gthread_key_create (__gthread_key_t *__key, void (*__func) (void *));
int __gthread_key_delete (__gthread_key_t __key);
void *__gthread_getspecific (__gthread_key_t __key);
int __gthread_setspecific (__gthread_key_t __key, const void *__v);
int __gthread_mutex_destroy (__gthread_mutex_t *__mutex);
int __gthread_mutex_lock (__gthread_mutex_t *__mutex);
int __gthread_mutex_trylock (__gthread_mutex_t *__mutex);
int __gthread_mutex_unlock (__gthread_mutex_t *__mutex);
int __gthread_recursive_mutex_lock (__gthread_recursive_mutex_t *__mutex);
int __gthread_recursive_mutex_trylock (__gthread_recursive_mutex_t *__mutex);
int __gthread_recursive_mutex_unlock (__gthread_recursive_mutex_t *__mutex);
int __gthread_recursive_mutex_destroy (__gthread_recursive_mutex_t *__mutex);

#ifdef __cplusplus
}
#endif

#endif // KERNEL_GTHR_BTHREAD_H
