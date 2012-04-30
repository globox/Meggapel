#ifndef LECTEURMESSAGE_H
#define LECTEURMESSAGE_H

/*!
 *  \file lecteurmessage.h
 */
#include <QObject>
#include <QCoreApplication>
#include <phonon/audiooutput.h>
#include <phonon/mediaobject.h>
#include <phonon/backendcapabilities.h>
#include <QList>
#include <QDir>
#include <QMessageBox>

#include "tools.h"

class QTimer;

/*!
 *  \class LecteurMessage
 *  \brief lecteur de musique annexe
 *
 *  Le lecteur ne le lance que  lorsqu'il en recoit le signal lui indiquant le chemin
 *  du media a jouer
 */
class LecteurMessage : public QObject
{
    Q_OBJECT
public:
    explicit LecteurMessage(QObject *parent = 0);
    
private:
    Phonon::MediaObject *mediaObject;   /*!< lecteur de media */
    Phonon::MediaObject *metaInformationResolver;   /*!< information sur le media en cours */
    Phonon::AudioOutput *audioOutput;   /*!< sortie audio */
    QList<Phonon::MediaSource> sources; /*!< playlist */
    QStringList jingleMessage;                 /*!< Liste des jingles à diffuser*/

    void creerConnect();
    void play();

private slots:
    void stateChanged(Phonon::State newState, Phonon::State oldState);
    void metaStateChanged(Phonon::State newState, Phonon::State oldState);
    void aboutToFinish();
    void playlist(QString chemin, int indexJingle);
    void setMessageJingle();

signals:
    void startPlaylist(); /*!< signal prévenant le controleur du lancement de la playlist */
    void endPlaylist(); /*!< signal prévenant le controleur de l'arret de la playlist*/
};

#endif // LECTEURMESSAGE_H
