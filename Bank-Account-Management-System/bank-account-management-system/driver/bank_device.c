/* 
 * Educational Linux character-device driver. 
 * This module stores a small text buffer in kernel memory. 
 * 
 * It is intentionally separate from the C++ banking application. 
 */ 
 
#include <linux/cdev.h> 
#include <linux/device.h> 
#include <linux/fs.h> 
#include <linux/init.h> 
#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/uaccess.h> 
 
 
#define DEVICE_NAME "bank_device" 
#define BUFFER_SIZE 256 
 
static dev_t device_number; 
static struct cdev bank_cdev; 
static struct class *bank_class; 
static char device_buffer[BUFFER_SIZE]; 
static size_t data_size; 
 
static int bank_open(struct inode *inode, struct file *file) 
{ 
    pr_info("bank_device: opened\n"); 
    return 0; 
} 
 
static int bank_release(struct inode *inode, struct file *file) 
{ 
    pr_info("bank_device: closed\n"); 
    return 0; 
} 
 
static ssize_t bank_read(struct file *file, char __user *buffer, 
                         size_t length, loff_t *offset) 
{ 
    size_t available; 
 
    if (*offset >= data_size) 
        return 0; 
 
    available = data_size - *offset; 
    if (length > available) 
        length = available; 
 
    if (copy_to_user(buffer, device_buffer + *offset, length)) 
        return -EFAULT; 
 
    *offset += length; 
    return length; 
} 
 
static ssize_t bank_write(struct file *file, const char __user *buffer, 
                          size_t length, loff_t *offset) 
{ 
    if (length >= BUFFER_SIZE) 
        length = BUFFER_SIZE - 1; 
 
    if (copy_from_user(device_buffer, buffer, length)) 
        return -EFAULT; 
 
    device_buffer[length] = '\0'; 
    data_size = length; 
 
    pr_info("bank_device: received %zu bytes\n", length); 
    return length; 
} 
 
static const struct file_operations bank_fops = { 
    .owner = THIS_MODULE, 
    .open = bank_open, 
    .read = bank_read, 
    .write = bank_write, 
    .release = bank_release, 
}; 
 
static int __init bank_driver_init(void) 
{ 
    int result; 
 
    result = alloc_chrdev_region(&device_number, 0, 1, DEVICE_NAME); 
    if (result < 0) 
        return result; 
 
    cdev_init(&bank_cdev, &bank_fops); 
    result = cdev_add(&bank_cdev, device_number, 1); 
    if (result < 0) { 
        unregister_chrdev_region(device_number, 1); 
        return result; 
    } 
 
    bank_class = class_create(DEVICE_NAME); 
    if (IS_ERR(bank_class)) { 
        cdev_del(&bank_cdev); 
        unregister_chrdev_region(device_number, 1); 
        return PTR_ERR(bank_class); 
    } 
 
    if (IS_ERR(device_create(bank_class, NULL, device_number, NULL, DEVICE_NAME))) { 
        class_destroy(bank_class); 
        cdev_del(&bank_cdev); 
        unregister_chrdev_region(device_number, 1); 
        return -EINVAL; 
    } 
 
    pr_info("bank_device: driver loaded, major=%d\n", MAJOR(device_number)); 
    return 0; 
} 
 
static void __exit bank_driver_exit(void) 
{ 
    device_destroy(bank_class, device_number); 
    class_destroy(bank_class); 
    cdev_del(&bank_cdev); 
    unregister_chrdev_region(device_number, 1); 
 
    pr_info("bank_device: driver unloaded\n"); 
} 
 
module_init(bank_driver_init); 
module_exit(bank_driver_exit); 
 
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Student Project"); 
MODULE_DESCRIPTION("Simple educational character device for Bank Account Management System"); 