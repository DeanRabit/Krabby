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
protected:
    void paintEvent(QPaintEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QRect genTargetTextRect(int i, int j);
    QRect genInputTextRect(int i, int j);
    QLineF genCursorLine(int i, int j);
    void drawWrongChar(QPainter &painter, int i, int j, QChar targetCh, QChar inputCh);
    void drawCorrectChar(QPainter &painter, int i, int j, QChar ch);
    void drawCursor(QPainter &painter);
    void nextPageJudge();
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
};

#endif // WIDGET_H
