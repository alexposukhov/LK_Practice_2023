#include <linux/module.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/delay.h>

static struct task_struct *thread1 = NULL;
static struct task_struct *thread2 = NULL;
static int t_num1 = 1;
static int t_num2 = 2;

/**
 * @brief Thread function
 *
 * @param arg - ptr to thread num 
 */
int thread_func(void* arg) {
    int i = 0;
    int tnum = 0;

    if (arg  == NULL) {
        printk("Error: wrong function argument\n");
        return -EINVAL;
    }
    tnum = *(int*) arg;

    while (!kthread_should_stop())
    {
        printk("Thread %d is executed. Counter value %d\n", tnum, i);
        i++;
        msleep(tnum * 1000);
    }
    printk("Thread %d has been stopped.\n", tnum); 
    return 0;
}

/**
 * @brief Func is called when module is loaded
 */
static int __init modInit(void) {
    printk("Thread module has started\n");
    thread1 = kthread_create(thread_func, &t_num1, "thread_1");
    if (IS_ERR(thread1)) {
        printk(KERN_ERR "Error thread 1 creation\n");
    } else {
        wake_up_process(thread1);
        printk("Thread1 created\n");
    }

    thread2 = kthread_run(thread_func, &t_num2, "thread_2");
    if (IS_ERR(thread2)) {
        printk(KERN_ERR "Error thread 2 creation\n");
        kthread_stop(thread1);
    } else {
        wake_up_process(thread2);
        printk("Thread2 created\n");
    }
    return 0;
}

/**
 * @brief Func is called when module removed
 */
static void __exit modExit(void) {
    printk("Thread module has been removed\n");
    kthread_stop(thread1);
    kthread_stop(thread2);
}

module_init(modInit);
module_exit(modExit);
/*Module information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("Just a simple module");
