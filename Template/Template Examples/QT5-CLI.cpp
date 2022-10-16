#if 0

#@USER@ @DATE@ @EMAIL@

g++ -Wall $(pkg-config --cflags --libs Qt5Core) -fPIC "$0"||exit 1
./a.out "$@"
retval=$?
rm ./a.out
exit $retval

#endif

#include <QTextStream>

int main(int argc, char **argv)
{
	QTextStream(stdout) << "Hello, world!" << Qt::endl;
	return 0;
}