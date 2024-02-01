#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h>

static ssize_t null_read(struct file *filp, char __user *data, size_t s, loff_t *off) {
  return 0;
}

static ssize_t null_write(struct file *filp, const char __user *data, size_t s, loff_t *off) {
  return s;
}

static struct file_operations null_operations = {
  .owner = THIS_MODULE,
  .read = null_read,
  .write = null_write,
};

static struct cdev null_device;
static dev_t major;
static struct class *null_class;

static int __init null_init(void) {

  cdev_init(&null_device, &null_operations);
  alloc_chrdev_region(&major, 0, 1, "my_null");
  cdev_add(&null_device, major, 1);

  null_class = class_create(THIS_MODULE, "my_null");
  device_create(null_class, NULL, major, NULL, "my_null");

  printk(KERN_ALERT "loading the  module 'my_null'...\n");
  return 0;

}

static void __exit null_exit(void) {

  device_destroy(null_class, major);
  class_destroy(null_class);

  unregister_chrdev_region(major, 1);
  cdev_del(&null_device);

  printk(KERN_ALERT "removing the module 'my_null'...\n");
}

module_init(null_init);
module_exit(null_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("nromero");
MODULE_DESCRIPTION("trying to mimic '/dev/null'");
