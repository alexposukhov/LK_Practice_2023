#include <linux/module.h> // Core header for loading LKMs into the kernel
#include <linux/init.h> 
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/err.h>
#include <linux/string.h>

static ssize_t value = 0;

static ssize_t hello_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
    sprintf(buf, "%lu\n", value);
    pr_info("Got new value %lu\n", value);
    return strlen(buf);
}

static ssize_t hello_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	sscanf(buf, "%lu\n", &value);
    return count;
}

static struct kobj_attribute param_attribute =
	__ATTR(myparam_name, 0664, hello_show, hello_store);

static struct kobj_attribute list_attribute =
	__ATTR(my2ndparam_name, 0664, hello_show, hello_store);

static struct kobject *my_sys_kobj;

static int __init hello_init(void)
{
	int res = 0;

	my_sys_kobj = kobject_create_and_add("my_sysfs_ent", kernel_kobj);
	if (!my_sys_kobj)
		return -ENOMEM;
	res = sysfs_create_file(my_sys_kobj, &list_attribute.attr);
	if (res)
		kobject_put(my_sys_kobj);
	res = sysfs_create_file(my_sys_kobj, &param_attribute.attr);
	if (res)
		kobject_put(my_sys_kobj);
    pr_info("module Loaded\n");
	return res;
}

static void __exit hello_exit(void)
{
	kobject_del(my_sys_kobj);
	pr_info("module exited\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Oleksandr Posukhov <alex.posukhov@gmail.com>");
MODULE_DESCRIPTION("SysFS example module");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");