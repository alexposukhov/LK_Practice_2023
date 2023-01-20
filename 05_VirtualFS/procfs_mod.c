#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/init.h>
#include <linux/err.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

static char msg[PAGE_SIZE];
static ssize_t msg_size;

static ssize_t mywrite(struct file *file, const char __user *pbuf, size_t count, loff_t *ppos)
{
	ssize_t not_copied;

	pr_info("mymodule: mywrite: count=%ld\n", count);

	not_copied = copy_from_user(msg, pbuf, count);

	msg_size = count - not_copied;

	pr_info("mymodule: mywrite: msg_size=%ld\n", msg_size);
	return msg_size;
}

static ssize_t myread(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
	ssize_t num, not_copied;

	pr_info("mymodule: myread: count=%ld\n", count);

	num = min_t(ssize_t, msg_size, count);
	if (num) {
		not_copied = copy_to_user(pbuf, msg, num);
		num -= not_copied;
	}

	msg_size = 0; /* Indicate EOF on next read */

	pr_info("mymodule: myread: return=%ld\n", num);
	return num;
}


static struct proc_ops myops =
{
	.proc_read = myread,
	.proc_write = mywrite,
};
static struct proc_dir_entry *ent;

static int __init hello_init(void)
{
	int res = 0;
	ent = proc_create("my_procfs", 0666, NULL, &myops);
	if (ent == NULL) {
		pr_err("mymodule: error creating procfs entry\n");
		return -ENOMEM;
	}

	pr_info("mymodule: module loaded\n");
	return res;
}

static void __exit hello_exit(void)
{
	proc_remove(ent);
	pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Oleksandr Posukhov <alex.posukhov@gmail.com>");
MODULE_DESCRIPTION("ProcFS module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");