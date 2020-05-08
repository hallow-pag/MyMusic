#ifndef GETMUSIC_H
#define GETMUSIC_H

#include <QObject>
#include<QMediaPlaylist>
#include <QMediaPlayer>
#include<QPaintEvent>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>
#include<QJsonValue>
#include<QJsonArray>
#include<QJsonDocument>
#include<QJsonObject>
#include<QPixmap>
struct songinf{
    QString name;
    QString photourl;
    QString playurl;
    int time;
};

class GetMusic : public QObject
{
    Q_OBJECT
public:

    explicit GetMusic(QObject *parent = nullptr);
    void getplayid(QString json); //解析搜索列表
    void getmusicinf(QString json); //解析每一首歌曲
    void tosend(QString codes);

    void inftosend(QString reqsong);
    void hotever();  //推荐歌曲
    void imgto();  //img
    int getmatchsong();//number

signals:
    void mediaAdd(QString playurl );
    void nameAdd(QString songname);
    void lrcAdd(QString lrcStr);

    void addtime(int t);
    void addurls(QString url);
    void addimg(QByteArray img);//添加图片。
    void addname(QString name);

public slots:
    void replyFinished(QNetworkReply *reply);
    void replyFinished2(QNetworkReply *reply);

    void replyFinishedimg(QNetworkReply *reply);
private:

    int songnumber;  //歌曲数

    QNetworkAccessManager* network_manager;
    QNetworkRequest* network_request;

    QNetworkAccessManager* network_manager2;
    QNetworkRequest* network_request2;

    //获取图片
    QNetworkAccessManager* network_managerimg;
    QNetworkRequest* network_requestimg;
};

#endif // GETMUSIC_H





















