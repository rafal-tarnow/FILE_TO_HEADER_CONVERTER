#include "widget.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Reyfel");
    QCoreApplication::setApplicationName("file_to_header_converter");

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
