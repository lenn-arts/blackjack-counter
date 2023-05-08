/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "cnn_io.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int cnn_fd;

/* Read and print the background color */
void print_value(void) {
  int value_local;
  
  /* behavior of ioctl defined in vga_ball.c args(file, cmd, arg) */
  /* this writes the color of the background from &vla */
  if (ioctl(cnn_fd, CNN_READ_VAL, &value_local)) {
      perror("ioctl(CNN_WRITE_VAL) failed");
      goto end;
  }
  printf("%d", value_local);

  end:;
}

/* Set the background color */
int* get_value(void)
{
  int* value_local;
  if (ioctl(cnn_fd, CNN_READ_VAL, value_local)) {
      perror("ioctl(CNN_READ_VAL) failed");
      return -1;
  }
  printf("\nUget_value: ptr: %d \t ptr[0]: %d", value_local, *(value_local));
  //printf("%d", value_local);
  return value_local;
};

/* Send value to FPGA */
void set_value(const int *value_local, int target)
{
    switch (target)
    {
    case 0:
        if (ioctl(cnn_fd, CNN_WRITE_VAL, value_local)) {
            perror("ioctl(CNN_WRITE_VAL) failed");
            return;
        }
        break;
    
    default:
        break;
    }
    
    printf("\nUwrite_value: written:");
    printf("%d", (int) value_local);
};

int main()
{
    int value_local;
    static const char filename[] = "/dev/cnn_mem";

    int img[255];
    int i;  // Loop variable
    for (i = 0; i < 255; i=i+1) // Using for loop we are initializing
    {
        img[i] = i;
    }

    int weights_l1 = {0};
    int weights_l2 = {0};
    int weights_l3 = {0};


    printf("CNN Userspace program started\n");

    if ( (cnn_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
    }

    // printf("initial state: ");

    //for (i = 0 ; i < 24 ; i++) {
    long arr_ptr = (long) img; // &arr
    printf("\nU arr_ptr: %d", arr_ptr);
    set_value(arr_ptr, 0);
    int* ptr = get_value();
    printf("main: got value %d", ptr);
    printf("main: got value %d", ptr[0]);
    usleep(400000);
    //}

    printf("CNN Userspace program terminating\n");
    return 0;
}
