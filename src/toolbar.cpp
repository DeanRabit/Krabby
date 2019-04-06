#include "toolbar.h"
#include <QtWidgets>
Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(QPixmap(":/images/logo32.png"));

    layout->addWidget(iconLabel);
    auto resetBtn = new QPushButton();
    resetBtn->setFlat(true);
    resetBtn->setCursor(Qt::PointingHandCursor);
    resetBtn->setIcon(QIcon(":/icon/reset.png"));
    resetBtn->setFocusPolicy(Qt::NoFocus);
    layout->addWidget(resetBtn);
    layout->addStretch(1);
    layout->addWidget(new QLabel(tr("Krabby")));
    layout->addStretch(1);

    connect(resetBtn, &QPushButton::clicked, this, &Toolbar::reset);
}
