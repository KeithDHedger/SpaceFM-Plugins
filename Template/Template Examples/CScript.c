#if 0

#©@USER@ @DATE@ @EMAIL@

gcc -Wall "$0"||exit 1
./a.out "$@"
retval=$?
rm ./a.out
exit $retval

#endif

#include <stdio.h>

int main(int argc, char **argv)
{
	printf("Hello World!\n");
	return 0;
}