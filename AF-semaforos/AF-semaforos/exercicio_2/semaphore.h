#ifndef __INE5410_SEMAPHORE_H__
#ifdef __APPLE__
    #define __INE5410_SEMAPHORE_H__
    
    #ifdef __deprecated
        #define OLD_DEPRECATED __deprecated
        #undef __deprecated
    #endif /*__deprecated*/

    #define __deprecated
    #include </usr/include/semaphore.h>
    #undef __deprecated
    
    #ifdef OLD_DEPRECATED
        #define __deprecated OLD_DEPRECATED
    #endif /*OLD_DEPRECATED*/
    
    struct timespec;
    
    typedef struct ine5410_sem_s {
        sem_t* ptr;
    } ine5410_sem_t;
    
    // daqui pra frente, sem_t é ine5410_sem_t
    #define sem_t ine5410_sem_t
    
    /******************************************************
     *                      ATENÇÃO                       *
     * Não chame essas 6 funções. chame as funções sem_*  *
     * diretamente e deixe as macros fazerem sua mágica   *
     ******************************************************/
    
    int  ine5410_sem_init(sem_t* sem, int pshared, unsigned int value);
    int  ine5410_sem_destroy(sem_t* sem);
    int  ine5410_sem_wait(sem_t* sem);
    int  ine5410_sem_timedwait(sem_t* sem, const struct timespec* ts);
    int  ine5410_sem_trywait(sem_t* sem);
    int  ine5410_sem_post(sem_t* sem);
    int  ine5410_sem_getvalue(sem_t* sem, int val);
    
    #define sem_init(ptr, pshared, val) ine5410_sem_init(ptr, pshared, val)
    #define sem_destroy(ptr)            ine5410_sem_destroy(ptr)
    #define sem_wait(ptr)               ine5410_sem_wait(ptr)
    #define sem_timedwait(ptr, ts)      ine5410_sem_timedwait(ptr, ts)
    #define sem_trywait(ptr)            ine5410_sem_trywait(ptr)
    #define sem_post(ptr)               ine5410_sem_post(ptr)
    #define sem_getvalue(ptr, out)      ine5410_sem_getvalue(ptr, out)
    
#else
    
    #include </usr/include/semaphore.h>
    
#endif /*__APPLE__*/
#endif /*__INE5410_SEMAPHORE_H__*/
