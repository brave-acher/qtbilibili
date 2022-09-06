#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/image/uninstallerIcon.ico"));

    newvideo = new Form;

    QVector <QLabel*> lab={ui->videoImg,ui->videoImg_2,ui->videoImg_3,ui->videoImg_4,ui->videoImg_5,ui->videoImg_6,
                          ui->videoImg_7,ui->videoImg_8,ui->videoImg_9,ui->videoImg_10,ui->videoImg_11,ui->videoImg_12};

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    videoWidget = new QVideoWidget;


    QNetworkRequest request;



    manager = new QNetworkAccessManager(this);
    picmanager = new QNetworkAccessManager(this);
    picmanager2 = new QNetworkAccessManager(this);

    connect(manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(networkReply(QNetworkReply*)));

    QString src2 = "https://api.bilibili.com/x/web-interface/popular?ps=12&pn=1";
    connect(picmanager,SIGNAL(finished(QNetworkReply*)),this, SLOT(networkget(QNetworkReply*)));
    request.setUrl(QUrl(src2));
    picmanager->get(request);
}

Widget::~Widget()
{
    delete ui;
    delete newvideo;
}

//获取视频流地址
void Widget::networkReply(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QJsonDocument json =  QJsonDocument::fromJson(data);
        QJsonObject object = json.object();
        QJsonObject dataObject = object.value("data").toObject();
        QJsonValue listValue = dataObject.value(QStringLiteral("list"));
        QJsonArray listValueArray = listValue.toArray();
        for(int i = 0;i<listValueArray.size();i++)
        {
            QJsonValue listArray = listValueArray.at(i);
            QJsonObject listObject = listArray.toObject();
            urlToken = listObject["url"].toString();
            urlList.append(urlToken);
            qDebug() << urlToken;
        }
    }
}

//获取封面图片地址和bvid
void Widget::networkget(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QJsonDocument json =  QJsonDocument::fromJson(data);
        QJsonObject object = json.object();
        QJsonObject dataObject = object.value("data").toObject();
        QJsonValue listValue = dataObject.value(QStringLiteral("list"));
        QJsonArray listValueArray = listValue.toArray();
        for(int i = 0;i<listValueArray.size();i++)
        {
            QJsonValue listArray = listValueArray.at(i);
            QJsonObject listObject = listArray.toObject();
            QString token = listObject["bvid"].toString();
            bvidList.append(token);
            //qDebug() << token;
            pic = listObject["pic"].toString();
            picList.append(pic);
            //qDebug() << pic;
        }
    }
}

//显示封面
void Widget::on_pushButton_clicked()
{
    QVector <QLabel*> lab={ui->videoImg,ui->videoImg_2,ui->videoImg_3,ui->videoImg_4,ui->videoImg_5,ui->videoImg_6,
                          ui->videoImg_7,ui->videoImg_8,ui->videoImg_9,ui->videoImg_10,ui->videoImg_11,ui->videoImg_12};
    for(int i = 0;i<picList.size();i++)
    {
        QString tmp = picList.at(i);
        QNetworkReply *rep = picmanager2->get(QNetworkRequest(QUrl(tmp)));
        connect(rep, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        QByteArray jpegData = rep->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(jpegData);
        QPixmap scaledPixmap = pixmap.scaled(220,120, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->videoImg->setPixmap(scaledPixmap);
        lab[i]->setPixmap(scaledPixmap);

    }
}



void Widget::on_pushButton_2_clicked()
{
    newvideo->show();
}




void Widget::on_pushButton_3_clicked()
{
    playlist->clear();
    newvideo->show();
    //videoWidget->show();
    player->setVideoOutput(videoWidget);

    videoWidget->installEventFilter(this);
    newvideo->ui->horizontalLayout->addWidget(videoWidget);
    //设置播放列表的播放模式：循环模式
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    //设置播放器的播放列表
    player->setPlaylist(playlist);

    int i = ui->spinBox->value();
    QString tmp = urlList.at(i);
    //qDebug() << tmp;
    playlist->addMedia(QMediaContent(QUrl(tmp)));
    player->play();
}


void Widget::on_pushButton_4_clicked()
{
    for(int i = 0;i<bvidList.size();i++)
    {
        QString src = "https://www.bilibili.com/video/";
        src += bvidList.at(i);
        QByteArray text = src.toLatin1();
        QByteArray by = text.toBase64();

        QString video_url = by;
        requestUrl = "https://www.mxnzp.com/api/bilibili/video?";
        requestUrl += QString("url=%1&").arg(video_url);
        requestUrl += QString("app_id=sqnmugmnspqditkh&");
        requestUrl += QString("app_secret=akw0SkhCOVoweWhWT3FQQ3dJNHgxUT09");

        QNetworkRequest request;
        request.setUrl(QUrl(requestUrl));
        manager->get(request);
    }
}
