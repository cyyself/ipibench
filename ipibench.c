#include <linux/module.h>
#include <linux/types.h>
#include <linux/atomic.h>

MODULE_AUTHOR("Yangyu Chen <cyy@cyyself.name>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Benchmark IPI performance");

ktime_t ipibench_ts;
bool ipibench_waiting;
atomic_long_t ipibench_counter;

static void ipibench_do(void *info) {
    if (atomic_long_add_return(-1l, &ipibench_counter) == 0) {
        ipibench_waiting = false;
        ipibench_ts = ktime_sub(ktime_get(), ipibench_ts);
    }
}

static ssize_t ipibench_status_load(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    ssize_t size = 0;
    if (ipibench_waiting) {
        size += sprintf(buf+size, "Waiting for IPI\n");
    } else {
        size += sprintf(buf+size, "%d of CPUs online.\nIPI takes %llu ktime, %llu ns\n", 
                        num_online_cpus(), ipibench_ts, ktime_to_ns(ipibench_ts));
    }
    return size;
}

static ssize_t ipibench_status_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t cnt) {
    if (cnt >= 1) {
        if (!ipibench_waiting) {
            int nr_ipi;
            sscanf(buf, "%d", &nr_ipi);
            ipibench_counter.counter = 1ll* num_online_cpus() * nr_ipi;
            ipibench_waiting = true;
            ipibench_ts = ktime_get();
            for (int i=0;i<nr_ipi;i++) {
                on_each_cpu(ipibench_do, NULL, 0);
            }
        }
    }
    return cnt;
}

static struct kobj_attribute ipibench_status_query = __ATTR(status, 0664, ipibench_status_load, ipibench_status_store);

static struct attribute *ipibench_attrs[] = {
    &ipibench_status_query.attr,
    NULL,
};

static struct attribute_group ipibench_attr_group = {
    .attrs = ipibench_attrs,
};

struct kobject *ipibench_kobj;

static int __init ipibench_init(void) {
    ipibench_kobj = kobject_create_and_add("ipibench", kernel_kobj);
    int ret = 0;
    if (!(ipibench_kobj)) {
        ret = -ENOMEM;
        goto fail;
    }
    ret = sysfs_create_group(ipibench_kobj, &ipibench_attr_group);
    if (ret) kobject_put(ipibench_kobj);
fail:
    return ret;
}

static void __exit ipibench_exit(void) {
    sysfs_remove_group(ipibench_kobj, &ipibench_attr_group);
    kobject_put(ipibench_kobj);
}

module_init(ipibench_init);
module_exit(ipibench_exit);