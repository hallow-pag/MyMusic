#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMediaPlaylist>
#include<QMediaPlayer>
#include"getmusic.h"
#include"lrc_widget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void get_lrcStrTime();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void listAdd(QString s);
    void lrcStrAdd(QString s);
    void urlListAdd(QString s);

    void geturls(QString url);
    void gettime(int t);
    void getimg(QByteArray img);
    void getname(QString name);//获取歌名.

private slots:
    void on_pushButton_clicked();


    void on_list_doubleClicked(const QModelIndex &index);


    void on_player_position_change(qint64);
    void on_mediaStatusChanged(QMediaPlayer::MediaStatus status);


    //void on_btn_clicked();

    void on_play_Btn_clicked();

    void on_next_btn_clicked();

    void on_close_btn_clicked();

    void on_min_btn_2_clicked();

    void on_max_btn_2_clicked();

    void on_last_btn_clicked();

    void on_horizontalSlider_2_sliderReleased();

    void on_btn_world_clicked();

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:


    QStringList songurls; //歌曲url

    int inde;  //当前播放序号
    bool issong; //是否有歌曲;
    bool songworld;
    int flg;//播放模式
    int bkgd;//背景

     bool bPressFlag;//窗体拖动
     QPoint beginDrag;

    GetMusic * Music;
    bool isplay;

    int music_position;
    int music_volume;
    int nowLrcListRow;
    int lrcListFirstAdd;

    QStringList lrcStr;
    QStringList lrcList;
    QStringList lrcTime;

    int lrcindex;
     bool nowIsFirst;

    //QMediaPlaylist * playList;
    QMediaPlayer * player;
    lrc_widget * myLrc_widget;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
