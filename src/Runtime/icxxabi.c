
// Required by GCC to register global destructors.
// This doesn't do anything because Global objects in my
// kernel are eternal.
int __cxa_atexit(void (*func) (void *), void * arg, void * dso_handle)
{
    (void)(func);
    (void)(arg);
    (void)(dso_handle);
    return 1;
}

void __cxa_pure_virtual() 
{

}
