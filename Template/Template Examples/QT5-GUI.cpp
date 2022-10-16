#if 0

#@USER@ @DATE@ @EMAIL@

g++ -Wall $(pkg-config --cflags --libs Qt5Core Qt5Widgets) -fPIC "$0"||exit 1
./a.out "$@"
retval=$?
rm ./a.out
exit $retval

#endif

#include <QtGui>
#include <QApplication>
#include <QLabel>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	QLabel label("Hello, world!");
	label.show();
	return app.exec();
}