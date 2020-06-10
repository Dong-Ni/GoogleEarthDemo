#include <QApplication>
#include "GoogleEarthDemo.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GoogleEarthDemo demo;
	demo.show();

	return a.exec();
}
