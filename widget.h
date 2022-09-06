#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QStringList>
#include <QVector>

#include "form.h"
#include "ui_form.h"

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QListWidgetItem>
#include <QMediaPlaylist>


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    Form *newvideo;

private slots:
    void networkReply(QNetworkReply* reply);
    void networkget(QNetworkReply* reply);



    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();



    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;

    QNetworkAccessManager *manager;
    QNetworkAccessManager *picmanager;
    QNetworkAccessManager *picmanager2;
    QString requestUrl;

    QString urlToken;
    QString pic;
    QString Bvid;

    QStringList picList;
    QStringList bvidList;
    QStringList urlList;

    QMediaPlayer* player;//播放器
    QMediaPlaylist *playlist;//播放列表
    QVideoWidget *videoWidget;//播放窗口

    QEventLoop loop;
};

#endif // WIDGET_H
