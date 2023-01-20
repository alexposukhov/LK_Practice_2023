#include <linux/module.h>
#include <linux/init.h>
#include <linux/hrtimer.h>
#include <linux/jiffies.h>

/* High resolution timer */
static struct hrtimer  my_hrtimer;
u64 start_time;

/**
 * @brief HR timer handler
 * 
 * @param timer 
 * @return enum hrtimer_restart 
 */
static enum hrtimer_restart test_hrtimer_handler(struct hrtimer *timer) {
	/* Get current time */
	u64 now_t = jiffies;
	printk("start_t - now_t = %u\n", jiffies_to_msecs(now_t - start_time));
   // hrtimer_start(&my_hrtimer, ms_to_ktime(5000), HRTIMER_MODE_REL);
    hrtimer_start(&my_hrtimer, ktime_set(1, 500000), HRTIMER_MODE_REL);
	return HRTIMER_NORESTART;
}

/**
 * @brief Func is called when module is loaded
 */
static int __init modInit(void) {
    printk("High res Timer module has started\n");
	/* Init of hrtimer */
	hrtimer_init(&my_hrtimer, CLOCK_REALTIME, HRTIMER_MODE_REL);
	my_hrtimer.function = &test_hrtimer_handler;
	start_time = jiffies;
	//hrtimer_start(&my_hrtimer, ms_to_ktime(64), HRTIMER_MODE_ABS);
    hrtimer_start(&my_hrtimer, ktime_set(1, 1000000), HRTIMER_MODE_REL);
    return 0;
}

/**
 * @brief Func is called when module removed
 */
static void __exit modExit(void) {
    printk("High res Timer module has removed\n");
    hrtimer_cancel(&my_hrtimer);
}

module_init(modInit);
module_exit(modExit);

/*Module information*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ME");
MODULE_DESCRIPTION("High resolution Timer example module");
MODULE_VERSION("0.1");
