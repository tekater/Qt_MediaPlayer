#include "widget.h"
#include "ui_widget.h"

#include <QStyle>
#include <QFileDialog>
#include <QDir>
#include <Qtime>
#include <QToolButton>

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
    m_player->setVolume(10);                                                                    // Громкость
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

    //connect(ui->pushButtonPause,&QToolButton::clicked,m_player,  &QMediaPlayer::pause);
    //connect(ui->pushButtonStop, &QToolButton::clicked,m_player,  &QMediaPlayer::stop);
    //connect(ui->pushButtonPrev, &QToolButton::clicked,m_playlist,&QMediaPlaylist::previous);
    //connect(ui->pushButtonNext, &QToolButton::clicked,m_playlist,&QMediaPlaylist::next);


    load_playlist(DEFAULT_PLAYLIST);

    connect(m_playlist,&QMediaPlaylist::currentIndexChanged,ui->tableViewPlaylist,
            [this](int index)
    {
        ui->labelComposition->setText(m_playlist_model->data(m_playlist_model->index(index,0)).toString());
        this->setWindowTitle("Winamp - " + ui->labelComposition->text());
        ui->tableViewPlaylist->selectRow(index);
    }
            );
/*
    connect(ui->pushButtonDelete,&QToolButton::click,
            [this]()
    {
        QItemSelectionModel* selection = ui->tableViewPlaylist->selectionModel();
        QModelIndexList rows = selection->selectedRows();
        for(QModelIndexList::iterator it = rows.begin();it!= rows.end();++it)
        {
            if(m_playlist->removeMedia((it->row())))
            {
                m_playlist_model->removeRows(it->row(),1);
            }
        }
    }
            );
            */


    /*
    connect(ui->tableViewPlaylist,&QTableView::doubleClicked,m_playlist_model,
            [this](const QModelIndex& index)
    {
    m_playlist->setCurrentIndex(index.row());
    m_player->play();
    }
            );
    */

}


Widget::~Widget() // Деструктор
{
    save_playlist(DEFAULT_PLAYLIST);

    delete m_player;
    delete m_playlist;
    delete m_playlist_model;

    delete ui;
}


void Widget::load_playlist(QString filename)
{
    QString format = filename.split('.').last();
    m_playlist->load(QUrl::fromLocalFile(filename),format.toStdString().c_str());

    for(int i = 0;i<m_playlist->mediaCount();i++)
    {
        QMediaContent content = m_playlist->media(i);
        QString url = content.canonicalUrl().url();

        QList<QStandardItem*> items;

        items.append(new QStandardItem(QDir(url).dirName()));
        items.append(new QStandardItem(url));

        m_playlist_model->appendRow(items);
    }

}

void Widget::save_playlist(QString filename)
{
    // Clear file
    QFile file(filename);
    file.write("");
    file.resize(file.pos());

    // Save music from playlist
    QString format = filename.split('.').last();
    m_playlist->save(QUrl::fromLocalFile(filename),format.toStdString().c_str());
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


void Widget::on_pushButtonMute_clicked()// Mute
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


void Widget::on_tableViewPlaylist_doubleClicked(const QModelIndex &index)
{
    m_playlist->setCurrentIndex(index.row());
    m_player->play();
}


void Widget::on_pushButtonClear_clicked() // Clear
{
    m_playlist_model->clear();
    m_playlist->clear();
}

void Widget::on_pushButtonDelete_clicked() // Delete
{

}


void Widget::on_checkBoxLoop_stateChanged(int arg1) // Loop
{
    //m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    m_playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Sequential);

    if(ui->checkBoxLoop->checkState())
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
    if(ui->checkBoxShuffle->checkState())
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
}


void Widget::on_checkBoxShuffle_stateChanged(int arg1) // Shuffle
{
    //m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    m_playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Sequential);

    if(ui->checkBoxLoop->checkState())
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Loop);
    }
    if(ui->checkBoxShuffle->checkState())
    {
        m_playlist->setPlaybackMode(QMediaPlaylist::Random);
    }
}
