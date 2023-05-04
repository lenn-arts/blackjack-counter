#ifndef _VGA_BALL_H
#define _VGA_BALL_H

#include <linux/ioctl.h>


/* ioctls and their arguments */
/* https://stackoverflow.com/questions/22496123/what-is-the-meaning-of-this-macro-iormy-macig-0-int */
#define MAGIC 'q'
#define CNN_WRITE_VAL _IOW(MAGIC, 1, int)
#define CNN_READ_VAL  _IOR(MAGIC, 2, int)

#endif