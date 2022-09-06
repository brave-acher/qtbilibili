#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    flag_start = 1;
    ui->pushButton->setStyleSheet("border-image: url(:/image/play.png);");
    ui->fullBtn->setStyleSheet("border-image: url(:/image/full.png);");

    this->setWindowTitle("VideoPlayer");
    this->setWindowIcon(QIcon(":/image/uninstallerIcon.ico"));

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    videoWidget = new QVideoWidget(this);
    //安装过滤器
    videoWidget->installEventFilter(this);
    ui->horizontalLayout->addWidget(videoWidget);//将播放视频的窗口添加到布局中
    //设置播放器显示的窗口
    player->setVideoOutput(videoWidget);
    //设置播放列表的播放模式：循环模式
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    //设置播放器的播放列表
    player->setPlaylist(playlist);

    connect(player, &QMediaPlayer::positionChanged,
                this, &Form::updatePosition);

    connect(player, &QMediaPlayer::stateChanged, this, &Form::stateChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged,
            this, &Form::mediaStatusChanged);
    connect(player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>
            (&QMediaPlayer::error), this, &Form::showError);

    //playlist->addMedia(QMediaContent(QUrl(urlToken)));
    //player->play();
}

Form::~Form()
{
    delete ui;
}

bool Form::eventFilter(QObject *watched,QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyevent = dynamic_cast<QKeyEvent *>(event);
        if(keyevent->key() == Qt::Key_Escape)
        {
            //恢复为子窗口
            videoWidget->setWindowFlag(Qt::SubWindow);
            //退出全屏，正常显示
            videoWidget->showNormal();
            videoWidget->setParent(this);
            ui->horizontalLayout->addWidget(videoWidget);
        }else if(keyevent->key() == Qt::Key_Space){ //按键触发事件 播放暂停
            if(flag_start)
            {
                ui->pushButton->setStyleSheet("border-image: url(:/image/pause.png);");
                player->play();
                flag_start = 0;
            }else{
                 ui->pushButton->setStyleSheet("border-image: url(:/image/play.png);");
                 player->pause();
                 flag_start = 1;
            }
        }else if(keyevent->key() == Qt::Key_Up){
            int vol_data = player->volume();
            player->setVolume(vol_data + 2);
            ui->vol->setText(QString("音量:%1").arg(vol_data + 2));
            ui->verticalSlider->setValue(vol_data + 2);
        }else if(keyevent->key() == Qt::Key_Down){
            int vol_data = player->volume();
            player->setVolume(vol_data - 2);
            ui->vol->setText(QString("音量:%1").arg(vol_data - 2));
            ui->verticalSlider->setValue(vol_data - 2);
        }
    }

    return QWidget::eventFilter(watched,event);
}
//按钮点击播放暂停
void Form::on_pushButton_clicked()
{
    if(flag_start)
    {
        ui->pushButton->setStyleSheet("border-image: url(:/image/pause.png);");
        player->play();
        flag_start = 0;
    }else{
         ui->pushButton->setStyleSheet("border-image: url(:/image/play.png);");
         player->pause();
         flag_start = 1;
    }
}

//视频进度条
void Form::on_horizontalSlider_sliderMoved(int position)
{
    player->setPosition(position * 1000);
}

// 更新滑块显示
void Form::updatePosition(qint64 position)
{
    ui->horizontalSlider->setMaximum(player->duration() / 1000);
    ui->horizontalSlider->setValue(position / 1000);
}

void Form::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
        ui->label->setText(tr("媒体未知状况！"));
        break;
    case QMediaPlayer::NoMedia:
        ui->label->setText(tr("没有媒体文件！"));
        break;
    case QMediaPlayer::BufferingMedia:
        ui->label->setText(tr("正在缓冲媒体文件！"));
        break;
    case QMediaPlayer::BufferedMedia:
        ui->label->setText(tr("媒体文件缓冲完成！"));
        break;
    case QMediaPlayer::LoadingMedia:
        ui->label->setText(tr("正在加载媒体！"));
        break;
    case QMediaPlayer::StalledMedia:
        ui->label->setText(tr("播放停滞！"));
        break;
    case QMediaPlayer::EndOfMedia:
        ui->label->setText(tr("播放结束！"));
        break;
    case QMediaPlayer::LoadedMedia:
        ui->label->setText(tr("媒体加载完成！"));
        break;
    case QMediaPlayer::InvalidMedia:
        ui->label->setText(tr("不可用的媒体文件！"));
        break;
    default: break;
    }
}

void Form::stateChanged(QMediaPlayer::State state)
{
    switch (state) {
    case QMediaPlayer::StoppedState:
        ui->label_2->setText(tr("停止状态！"));
        break;
    case QMediaPlayer::PlayingState:
        ui->label_2->setText(tr("播放状态！"));
        break;
    case QMediaPlayer::PausedState:
        ui->label_2->setText(tr("暂停状态！"));
        break;
    default: break;
    }
}

void Form::showError(QMediaPlayer::Error error)
{
    switch (error) {
    case QMediaPlayer::NoError:
        ui->label_3->setText(tr("没有错误！"));
        break;
    case QMediaPlayer::ResourceError:
        ui->label_3->setText(tr("媒体资源无法被解析！"));
        break;
    case QMediaPlayer::FormatError:
        ui->label_3->setText(tr("不支持该媒体格式！"));
        break;
    case QMediaPlayer::NetworkError:
        ui->label_3->setText(tr("发生了一个网络错误！"));
        break;
    case QMediaPlayer::AccessDeniedError:
        ui->label_3->setText(tr("没有播放权限！"));
        break;
    case QMediaPlayer::ServiceMissingError:
        ui->label_3->setText(tr("没有发现有效的播放服务！"));
        break;
    default: break;
    }
}
//获取元数据
/*
void Form::metaDataAvailableChanged(bool available)
{
    if(available) {
        QString title = player->metaData("Title").toString();
        QString author = player->metaData("Author").toString();
        this->setWindowTitle(title + "-" + author);
    }
}
*/
//添加播放列表
void Form::on_addbtn_clicked()
{
    QStringList video_files = QFileDialog::getOpenFileNames(this,"选择视频文件",QDir::currentPath(),"*.mp4 *.avi *.mkv");
    for(int i = 0; i < ui->listWidget->count();i++)
    {
        for(int j = 0; j < video_files.count(); j++)
        {
            if(ui->listWidget->item(i)->text() == video_files.at(j))
            {
                video_files.removeAt(j);
                break;
            }
        }
    }
    if(video_files.isEmpty())
        return;

    for(int i = 0;i<video_files.count();i++)
    {
        QString file = video_files.at(i);
        playlist->addMedia(QUrl::fromLocalFile(file));
    }
    ui->listWidget->addItems(video_files);
    ui->listWidget->setCurrentRow(0);
}
//删除播放列表
void Form::on_delbtn_clicked()
{
    int row = ui->listWidget->currentRow();

    QListWidgetItem *item = ui->listWidget->takeItem(row);
    if(item)
    {
        delete item;
    }

    playlist->removeMedia(row);
}
//清空播放列表
void Form::on_clearBtn_clicked()
{
    ui->listWidget->clear();
    playlist->clear();
}
//双击播放视频
void Form::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    int row = ui->listWidget->currentRow();
    playlist->setCurrentIndex(row);

    player->play();
    if(flag_start)
    {
        ui->pushButton->setStyleSheet("border-image: url(:/image/pause.png);");
        player->play();
        flag_start = 0;
    }else{
         ui->pushButton->setStyleSheet("border-image: url(:/image/play.png);");
         player->pause();
         flag_start = 1;
    }
}
//设置音量
void Form::on_verticalSlider_sliderMoved(int position)
{
    player->setVolume(position);
    ui->vol->setText(QString("音量:%1").arg(position));
}
//设置亮度
void Form::on_verticalSlider_2_sliderMoved(int position)
{
    videoWidget->setBrightness(position);
    ui->light->setText(QString("亮度:%1").arg(position));
}
//全屏
void Form::on_fullBtn_clicked()
{
    videoWidget->setWindowFlag(Qt::Window);
    videoWidget->showFullScreen();
}
