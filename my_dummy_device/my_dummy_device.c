#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/module.h>

int major = 1;
int minor = 0;
int dev_count = 1;
dev_t device_number;

struct cdev dummy_cdev; /* представление символьного устройства в ядре*/

struct file_operations
    dummy_fops; /*мой драйвер ничего не делает, поэтому
                   объект стр-ры file_operations не инициализируется */

struct class* class_dummy;
struct device* device_dummy;

static int __init dummy_init(void) {
  int rv, err;

  /* динамическое выделение номера символьного устройства */
  rv = alloc_chrdev_region(&device_number, minor, dev_count,
                           "my_dummy_device_num");
  if (rv) {
    printk(KERN_WARNING
           "my_dummy_device: number cannot be allocated! error: %d\n",
           rv);
    return rv;
  } else {
    printk(KERN_INFO "my_dummy_device: device number <major>:<minor> = %d:%d\n",
           MAJOR(device_number), MINOR(device_number));
  }

  /* инициализация объекта структуры cdev объектом file_operations  */
  cdev_init(&dummy_cdev, &dummy_fops);

  dummy_cdev.owner =
      THIS_MODULE; /* чтобы драйвер не выгружался во время работы*/

  /* регистрация символьного устройства с полученным номером в ядре*/
  err = cdev_add(&dummy_cdev, device_number, dev_count);

  if (err) {
    printk(KERN_WARNING "my_dummy_device: cdev add error: %d!\n", err);
    return err;
  }

  /* создаем класс — директорию в /sys/class/ */
  class_dummy = class_create(THIS_MODULE, "class_dummy");

  /* заносим информацию об устройстве в этот класс */
  device_dummy =
      device_create(class_dummy, NULL, device_number, NULL, "my_dummy_device");

  printk(KERN_INFO "my_dummy_device: module loaded!\n");
  return 0;
}

static void __exit dummy_exit(void) {
  /* вызываем cleanup функции для объектов, созданных при загрузке драйвера */
  device_destroy(class_dummy, device_number);
  class_destroy(class_dummy);
  cdev_del(&dummy_cdev);
  unregister_chrdev_region(device_number, dev_count);
  printk(KERN_INFO "my_dummy_device: module is unloaded!\n");
}

module_init(dummy_init);
module_exit(dummy_exit);

MODULE_LICENSE("GPL");
