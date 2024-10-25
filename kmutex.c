/******************************************************************************/
/* Important Fall 2024 CSCI 402 usage information:                            */
/*                                                                            */
/* This fils is part of CSCI 402 kernel programming assignments at USC.       */
/*         53616c7465645f5fd1e93dbf35cbffa3aef28f8c01d8cf2ffc51ef62b26a       */
/*         f9bda5a68e5ed8c972b17bab0f42e24b19daa7bd408305b1f7bd6c7208c1       */
/*         0e36230e913039b3046dd5fd0ba706a624d33dbaa4d6aab02c82fe09f561       */
/*         01b0fd977b0051f0b0ce0c69f7db857b1b5e007be2db6d42894bf93de848       */
/*         806d9152bd5715e9                                                   */
/* Please understand that you are NOT permitted to distribute or publically   */
/*         display a copy of this file (or ANY PART of it) for any reason.    */
/* If anyone (including your prospective employer) asks you to post the code, */
/*         you must inform them that you do NOT have permissions to do so.    */
/* You are also NOT permitted to remove or alter this comment block.          */
/* If this comment block is removed or altered in a submitted file, 20 points */
/*         will be deducted.                                                  */
/******************************************************************************/

#include "globals.h"
#include "errno.h"

#include "util/debug.h"

#include "proc/kthread.h"
#include "proc/kmutex.h"

/*
 * IMPORTANT: Mutexes can _NEVER_ be locked or unlocked from an
 * interrupt context. Mutexes are _ONLY_ lock or unlocked from a
 * thread context.
 */

void
kmutex_init(kmutex_t *mtx)
{
		dbg(DBG_PRINT, "(GRADING1A 6)\n");
		sched_queue_init(&mtx->km_waitq);
        // NOT_YET_IMPLEMENTED("PROCS: kmutex_init");
}

/*
 * This should block the current thread (by sleeping on the mutex's
 * wait queue) if the mutex is already taken.
 *
 * No thread should ever try to lock a mutex it already has locked.
 */
void
kmutex_lock(kmutex_t *mtx)
{
		// curthr must be valid and it must not be holding the mutex (mtx) already
		KASSERT(curthr && (curthr != mtx->km_holder));
		dbg(DBG_PRINT, "(GRADING1A 6.a)\n");
		if (mtx->km_holder == NULL){
			mtx->km_holder = curthr;
			dbg(DBG_PRINT, "(GRADING1A 6)\n");
		}
		else{
			sched_sleep_on(&mtx->km_waitq);
			dbg(DBG_PRINT, "(GRADING1A 6)\n");
		}
        // NOT_YET_IMPLEMENTED("PROCS: kmutex_lock");
}

/*
 * This should do the same as kmutex_lock, but use a cancellable sleep
 * instead. Also, if you are cancelled while holding mtx, you should unlock mtx.
 */
int
kmutex_lock_cancellable(kmutex_t *mtx)
{
        /* please use TWO consecutive "conforming dbg() calls" for this since this function is not called if you just start and stop weenix */
    	KASSERT(curthr && (curthr != mtx->km_holder)); /* curthr must be valid and it must not be holding the mutex (mtx) already */
		dbg(DBG_PRINT, "(GRADING1A 6.b)\n");
		dbg(DBG_PRINT, "(GRADING1C)\n");

		if (mtx->km_holder == NULL){
			mtx->km_holder = curthr;
			dbg(DBG_PRINT, "(GRADING1A 6)\n");
		}else{
			if (mtx->km_holder->kt_cancelled == 1){
				kmutex_unlock(mtx);
				dbg(DBG_PRINT, "(GRADING1A 6)\n");
			}
			dbg(DBG_PRINT, "(GRADING1A 6)\n");
			return sched_cancellable_sleep_on(&mtx->km_waitq); //return status 
		}
		// NOT_YET_IMPLEMENTED("PROCS: kmutex_lock_cancellable");
        return 0;
}

/*
 * If there are any threads waiting to take a lock on the mutex, one
 * should be woken up and given the lock.
 *
 * Note: This should _NOT_ be a blocking operation!
 *
 * Note: Ensure the new owner of the mutex enters the run queue.
 *
 * Note: Make sure that the thread on the head of the mutex's wait
 * queue becomes the new owner of the mutex.
 *
 * @param mtx the mutex to unlock
 */
void
kmutex_unlock(kmutex_t *mtx)
{
		// curthr must be valid & it must currently holding the mutex (mtx)
		KASSERT(curthr && (curthr == mtx->km_holder));
		dbg(DBG_PRINT, "(GRADING1A 6.c)\n");

		if (!sched_queue_empty(&mtx->km_waitq)){
			mtx->km_holder = sched_wakeup_on(&mtx->km_waitq);
			// on return, curthr must not be the mutex (mtx) holder
			KASSERT(curthr != mtx->km_holder);
			dbg(DBG_PRINT, "(GRADING1A 6.c)\n");
		}
		else{
			mtx->km_holder =  NULL;
			// on return, curthr must not be the mutex (mtx) holder
			KASSERT(curthr != mtx->km_holder);
			dbg(DBG_PRINT, "(GRADING1A 6.c)\n");
		}
		
        // NOT_YET_IMPLEMENTED("PROCS: kmutex_unlock");
}
