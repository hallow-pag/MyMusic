 #include "getmusic.h"
#include<QTextCodec>
GetMusic::GetMusic(QObject *parent) : QObject(parent)
{
        network_manager = new QNetworkAccessManager();
        network_request = new QNetworkRequest();
        network_manager2 = new QNetworkAccessManager();
        network_request2 = new QNetworkRequest();

        network_managerimg = new QNetworkAccessManager();
        network_requestimg = new QNetworkRequest();

        connect(network_manager2, &QNetworkAccessManager::finished, this, &GetMusic::replyFinished2);
        connect(network_manager, &QNetworkAccessManager::finished, this, &GetMusic::replyFinished);

        connect(network_managerimg, &QNetworkAccessManager::finished, this, &GetMusic::replyFinishedimg);
}
///搜索歌曲响应.
void GetMusic::replyFinished(QNetworkReply *reply){
    //获取响应的信息，状态码为200表示正常
       QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

       //无错误返回
       if(reply->error() == QNetworkReply::NoError)
       {
           QByteArray bytes = reply->readAll();  //获取字节
           QString result(bytes);  //转化为字符串
           QTextCodec *t = QTextCodec::codecForName("UTF8");
           QByteArray ba = bytes;
           QString jsons =t->toUnicode(ba);
           qDebug()<<"begin";
           getplayid(jsons);  //开始解析歌曲数据
       }
       else
       {
           //处理错误
           qDebug()<<"处理错误";
       }
}
void GetMusic::replyFinishedimg(QNetworkReply *reply){
    //获取响应的信息，状态码为200表示正常
       QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

       //无错误返回
       if(reply->error() == QNetworkReply::NoError)
       {
           QByteArray bytes = reply->readAll();  //获取字节

                   emit addimg(bytes);
       }
       else
       {
           //处理错误
           qDebug()<<"处理错误";
       }
}
void GetMusic::replyFinished2(QNetworkReply *reply){

    //获取响应的信息，状态码为200表示正常
       QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

       //无错误返回
       if(reply->error() == QNetworkReply::NoError)
       {
           QByteArray bytes = reply->readAll();  //获取字节


           QString result(bytes);  //转化为字符串
           QTextCodec *t = QTextCodec::codecForName("UTF8");
           QByteArray ba = bytes;
           QString jsons =t->toUnicode(ba);
           getmusicinf(jsons);  //开始解析歌曲数据
       }
       else
       {
           //处理错误
           qDebug()<<"处理错误";
       }

}




void GetMusic::getplayid(QString json){
    qDebug()<<"搜索解析";
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(json.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        qDebug()<<"//beegin\n";
        QJsonObject rootObj = parse_doucment.object();
         if(rootObj.contains("data")){
             QJsonValue valuedata = rootObj.value("data");
              QJsonObject song = valuedata.toObject();
              ///拿到song josn格式
              if(song.contains("info")){
                  QJsonValue arrays_value = song.value("info");
                  QJsonArray heeloArray = arrays_value.toArray();

                  for (int i = 0; i < heeloArray.size(); i++){
                   QJsonValue helloArrayValue = heeloArray.at(i);
                      if (helloArrayValue.isObject())
                       {
                          QJsonObject valueArray = helloArrayValue.toObject();
                           QString has,alid;
                          if(valueArray.contains("album_id"))
                              {
                                  QJsonValue album_id = valueArray.take("album_id");


                                  if(album_id.isString())
                                  {
                                      alid = album_id.toString();
                                  }
                              }
                          if(valueArray.contains("hash"))
                              {
                                  QJsonValue hash = valueArray.take("hash");
                                  if(hash.isString())
                                  {
                                      has = hash.toString();       //hash
                                  }
                              }
                          //album_name
                          QString namer;
                          int paytype;
                          if(valueArray.contains("songname"))
                              {
                                  QJsonValue album_name = valueArray.take("songname");
                                  if(album_name.isString())
                                  {
                                   namer = album_name.toString() + "-";

                                  }
                              }
                          if(valueArray.contains("singername"))
                              {
                                  QJsonValue singername = valueArray.take("singername");
                                  if(singername.isString())
                                  {
                                   namer += singername.toString();

                                  }
                              }

                          if(valueArray.contains("pay_type"))
                              {
                                  QJsonValue pay_type = valueArray.take("pay_type");

                                      paytype = pay_type.toInt();
                                      if(pay_type==0){
                                          songnumber++;

                                          emit nameAdd(namer);

                                          QString reqsong = QString("https://www.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2&_=1497972864535").arg(has).arg(alid);
                                             emit addurls(reqsong);
                                      }

                              }

                       }
                   }


              }
         }
    }

}
void GetMusic::getmusicinf(QString json){
    //qDebug()<<json;
    //qDebug()<<"\n-----------------------------------------\n";
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(json.toUtf8(), &json_error);
    if(json_error.error == QJsonParseError::NoError){
        qDebug()<<"//beegin\n";
        QJsonObject rootObj = parse_doucment.object();
         if(rootObj.contains("data")){
             QJsonValue valuedata = rootObj.value("data");
              QJsonObject data = valuedata.toObject();
              //song_name
             // QString song;
//              if(data.contains("song_name")){
//                  QJsonValue song_name = data.value("song_name");
//                  song = song_name.toString();
//                  //qDebug()<<song_name;
//              }

              if(data.contains("play_url")){
                  QJsonValue play_url = data.value("play_url");

                  emit mediaAdd(play_url.toString());
              }
              if(data.contains("lyrics")){
                  QJsonValue lyrics = data.value("lyrics");
                  emit lrcAdd(lyrics.toString()==""?"没有歌词":lyrics.toString() );
              }
              if(data.contains("timelength")){
                  QJsonValue time = data.value("timelength");

                  emit addtime(time.toInt());
              }
              if(data.contains("img")){
                  QJsonValue img = data.value("img");
                  qDebug()<<"img:::"<<img.toString();
                  network_requestimg->setUrl(QUrl(img.toString()));
                  network_managerimg->get(*network_requestimg);
                  //emit addimg(img.toString());
              }
              if(data.contains("song_name")){
                   QJsonValue name = data.value("song_name");
                   emit addname(name.toString());


              }


         }}



}
void GetMusic::inftosend(QString reqsong){

      qDebug()<<"song_reqsong"<<reqsong;
   //  network_request2->setRawHeader("Host","180.97.33.218:80");
    network_request2->setRawHeader("cookie","kg_mid=381eaafcd47ff3f52d9dc3d4414149cb; kg_dfid=0hoRyA3Gxgm70oKN891nilAS; kg_dfid_collect=d41d8cd98f00b204e9800998ecf8427e; Hm_lvt_aedee6983d4cfc62f509129360d6bb3d=1587956519,1587977173");
      // network_request2->setRawHeader("Host","180.97.33.218:80");
    network_request2->setUrl(QUrl(reqsong));
    network_manager2->get(*network_request2);
}

void GetMusic::tosend(QString codes){
    songnumber = 0;
    QString KGAPISTR1 = QString("http://mobilecdn.kugou.com/api/v3/search/song?format=json&keyword="
    "%1&page=1&pagesize=30&showtype=1").arg(codes);
    //QString KGAPISTR1 = QString("http://songsearch.kugou.com/song_search_v2?keyword="
                           // "%1&page=3&pagesize=&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0").arg(codes);
    qDebug()<<KGAPISTR1;
    network_request->setUrl(QUrl(KGAPISTR1));
    network_manager->get(*network_request);
}
//http://mobilecdnbj.kugou.com/api/v3/rank/song?version=9108&ranktype=2&plat=0&pagesize=100&area_code=1&page=1&volid=35050&rankid=6666&with_res_tag=1
void GetMusic::hotever(){
    network_request->setUrl(QUrl("http://mobilecdnbj.kugou.com/api/v3/rank/song?version=9108&ranktype=2&plat=0&pagesize=100&area_code=1&page=1&volid=35050&rankid=6666&with_res_tag=1"));
    network_manager->get(*network_request);

}
int GetMusic::getmatchsong(){
    return songnumber;
}




