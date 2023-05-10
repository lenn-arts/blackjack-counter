#include <linux/module.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "img_reader.h"

#define DRIVER_NAME "img_reader"

/* Device registers */
#define ADDR(x) (x+1)  // local helper that maps x (address) to right bit in target space (returns new address)

struct my_comp {
    struct resource res; /* Resource: our registers */
	void __iomem *virtbase; /* Where registers can be accessed in memory */
    int value;
} dev;

// TODO if desired to write images 
static void write_value(int val[], int max_addr){
    /* iowrite8(value, adress-to-write-to)*/
    //int addr = 0;
    //int[] val = *val_addr
    /* val_addr is pointer to array */
    //int arr;
    int addr_local = 0;
    //int max_addr = (sizeof(val)*8)/8;  // sizeof gives bytes
    pr_info(" max_addr %d", max_addr);
    pr_info(" val[0] %d", val[0]);
    pr_info(" val[1] t %d", (int) *(val+1));
    pr_info(" val[9] %d", val[9]);
    //iowrite8(val[0], dev.virtbase); // write 8 bits
    for (addr_local = 0; addr_local < max_addr; addr_local = addr_local +1){
        // arr[addr]
       iowrite16(val[addr_local], dev.virtbase + addr_local*2); // write 8 bits
       pr_info("written %d to %d (%d) with size %d", val[addr_local], addr_local, dev.virtbase + addr_local, sizeof(val[addr_local]));
    }
    pr_info("Kwrite_value: done iowrite");
};


// cannot return array so will return pointer to array
static int* read_img(int max_reads){
    /* ioread(adress-to-read-from)*/
    //static int out[max_addr-addr]; // doesnt work because dynamic size and static (needs static to retain mem addr outside the fucntion)
    int* out_ptr;
    if ( (out_ptr = kmalloc(sizeof(int)*(max_reads), GFP_USER)) == -1){
        pr_info("ERROR: could not allocate %d bytes in memory\n", sizeof(int)*(max_reads));
        } // dynamic allocation
    int i_read;
    for (i_read = 0; i_read < max_reads; i_read = i_read + 1){
        //*(out_ptr+addr_local) = ioread16(dev.virtbase+addr+addr_local);
        *(out_ptr+i_read) = ioread32(dev.virtbase+0); // here; 

        pr_info("Kread_value: from %d (%d) read %d (%b)", i_read, dev.virtbase, *(out_ptr+i_read), *(out_ptr+i_read));
    }
    int offset_zero = ioread32(dev.virtbase+4);
    pr_info("Kread_value: offset zero %d", offset_zero);
    pr_info("Kread_value: returning %d", out_ptr);
    return out_ptr;
};

static long img_reader_ioctl(struct file *f, unsigned int cmd, unsigned long val_arg)
{
    int size = 5000;
    // new array of same size as input
    // changes
    int (*arr_ptr)[size] = val_arg; // int (*arr_ptr)[10] = val_arg;
    //int (*a)[10] = l;
    pr_info("iooctl: val_local size %d", sizeof(*arr_ptr)/sizeof((*arr_ptr)[0]));
    int val_local[sizeof(*arr_ptr)/sizeof((*arr_ptr)[0])];

    switch(cmd){
        case IMG_WRITE:;
            /* copy_from_user(to, from, length) */
            /* copy from arg to vla (to dev.virtbase)*/
            //if (copy_from_user(&val_local, (int *) val_arg, sizeof(int)))
            // if (copy_from_user(val_local, (*arr_ptr), sizeof(int)))
            int i = 0;
            /*for (i = 0; i < 10; i = i+1){
                if (copy_from_user(val_local+i, arr_ptr+i, sizeof(val_local)))
                    return -EACCES;
                pr_info("ictl_write val_local[%d]: %d , %d \t arr_ptr %d, %d, %d", i, *(val_local+i), val_local[i], *(arr_ptr+i), arr_ptr[i]);
            }*/
            if (copy_from_user(val_local, arr_ptr, sizeof(val_local)))
                    return -EACCES;
            pr_info("ictl_write: done copying");
            write_value(val_local, size);
            pr_info("ioctl_write: done writing");
            break;

        case IMG_READ:;
            int* arr_ptr_local = read_img(size);
            pr_info("ictl_reading: done reading %d", arr_ptr_local);
            pr_info("ictl_reading: val_local[0] %d, %d", *(arr_ptr_local), (int) arr_ptr_local[0]);
            pr_info("\n");
            if (copy_to_user(arr_ptr, arr_ptr_local, sizeof(val_local)))
                return -EACCES;
            kfree(arr_ptr_local);
            break;
    
        default:
            return -EINVAL;
        }

    return 0;
};

/* 1) The operations our device knows how to do (turn ioctl into file operations) */
static const struct file_operations img_reader_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl = img_reader_ioctl
};

/* 2) Information about our device for the "misc" framework -- like a char dev */
static struct miscdevice img_reader_misc_device = {
	.minor		= MISC_DYNAMIC_MINOR,
	.name		= DRIVER_NAME,
	.fops		= &img_reader_fops,
};

/*
 * Initialization code: get resources (registers) and display
 * a welcome message
 */
// 3.1) setup/register device
static int __init img_reader_probe(struct platform_device *pdev)
{
    //int initial = 0;
    int initial[] = {0};
    long initial_ptr = (long) &initial;
	int ret;

	/* Register ourselves as a misc device: creates /dev/vga_ball */
	ret = misc_register(&img_reader_misc_device);

	/* Get the address of our registers from the device tree */
	// int of_address_to_resource(struct device_node *dev, int index, struct resource *r)¶
	ret = of_address_to_resource(pdev->dev.of_node, 0, &dev.res);
	if (ret) {
		ret = -ENOENT;
		goto out_deregister;
	}

	/* Make sure we can use these registers */
	if (request_mem_region(dev.res.start, resource_size(&dev.res),
			       DRIVER_NAME) == NULL) {
		ret = -EBUSY;
		goto out_deregister;
	}

	/* Arrange access to our registers */
	dev.virtbase = of_iomap(pdev->dev.of_node, 0);
	if (dev.virtbase == NULL) {
		ret = -ENOMEM;
		goto out_release_mem_region;
	}
        
	/* Set an initial color */
    pr_info("WRITING INITIAL");
    write_value(initial, 1);
    pr_info("DONE WRITING INITIAL");

	return 0;

// jumped to from before in case of error
out_release_mem_region:
	release_mem_region(dev.res.start, resource_size(&dev.res));
out_deregister:
	misc_deregister(&img_reader_misc_device);
	return ret;
}

/* Clean-up code: release resources */
// 3.2) remove
static int img_reader_remove(struct platform_device *pdev)
{
	iounmap(dev.virtbase);
	release_mem_region(dev.res.start, resource_size(&dev.res));
	misc_deregister(&img_reader_misc_device);
	return 0;
}


/* Which "compatible" string(s) to search for in the Device Tree */
#ifdef CONFIG_OF
static const struct of_device_id img_reader_of_match[] = {
	{ .compatible = "csee4840,img_reader-1.0" },
	{},
};
MODULE_DEVICE_TABLE(of, img_reader_of_match);
#endif



/* Information for registering ourselves as a "platform" driver */
static struct platform_driver img_reader_driver = {
	.driver	= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(img_reader_of_match),
	},
	.remove	= __exit_p(img_reader_remove),
};

/* Called when the module is loaded: set things up */
static int __init img_reader_init(void)
{
	pr_info(DRIVER_NAME ": init\n");
	return platform_driver_probe(&img_reader_driver, img_reader_probe);
}

/* Calball when the module is unloaded: release resources */
static void __exit img_reader_exit(void)
{
	platform_driver_unregister(&img_reader_driver);
	pr_info(DRIVER_NAME ": exit\n");
}

module_init(img_reader_init);
module_exit(img_reader_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lennart Schulze");
MODULE_DESCRIPTION("Img reader driver for FPGA to get camera data");


