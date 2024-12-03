// SPDX-License-Identifier: DUAL BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Antipov O. <minigameapple@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_PARM_DESC(times, "Time to print 'Hello, world!'");
static unsigned int times = 1;
module_param(times, uint, 0444);

struct event_data {
	struct list_head list;
	ktime_t timestamp;
};

static LIST_HEAD(event_list);

static int __init hello_init(void)
{
	unsigned int i = 0;

	if (times == 0 || (times >= 5 && times <= 10)) {
		pr_warn("Warning. Continuing...\n");
	} else if (times > 10) {
		pr_err("Error. Module not loaded!\n");
		return -EINVAL;
	}

	for (i = 0; i < times; i++) {
		struct event_data *new_event = kmalloc(sizeof(*new_event),
			 GFP_KERNEL);

		new_event->timestamp = ktime_get();
		list_add_tail(&new_event->list, &event_list);
		pr_info("Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct event_data *event;
	struct list_head *pos, *q;

	list_for_each_safe(pos, q, &event_list) {
		event = list_entry(pos, struct event_data, list);
		pr_info("Event time: %lld ns\n", ktime_to_ns(event->timestamp));
		list_del(pos);
		kfree(event);
	}
}

module_init(hello_init);
module_exit(hello_exit);
