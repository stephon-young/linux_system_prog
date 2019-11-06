// for copy file from old file to new file.

#include <sys/stat.h>
#include <fcntl.h>
#include <header.h>

#ifndef BUF_SIZE
# define BUF_SIZE 1024
#endif 

int main(int argc, char *argv[]) {
  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s old-file new-file\n", argv[0]);

  int inputFd;
  inputFd = open(argv[1], O_RDONLY);
  if (inputFd == -1)
    errExit("open file %s", argv[1]);

  int outputFd;
  int openFlags = O_CREAT | O_WRONLY | O_TRUNC;
  mode_t filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  outputFd = open(argv[2], openFlags, filePerms);
  if (outputFd == -1)
    errExit("open file %s", argv[2]);

  char buf[BUF_SIZE];  
  int numRead;
  while ((numRead = read(inputFd, buf, sizeof(buf))) > 0)
    if (write(outputFd, buf, numRead) != numRead)
      errExit("couldn't write whole buffer");

  if (numRead < 0)
    errExit("read");

  if (close(inputFd) == -1)
    errExit("close input");
  if (close(outputFd) == -1)
    errExit("close output");

  exit(EXIT_SUCCESS);
}