// errcode for system call or library call, then errno value.
//

#include <stdio.h> // for perror()
#include <string.h> // for strerror()
#include <fcntl.h>  // for open()
#include <unistd.h> // for close()
#include <errno.h> // for errno

int main() {
	int err = EINTR;
	// strerror() will return a static buffer, so you should copy it to you own buffer.
	char *msg = strerror(err);
	printf("strerror(EINTR): %s\n", msg);

	// strerror() implement is not sure if use the same static buffer, so, you should copy it to you own buffer.
	// but test, i find return message is not the same buffer.
	// if use thread safe you should use strerror_r() function.
	strerror(EAGAIN);
	printf("strerror(AGAIN): %s\n", msg);

	int fd;
	char *path = "abc.txt";
	int flags = O_RDWR;

	if ((fd = open(path, flags)) == -1) {
		int err = errno;
		printf("open failed: %s\n", strerror(err));
		perror("open");
		return -1;
	}

	if (close(fd) == -1) {
		int err = errno;
		printf("close failed: %s\n", strerror(err));
		return -1;
	}

	return 0;
}
