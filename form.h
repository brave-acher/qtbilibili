#ifndef FORM_H
#define FORM_H

#include <QWidget>

#include <QMediaPlayer>
#include <QVideoWidget>
#include <QListWidgetItem>
#include <QMediaPlaylist>
#include <QFileDialog>

#include <QStringList>
#include <QKeyEvent>

class QMediaPlaylist;

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    bool eventFilter(QObject *watched,QEvent *event);

    Ui::Form *ui;
private slots:


    void on_pushButton_clicked();

    //void on_pushButton_3_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_addbtn_clicked();

    void on_delbtn_clicked();

private:


    int flag_start;

    QMediaPlayer* player;//播放器
    QMediaPlaylist *playlist;//播放列表
    QVideoWidget *videoWidget;//播放窗口

private slots:
    void updatePosition(qint64 position);

    void stateChanged(QMediaPlayer::State state);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void showError(QMediaPlayer::Error error);

    //void metaDataAvailableChanged(bool available);


    void on_clearBtn_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_verticalSlider_sliderMoved(int position);
    void on_verticalSlider_2_sliderMoved(int position);
    void on_fullBtn_clicked();

};

#endif // FORM_H
