#include "mainwindow.h"
#include "typewidget.h"
MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent)
{
    m_typeWidget = new TypeWidget();
    setCentralWidget(m_typeWidget);
    resize(m_typeWidget->width(), height());
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_typeWidget->keyReleaseEvent(event);
}
