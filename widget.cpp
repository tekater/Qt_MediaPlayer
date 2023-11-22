#include "widget.h"
#include "ui_widget.h"

#include <QStyle>
#include <QFileDialog>
#include <QDir>

Widget::Widget(QWidget *parent) // Конструктор
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //                 Button style:
    ui->pushButtonOpen->setIcon(style()->standardIcon(QStyle::SP_DriveCDIcon));         //      Open
    ui->pushButtonPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));   //      Prev
    ui->pushButtonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));           //      Play
    ui->pushButtonNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));    //      Next
    ui->pushButtonPause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));         //      Pause
    ui->pushButtonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));           //      Stop

    //                  Player Init
    m_player = new QMediaPlayer(this);                                                          // Передаём родителя
    m_player->setVolume(40);                                                                    // Громкость
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));  // Передача в LabelVolume
    ui->horizontalSliderVolume->setValue(m_player->volume());                                   // Передача в SliderVolume
}


Widget::~Widget() // Деструктор
{
    delete ui;
}


void Widget::on_pushButtonOpen_clicked()
{
    QString file = QFileDialog::getOpenFileName
            (
                this,                                       // Родительское окно
                tr("Open file"),                            // Заголовок окна диалога
                "C:\\ItProjects\\Qt\\MediaPlayer2\\music",  // Рабочий каталог
                tr("Audio files (*.mp3 *.flac)")            // Расширение ?
            );
    ui->labelComposition->setText(file);                    // Устанавливаем название песни в Label
    m_player->setMedia(QUrl::fromLocalFile(file));          // Устанавливаем саму песню в медиа
    //ui->labelDuration->setText(QString("Duration: ").append(QString::number(m_player->duration())));
}


void Widget::on_horizontalSliderVolume_valueChanged(int value)
{
    m_player->setVolume(value);                                                                 // Устанавливаем громкость со Slider'a
    ui->labelVolume->setText(QString("Volume: ").append(QString::number(m_player->volume())));  // Меняем текст Label
}


void Widget::on_pushButtonPlay_clicked()
{
    m_player->play();  // запуск песни

    int minutes = (m_player->duration() / 1000) / 60;
    int seconds = (m_player->duration() / 1000) % 60;

    ui->labelDuration->setText(QString("Duration: ").append(QString::number(minutes)).append(QString(":")).append(QString::number(seconds)));
    //ui->labelDuration->setText(QString("").append(QString::number(minutes)).append(QString(":")).append(QString::number(seconds)));

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
    // нет идей
    ui->labelProgress->setText(QString("Progress: ").append(QString::number(m_player->position())));
}



void Widget::on_pushButtonAudioSpeed1_clicked()
{
    m_player->setPlaybackRate(1);       // Дефолтная скорость воспроизведения
}


void Widget::on_pushButtonAudioSpeed2_clicked()
{
    m_player->setPlaybackRate(2);       // 2x скорость воспроизведения
}

