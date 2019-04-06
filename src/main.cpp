#include "mainwindow.h"
#include <QApplication>
#include <DApplication>
#include <DMainWindow>
#include <DWidgetUtil>
DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.loadTranslator();
    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.setTheme("light");
    a.setOrganizationName("pikachu");
    a.setApplicationName("krabby");
    a.setApplicationVersion("1.0");
    a.setProductIcon(QIcon(":/images/logo.png"));
    a.setProductName("Krabbby");
    a.setApplicationDescription("This is a simple type application.");

    MainWindow w;
    w.show();
    Dtk::Widget::moveToCenter(&w);
    return a.exec();
}
