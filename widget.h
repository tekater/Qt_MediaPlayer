#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QStandardItemModel>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void load_playlist(QString filename);
    void save_playlist(QString filename);

    void load_cue_playlist(QString filename);

private slots:
    void on_pushButtonOpen_clicked();

    void on_horizontalSliderVolume_valueChanged(int value);

    void on_pushButtonPlay_clicked();

    void on_pushButtonPause_clicked();

    void on_pushButtonStop_clicked();

    void on_horizontalSliderProgress_valueChanged(int value);

    void on_pushButtonAudioSpeed1_clicked();

    void on_pushButtonAudioSpeed2_clicked();

    void on_position_changed(qint64 position);

    void on_duration_changed(qint64 duration);


    void on_horizontalSliderProgress_sliderMoved(int position);

    void on_pushButtonMute_clicked();

    void on_pushButtonPrev_clicked();

    void on_pushButtonNext_clicked();

    void on_tableViewPlaylist_doubleClicked(const QModelIndex &index);

    void on_pushButtonClear_clicked();

    void on_checkBoxLoop_stateChanged(int arg1);

    void on_checkBoxShuffle_stateChanged(int arg1);

    void on_pushButtonDelete_clicked();


private:
    Ui::Widget *ui;

    QMediaPlayer* m_player;

    bool muted;
    const char* DEFAULT_PLAYLIST = "C:\\ItProjects\\Qt\\build-MediaPlayer2-Desktop_Qt_5_12_12_MSVC2015_64bit-Debug\\debug\\playlist.m3u";

    QMediaPlaylist* m_playlist;
    QStandardItemModel* m_playlist_model;
};
#endif // WIDGET_H
