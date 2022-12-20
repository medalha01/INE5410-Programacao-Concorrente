#include <semaphore.h>
#ifdef __INE5410_SEMAPHORE_H__

/* desfaz redirecionamentos em nível de preprocessador */
#undef sem_init
#undef sem_destroy
#undef sem_wait
#undef sem_timedwait
#undef sem_trywait
#undef sem_post
#undef sem_getvalue
#undef sem_t

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>


int ine5410_sem_init(ine5410_sem_t* sem, int pshared, unsigned int value) {
    //    printf("++++++++++++++++++\n");
    fflush(stdout);
    char name[64] = {0};
    snprintf(name, 64, "/%ld-%p", (size_t)getpid(), sem);
    errno = 0;
    sem->ptr = sem_open(name, O_CREAT, 0777, value);
    int old_errno = errno;
    if (sem->ptr == SEM_FAILED) {
        printf("[validator] [sem_init no Mac OS] Erro ao criar semáforo %s "
               "(sem_t* = %p): %s", name, sem, strerror(old_errno));
        fflush(stdout);
        errno = old_errno;
        return -1;
    }
    return 0;
}

int ine5410_sem_destroy(ine5410_sem_t* sem) {
    char name[64] = {0};
    snprintf(name, 64, "/%ld-%p", (size_t)getpid(), sem);
    errno = 0;
    int code = sem_unlink(name);
    int old_errno = errno;
    if (code != 0) {
        printf("[validator] [sem_destroy no Mac OS] Erro ao fechar semáforo %s "
               "(sem_t* = %p, sem_t.ptr = %p): %s", name, sem, sem->ptr,
               strerror(old_errno));
        fflush(stdout);
    }
    errno = old_errno;
    return code;
}

int ine5410_sem_wait(ine5410_sem_t* sem) {
    return sem_wait(sem->ptr);
}

int ine5410_sem_timedwait(ine5410_sem_t* sem, const struct timespec* ts) {
    // sem_timedwait() não está disponível no Mac OS. Logo, é
    // necessário fazer uma gambiarra nojenta aqui. Não esse tipo de
    // coisa nos seus trabalhos!
    long ns_step = 50*1000000;
    size_t n_loops = (double)ts->tv_sec/0.050 + (double)ts->tv_nsec/ns_step;
    int err = EAGAIN;
    for (size_t i = 0; i  < n_loops && (err=ine5410_sem_trywait(sem)) != 0; ++i) {
        struct timespec step = {0, ns_step};
        nanosleep(&step, NULL);
    }
    return err;
}

int ine5410_sem_trywait(ine5410_sem_t* sem) {
    return sem_trywait(sem->ptr);
}

int ine5410_sem_post(ine5410_sem_t* sem) {
    return sem_post(sem->ptr);
}

int ine5410_sem_getvalue(ine5410_sem_t* sem, int val) {
    printf("[validator] sem_getvalue() não funciona no Mac OS!!!!\n");
    fflush(stdout);
    errno = ENOTSUP;
    return -1;
    
}

#endif /*__INE5410_SEMAPHORE_H__*/
