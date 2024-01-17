#ifndef __xqueue_h__
#define __xqueue_h__

#include <zephyr/kernel.h>

typedef struct
{
    size_t len;
    uint8_t *data;
} queue_msg_t;

typedef struct
{
    const struct k_mem_slab *msgs_slab;
    const struct k_mem_slab *data_slab;
    struct k_queue *queue;
    const size_t msg_max_length;
} queue_data_spec_t;

#define DEFINE_QUEUE_DATA(_queue, _p_msg_size, _p_msgs_limit)                     \
    K_QUEUE_DEFINE(_queue##_msgs);                                                \
    const size_t _queue##_msgs_limit = _p_msgs_limit;                             \
    const size_t _queue##_msg_size = _p_msg_size;                                 \
    K_MEM_SLAB_DEFINE(_queue##_msgs_slab, sizeof(queue_msg_t), _p_msgs_limit, 2); \
    K_MEM_SLAB_DEFINE(_queue##_msgs_data_slab, _p_msg_size, _p_msgs_limit, 2)

#define INIT_QUEUE_SPEC(_queue)                \
    {                                          \
        .msg_max_length = _queue##_msgs_limit, \
        .msgs_slab = &_queue##_msgs_slab,      \
        .data_slab = &_queue##_msgs_data_slab, \
        .queue = &_queue##_msgs                \
    }

void free_queue_msg(queue_data_spec_t *spec, queue_msg_t *msg);
queue_msg_t *alloc_queue_msg(queue_data_spec_t *spec);

#endif