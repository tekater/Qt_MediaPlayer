#include "widget.h"
#include "ui_widget.h"

#include <QStyle>
#include <QFileDialog>
#include <QDir>
#include <Qtime>

Widget::Widget(QWidget *parent) // Конструктор
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    muted = false;

    //                 Button style:
    ui->pushButtonOpen->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));         //      Open
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));   //      Prev
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));           //      Play
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));    //      Next
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));         //      Pause
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));           //      Stop
    ui->pushButtonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));         //      Mute

    //                  Player Init
    m_player = new QMediaPlayer(this);                                                          // Передаём родителя
    m_player->setVolume(40);                                                                    // Громкость
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));  // Передача в LabelVolume
    ui->horizontalSliderVolume->setValue(m_player->volume());                                   // Передача в SliderVolume

    connect(m_player,&QMediaPlayer::positionChanged,this,&Widget::on_position_changed);
    connect(m_player,&QMediaPlayer::durationChanged,this,&Widget::on_duration_changed);

    //              Playlist init:
    m_playlist_model = new QStandardItemModel(this);
    ui->tableViewPlaylist->setModel(m_playlist_model);
    m_playlist_model->setHorizontalHeaderLabels(QStringList() << "Audio track" << "File path" << "Duration");
    ui->tableViewPlaylist->hideColumn(1);
    ui->tableViewPlaylist->hideColumn(2);

    ui->tableViewPlaylist->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewPlaylist->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_playlist = new QMediaPlaylist(m_player);
    m_player->setPlaylist(m_playlist);

    //connect(m_player,&QMediaPlayer::)

}


Widget::~Widget() // Деструктор
{
    delete m_player;
    delete ui;
}


void Widget::on_pushButtonOpen_clicked()
{
    /*
    QString file = QFileDialog::getOpenFileName
            (
                this,                                       // Родительское окно
                tr("Open file"),                            // Заголовок окна диалога
                "C:\\ItProjects\\Qt\\MediaPlayer2\\music",  // Рабочий каталог
                tr("Audio files (*.mp3 *.flac)")            // Расширение ?322
            );
    ui->labelComposition->setText(file.split('/').last());  // Устанавливаем название песни в Label
    this->setWindowTitle(QString("Winamp - ").append(QString(file.split('/').last())));
    m_player->setMedia(QUrl::fromLocalFile(file));          // Устанавливаем саму песню в медиа
    //ui->labelDuration->setText(QString("Duration: ").append(QString::number(m_player->duration())));
    */

    QStringList files = QFileDialog::getOpenFileNames
            (
                this,
                "Open files",
                "C:\\ItProjects\\Qt\\MediaPlayer2\\music",
                "Audio files (*.mp3 *.flac)"
                );
    for(QString filesPath: files)
    {
        QList<QStandardItem*> items;

        items.append(new QStandardItem(QDir(filesPath).dirName()));
        items.append(new QStandardItem(filesPath));

        m_playlist_model->appendRow(items);
        m_playlist->addMedia(QUrl(filesPath));
    }
}


void Widget::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);                                                                 // Устанавливаем громкость со Slider'a
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));  // Меняем текст Label
}


void Widget::on_pushButtonPlay_clicked()
{
    m_player->play();  // запуск песни
    /*
    int minutes = (m_player->duration() / 1000) / 60;
    int seconds = (m_player->duration() / 1000) % 60;

    ui->labelDuration->setText(QString("Duration: ").append(QString::number(minutes)).append(QString(":")).append(QString::number(seconds)));
     - мой вариант добавления Duration
     */
}


void Widget::on_pushButtonPause_clicked()
{
    m_player->pause();      // Пауза
}


void Widget::on_pushButtonStop_clicked()
{
    m_player->stop();       // Стоп
}


void Widget::on_horizontalSliderProgress_valueChanged(int value)
{
    /*
    int minutes = (m_player->position() / 1000) / 60;
    int seconds = (m_player->position() / 1000) % 60;

    ui->labelProgress->setText(QString("Progress: ").append(QString::number(minutes)).append(QString(":")).append(QString::number(seconds)));
    - мой вариант с домашней работы
    */
}


void Widget::on_position_changed(qint64 position)
{
    ui->horizontalSliderProgress->setValue(position); // задаём прогресс бар по позиции
    QTime qt_position = QTime::fromMSecsSinceStartOfDay(position);
    ui->labelProgress->setText(QString("Position: ").append(qt_position.toString("mm:ss")));
}

void Widget::on_duration_changed(qint64 duration)
{
    ui->horizontalSliderProgress->setMaximum(duration);
    QTime qt_duration = QTime::fromMSecsSinceStartOfDay(duration);
    ui->labelDuration->setText(QString("Duration: ").append(qt_duration.toString("mm:ss")));
}

void Widget::on_pushButtonAudioSpeed1_clicked()
{
    m_player->setPlaybackRate(1);       // Дефолтная скорость воспроизведения
}


void Widget::on_pushButtonAudioSpeed2_clicked()
{
    m_player->setPlaybackRate(2);       // 2x скорость воспроизведения
}


void Widget::on_horizontalSliderProgress_sliderMoved(int position)
{
    m_player->setPosition(position);
}


void Widget::on_pushButtonMute_clicked()
{
    muted = !muted;
    m_player->setMuted(muted);
    ui->pushButtonMute->setIcon(style()->standardIcon(muted ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
}


void Widget::on_pushButtonPrev_clicked()
{
    m_playlist->previous();     //      Предыдущий
}


void Widget::on_pushButtonNext_clicked()
{
    m_playlist->next();     //      Следующий
}

