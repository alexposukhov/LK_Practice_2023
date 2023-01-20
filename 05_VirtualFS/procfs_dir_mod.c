#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>


#define DIR_NAME	"my_dir"
#define ENT1_NAME	"my_ent1"
#define ENT2_NAME	"my_ent2"


static ssize_t myread(struct file *file, char __user *pbuf, size_t count, loff_t *ppos)
{
	static char buf[PAGE_SIZE];
	char *path;

	pr_info("mymodule: read file path = %s\n", file->f_path.dentry->d_iname);

	path = dentry_path_raw(file->f_path.dentry, buf, sizeof(buf));
	pr_info("mymodule: read file full path = %s\n", path);

	return 0;
}


static struct proc_ops myops =
{
	.proc_read = myread,
};

static struct proc_dir_entry *dir;
static struct proc_dir_entry *ent1, *ent2;


static int __init mymodule_init(void)
{
	dir = proc_mkdir(DIR_NAME, NULL);
	if (dir == NULL) {
		pr_err("mymodule: error creating procfs directory\n");
		return -ENOMEM;
	}

	ent1 = proc_create(ENT1_NAME, 0444, dir, &myops);
	if (ent1 == NULL) {
		pr_err("mymodule: error creating procfs entry 1\n");
		remove_proc_entry(DIR_NAME, NULL);
		return -ENOMEM;
	}

	ent2 = proc_create(ENT2_NAME, 0444, dir, &myops);
	if (ent2 == NULL) {
		pr_err("mymodule: error creating procfs entry 2\n");
		remove_proc_entry(ENT1_NAME, dir);
		remove_proc_entry(DIR_NAME, NULL);
		return -ENOMEM;
	}

	pr_info("mymodule: module loaded\n");
	return 0;
}

static void __exit mymodule_exit(void)
{
	proc_remove(dir);
//	remove_proc_entry(ENT2_NAME, dir);
//	remove_proc_entry(ENT1_NAME, dir);
//	remove_proc_entry(DIR_NAME, NULL);

	pr_info("mymodule: module exited\n");
}

module_init(mymodule_init);
module_exit(mymodule_exit);

MODULE_AUTHOR("Oleksandr Posukhov <Oleksandr.Posukhov@globallogic.com>");
MODULE_DESCRIPTION("Procfs Dir example");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
