#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include<iostream>

#include <opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include "base64.h"

using namespace cv;
using namespace std;

int main() {

  int fd = shm_open(NAME, O_RDONLY, 0666);
  if (fd < 0) {
    perror("shm_open()");
    return EXIT_FAILURE;
  }

  char *data =
      (char *)mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
  printf("receiver mapped address: %p\n", data);
  
     
  string dec_jpg =  base64_decode(data);
  std::vector<uchar> data_img(dec_jpg.begin(), dec_jpg.end());
  cv::Mat img = cv::imdecode(cv::Mat(data_img), 1);
  
  cv::namedWindow("My Image");
  // show the image on window
  cv::imshow("My Image", img);
  // wait key for 5000 ms
  cv::waitKey(5000);

  munmap(data, SIZE);

  close(fd);

  shm_unlink(NAME);

  return EXIT_SUCCESS;
}