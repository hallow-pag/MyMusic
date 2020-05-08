#include "lrc_widget.h"
#include "ui_lrc_widget.h"
#include<QPainter>
#include<QDebug>
#include<QTimer>
#include<QFontMetrics>

lrc_widget::lrc_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::lrc_widget)
{
    ui->setupUi(this);
    //歌词的线性渐变填充
    linearGradient.setStart(0,10);
    linearGradient.setFinalStop(0,40);
    linearGradient.setColorAt(0.1,QColor(14,179,255));
    linearGradient.setColorAt(0.5,QColor(14,32,255));
    linearGradient.setColorAt(0.9,QColor(14,179,255));

    //遮罩的线性渐变填充
    maskLinearGradient.setStart(0,10);
    maskLinearGradient.setFinalStop(0,40);
    maskLinearGradient.setColorAt(0.1,QColor(14,179,255));
    maskLinearGradient.setColorAt(0.5,QColor(14,32,255));
    maskLinearGradient.setColorAt(0.9,QColor(14,179,255));

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(30);

//    fm = QFontMetrics(font);
    isPress = false;
    str1 = QString("");
    str2 = QString("感谢聆听... ");

    myTimer = new QTimer(this);
    connect(myTimer,SIGNAL(timeout()),this,SLOT(doTimer()));
    lrci = 0;
}

lrc_widget::~lrc_widget()
{
    delete ui;
}
void lrc_widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setFont(font);

    //先绘制底层文字，作为阴影
    painter.setPen(Qt::white);
    painter.drawText(0,0,QFontMetrics(font).boundingRect(str1).width(),this->height()/2,Qt::AlignLeft,str1);
    painter.drawText(0,this->height()/2,QFontMetrics(font).boundingRect(str2).width(),this->height()/2,Qt::AlignLeft,str2);

    //设置歌词遮罩
    if(isFirst)
    {
        painter.setPen(QPen(linearGradient,0));
        if(Proportion != 0)
        {
            painter.drawText(0,0,QFontMetrics(font).boundingRect(str1).width()*(lrci/double(Proportion)),this->height()/2,Qt::AlignLeft,str1);
        }
    }
    else
    {
        painter.setPen(QPen(linearGradient,0));
        if(Proportion != 0)
        {
            painter.drawText(0,this->height()/2,QFontMetrics(font).boundingRect(str2).width()*(lrci/double(Proportion)),this->height()/2,Qt::AlignLeft,str2);
        }
    }
}
void lrc_widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = true;
        pressPoint = event->pos();
    }
}
void lrc_widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isPress = false;
    }
}

void lrc_widget::mouseMoveEvent(QMouseEvent *event)
{
    if(isPress == true)
    {
        move(event->pos()- pressPoint + pos());
    }
}
void lrc_widget::setLabel_1_text(QString text)
{
    str1 = text;
    isFirst = false;
    update();
}
void lrc_widget::setLabel_2_text(QString text)
{
    str2 = text;
    isFirst = true;
    update();
}
void lrc_widget::setProportion(int x, int y)
{
    Proportion = y;
    lrci = x;
//    myTimer->start(1);
//    qDebug()<<Proportion;
    update();
}
void lrc_widget::doTimer()
{
    if(lrci < Proportion)
    {
        lrci+=1;
        update();
    }
    else
    {
        update();
    }
}
void lrc_widget::startLrc()
{
    myTimer->start(1);
}
void lrc_widget::stopLrc()
{
    myTimer->stop();
}
