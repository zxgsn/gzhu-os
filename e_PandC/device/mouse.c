#include "interrupt.h"
#include "stdint.h"
#include "global.h"
#include "io.h"
#include "print.h"

#define MSE_BUF_PORT 0x60 // 鼠标buffer寄存器端口号为0x60

// static unsigned int nxt_scancode = 0;

static int count = 0;

// struct ioqueue mse_buf;	   // 定义mouse缓冲区

/* 鼠标中断处理程序 */
static void intr_mouse_handler(void)
{
    // put_str("BEGIN MOUSE HANDLER\n");
    uint16_t scancode = inb(MSE_BUF_PORT);
    if (scancode == 0xfa)
    {
        put_int(scancode);
        put_str("\n");
        return;
    }

    switch (count % 3)
    {
    case 2: // 定义键盘缓冲区
    {
        put_str("mouse y: ");
        put_int(scancode);
        put_str("\n");
        count++;
        break;
    }
    case 1:
    {
        put_str("mouse x: ");
        put_int(scancode);
        put_str("  ");
        count++;
        break;
    }
    case 0:
    {
        put_str("mouse key: ");
        put_int(scancode);
        put_str("  ");
        count++;
        break;
    }
    default:
    {
        break;
    }
    }
    // put_str("END MOUSE HANDLER\n");
}

/* 鼠标初始化 */
void mouse_init()
{
    put_str("mouse init start\n");
    // ioqueue_init(&mse_buf);
    // get fa
    // put_int(inb(MSE_BUF_PORT));
    register_handler(0x2c, intr_mouse_handler);

    put_str("mouse init done\n");
}