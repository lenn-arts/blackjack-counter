#ifndef _IMG_READER_H
#define _IMG_READER_H

#include <linux/ioctl.h>


/* ioctls and their arguments */
/* https://stackoverflow.com/questions/22496123/what-is-the-meaning-of-this-macro-iormy-macig-0-int */
#define MAGIC 'q'
#define IMG_WRITE _IOW(MAGIC, 1, int)
#define IMG_READ  _IOR(MAGIC, 2, int)


#endif