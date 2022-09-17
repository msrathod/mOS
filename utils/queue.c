/** 
 * @file 	queue.c
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 09:03:11 pm
 * -----
 * Last Modified: 17 09 2022, 10:06:09 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Queue implementation (FIFO) to synchronise processes.
 */
#include <queue.h>
#include <mosconfig.h>

typedef unsigned char uchar_t;

#if MOS_GET(MAX_QUEUE)
#define QUEUE_MAX               MOS_GET(MAX_QUEUE)
#else
#define QUEUE_MAX               (2)
#endif

/**
 * @brief Queue struct
 */
typedef struct
{
    size_t elen;        /* size of a member element in the queue */
    size_t qlen;        /* number of members in the queue */
    uchar_t *buf;           /* queue buffer */
    volatile size_t head;   /* head of queue */
    volatile size_t tail;   /* tail of queue */
}queue_t;

static queue_t _Q[QUEUE_MAX];

/**
 * @brief   Queue full test function
 * 
 * @param   pQ  pointer to queue 
 * @return  1 when full, 0 otherwise.
 */
static size_t isQFull(queue_t *pQ)
{
    return ((size_t)(pQ->head - pQ->tail) == pQ->qlen) ? 1 : 0;
}

/**
 * @brief   Queue empty test function
 * 
 * @param   pQ  pointer to queue 
 * @return  1 when empty, 0 otherwise.
 */
static size_t isQEmpty(queue_t *pQ)
{
  return ((size_t)(pQ->head - pQ->tail) == 0U) ? 1 : 0;
}
