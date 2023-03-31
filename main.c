#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "wallyingLib.h"
#include "tycl_ringbuf.h"



#define RINGBUF_BUF_SIZE 14//(20)


static tycl_ringbuf_t my_ringbuf;

static uint8_t my_ringbuf_buf[RINGBUF_BUF_SIZE];



/*----------------------------------------------------------------------------*/

static void print_buffer(void)
{
    hexdump("my_ringbuf_buf", my_ringbuf_buf, sizeof(my_ringbuf_buf));

    uint32_t data_len = tycl_ringbuf_data_len(&my_ringbuf);
    uint32_t free_len = tycl_ringbuf_free_len(&my_ringbuf);

    printf(" => ");
    if (tycl_ringbuf_is_empty(&my_ringbuf)) {
        printf("(data=%d, free=%d) empty!!!\r\n", data_len, free_len);
    } else if (tycl_ringbuf_is_full(&my_ringbuf)) {
        printf("(data=%d, free=%d) full!!!\r\n", data_len, free_len);
    } else {
        printf("(data=%d, free=%d)\r\n", data_len, free_len);
    }
    printf("\r\n");
}



uint32_t cnt = 0;
uint32_t num = 0;
uint8_t put_data[100];
uint8_t get_data[100];


void key_0_down_callback(char key)
{
    put_data[cnt] = rand_number(0x00, 0xFF);
    cnt++;
    printf("    count = %d\r\n", cnt);
}


void key_1_down_callback(char key)
{
    if (cnt == 0) {
        printf("please set cnt by press KEY[0] !!!\r\n");
        return;
    }

    hexdump("put_data", put_data, cnt);
    num = tycl_ringbuf_put(&my_ringbuf, put_data, cnt);
    if (num > 0) {
        printf("tycl_ringbuf_put(%d)\r\n", num);
    } else {
        printf("tycl_ringbuf_put: full!\r\n");
    }
    cnt = 0;
    print_buffer();
}


void key_7_down_callback(char key)
{
    if (cnt == 0) {
        printf("please set cnt by press KEY[0] !!!\r\n");
        return;
    }

    num = tycl_ringbuf_get(&my_ringbuf, get_data, cnt);
    if (num > 0) {
        printf("tycl_ringbuf_get(%d)\r\n", num);
        for (int i = 0; i < num; ++i) {
            printf("    %d -- (%02X)\r\n", i, get_data[i]);
        }
    } else {
        printf("tycl_ringbuf_get: empty!\r\n");
    }
    cnt = 0;
    print_buffer();
}


void key_8_down_callback(char key)
{
    if (cnt == 0) {
        printf("please set cnt by press KEY[0] !!!\r\n");
        return;
    }

    num = tycl_ringbuf_peek(&my_ringbuf, get_data, cnt);
    if (num > 0) {
        printf("tycl_ringbuf_peek(%d)\r\n", num);
        for (int i = 0; i < num; ++i) {
            printf("    %d -- %02X\r\n", i, get_data[i]);
        }
    } else {
        printf("tycl_ringbuf_peek: empty!\r\n");
    }
    cnt = 0;
    print_buffer();
}


void key_9_down_callback(char key)
{
    tycl_ringbuf_clear(&my_ringbuf);
    printf("tycl_ringbuf_clear()\r\n");
    cnt = 0;
    print_buffer();
}







int main(int argc, char *argv[])
{
    printf("Hello World!\r\n");


    printf("\r\n");
    printf("\tKEY[0] : set_count \r\n");
    printf("\tKEY[1] : tycl_ringbuf_put() \r\n");
    printf("\tKEY[7] : tycl_ringbuf_get() \r\n");
    printf("\tKEY[8] : tycl_ringbuf_peek() \r\n");
    printf("\tKEY[9] : tycl_ringbuf_clear() \r\n");
    printf("\r\n");


    key_register('0', key_0_down_callback, NULL);
    key_register('1', key_1_down_callback, NULL);
    key_register('7', key_7_down_callback, NULL);
    key_register('8', key_8_down_callback, NULL);
    key_register('9', key_9_down_callback, NULL);



    tycl_ringbuf_init(&my_ringbuf, my_ringbuf_buf, sizeof(my_ringbuf_buf));
    print_buffer();


    while (1)
    {

    }


    system("pause");
    return 0;
}


