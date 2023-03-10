#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static cycles_t cycles1 = 0;
static cycles_t cycles2 = 0;
static unsigned int i = 0;
static unsigned int j = 0;
static int context = 0;
const char tlet_data[] = "tasklet called";

//Bottom half handler
void my_tasklet_func_old(unsigned long data)
{
    context = in_softirq();
    j = jiffies;
    cycles2 = get_cycles();
    pr_info("Tasklet old type\n");
    pr_info("%010lld [%05d]: %s in context <%d> \n",
        (unsigned long long) cycles2, j, (char*) data, context);
    pr_info("in irq(): %s\n", in_irq()? "Y" : "N");
    pr_info("in softirq(): %s\n", in_softirq()? "Y" : "N");
    pr_info("in atomic(): %s\n", in_atomic()? "Y" : "N");
    pr_info("---------------------------------------\n");
    return;
}

DECLARE_TASKLET_OLD(my_taskl, my_tasklet_func_old);

void my_tasklet_func_new(struct tasklet_struct *t)
{
    context = in_softirq();
    j = jiffies;
    cycles2 = get_cycles();
    pr_info("Tasklet new type\n");
    pr_info("%010lld [%05d]: %p in context <%d> \n",
        (unsigned long long) cycles2, j, t, context);
    pr_info("in irq(): %s\n", in_irq()? "Y" : "N");
    pr_info("in softirq(): %s\n", in_softirq()? "Y" : "N");
    pr_info("in atomic(): %s\n", in_atomic()? "Y" : "N");
    pr_info("---------------------------------------\n");
    return;
}

DECLARE_TASKLET(my_taskl_new, my_tasklet_func_new);

static int __init hello_init(void)
{
	int res = 0;
    context = in_atomic();
    i = jiffies;
    cycles1 = get_cycles();
    tasklet_schedule(&my_taskl);
    tasklet_schedule(&my_taskl_new);
    pr_info("%010lld [%05d]: in context <%d> \n",
        (unsigned long long) cycles1, i, context);
	return res;
}

static void __exit hello_exit(void)
{
    tasklet_kill(&my_taskl);
    tasklet_kill(&my_taskl_new);
	pr_info("module exited\n");
    return;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Oleksandr Posukhov <alex.posukhov@gmail.com>");
MODULE_DESCRIPTION("Tasklet example module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");