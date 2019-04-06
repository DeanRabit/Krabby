#include "mainwindow.h"
#include "typewidget.h"
#include "bottom.h"
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent) :
    DMainWindow(parent)
{
    m_typeWidget = new TypeWidget();
    setCentralWidget(m_typeWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_typeWidget);
    auto bottom = new Bottom(this);
    mainLayout->addWidget(bottom);

    auto w = new QWidget();
    w->setLayout(mainLayout);
    w->resize(m_typeWidget->width(), height() + 100);
    setCentralWidget(w);
    resize(m_typeWidget->width(), height() + 100);

    connect(m_typeWidget, &TypeWidget::updateTime, bottom, &Bottom::updateTime);
    connect(m_typeWidget, &TypeWidget::updateAccuracy, bottom, &Bottom::updateAccuracy);
    connect(m_typeWidget, &TypeWidget::updateProgress, bottom, &Bottom::updateProgress);
    connect(m_typeWidget, &TypeWidget::updateSpeed, bottom, &Bottom::updateSpeed);
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    m_typeWidget->keyReleaseEvent(event);
}
