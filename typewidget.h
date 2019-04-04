#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class TypeWidget : public QWidget
{
    Q_OBJECT

public:
    TypeWidget(QWidget *parent = 0);
    ~TypeWidget();


    // QWidget interface
    void keyReleaseEvent(QKeyEvent *event) override;
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawWrongChar(QPainter &painter, int x, int y, int w, QString targetCh, QString inputCh);
    void drawCorrectChar(QPainter &painter, int x, int y, int w, QString chStr);
    void drawCursor(QPainter &painter, int x, int y);
    void drawBottom(QPainter &painter);
    void nextPageJudge();
    int countWrongCh();
private:
    QString m_text;
    QString m_input;
    QStringList m_pageText;
    int m_eachLineCharCount;
    int m_fontWidth;
    int m_lineHeight;
    bool m_cursorShow;
    int m_pageNum;
    int m_eachPageLineCount;
    int m_time;
    int m_inputTotal;
    int m_textTotal;
    int m_prevWrongCount;
    bool m_start;
};

#endif // WIDGET_H
