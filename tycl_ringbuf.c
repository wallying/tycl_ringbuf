/**
 * file:    tycl_ringbuf.c
 * author:  wallying@foxmail.com
 * date:    2023-03-30
 **/

#include <string.h>
#include "tycl_ringbuf.h"


#define RINGBUF_DEBUG 1//0


void tycl_ringbuf_init(tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len)
{
    if ((rb == NULL) || (buf == NULL) || (len == 0)) {
        return;
    }

    rb->buf = buf;
    rb->len = len;
    rb->cnt = 0;
    rb->wptr = 0;
    rb->rptr = 0;

    #if (RINGBUF_DEBUG)
    memset(rb->buf, 0x00, rb->len);
    #endif
}


void tycl_ringbuf_clear(tycl_ringbuf_t *rb)
{
    if ((rb == NULL) || (rb->buf == NULL)) {
        return;
    }

    rb->cnt = 0;
    rb->wptr = 0;
    rb->rptr = 0;

    #if (RINGBUF_DEBUG)
    memset(rb->buf, 0x00, rb->len);
    #endif
}


uint8_t tycl_ringbuf_is_empty(const tycl_ringbuf_t *rb)
{
    if (rb == NULL) {
        return 0;
    }

    return (rb->cnt == 0) ? 1 : 0;
}


uint8_t tycl_ringbuf_is_full(const tycl_ringbuf_t *rb)
{
    if (rb == NULL) {
        return 0;
    }

    return (rb->cnt == rb->len) ? 1 : 0;
}


uint32_t tycl_ringbuf_data_len(const tycl_ringbuf_t *rb)
{
    if (rb == NULL) {
        return 0;
    }

    return rb->cnt;
}


uint32_t tycl_ringbuf_free_len(const tycl_ringbuf_t *rb)
{
    if (rb == NULL) {
        return 0;
    }

    return (rb->len - rb->cnt);
}


uint32_t tycl_ringbuf_put(tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len)
{
    if ((rb == NULL) || (rb->buf == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }

    if (len > (rb->len - rb->cnt)) {
        return 0;
    }

    uint32_t seg;
    seg = (len < (rb->len - rb->wptr)) ? len : (rb->len - rb->wptr);
    memcpy(rb->buf + rb->wptr, buf, seg);
    rb->wptr += seg;
    if (len - seg) {
        memcpy(rb->buf, buf + seg, len - seg);
        rb->wptr = len - seg;
    }
    rb->cnt += len;

    return len;
}


uint32_t tycl_ringbuf_get(tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len)
{
    if ((rb == NULL) || (rb->buf == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }

    if (len > rb->cnt) {
        return 0;
    }

    uint32_t seg;
    seg = (len < (rb->len - rb->rptr)) ? len : (rb->len - rb->rptr);
    memcpy(buf, rb->buf + rb->rptr, seg);
    #if (RINGBUF_DEBUG)
    memset(rb->buf + rb->rptr, 0x00, seg);
    #endif
    rb->rptr += seg;
    if (len - seg) {
        memcpy(buf + seg, rb->buf, len - seg);
        #if (RINGBUF_DEBUG)
        memset(rb->buf, 0x00, len - seg);
        #endif
        rb->rptr = len - seg;
    }
    rb->cnt -= len;

    return len;
}


uint32_t tycl_ringbuf_peek(const tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len)
{
    if ((rb == NULL) || (rb->buf == NULL) || (buf == NULL) || (len == 0)) {
        return 0;
    }

    if (rb->cnt == 0) {
        return 0;
    }

    len = (len > rb->cnt) ? rb->cnt : len;

    uint32_t seg;
    uint32_t rptr = rb->rptr;
    seg = (len < (rb->len - rb->rptr)) ? len : (rb->len - rb->rptr);
    memcpy(buf, rb->buf + rptr, seg);
    rptr += seg;
    if (len - seg) {
        memcpy(buf + seg, rb->buf, len - seg);
    }

    return len;
}


