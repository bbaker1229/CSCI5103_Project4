#include "scullBuffer.h"

/* Parameters which can be set at load time */
int scull_major = SCULL_MAJOR;
int scull_minor = 0;
int scull_size = SCULL_SIZE;	/* number of scull Buffer items */

module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_size, int, S_IRUGO);

MODULE_AUTHOR("Piyush");
MODULE_LICENSE("Dual BSD/GPL");

static DECLARE_WAIT_QUEUE_HEAD(reader_wq);
static DECLARE_WAIT_QUEUE_HEAD(writer_wq);

struct scull_buffer scullBufferDevice;	/* instance of the scullBuffer structure */

/* file ops struct indicating which method is called for which io operation */
struct file_operations scullBuffer_fops = {
	.owner =    THIS_MODULE,
	.read =     scullBuffer_read,
	.write =    scullBuffer_write,
	.open =     scullBuffer_open,
	.release =  scullBuffer_release,
};

/*
 * Method used to allocate resources and set things up when the module
 * is being loaded. 
*/
int scull_init_module(void)
{
	int result = 0;
	dev_t dev = 0;
	
	/* first check if someone has passed a major number */
	if (scull_major) {
		dev = MKDEV(scull_major, scull_minor);
		result = register_chrdev_region(dev, SCULL_NR_DEVS, "scullBuffer");
	} else {
		/* we need a dynamically allocated major number..*/
		result = alloc_chrdev_region(&dev, scull_minor, SCULL_NR_DEVS,
				"scullBuffer");
		scull_major = MAJOR(dev);
	}
	if (result < 0) {
		printk(KERN_WARNING "scullBuffer: can't get major %d\n", scull_major);
		return result;
	}
	
	/* alloc space for the buffer (scull_size bytes) */
	scullBufferDevice.bufferPtr = kmalloc( scull_size , GFP_KERNEL);
	if(scullBufferDevice.bufferPtr == NULL)
	{
		scull_cleanup_module();
		return -ENOMEM;
	}

	/* Init the count vars */
	scullBufferDevice.readerCnt = 0;
	scullBufferDevice.writerCnt = 0;
	scullBufferDevice.start = 0;
	scullBufferDevice.end = 0;
	scullBufferDevice.itemCnt = 0;
		
	/* Initialize the semaphore*/
	sema_init(&scullBufferDevice.sem, 1);
	
	/* Finally, set up the c dev. Now we can start accepting calls! */
	scull_setup_cdev(&scullBufferDevice);
	printk(KERN_DEBUG "scullBuffer: Done with init module ready for requests buffer size= %d\n",scull_size);
	return result; 
}

/*
 * Set up the char_dev structure for this device.
 * inputs: dev: Pointer to the device struct which holds the cdev
 */
static void scull_setup_cdev(struct scull_buffer *dev)
{
	int err, devno = MKDEV(scull_major, scull_minor);
    
	cdev_init(&dev->cdev, &scullBuffer_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &scullBuffer_fops;
	err = cdev_add (&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding scullBuffer\n", err);
}

/*
 * Method used to cleanup the resources when the module is being unloaded
 * or when there was an initialization error
 */
void scull_cleanup_module(void)
{
	dev_t devno = MKDEV(scull_major, scull_minor);

	/* if buffer was allocated, get rid of it */
	if(scullBufferDevice.bufferPtr != NULL)
		kfree(scullBufferDevice.bufferPtr);
		
	/* Get rid of our char dev entries */
	cdev_del(&scullBufferDevice.cdev);

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, SCULL_NR_DEVS);
	
	printk(KERN_DEBUG "scullBuffer: Done with cleanup module \n");
}

module_init(scull_init_module);
module_exit(scull_cleanup_module);

/*
 * Implementation of the open system call
*/
int scullBuffer_open(struct inode *inode, struct file *filp)
{
	struct scull_buffer *dev;
	/* get and store the container scull_buffer */
	dev = container_of(inode->i_cdev, struct scull_buffer, cdev);
	filp->private_data = dev;

	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	if (filp->f_mode & FMODE_READ)
		dev->readerCnt++;
	if (filp->f_mode & FMODE_WRITE)
		dev->writerCnt++;

	printk("scullBuffer: open reader count: %d, writer count: %d\n", dev->readerCnt, dev->writerCnt);
		
	up(&dev->sem);
	return 0;
}

/* 
 * Implementation of the close system call
*/
int scullBuffer_release(struct inode *inode, struct file *filp)
{
	struct scull_buffer *dev = (struct scull_buffer *)filp->private_data;
	if (down_interruptible(&dev->sem) )
		return -ERESTARTSYS;
		
	if (filp->f_mode & FMODE_READ) {
		dev->readerCnt--;
		wake_up_interruptible(&writer_wq);
	}
	if (filp->f_mode & FMODE_WRITE) {
		dev->writerCnt--;
		wake_up_interruptible(&reader_wq);
	}

	printk("scullBuffer: close reader count: %d, writer count: %d\n", dev->readerCnt, dev->writerCnt);
		
	up(&dev->sem);
	return 0;
}

/*
 * Implementation of the read system call
*/
ssize_t scullBuffer_read(
	struct file *filp,
	char __user *buf,
	size_t count,
	loff_t *f_pos)
{
	struct scull_buffer *dev = (struct scull_buffer *)filp->private_data;
	ssize_t countRead = 0;
	
	/* get exclusive access */
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;	
		
	/* check if there is an item */
	while (dev->itemCnt == 0) {
		if (dev->writerCnt == 0) {
			goto out;
		}
		/* sleep while minding the semaphore */
		up(&dev->sem);
		printk(KERN_DEBUG "scullBuffer: reader sleeping\n");
		wait_event_interruptible(reader_wq, dev->itemCnt > 0);
		printk(KERN_DEBUG "scullBuffer: reader woken\n");
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;	
	}	
	
	printk(KERN_DEBUG "scullBuffer: reading %d bytes\n", dev->itemDataCnt[dev->start]);
	/* copy data to user space buffer */
	if (raw_copy_to_user(buf, dev->bufferPtr + dev->start * SCULL_ITEM_SIZE, dev->itemDataCnt[dev->start])) {
		countRead = -EFAULT;
		goto out;
	}

	countRead = dev->itemDataCnt[dev->start];
	dev->start = (dev->start + 1) % SCULL_ITEM_COUNT;
	dev->itemCnt--;	
	wake_up_interruptible(&writer_wq);

	printk(KERN_DEBUG "scullBuffer: read new item count= %d\n", dev->itemCnt);
	
	/* now we're done release the semaphore */
	out: 
	up(&dev->sem);
	return countRead;
}

/*
 * Implementation of the write system call
*/
ssize_t scullBuffer_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	int countWritten = 0;
	struct scull_buffer *dev = (struct scull_buffer *)filp->private_data;
	
	if (down_interruptible(&dev->sem))
		return -ERESTARTSYS;

	if (dev->itemCnt >= SCULL_ITEM_COUNT) {
		if (dev->readerCnt == 0) {
			goto out;
		}
		/* sleep while minding the semaphore */
		up(&dev->sem);
		printk(KERN_DEBUG "scullBuffer: writer sleeping\n");
		wait_event_interruptible(writer_wq, dev->itemCnt < SCULL_ITEM_COUNT);
		printk(KERN_DEBUG "scullBuffer: writer woken\n");
		if (down_interruptible(&dev->sem))
			return -ERESTARTSYS;	
	}	
	
	/* have we crossed the size of the buffer? */
	printk(KERN_DEBUG "scullBuffer: write called count= %d\n", (int)count);

	/* write till the end of buffer */
	if( count > SCULL_ITEM_SIZE)
		count = SCULL_ITEM_SIZE;
	printk(KERN_DEBUG "scullBuffer: writing %d bytes \n", (int)count);

	/* write data to the buffer */
	if (raw_copy_from_user(dev->bufferPtr + dev->end * SCULL_ITEM_SIZE, buf, count)) {
		countWritten = -EFAULT;
		goto out;
	}
	
	dev->itemDataCnt[dev->end] = count;
	dev->end = (dev->end + 1) % SCULL_ITEM_COUNT;
	dev->itemCnt++;
	countWritten = count;
	wake_up_interruptible(&reader_wq);

	printk(KERN_DEBUG "scullBuffer: new item count= %d\n", dev->itemCnt);

	out:
	up(&dev->sem);
	return countWritten;
}
