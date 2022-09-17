/** 
 * @file 	queue.h
 * @author 	Mohit Rathod
 * Created: 17 09 2022, 05:38:04 pm
 * -----
 * Last Modified: 17 09 2022, 11:48:43 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Queue implementation (FIFO) to synchronise processes.
 */
#ifndef utils_queue_h
#define utils_queue_h
#include <stdint.h>
#include <string.h>

/**
 * @typedef typedef unsigned int qid_t;
 * @brief   Queue identifier
 *          Describes the Queue that needs to be accessed.
 *          A unique value represents a unique queue.
 */
typedef unsigned int qid_t;

/**
 * @typedef struct q_attr_t
 * @brief   user defined queue attributes.
 */
typedef struct {
    size_t elen;    /* size of an individual element in bytes. */
    size_t qlen;    /* size of queue(ie the number of elements).
                     * It has to be a power of 2 ie 2,4,8...etc
                     */
    void *buffer;   /* a pointer to buffer that will hold the data. */
}q_attr_t;

/**
 * @fn      int q_init(qid_t *q_handle, q_attr_t *attr);
 * @brief   Initialize a queue member.
 * 
 * @param[out]  q_handle    queue ID for the initialized member.
 * @param[in]   attr        queue attribute to init the queue.
 * @return      0 on success,
 *              -1 for max init calls or null handle/attr pointers
 *              -2 for invalid buffer in attr
 *              -3 for buffer not sized to a power of 2
 * @note    The attributes must contain a buffer which is sized
 *          to an even power of 2. This should be reflected by 
 *          the attribute qlen.
 *          => qlen = 2^n where n is a positive integer.
 */
int q_init(qid_t *q_handle, q_attr_t *attr);

/**
 * @fn      int qEnqueue(qid_t uQ, const void *pdata);
 * @brief   Add an element to the queue.
 * @param   uQ      the queue identifier.    
 * @param   pdata   pointer to data.
 * @return      0 on success,
 *             -1 queue full,
 *             -2 invalid queue identifier
 */
int qEnqueue(qid_t uQ, const void *pdata);

/**
 * @fn      int qDequeue(qid_t uQ, void *pdata);
 * @brief   Fetch(remove) an element from the queue
 * @param   uQ      queueID to remove data from
 * @param   pdata   pointer to load the removed data.
 * @return      0 on success,
 *             -1 queue empty
 *             -2 invalid queue identifier
 */
int qDequeue(qid_t uQ, void *pdata);

/**
 * @fn      int qCount(qid_t uQ);
 * @brief   Number of data slots occupied in a queue.
 * @param   uQ      queue identifier
 * @return  count(int) 
 */
int qCount(qid_t uQ);


#endif /* utils_queue_h */
