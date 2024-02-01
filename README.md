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

The first parameter is a structure representing the device. The second structure, whose type is defined in <linux/fs.h>, allows us to define the operations forming the driver's interface; we'll revisit this shortly. We need to define both structures as static at the beginning of our module.
  

