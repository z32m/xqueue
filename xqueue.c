#include "xqueue.h"
#include <string.h>
#include <xlog.h>

queue_msg_t *alloc_queue_msg(queue_data_spec_t *spec)
{
    queue_msg_t *msg;

    SURE(k_mem_slab_alloc, spec->msgs_slab, (void **)&msg, K_NO_WAIT);
    SURE(k_mem_slab_alloc, spec->data_slab, (void **)&(msg->data), K_NO_WAIT);

    return msg;
}

void free_queue_msg(queue_data_spec_t *spec, queue_msg_t *msg)
{
    k_mem_slab_free(spec->data_slab, (void *)(msg->data));
    k_mem_slab_free(spec->msgs_slab, (void *)msg);
}

void xqueue_send_msg(queue_data_spec_t *spec, queue_msg_t *msg)
{
    k_queue_alloc_append(spec->queue, msg);

    spec->len++;
}

queue_msg_t *xqueue_recv_msg(queue_data_spec_t *spec, k_timeout_t timeout)
{
    if (!spec->len)
        return NULL;

    queue_msg_t *res = k_queue_get(spec->queue, timeout);
    if (res == NULL)
        return NULL;

    spec->len--;
    return res;
}