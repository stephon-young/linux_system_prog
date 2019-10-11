// glibc version get.
//

#include <stdio.h>
#include <gnu/libc-version.h> // for gnu_get_libc_version()
#include <unistd.h> // for confstr()

int main() {
	printf("get glibc version method:\n");

	int major = __GLIBC__;
	int minor = __GLIBC_MINOR__;
	printf("\t1. by __GLIBC__/__GLIBC_MINOR__ macro: %d.%d\n", major, minor);

	const char* ver = gnu_get_libc_version();
	printf("\t2. by gnu_get_libc_version() function: %s\n", ver);

	char buf[64] = {0};
	size_t len = confstr(_CS_GNU_LIBC_VERSION, buf, sizeof(buf));
	buf[len] = '\0';
	printf("\t3. by confstr() function: %s\n", buf);

	printf("\t4. by execute `/lib/libc.so.6(x86) or /lib/x86_64-linux-gnu/libc.so.6(x64)`!\n");
	printf("\t\t1. you can execute `ldd <bin-file> | grep libc` for find libc.so.6 path.!\n");
	return 0;
}
