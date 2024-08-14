#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int my_init(void) {
    printk("test");
    return 0;
}

static void my_exit(void) {
    printk("exiting");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LISENCE("GPL");
MODULE_AUTHOR("NOONE");
MODULE_DESCRIPTION("some desc");
