#include "widget.h"
#include <QtCore>
#include <QtWidgets>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    m_eachLineCharCount = 50;
    m_fontWidth = 24;
    m_lineHeight = 30;
    m_cursorShow = true;
    m_pageNum = 0;
    m_eachPageLineCount = 5;
    resize(m_eachLineCharCount * m_fontWidth + 10, height());
    QFile file(":/test.txt");
    file.open(QIODevice::ReadOnly);
    auto text = file.readAll();
    for(int i = 0; i * m_eachPageLineCount * m_eachLineCharCount < text.length(); i++ ) {
        int index = i * m_eachPageLineCount * m_eachLineCharCount;
        int len = m_eachPageLineCount * m_eachLineCharCount;
        m_pageText.append(text.mid(index, len));
    }
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
       this->update();
    });
    timer->setInterval(500);
    timer->start();
    m_input = "Go0d ";
    m_text = m_pageText[m_pageNum];
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    auto font = painter.font();
    font.setPixelSize(m_fontWidth);
    painter.setFont(font);
    for(int i = 0; i * m_eachLineCharCount < m_text.length(); i++) {
        for(int j = 0; j < m_eachLineCharCount; j++) {
            int index = i * m_eachLineCharCount + j;
            if (m_input.length() <= index) {
                auto textRect = genTargetTextRect(i, j);
                painter.drawText(textRect, Qt::AlignCenter, QString(m_text[index]));
            } else {
                if (m_text[index] == m_input[index]) {
                    drawCorrectChar(painter, i, j, m_text[index]);
                } else {
                    drawWrongChar(painter, i, j, m_text[index], m_input[index]);
                }
            }
        }
    }
    drawCursor(painter);
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Backspace:
        m_input.remove(m_input.length() -1, 1);
        break;
    default:
        auto key = event->text();
        if (key.isEmpty()) {
            // ignore
        } else {
            m_input.append(key);
            nextPageJudge();
        }
        break;
    }
    update();
}

QRect Widget::genTargetTextRect(int i, int j)
{
    int y = i * m_lineHeight * 2;
    int x = j * m_fontWidth;
    return QRect(x, y, m_fontWidth, m_lineHeight);
}

QRect Widget::genInputTextRect(int i, int j)
{
    int y = i * m_lineHeight * 2 + m_lineHeight;
    int x = j * m_fontWidth;
    return QRect(x, y, m_fontWidth, m_lineHeight);
}

QLineF Widget::genCursorLine(int i, int j)
{
    int y = i * m_lineHeight * 2 + m_lineHeight;
    int x = j * m_fontWidth;
    return QLineF(
                x + 1,
                y,
                x + 1,
                y + m_lineHeight - 2
                );
}

void Widget::drawWrongChar(QPainter& painter, int i, int j, QChar targetCh, QChar inputCh)
{
    painter.save();
    painter.setPen(Qt::red);
    auto targetRect = genTargetTextRect(i, j);
    painter.drawText(targetRect, Qt::AlignCenter, QString(targetCh));
    auto inputRect = genInputTextRect(i, j);
    painter.drawText(inputRect, Qt::AlignCenter, QString(inputCh));
    painter.restore();
}

void Widget::drawCorrectChar(QPainter& painter, int i, int j, QChar ch)
{
    painter.save();
    painter.setPen(Qt::gray);
    auto targetRect = genTargetTextRect(i, j);
    painter.drawText(targetRect, Qt::AlignCenter, QString(ch));
    auto inputRect = genInputTextRect(i, j);
    painter.drawText(inputRect, Qt::AlignCenter, QString(ch));
    painter.restore();
}

void Widget::drawCursor(QPainter &painter)
{
    if (m_cursorShow) {
        int i = m_input.length() / m_eachLineCharCount;
        int j = m_input.length() % m_eachLineCharCount;
        painter.drawLine(genCursorLine(i, j));
    }
    m_cursorShow = !m_cursorShow;
}

void Widget::nextPageJudge()
{
    if (m_pageNum == m_pageText.length()) {
        return;
    }
    if (m_input.length() == m_text.length()) {
        m_pageNum++;
        m_input.clear();
        m_text = m_pageText[m_pageNum];
    }
}
