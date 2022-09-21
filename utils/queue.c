/** 
 * @file 	queue.c
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 09:03:11 pm
 * -----
 * Last Modified: 18 09 2022, 12:04:08 am
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
    size_t elen;            /* size of a member element in the queue */
    size_t qlen;            /* number of members in the queue */
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

int q_init(qid_t *qID, q_attr_t *attr)
{
    static qid_t qidx = 0;
    int ret = -1;
    if ((qidx < QUEUE_MAX) && (qID != NULL) && (attr != NULL)) {
        
        ret = -2;
        if ((attr->buffer != NULL) && (attr->elen > 0)) {
        
            ret = -3;
            /* Check that the size of the queue is a power of 2 */
            if (((attr->qlen - 1) & attr->qlen) == 0) {
                /* Initialize the queue internal variables */
                _Q[qidx].head = 0;
                _Q[qidx].tail = 0;
                _Q[qidx].elen = attr->elen;
                _Q[qidx].qlen = attr->qlen;
                _Q[qidx].buf = attr->buffer;
                *qID = qidx++;
                ret = 0;
            }
        }
    } 
    return ret;
}

int qEnqueue(qid_t qID, const void *pdata)
{
    int ret = -2;
    if (qID < QUEUE_MAX) {
        ret = -1;
        /* Queue not full? */
        if (isQFull(&(_Q[qID])) == 0) {
            const size_t offset = (_Q[qID].head & (_Q[qID].qlen - 1)) 
                                    * _Q[qID].elen;
            memcpy(&(_Q[qID].buf[offset]), pdata, _Q[qID].elen);
            _Q[qID].head++;
            ret = 0;
        }
    }
    return ret;
}

int qDequeue(qid_t qID, void *pdata)
{
    int ret = -2;
    if (qID < QUEUE_MAX) {
        ret = -1;
        /* Queue not empty? */
        if (isQEmpty(&(_Q[qID])) == 0) {
            const size_t offset = (_Q[qID].tail & (_Q[qID].qlen - 1)) 
                                    * _Q[qID].elen;
            memcpy(pdata, &(_Q[qID].buf[offset]), _Q[qID].elen);
            _Q[qID].tail++;
            ret = 0;
        }
    }
    return ret;
}

int qCount(qid_t qID)
{
    int count = -1;
    if (qID < QUEUE_MAX) {
        count = (size_t)(_Q[qID].head - _Q[qID].tail);
    }
    return count;
}