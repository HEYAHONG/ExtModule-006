
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

/*
 * 初始化堆(可使用SDRAM作为堆)
 * 当内存堆足够大时，可运行zlib与lz4解压缩。
 */
static void HeapInit(void)
{
#if (FREERTOS_KERNEL_MEMMANG_HEAP)==5

#if ((SDRAM_SIZE)==(SDRAM_SIZE_2MB)) || ((SDRAM_SIZE)==(SDRAM_SIZE_8MB)) ||((SDRAM_SIZE)==(SDRAM_SIZE_16MB)) || ((SDRAM_SIZE)==(SDRAM_SIZE_32MB))
    {
        uint8_t *       sdram_heap_base=(uint8_t *)(uintptr_t)SDRAMM_BASE;
        const size_t    sdram_heap_size=SDRAMSize();
        memset(sdram_heap_base,0,sdram_heap_size); //清空SDRAM的堆区域
        static const HeapRegion_t xHeapRegions[] =
        {
            { (uint8_t *)sdram_heap_base, sdram_heap_size },   // SDRAM
            { NULL, 0 }                                        // 结束标记
        };
        vPortDefineHeapRegions(xHeapRegions);
    }
#else
    {
        static uint8_t ucHeap[configTOTAL_HEAP_SIZE]= {0};
        static const HeapRegion_t xHeapRegions[] =
        {
            { (uint8_t *)ucHeap, sizeof(ucHeap) },             // SRAM
            { NULL, 0 }                                        // 结束标记
        };
        vPortDefineHeapRegions(xHeapRegions);
    }
#endif
#endif
}

int main()
{
    SystemInit();

    SDRAMInit();

    HeapInit();

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


