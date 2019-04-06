#include "typewidget.h"
#include "finishdialog.h"
#include <QtCore>
#include <QtWidgets>
#include <QMediaPlayer>
QString formatTime(int time) {
    QString str;
    int h = time / (60 * 60);
    time =  time % (60 * 60);
    if (h==0) {
        str.append("00");
    } else if (h < 10) {
        str.append("0").append(h);
    } else {
        str.append(h);
    }
    str.append(":");

    int m = time / 60;
    time =  time % 60;
    if (m==0) {
        str.append("00");
    } else if (m < 10) {
        str.append("0").append(h);
    } else {
        str.append(m);
    }
    str.append(":");

    if (time < 10) {
        str.append("0");
    }
    str.append(QString::number(time));
    return str;
}
TypeWidget::TypeWidget(QWidget *parent)
    : QWidget(parent)
{
    m_eachLineCharCount = 50;
    m_fontWidth = 24;
    m_lineHeight = 36;
    m_cursorShow = true;
    m_pageNum = 0;
    m_eachPageLineCount = 5;
    m_start = false;
    m_finish = false;
    m_time ++;
    m_audioPlayer = new QMediaPlayer(this);
    resize(m_eachLineCharCount * 14 + 10, height());
    QFile file(":/test.txt");
    file.open(QIODevice::ReadOnly);
    auto text = file.readAll();
    text = "abc";
    m_textTotal = text.length();
    m_inputTotal = 0;
    for(int i = 0; i * m_eachPageLineCount * m_eachLineCharCount < text.length(); i++ ) {
        int index = i * m_eachPageLineCount * m_eachLineCharCount;
        int len = m_eachPageLineCount * m_eachLineCharCount;
        m_pageText.append(text.mid(index, len));
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [=]() {
        if (m_start) {
            m_time ++;
            emit updateTime(formatTime(m_time / 4));
            int speed = 1.0 * m_inputTotal / m_time * 4 * 60;
            emit updateSpeed(speed);
        }
       this->update();
    });
    m_timer->setInterval(250);
    m_timer->start();
    m_input = "";
    m_text = m_pageText[m_pageNum];
}

TypeWidget::~TypeWidget()
{

}

void TypeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.save();
    auto font = painter.font();
    font.setPixelSize(m_fontWidth);
    font.setFamily("Noto Mono");
    painter.setFont(font);
    QFontMetrics fontMetrics(font);
    int x = 0;
    int y = 0;
    for(int i = 0; i<m_text.length(); i++) {
        int w = 0;
        if (m_input.length() <= i) {
            if (m_input.length() == i) {
                drawCursor(painter, x, y);
            }
            QString chStr = QString(m_text[i]);
            w = fontMetrics.width(chStr);
            auto rect = QRect(x, y, w, m_lineHeight);
            painter.drawText(rect, Qt::AlignCenter, chStr);
        } else {
            if (m_text[i] == m_input[i]) {
                QString chStr = QString(m_text[i]);
                w = fontMetrics.width(chStr);
                drawCorrectChar(painter, x, y, w, chStr);
            } else {
                QString chTarget = QString(m_text[i]);
                QString chInput = QString(m_input[i]);
                w = qMax(fontMetrics.width(chTarget), fontMetrics.width(chInput));
                drawWrongChar(painter, x, y, w, chTarget, chInput);
            }
        }
        if (x + 2 * w > width()) {
            x = 0;
            y += 2 * m_lineHeight;
        } else {
            x += w;
        }
    }

    painter.restore();
}

void TypeWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() & (Qt::ShiftModifier | Qt::ControlModifier)) {
        return;
    }
    if (m_finish) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Backspace:
        m_input.remove(m_input.length() -1, 1);
        if (m_inputTotal > 0) {
            m_inputTotal --;
        }
        break;
    case Qt::Key_Escape:
        qDebug() << "ESC";
        break;
    default:
        auto key = event->text();
        if (key.isEmpty()) {
            // ignore
        } else {
            m_start = true;
            m_input.append(key);
            m_inputTotal++;
            playAudio();
            int accuracy = 100 - 100 * (m_prevWrongCount + countWrongCh()) / (m_eachPageLineCount * m_eachLineCharCount * m_pageNum + m_input.length());
            emit updateAccuracy(accuracy);
            int progress = 100 * m_inputTotal / m_textTotal;
            emit updateProgress(progress);
            int speed = 1.0 * m_inputTotal / m_time * 2 * 60;
            emit updateSpeed(speed);
            nextPageJudge();
        }
        break;
    }
    update();
}



void TypeWidget::drawWrongChar(QPainter &painter, int x, int y, int w, QString targetCh, QString inputCh)
{
    painter.save();
    painter.setPen(Qt::red);
    auto targetRect = QRect(x, y, w, m_lineHeight);
    painter.drawText(targetRect, Qt::AlignCenter, QString(targetCh));
    auto inputRect = QRect(x, y + m_lineHeight, w, m_lineHeight);
    painter.drawText(inputRect, Qt::AlignCenter, QString(inputCh));
    painter.restore();
}

void TypeWidget::drawCorrectChar(QPainter &painter, int x, int y, int w, QString chStr)
{
    painter.save();
    painter.setPen(Qt::gray);
    auto targetRect = QRect(x, y, w, m_lineHeight);
    painter.drawText(targetRect, Qt::AlignCenter, chStr);
    auto inputRect = QRect(x, y + m_lineHeight, w, m_lineHeight);
    painter.drawText(inputRect, Qt::AlignCenter, chStr);
    painter.restore();
}

void TypeWidget::drawCursor(QPainter &painter, int x, int y)
{
    if (m_cursorShow) {
        auto line = QLineF(
                    x + 1,
                    y + m_lineHeight + 5,
                    x + 1,
                    y + m_lineHeight + m_lineHeight - 2 -5
                    );
        painter.drawLine(line);
    }
    m_cursorShow = !m_cursorShow;
}

void TypeWidget::nextPageJudge()
{
    if (m_input.length() == m_text.length()) {
        if (m_pageNum + 1 == m_pageText.length()) {
            QTimer::singleShot(100, this, &TypeWidget::finishTest);

            return;
        }
        m_pageNum++;
        m_prevWrongCount = countWrongCh();
        m_input.clear();
        m_text = m_pageText[m_pageNum];
    }
}

int TypeWidget::countWrongCh()
{
    int count = 0;
    for(int i=0;i < m_input.length();i++) {
        if (m_input[i] != m_text[i]) {
            count ++;
        }
    }
    return count;
}

void TypeWidget::playAudio()
{
    if(m_input[m_input.length() - 1] == m_text[m_input.length()-1]) {
        m_audioPlayer->setMedia(QUrl("qrc:/audio/type.wav"));
    } else {
        m_audioPlayer->setMedia(QUrl("qrc:/audio/error.wav"));
    }
    m_audioPlayer->play();
}

void TypeWidget::finishTest()
{
    m_timer->stop();
    m_finish = true;
    int speed = 1.0 * m_inputTotal / m_time * 4 * 60;
    int accuracy = 100 - 100 * (m_prevWrongCount + countWrongCh()) / (m_eachPageLineCount * m_eachLineCharCount * m_pageNum + m_input.length());
    int score = speed * accuracy / 100;
    FinishDialog d(
                score,
                formatTime(m_time / 4),
                m_inputTotal,
                speed,
                accuracy,
                this
                );
    d.exec();
}

