#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
DWIDGET_USE_NAMESPACE
class TypeWidget;
class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // QWidget interface
protected:
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    TypeWidget* m_typeWidget;
};

#endif // MAINWINDOW_H
