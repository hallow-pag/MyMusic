#ifndef LRC_WIDGET_H
#define LRC_WIDGET_H

#include <QWidget>
#include<QPaintEvent>
#include<QMouseEvent>

namespace Ui {
class lrc_widget;
}

class lrc_widget : public QWidget
{
    Q_OBJECT

public:
    explicit lrc_widget(QWidget *parent = 0);
    ~lrc_widget();
    void setLabel_1_text(QString text);
    void setLabel_2_text(QString text);
    void setProportion(int x,int y);
    void startLrc();
    void stopLrc();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void doTimer();
private:
    Ui::lrc_widget *ui;
    QFont font;
    QLinearGradient linearGradient;
    QLinearGradient maskLinearGradient;
    QPoint pressPoint;
    bool isPress;

    QString str1;
    QString str2;
    bool isFirst;

    QTimer* myTimer;
    float Proportion;
    double lrci;
};

#endif // LRC_WIDGET_H
