#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButtonOpen_clicked();

    void on_horizontalSliderVolume_valueChanged(int value);

    void on_pushButtonPlay_clicked();

    void on_pushButtonPause_clicked();

    void on_pushButtonStop_clicked();

    void on_horizontalSliderProgress_valueChanged(int value);

    void on_pushButtonAudioSpeed1_clicked();

    void on_pushButtonAudioSpeed2_clicked();


private:
    Ui::Widget *ui;

    QMediaPlayer* m_player;
};
#endif // WIDGET_H
