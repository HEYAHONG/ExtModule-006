
#if !defined(__SDRAM_H__)
#define __SDRAM_H__

#if defined(__cplusplus)
extern "C"
{
#endif
#include "SWM341.h"

/*
 *  定义SDRAM大小(根据实际芯片型号修改)
 */
#define SDRAM_SIZE SDRAM_SIZE_8MB

/*
 * 初始化SDRAM
 */
void SDRAMInit(void);

/*
 * 获取SDRAM大小（字节）
 */
unsigned long SDRAMSize(void);

#if defined(__cplusplus)
}
#endif

#endif
