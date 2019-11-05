/*
 * /dev/dice skeleton code
 *
 * Wonsun Ahn <wahn@pitt.edu>
 *
 */
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>


/*
 * dice_read is the function called when a process calls read() on
 * /dev/dice.  It writes count bytes to the buffer passed in the
 * read() call.
 */

unsigned int sides;

unsigned char get_random_byte(int max){
	unsigned char roll;
	get_random_bytes(&roll,1);
	return roll%max;		
}

static ssize_t dice_read(struct file * file, char * buf,
			  size_t count, loff_t *ppos)
{
	char* data;	
	if(count == 0){
		return 0;
	}
	
	data = kmalloc(count,GFP_KERNEL);
	int i;	
	for(i = 0;i<count;i++){
		data[i] = get_random_byte(sides)+1;
	}

	if(copy_to_user(buf,data,count)){
		kfree(data);
	}

	return sizeof(data);
}



/*
 * dice_write is the function called when a process calls write() on
 * /dev/dice.  It reads count bytes from the buffer passed in the
 * write() call.
 */

static ssize_t dice_write(struct file * file, const char * buf,
			  size_t count, loff_t *ppos)
{
	sides = buf[count-1];
	return count;
}

/*
 * Now we have two file operations: read, write
 */

static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
        .write          = dice_write,
};

static struct miscdevice dice_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_fops
};

static int __init
dice_init(void)
{
	int ret;
	sides=6;
	/*
	 * Create the "dice" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_dev);
	if (ret)
                printk(KERN_ERR "Unable to register dice misc device\n");
        else {
                printk(KERN_ERR "Registered device under <MAJOR , MINOR><%d %d>\n" , 10, dice_dev.minor);
                printk(KERN_ERR "Create device file using: mknod /dev/dice c %d %d\n", 10, dice_dev.minor);
        }

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wonsun Ahn <wahn@pitt.edu>");
MODULE_DESCRIPTION("/dev/dice skeleton code");
MODULE_VERSION("dev");
