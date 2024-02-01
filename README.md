# MinimalistDriver
 This repository hosts a minimalist, yet functional device driver designed for educational and illustrative purposes.

* What is a driver? What function does it perform?
* How does a driver differ from conventional software?
* What type of interface should the driver provide?

A driver is a specialized software component that facilitates communication between a computer's operating system (OS) and a hardware device. Is a specialized software component designed to enable communication between the operating system and hardware devices. Users in a UNIX system interact with drivers through system calls, device files and command-line utilities. The transparent handling of hardware devices by drivers and the abstraction provided by device files contribute to a standardized and consistent user experience when working with diverse hardware components in a UNIX environment. The type of interface that a driver should provide depends on the nature of the hardware like Device File, Control, API, and Event Notification for effective interaction with hardware.

# Preparing a driver.

For a driver to be useful, users must access it. In Linux, this is done through the file system: devices (physical or virtual) appear as files in the **/dev** directory. If we run **ls /dev -l**, we can see something like:

    $ ls -l /dev
    lrwxrwxrwx 1 root root 3 2010-10-08 20:00 cdrom -> sr0
    ...
    crw-rw-rw- 1 root root 1, 8 2010-10-08 20:00 random
    ...
    brw-rw---- 1 root disk 8, 0 2010-10-08 20:00 sda
    brw-rw---- 1 root disk 8, 1 2010-10-08 20:00 sda1
    ...

The first character of each line represents the file type:

* **l** is a symlink (symbolic link).
* **c** is a char device.
* **b** is a block device.
  
In addition, devices have a pair of associated numbers:
* **major**: associated with a specific driver (first number after the group).
* **minor**: identifies a specific device the driver manages (second number after the group).

This implies that a module needs to undergo certain procedural steps for seamless integration into the system:

1. Device initialization, particularly on programming char devices. **#include <linux/cdev.h>** and call the function accordingly.

        void cdev_init(struct cdev *cdev, struct file_operations *fops);

   The first parameter is a structure representing the device. The second structure, whose type is defined in **<linux/fs.h>**, allows us to define the operations forming the driver's interface.

2. Obtaining a major and a minor. It's advisable to request the kernel to dynamically allocate the major, which can be done using:

       int alloc_chrdev_region(dev_t *num, unsigned int firstminor, unsigned int count, char *name);

   The parameter **num** corresponds to the major, which will be returned after executing the function. The parameters **firstminor** and **count** can be set to 0 and 1, respectively, making the minor 0. **name** corresponds to a name for the device. Then, we need to assign the numbers to the device we previously initialized through:

       int cdev_add(struct cdev *dev, dev_t num, unsigned int count);

3. Create the corresponding nodes in the file system. For that, we should **#include <linux/device.h>**, define a static variable static struct class ***my_class**, and then, during the module initialization, execute:

       my_class = class_create(THIS_MODULE, DEVICE_NAME);
       device_create(mi_class, NULL, num, NULL, DEVICE_NAME);

    where DEVICE_NAME is the name by which the device will appear in the file system, and num is the major.

4. To deactivate the module, we must release these numbers by calling:

       void unregister_chrdev_region(dev_t num, unsigned int count);
       void cdev_del(struct cdev *dev);
       device_destroy(my_class, num);
       class_destroy(my_class);

5. What our module is missing are the operations that will implement the interface accessed by its users. This is where the file_operations structure we mentioned earlier comes into play:

       struct file_operations {
          struct module *owner;
          ...
          ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
          ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);
          ssize_t (*open) (struct inode *, struct file *);
          ssize_t (*release) (struct inode *, struct file *);
          ...
       }

   Each of its fields contains a pointer to the function that will be executed when certain actions are performed on the file from user mode. For example, the four detailed correspond to the *read, write, open, and close* operations. If some of the pointers are left as *NULL*, they will be associated with default operations.


# First simple driver: dev/null:
Our first driver will mimic the functionality of */dev/null** is a special device file in Unix-like operating systems. It is commonly referred to as the *null device*. In Unix terminology, it represents a file that discards all data written to it and reports that there is no data when read. Essentially, any data written to **/dev/null** is silently discarded and reading always returns an end-of-file (EOF) condition. The code for this driver is located in *src/null*

* Setup

      mkdir linux_drivers
      git clone https://github.com/nicoRomeroCuruchet/MinimalistDriver.git
      cd src/
      make                  # compile my_null driver
      insmod my_null.ko     # insert a module into the Linux Kernel





