# MyMusic

# 页面展示

![img](https://github.com/hallow-pag/MyMusic/raw/master/src/1.png)

![img](https://github.com/hallow-pag/MyMusic/raw/master/src/2.png)

# 组要功能

1、歌曲、歌手、专辑等关键词搜索。

2、播放、暂停、快进、下一曲、上一曲、音量调节。

3、展示歌曲封面图片、歌名、歌曲时长。

4、播放模式，单曲循环、随机播放、顺序播放。

5、歌词展示，歌词页面内展示、桌面歌词

6、更换皮肤。

7、付费歌曲过滤。

![img](https://github.com/hallow-pag/MyMusic/raw/master/src/7.png)



# 功能展示



3.2 播放器功能设计

1、关键词搜索。将关键词

请求地址：http://songsearch.kugou.com/song_search_v2?keyword=周杰伦&page=&pagesize=&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0

参数：（必要）Keyword 检索内容（歌手、专辑、歌单、歌名……）。

​      Page 页数。

​      Pagesize 返回数量。

通过向酷狗api发出请求得到相应的json，通过解析，得到歌曲名（SongName）歌手（SingerName），是否付费（PayType）。以及  AlbumID、FileHash 用于拼接出更加详细的歌曲信息

2、请求歌曲详细：

请求地址：https://www.kugou.com/yy/index.php?r=play/getdata&hash=3C3D93A5615FB42486CAB22024945264&album_id=1645030&_=1497972864535

参数hash：FileHash

  album_id：AlbumID

在返回得json数据中有歌曲的详细信息。歌词、播放地址、歌名、歌手、封面图片、时长、大小。

3、播放暂停、快进。

通过调用函数pause() 和play() 实现音乐得播放与暂停。

通过改变setVolume中的参数改变当前歌曲进度。

 

|      |                                                              |
| ---- | ------------------------------------------------------------ |
|      | ![](https://github.com/hallow-pag/MyMusic/raw/master/src/5.png) |


4、下一曲、上一曲。



Inde：当前播放编号。

Int Getmatchsong（）返回歌曲总数

通过计算得到上一首歌曲编号。然后求其进行播放。下一曲原理相同。

 

5、音量调节。

setVolume 设置音量。

 

|                                                              |      |
| ------------------------------------------------------------ | ---- |
| ![img](https://github.com/hallow-pag/MyMusic/raw/master/src/3.png)![img](https://github.com/hallow-pag/MyMusic/raw/master/src/4.png) |      |


  通过拖动滑条改变音量，当值为0时改变图标。



 

6、获取封面、歌词、歌名、歌手、歌曲时长。

通过信号函数，对歌曲请求进行解析时当解析好封面、歌词、歌名、歌手、歌曲时长。触发对应的信号函数将数据拿到。并进行相应的展示。

7、封面、歌词、时长的处理。

封面：

直接将获取的二进制数据画在播放器上。无需保存至本地。

歌词：

 

|      |                                                              |
| ---- | ------------------------------------------------------------ |
|      | ![img](https://github.com/hallow-pag/MyMusic/raw/master/src/6.png) |





拿到歌词后使用切片函数对[、]进行两次切片。将其时间与内容分开存储在stringlist中。

根据歌曲进度显示歌词。

![img](https://github.com/hallow-pag/MyMusic/raw/master/src/13.png)