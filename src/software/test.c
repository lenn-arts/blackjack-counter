/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "cnn_io.h"
#include "img_reader.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int cnn_fd;
int img_reader_fd;
char *test;

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
int* get_value(int mode)
{
  int* value_local = malloc(640 * sizeof(int));
  char* lsbyte; // To check that the least significant byte is always 0
  if (mode==0){ // regular mode
    printf("get_val: READ_VAL");
    if (ioctl(cnn_fd, CNN_READ_VAL, value_local)) {
        perror("ioctl(CNN_READ_VAL) failed");
        return -1;
    }
  } else if (mode==1){ // read image mode
    printf("get_val: READ_IMG");
    if (ioctl(img_reader_fd, IMG_READ, value_local)) {
        perror("ioctl(IMG_READ) failed");
        return -1;
    }
  }
  printf("Uget_value: ptr: %d \t ptr[0]: %d\n", value_local, *(value_local));
  lsbyte = value_local;
  printf("Least significant byte is: %d\n", *lsbyte);
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
    static const char filename_cnn[] = "/dev/cnn_mem";
    static const char filename_img_reader[] = "/dev/img_reader";
    int numlines=480;
    int size=640;
    int i;  // Loop variable

    int weights_l1 = {0};
    int weights_l2 = {0};
    int weights_l3 = {0};


    printf("CNN Userspace program started\n");

    /*if ( (cnn_fd = open(filename_cnn, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", filename_cnn);
        return -1;
    }*/
    if ( (img_reader_fd = open(filename_img_reader, O_RDWR)) == -1) {
        fprintf(stderr, "could not open %s\n", filename_img_reader);
        return -1;
    }

    int* ptr[numlines];
    for (i = 0; i < numlines; i++) {
      printf("Line number %d\n", i);
      ptr[i] = get_value(1);
    }
    for (i = 0; i < numlines; i++) {
      free(ptr[i]); 
    }
    printf("main: got value %d", ptr[0]);
    printf("main: got value %d", ptr[0][0]);
    usleep(400000);
    //}

    printf("CNN Userspace program terminating\n");
    return 0;
}
