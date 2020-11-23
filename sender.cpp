#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include "base64.h"

using namespace std;
using namespace cv;

int main() {

  cout << "size" << SIZE;
  int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd < 0) {
    perror("shm_open()");
    return EXIT_FAILURE;
  }

  ftruncate(fd, SIZE);
  
  char *data = (char *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("sender mapped address: %p\n", data);
  
  string image_path = "./image.jpg";
  Mat img = imread(image_path, IMREAD_COLOR);

  std::vector<uchar> buf;
  cv::imencode(".jpg", img, buf);
  auto *enc_msg = reinterpret_cast<unsigned char*>(buf.data());
  std::string encoded = base64_encode(enc_msg, buf.size());
  
  for(int i=0; i<encoded.length(); i++){
    data[i] = encoded[i];
  }
  
  munmap(data, SIZE); //unmap the shared memory

  close(fd);

  return EXIT_SUCCESS;
}


