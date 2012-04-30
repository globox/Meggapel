#include "lecteurmessage.h"

#include <QDebug>

/*!
 *  \file lecteurmessage.cpp
 *  \class LecteurMessage
 *  \fn LecteurMessage::LecteurMessage(QObject *parent)
 *  \param[in] parent Parent de l'application ( toujours 0)
 *  \brief constructeur de la class LecteurMessage
 *
 */
LecteurMessage::LecteurMessage(QObject *parent) :
    QObject(parent)
{
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInformationResolver = new Phonon::MediaObject(this);
    creerConnect();
    Phonon::createPath(mediaObject, audioOutput);
    setMessageJingle();
}

/*!
 *  \fn void LecteurMessage::playlist(QString chemin)
 *  \brief Ajoute un media à la playlist
 *  \param[in] chemin Chemin vers le media à ajouter
 *
 *  Ajoute le media à la playlist si le media n'existe pas
 *  rien ne ce passe et le lecteur redonne la main au programme principale
 *  sinon lance la lecture de la playlist
 */
void LecteurMessage::playlist(QString chemin, int indexJingle)
{
    qDebug()<<"\nplay music :: "<<chemin<<"\n";

    QStringList splited = chemin.split("/");
    QString name = splited.last();
    splited.removeLast();
    QString path = splited.join("/");
    QDir repertoire = Tools::cd(path);
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList() << name);

    /*!
     *  Parcours le répertoire src/musique à la recherche de mp3
     */
    if(mediaObject->state() != Phonon::PlayingState) sources.clear();
    foreach(QFileInfo fileInfo, repertoire.entryInfoList())
    {
        Phonon::MediaSource jingle(Tools::absolutePathFile(jingleMessage.at(indexJingle)));
        sources.append(jingle);
        Phonon::MediaSource source(fileInfo.absoluteFilePath());
        sources.append(source);
    }

    if(sources.size()==0)
        emit endPlaylist();

    if(mediaObject->state() != Phonon::PlayingState && sources.size() != 0)
        play();
}

/*!
 *  \fn void LecteurMessage::play()
 *  \brief Lance la playist
 *
 */
void LecteurMessage::play()
{
    emit startPlaylist();
    metaInformationResolver->setCurrentSource(sources.at(0));
    mediaObject->setCurrentSource(sources.at(0));
    mediaObject->play();
}


/*!
 *  \fn void LecteurMessage::creerConnect()
 *  \brief Créer les connections
 *
 *  Crées les connections entre les différents widgets
 */
void LecteurMessage::creerConnect()
{
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
}


/*!
 *  \fn void LecteurMessage::stateChanged(Phonon::State newState, Phonon::State)
 *  \brief fait différente action en fonction de l'état du lecteure
 *
 *  Cette fonction est appelé quand le lecteur change d'état, en cas d'erreur
 *  dans la playlist un message s'affiche si la playlist est fini on envoi un signal
 *  au programme principal pour lui dire que la lecture est fini
 */
void LecteurMessage::stateChanged(Phonon::State newState, Phonon::State)
{
    switch (newState) {
    case Phonon::ErrorState:
        if (mediaObject->errorType() == Phonon::FatalError)
            QMessageBox::warning(0, tr("Fatal Error"), mediaObject->errorString());
        else
            QMessageBox::warning(0, tr("Error"), mediaObject->errorString());
        break;
    case Phonon::StoppedState:
        if(mediaObject->currentSource() == sources.last())
            emit endPlaylist();
        break;
    case Phonon::PlayingState:
        break;
    default:
        ;
    }
}

/*!
 *  \fn void LecteurMessage::metaStateChanged(Phonon::State newState, Phonon::State)
 *  \brief Action lors du chagement d'état du media en cours
 */
void LecteurMessage::metaStateChanged(Phonon::State newState, Phonon::State)
{
    /*!
     *  Si le media contient un erreur : afficher un dialog d'erreur
     */
    if (newState == Phonon::ErrorState) {
        QMessageBox::warning(0, tr("Error opening files"),
                             metaInformationResolver->errorString());
        while (!sources.isEmpty() &&
               !(sources.takeLast() == metaInformationResolver->currentSource())) {};
        return;
    }

    /*!
     *  Si le media est invalid : TODO
     */
    if (metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
        return;


    /*!
     *  Sinon on passe donc a la suivante
     */
    int index = sources.indexOf(metaInformationResolver->currentSource()) + 1;
    if (sources.size() > index) {
        metaInformationResolver->setCurrentSource(sources.at(index));
    }
}

/*!
 *  \fn void MusiqueWidget::aboutToFinish()
 *  \brief action à faire lorsque la musique ce fini
 */
void LecteurMessage::aboutToFinish()
{
    int index = sources.indexOf(mediaObject->currentSource()) + 1;
    if (sources.size() > index) {
        mediaObject->enqueue(sources.at(index));
    }
}

/*!
 *  \fn void ControleWidget::setMessageJingle()
 *  \brief  Créer la liste des jingle a lancer
 *
 *  Créer la liste des jingles à lancer
 */
void LecteurMessage::setMessageJingle()
{
    /*!
     * index 0 : avant ouverture
     * index 1 : avant fermeture
     * index 2 : avant promotion
     * index 3 : avant programmés
     * index 4 : avant envoi message
     */
    jingleMessage.clear();
    QString string;
    QFile fichier(Tools::absolutePathFile("src/jingle.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        for(int i=0; i<5;i++){
            string = flux.readLine();
            if(string != "") string = "src/jingle/" + string + ".wav";
            jingleMessage.append(string);
        }
        fichier.close();
    }
}
