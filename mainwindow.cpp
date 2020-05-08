#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"getmusic.h"
#include<QMessageBox>
#include<QPixmap>
#include<QBitmap>
#include<QMediaMetaData>
#include<QRect>
#include<QDesktopWidget>
#include<QStringList>
#include <ctime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     Music = new GetMusic(this);
     //Music->hotever();

     //页面美化
    setWindowFlags(Qt::FramelessWindowHint);

    ui->play_Btn->setStyleSheet("QPushButton"
                                " {"
                                "background-image:url(:/image/btnimg/Pausezanting.png);"
                                "   background-repeat:no-repeat;"
                                "  background-position:center center;"
                                "   border:none;"

                                "    }"

                                "QPushButton:hover{"
                                "  background-repeat:no-repeat;"
                                "   background-position:center center;"
                                "background-image:url(:/image/btnimg/rPausezanting.png);"

                                " }"
                                " QPushButton:pressed{"
                                "background-repeat:no-repeat;"
                                " background-position:center center;"
                                " background-image:url(:/image/btnimg/Pausezanting.png);"

                                "}");

    //Init 初始化
    isplay = false;
    issong = false;
    songworld = false;
    flg = 0;
    bkgd = 0;
    setWindowIcon(QIcon(":/image/background/111.ico"));

    music_volume = 50;  //音量
    music_position = 0;//进度;

    ui->time_label->setText("00:00:00");
    ui->now_time_label->setText("00:00:00");
    ui->Lrc_list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //隐藏垂直滚动条
    ui->list->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->pushButton->setShortcut(Qt::Key_Enter);  //设置快捷键为enter键



    myLrc_widget = new lrc_widget();

     QDesktopWidget* pDesktopWidget = QApplication::desktop();
           //获取可用桌面大小
     QRect deskRect = QApplication::desktop()->availableGeometry();

     myLrc_widget->move(deskRect.width()/3,deskRect.height()-300);//调整歌词位置
    player = new QMediaPlayer;
    player->setVolume(music_volume);
    //设置音量
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(100);
    ui->horizontalSlider->setValue(music_volume);

    connect(Music,SIGNAL(nameAdd(QString)),this,SLOT(listAdd(QString)));  //歌曲列表增加
    connect(Music,SIGNAL(lrcAdd(QString)),this,SLOT(lrcStrAdd(QString))); /////歌词处理
    connect(Music,SIGNAL(mediaAdd(QString)),this,SLOT(urlListAdd(QString)));////添加歌曲

    connect(Music,SIGNAL(addurls(QString)),this,SLOT(geturls(QString))); //存放播放地址
    connect(Music,SIGNAL(addtime(int)),this,SLOT(gettime(int)));  //歌曲时长
    connect(Music,SIGNAL(addimg(QByteArray)),this,SLOT(getimg(QByteArray))); //图片
    connect(Music,SIGNAL(addname(QString)),this,SLOT(getname(QString))); //存放播放地址


    //connect(player,SIGNAL(metaDataAvailableChanged(bool)),this,SLOT(on_player_Data_change()));
    connect(player,SIGNAL(positionChanged(qint64)),this,SLOT(on_player_position_change(qint64))); //播放进度
    connect(player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SLOT(on_mediaStatusChanged(QMediaPlayer::MediaStatus)));


    nowIsFirst = true;

}

void MainWindow::getname(QString name){
    ui->Name->setText(name);
}
void MainWindow::geturls(QString url){
    songurls.append(url);

}
void MainWindow::listAdd(QString s){

    QListWidgetItem *add_item = new QListWidgetItem(QIcon(":/images/heart00.png"),s);
    ui->list->addItem(add_item);
}
///歌词处理
void MainWindow::lrcStrAdd(QString s){

     //lrcList.append(s);

    nowLrcListRow = 0;
     lrcTime.clear();
    lrcStr.clear();


    QStringList li = s.split("[");
    qDebug()<<"split"<<endl;
    foreach (QString temp, li) {
        //qDebug()<<temp;
        if(temp.contains("]"))
        {
            QString tempstr = temp.split("]").at(0);
               // qDebug()<<tempstr;
            //                qDebug()<<tempstr;
            //                lrcTime.append(tempstr);
            //                lrcTime.append(tempstr.split(".").at(0));
            QString ms = tempstr.split(".").at(0);
            //qDebug()<<ms;
           ms = QString("%1").arg(ms.toInt(),3,10,QLatin1Char('0'));
            //qDebug()<<"--"<<ms;
            lrcTime.append(tempstr.split(".").at(0)+":"+ms);
            if(QString(temp.split("]").at(1)).contains("\n"))
            {
                lrcStr.append(QString(temp.split("]").at(1)).remove("\n"));
            }
        }
    }
    if(lrcStr.count()>10)
    {
        for(int i = 0;i < 10;i++)
        {
            QListWidgetItem *add_item = new QListWidgetItem(ui->Lrc_list);
            add_item->setSizeHint(QSize(90, 67));
            add_item->setText(lrcStr.at(i));
            //设置item项中的文字位置
            add_item->setTextAlignment(Qt::AlignHCenter);
            //add_item->setSizeHint(QSize(ui->Lrc_list->width(),ui->Lrc_list->height()/10));
            ui->Lrc_list->insertItem(i,add_item);
        }
    }
    else
    {
        for(int i = 0;i < lrcStr.count();i++)
        {
            QListWidgetItem *add_item = new QListWidgetItem(ui->Lrc_list);
            add_item->setSizeHint(QSize(90, 67));
            add_item->setText(lrcStr.at(i));
            //设置item项中的文字位置
            add_item->setTextAlignment(Qt::AlignHCenter);
            //add_item->setSizeHint(QSize(ui->Lrc_list->width(),ui->Lrc_list->height()/10));
            ui->Lrc_list->insertItem(i,add_item);
        }
        for(int i = lrcStr.count();i < 10;i++)
        {
            QListWidgetItem *add_item = new QListWidgetItem(ui->Lrc_list);
            add_item->setSizeHint(QSize(90, 67));
            add_item->setText("");
            //设置item项中的文字位置
            add_item->setTextAlignment(Qt::AlignHCenter);
            //add_item->setSizeHint(QSize(ui->Lrc_list->width(),ui->Lrc_list->height()/10));
            ui->Lrc_list->insertItem(i,add_item);
        }
    }
    lrcListFirstAdd = 0;
    nowLrcListRow = 0;                                                                            
}
void MainWindow::urlListAdd(QString s){
    ////添加歌曲
    issong = true;
    player->setMedia(QUrl(s));
    player->play();
    on_play_Btn_clicked();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::gettime(int t){  //进度条


    int h = t/3600000;
    int m = (t%3600000)/60000;
    int s = ((t%3600000)%60000)/1000;
    int ms = ((t%3600000)%60000)%1000;
    QTime time(h,m,s,ms);     //音乐时长
    qDebug()<<"time ======"<<time;
    ui->time_label->setText(time.toString("hh:mm:ss"));
    ui->now_time_label->setText("00:00:00");

    ui->horizontalSlider_2->setMinimum(0);
    ui->horizontalSlider_2->setMaximum(t);
    music_position = 0;

}

void MainWindow::getimg(QByteArray img){


    QPixmap * P = new QPixmap;
    P->loadFromData(img);
    ui->music_name_label->setPixmap(*P);
    ui->music_name_label->setScaledContents(true);
    ui->music_name_label->show();
}

void MainWindow::on_player_position_change(qint64 position)         //播放歌曲进度改变，改变时间显示，歌词改变
{
    ui->horizontalSlider_2->setValue(position);
    music_position = position;

    int h = music_position/3600000;
    int m = (music_position%3600000)/60000;
    int s = ((music_position%3600000)%60000)/1000;
    int ms = ((music_position%3600000)%60000)%1000;
    QString timestr;
    QTime time(h,m,s,ms);
    ui->now_time_label->setText(time.toString("hh:mm:ss"));

    QTime time2(h,m,s,ms);
    timestr = time2.toString("mm:ss");

    for(int i = 0;i < lrcTime.count();i++)
    {
        if(time <= QTime::fromString(lrcTime.at(i),"mm:ss:zzz"))
        {
            // i 为下一次唱的歌词
            QString mm1;
            QString ss1;
            QString zzz1;
            QString mm2;
            QString ss2;
            QString zzz2;

            if(lrcindex != i || i == 0)
            {
                if(i == 0)
                {
                    myLrc_widget->setLabel_1_text(lrcStr.at(i));
                    myLrc_widget->setLabel_2_text(lrcStr.at(i + 1));
                }
                else
                {
                    if(nowIsFirst)
                    {
                        myLrc_widget->setLabel_2_text(lrcStr.at(i));
                        nowIsFirst = !nowIsFirst;
                    }
                    else
                    {
                        myLrc_widget->setLabel_1_text(lrcStr.at(i));
                        nowIsFirst = !nowIsFirst;
                    }
                }
                if(lrcindex <= 4)              //前5行   直接下来  歌词不变
                {
                    ui->Lrc_list->item(lrcindex)->setTextColor(Qt::red);
                    for(int j = 0;j < lrcindex;j++)
                    {
                        ui->Lrc_list->item(j)->setTextColor(Qt::black);
                    }
                }
                else            //超过5   通过改变每行显示的歌词来改变进度
                {
                    for(int j = 0;j < 4;j++)
                    {
                        ui->Lrc_list->item(j)->setText(lrcStr.at(lrcindex - 4 + j));
                    }
                    for(int j = 0;j < 6;j++)
                    {
                        if(lrcindex + j < lrcStr.count())
                        {
                            ui->Lrc_list->item(4+j)->setText(lrcStr.at(lrcindex + j));
                        }
                        else
                        {
                            ui->Lrc_list->item(4+j)->setText("");
                        }
                    }
                }
                lrcindex = i;
            }
            if(i != 0)
            {
                mm1 = QString(lrcTime.at(i)).split(":").at(0);
                ss1 = QString(lrcTime.at(i)).split(":").at(1);
                zzz1 = QString(lrcTime.at(i)).split(":").at(2);

                mm2 = QString(lrcTime.at(i - 1)).split(":").at(0);
                ss2 = QString(lrcTime.at(i - 1)).split(":").at(1);
                zzz2 = QString(lrcTime.at(i - 1)).split(":").at(2);
                int y = (mm1.toInt() - mm2.toInt())*60*1000 + (ss1.toInt() - ss2.toInt())*1000 + (zzz1.toInt() - zzz2.toInt());
                int x = (m - mm2.toInt())*60*1000 + (s - ss2.toInt())*1000 + (ms - zzz2.toInt());
                myLrc_widget->setProportion(x,y);
            }
            break;
        }
    }

}


void MainWindow::on_mediaStatusChanged(QMediaPlayer::MediaStatus status)            //播放完成自动跳转下一曲
{
    if(status == QMediaPlayer::EndOfMedia)
    {
        //播放完成
        if(flg%3==0){
            qDebug()<<"顺序播放";
            on_next_btn_clicked();
             isplay = false;
        }
        else if(flg%3==1){
            qDebug()<<"随机播放";
            qsrand(time(NULL));
            inde = qrand() % inde;
             isplay = false;
            Music->inftosend(songurls.at(inde));


        }
        else{
            qDebug()<<"循环播放";
            Music->inftosend(songurls.at(inde));
             isplay = false;
        }

    }
}





void MainWindow::on_pushButton_clicked()             ///搜索
{

    inde = 0;  //每次搜索初始化播放坐标。
    if(!ui->lineEdit->text().isEmpty())
        {
             songurls.clear();

            //player->setPlaylist(NULL);
            //lrcList.clear();
            //lrcStr.clear();
            //lrcTime.clear();
            ///////////playList->clear();
            ui->list->clear();
            //发送歌曲搜索请求

            Music->tosend(ui->lineEdit->text());
    //        WyySearch->search(ui->search_LineEdit->text());
    //        QqMusicSearch->search(ui->search_LineEdit->text());

            ///////player->setPlaylist(playList);



        }
   // ui->list->clear();

}


void MainWindow::on_list_doubleClicked(const QModelIndex &index)
{
        inde = index.row();
            //qDebug()<<"index::::"<<index.row();
            qDebug()<<songurls.at(inde);

        Music->inftosend(songurls.at(inde));

        player->setMedia(NULL);
       // playList->setCurrentIndex(index.row());
        isplay = false;
        //on_play_Btn_clicked();
        lrcindex = 0;

}



void MainWindow::on_play_Btn_clicked()
{
    if(!issong){
        QMessageBox::warning(this,"提示: ","播放列表为空!\n请先添加歌曲!");
        return;
    }
            isplay = !isplay;
            if(!isplay)
            {
                ui->play_Btn->setToolTip(QString("播放"));
                ui->play_Btn->setStyleSheet("QPushButton"
                                            " {"
                                            "background-image:url(:/image/btnimg/rPausezanting.png);"
                                            "   background-repeat:no-repeat;"
                                            "  background-position:center center;"
                                            "   border:none;"

                                            "    }"

                                            "QPushButton:hover{"
                                            "  background-repeat:no-repeat;"
                                            "   background-position:center center;"
                                            "background-image:url(:/image/btnimg/rPausezanting.png);"

                                            " }"
                                            " QPushButton:pressed{"
                                            "background-repeat:no-repeat;"
                                            " background-position:center center;"
                                            " background-image:url(:/image/btnimg/Pausezanting.png);"

                                            "}");
                player->pause();
               // myLrc_widget->stopLrc();
            }
            else
            {
                ui->play_Btn->setToolTip(QString("暂停"));
                ui->play_Btn->setStyleSheet("QPushButton"
                                            " {"
                                            "background-image:url(:/image/btnimg/Playbofang.png);"
                                            "   background-repeat:no-repeat;"
                                            "  background-position:center center;"
                                            "   border:none;"

                                            "    }"

                                            "QPushButton:hover{"
                                            "  background-repeat:no-repeat;"
                                            "   background-position:center center;"
                                            "background-image:url(:/image/btnimg/rPlaybofang.png);"

                                            " }"
                                            " QPushButton:pressed{"
                                            "background-repeat:no-repeat;"
                                            " background-position:center center;"
                                            " background-image:url(:/image/btnimg/Playbofang.png);"

                                            "}");
                player->setPosition(music_position);
                player->play();
                //myLrc_widget->startLrc();
            }
        }



void MainWindow::on_next_btn_clicked()
{
       inde = (inde+1)%Music->getmatchsong();
       Music->inftosend(songurls.at(inde));
        isplay = false;
        //on_play_Btn_clicked();

}
void MainWindow::on_last_btn_clicked()
{
    inde = (inde-1+Music->getmatchsong())%Music->getmatchsong();
    Music->inftosend(songurls.at(inde));
     isplay = false;
     //on_play_Btn_clicked();
}

void MainWindow::on_close_btn_clicked()
{
    QCoreApplication::quit();

}

void MainWindow::on_min_btn_2_clicked()
{
   // showMaximized();
}

void MainWindow::on_max_btn_2_clicked()
{
    showMinimized();
}


//调节 horizontalSlider
void MainWindow::on_horizontalSlider_2_sliderReleased()
{
        music_position = ui->horizontalSlider_2->value();  //歌词随着music_position变化改变
       player->setPosition(music_position);


}
///窗体拖动--------
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    bPressFlag = true;
    beginDrag = event->pos();
    QWidget::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    bPressFlag = false;
    QWidget::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(bPressFlag)
    {
        QPoint relaPos(QCursor::pos() - beginDrag);
        move(relaPos);
    }
    QWidget::mouseMoveEvent(event);
}
//-------------
//歌词显示
void MainWindow::on_btn_world_clicked()
{
    if(!songworld)
    myLrc_widget->show();
    else
        myLrc_widget->hide();
    songworld =!songworld;
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
     //player.setVolume(ui->horizontalSlider->value());
    music_volume = ui->horizontalSlider->value();
     player->setVolume(music_volume);
     if(music_volume==0){
         ui->pushButton_2->setStyleSheet("background-image: url(:/image/btnimg/Mutejingyin.png)32 repeat;"
                                         "background-color: rgba(255, 255, 255,0);"
                                        " background-repeat:no-repeat;"
                                         "border:none;"
                                         "background-position:center center;");
     }
     else{
         ui->pushButton_2->setStyleSheet("background-image: url(:/image/btnimg/Volumeyinliang.png)32 repeat;"
                                         "background-color: rgba(255, 255, 255,0);"
                                        " background-repeat:no-repeat;"
                                         "border:none;"
                                         "background-position:center center;");

     }
}

///播放模式
void MainWindow::on_pushButton_3_clicked()
{
    flg++;
    if((flg%3)==0){
        ui->pushButton_3->setStyleSheet("QPushButton"
                                        "{"

                                        "background: transparent;"
                                        "background-image: url(:/image/btnimg/ListCycleliebiaoxunhuan.png)32 repeat;"
                                        "background-repeat:no-repeat;"
                                        "background-position:center center;"
                                        "border:none;"
                                        " }"
                                        "QPushButton:hover{"
                                        "background-image: url(:/image/btnimg/rListCycleliebiaoxunhuan.png)32 repeat;"
                                       " background-repeat:no-repeat;"
                                        "background-position:center center;"
                                        "}" );
    }
    else if((flg%3)==1){
        ui->pushButton_3->setStyleSheet("QPushButton"
                                        "{"

                                        "background: transparent;"
                                        "background-image: url(:/image/btnimg/RandomPlaysuijibofang.png)32 repeat;"
                                        "background-repeat:no-repeat;"
                                        "background-position:center center;"
                                        "border:none;"
                                        " }"
                                        "QPushButton:hover{"

                                        "background-image: url(:/image/btnimg/rRandomPlaysuijibofang.png)32 repeat;"
                                       " background-repeat:no-repeat;"
                                        "background-position:center center;"
                                       " }");

    }
    else{
        ui->pushButton_3->setStyleSheet("QPushButton"
                                        "{"

                                        "background: transparent;"
                                        "background-image: url(:/image/btnimg/SingleCycledanquxunhuan.png)32 repeat;"
                                        "background-repeat:no-repeat;"
                                        "background-position:center center;"
                                        "border:none;"
                                        " }"
                                        "QPushButton:hover{"

                                        "background-image: url(:/image/btnimg/rSingleCycledanquxunhuan.png)32 repeat;"
                                       " background-repeat:no-repeat;"
                                        "background-position:center center;"
                                        "}" );
    }
    qDebug()<<"播放模式"<<flg;


}
//皮肤管理
void MainWindow::on_pushButton_4_clicked()
{
    bkgd++;
    bkgd %=5;
    switch (bkgd) {
    case 0:
         ui->frame->setStyleSheet("QFrame#frame{ border-radius:5px;border-image: url(:/image/background/bakeground.jpg)}");
        break;
    case 1:
         ui->frame->setStyleSheet("QFrame#frame{ border-radius:5px;border-image: url(:/image/background/1.jpg) }");
        break;
    case 2:
         ui->frame->setStyleSheet("QFrame#frame{ border-radius:5px;border-image: url(:/image/background/2.jpg) }");
        break;
    case 3:
         ui->frame->setStyleSheet("QFrame#frame{ border-radius:5px;border-image: url(:/image/background/3.jpg) }");
        break;
    case 4:
         ui->frame->setStyleSheet("QFrame#frame{ border-radius:5px;border-image: url(:/image/background/4.jpg) }");
        break;
    }

}
