#include <linux/module.h>
#include <linux/init.h>

/**
 * @brief Func is called when module is loaded
 */
static int __init modInit(void) {
    printk("Simple module has started\n");
    return 0;
}

/**
 * @brief Func is called when module removed
 */
static void __exit modExit(void) {
    printk("Simple module has removed\n");
}

module_init(modInit);
module_exit(modExit);

/*Module information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("Just a simple module");
