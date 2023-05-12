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
#include <stdlib.h>
#define _OPEN_SYS_ITOA_EXT

int cnn_fd;
int img_reader_fd;
char path_to_img[] = "./rgb888img";
char path_to_img_bin[] = "./rgb888imgbin";

// as long as numrows and cols is correct, it doesnt matter that last value is offset
void print_image(int *arr, int numrows, int numcols) {
  int *ptr;
  char* rbyte, *gbyte, *bbyte, *lsbyte;
  FILE* fp, *fpbin;
  char buffer[4];
  fp = fopen(path_to_img, "w");
  if (fp == NULL) {
      perror("Failed: ");
      return 1;
  }
  fpbin = fopen(path_to_img_bin, "w");
  if (fpbin == NULL) {
      perror("Failed: ");
      return 1;
  }
 
  for (int i = 0; i < numrows; i++) {
      for (int j = 0; j < numcols; j++) {
          //if (i % 12 != 0) continue;
          ptr = (int *) arr + i*numcols + j;
          //printf("Image at row %d, col %d:\n", i, j);
          /*lsbyte = (char *) ptr + 0;
          sprintf(buffer, "%d", *lsbyte);
          fputs(buffer, fp);
          fputs(" ", fp);*/
          //printf("Least significant byte is: %s ", buffer);
          rbyte = (char *) ptr + 3;
          sprintf(buffer, "%d", *rbyte);
          fputs(buffer, fp);
          fwrite(rbyte, sizeof(char), 1, fpbin);
          fputs(" ", fp);
          //printf("Red byte is: %s ", buffer);
          gbyte = (char *) ptr + 2;
          sprintf(buffer, "%d", *gbyte);
          fputs(buffer, fp);
          fwrite(gbyte, sizeof(char), 1, fpbin);
          fputs(" ", fp);
          //printf("Green byte is: %s ", buffer);
          bbyte = (char *) ptr + 1;
          sprintf(buffer, "%d", *bbyte);
          fputs(buffer, fp);
          fwrite(bbyte, sizeof(char), 1, fpbin);
          fputs("\n", fp);
          //printf("Blue byte is: %s \n", buffer);
      } 
  }
  
  fclose(fp);
  fclose(fpbin);
  
}
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

// inspired by online resources on array rotating
void reverse(int arr[], int start, int end)
{
  while (start <= end)
  {
    int temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;
    start++;
    end--;
  }
}
// Function to Rotate k elements to right
void rotatetoright(int arr[], int num_elem, int k)
{
  // Reverse first n-k elements
  reverse(arr, 0, num_elem - k - 1);
  // Reverse last k elements
  reverse(arr, num_elem - k, num_elem - 1);
  // Reverse whole array
  reverse(arr, 0, num_elem - 1);
}

/* Set the background color */
int* get_value(int mode)
{
  int* value_local = malloc((640 * 480 + 1) * sizeof(int)); // plus 1 for the offset bit
  char* rbyte, *gbyte, *bbyte, *lsbyte; // To check that the least significant byte is always 0
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
    lsbyte = value_local;
    int offset = *(value_local+640*480);
    printf("pointer is %d\n", offset);
    printf("Least significant byte is: %d\n", *lsbyte);
    rbyte = (char *) value_local + 3;
    printf("Red byte is: %d\n", *rbyte);
    gbyte = (char *) value_local + 2;
    printf("Green byte is: %d\n", *gbyte);
    bbyte = (char *) value_local + 1;
    printf("Blue byte is: %d\n", *bbyte);
    //printf("%d", value_local);

    //memmove(&lsbyte[k+1], &lsbyte[k], (numItems-k-1)*sizeof(double));
    //items[k] = value;
    //rotatetoright(lsbyte, 640*480, 500);
  }
  printf("Uget_value: ptr: %d \t ptr[0]: %d\n", value_local, *(value_local));
  
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

    /*int* ptr[numlines];
    for (i = 0; i < numlines; i++) {
      printf("Line number %d\n", i);
      ptr[i] = get_value(1);
    }
    for (i = 0; i < numlines; i++) {
      free(ptr[i]); 
    }*/
    usleep(2000000);
    int *ptr = get_value(1);
    print_image(ptr, 480, 640);
    printf("main: got value %d", ptr);
    printf("main: got value %d", ptr[0]);
    free(ptr);

    usleep(400000);
    //}

    printf("CNN Userspace program terminating\n");
    return 0;
}
