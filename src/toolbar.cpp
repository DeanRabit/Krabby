#include "toolbar.h"
#include <QtWidgets>
Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/images/logo32.png"));

    layout->addWidget(iconLabel);
    layout->addStretch(1);
    layout->addWidget(new QLabel(tr("Krabby")));
    layout->addStretch(1);
}
