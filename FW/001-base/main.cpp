
#include "main.h"

/*
 * 栈溢出钩子
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask,char * pcTaskName )
{
    while(true);
}

/*
 * 空闲钩子
 */
void vApplicationIdleHook( void )
{
    if(hwatchdog_is_valid())
    {
        HWATCHDOG_FEED();
    }
    else
    {
        hruntime_loop_enable_softwatchdog(false);
    }
}

/*
 * hbox任务
 */
static void hbox_task_entry(void *usr)
{
    hcpprt_init();
    while(1)
    {
        hcpprt_loop();
        vTaskDelay(1);
    }
}

/*
 * 主任务
 */
static void main_task_entry(void *usr)
{
    while(true)
    {
        vTaskDelay(1);
    }
}


int main()
{
    SystemInit();

    SDRAMInit();

    hruntime_init_lowlevel();

    xTaskCreate( hbox_task_entry, "hbox_task",4096/sizeof(StackType_t), NULL, 1, NULL );

    xTaskCreate( main_task_entry, "main_task",4096/sizeof(StackType_t), NULL, 2, NULL );

    vTaskStartScheduler();

    while(true)
    {

    }
    return 0;
}

static void  main_init(const hruntime_function_t *func)
{

}
HRUNTIME_INIT_EXPORT(main,0xFFFF,main_init,NULL);

static void  main_loop(const hruntime_function_t *func)
{

}
HRUNTIME_LOOP_EXPORT(main,0xFFFF,main_loop,NULL);


