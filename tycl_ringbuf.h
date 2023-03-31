/**
 * file:    tycl_ringbuf.h
 * author:  wallying@foxmail.com
 * date:    2023-03-30
 **/

#ifndef __TYCL_RINGBUF_H__
#define __TYCL_RINGBUF_H__


#include <stdint.h>


typedef struct {
    uint8_t *buf;       /* ringbuf buffer pointer */
    uint32_t len;       /* ringbuf buffer length (byte) */
    uint32_t cnt;       /* ringbuf data length (byte) */
    uint32_t wptr;
    uint32_t rptr;
} tycl_ringbuf_t;


void  tycl_ringbuf_init(tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len);
void  tycl_ringbuf_clear(tycl_ringbuf_t *rb);

uint8_t tycl_ringbuf_is_empty(const tycl_ringbuf_t *rb);
uint8_t tycl_ringbuf_is_full(const tycl_ringbuf_t *rb);

uint32_t tycl_ringbuf_data_len(const tycl_ringbuf_t *rb);
uint32_t tycl_ringbuf_free_len(const tycl_ringbuf_t *rb);

uint32_t tycl_ringbuf_put(tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len);
uint32_t tycl_ringbuf_get(tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len);
uint32_t tycl_ringbuf_peek(const tycl_ringbuf_t *rb, uint8_t *buf, uint32_t len);


#endif /* __TYCL_RINGBUF_H__ */

