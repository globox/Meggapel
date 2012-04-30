#ifndef MUSIQUEWidget_H
#define MUSIQUEWidget_H

/*!
 *  \file musiquewidget.h
 *  \brief  Lecteur audio
 */

#include <QWidget>
#include <QCoreApplication>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/backendcapabilities.h>
#include <QList>
#include <QDir>
#include <QMessageBox>

#include "tools.h"

class QCursor;

namespace Ui {
    class MusiqueWidget;
}

/*! \class MusiqueWidget
 * \brief classe representant le lecteur audio
 *  La classe créer un lecteur audio
 */
class MusiqueWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusiqueWidget(QWidget *parent = 0);
    ~MusiqueWidget();
    Phonon::AudioOutput* getAudioOutput(){return audioOutput;}

private slots:
    void stateChanged(Phonon::State newState, Phonon::State oldState);
    void metaStateChanged(Phonon::State newState, Phonon::State oldState);
    void aboutToFinish();
    void play();
    void stop();
    void pause();

public slots:
    void playExterne();
    void playInterne();
    void playCDRom();
    void setRandom(bool r);

private:
    Ui::MusiqueWidget *ui;  /*!< Interface graphique généer avec Qt Designer */
    void creerConnect();
    void playlist();
    bool random;            /*!< état de la lecture random */
    bool initPlaylist;      /*!< état de la liste de lecture (initaliser ou non) */

    Phonon::MediaObject *mediaObject;   /*!< lecteur de media */
    Phonon::MediaObject *metaInformationResolver;   /*!< information sur le media en cours */
    Phonon::AudioOutput *audioOutput;   /*!< sortie audio */
    QList<Phonon::MediaSource> sources; /*!< playlist */
};

#endif // MUSIQUEWidget_H
