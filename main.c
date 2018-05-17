#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "freertos_warp.h"

TaskHandle_t        g_task;
QueueHandle_t       g_QH;

typedef struct foo
{
    uint32_t        value;
    uint32_t        reserved;

} foo_t;


static void
_task_test(void *argv)
{
    uint32_t    *pIs_exit = (uint32_t*)argv;

    while( *pIs_exit == false )
    {
        foo_t       foo = {0};

        xQueueReceive(g_QH, &foo, portMAX_DELAY);
        printf("%d, %x\n", foo.value, foo.reserved);
        vTaskDelay(2);
    }

    vTaskDelete(NULL);
    return;
}

int main()
{
    int                 g_cnt = 50;
    foo_t               foo = {0};
    uint32_t            g_is_exit = false;
    init_rtos_wrap();

    g_QH = xQueueCreate(1, sizeof(foo_t));
    xTaskCreate(_task_test, "test", 512, &g_is_exit, 2, &g_task);

    while( g_cnt-- )
        Sleep(100);

    foo.value    = 0x111;
    foo.reserved = 0xAAA;
    xQueueSend(g_QH, &foo, portMAX_DELAY);

    vTaskResume(g_task);

    while(1)        Sleep(1000);
    return 0;
}
