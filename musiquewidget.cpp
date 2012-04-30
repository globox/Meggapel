/*!
 *  \file musiquewidget.cpp
 *  \headerfile musiquewidget.h
 */
#include "musiquewidget.h"
#include "ui_musiquewidget.h"
#include <QStyle>
#include <QDebug>

/*!
 *  \fn MusiqueWidget::MusiqueWidget(QWidget *parent)
 *  \brief Constructeur de la classe MusiqueWidget
 *  \param[in]  parent Parent de l'application
 *              soit la zone qui lui est réservé dans la MainWindow
 */
MusiqueWidget::MusiqueWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MusiqueWidget), random(false), initPlaylist(true)
{
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInformationResolver = new Phonon::MediaObject(this);

    ui->setupUi(this);
    QStyle *style = QApplication::style();
    ui->playButton->setIcon(style->standardIcon(QStyle::SP_MediaPlay));
    ui->stopButton->setIcon(style->standardIcon(QStyle::SP_MediaStop));
    ui->stopButton->setEnabled(false);
    Phonon::createPath(mediaObject, audioOutput);
    creerConnect();
    playlist();
}


/*!
 *  \fn void MusiqueWidget::playlist()
 *  \brief créer la playlist à jouer
 */
void MusiqueWidget::playlist()
{
    QDir repertoire = Tools::cd("src/musique");
    qDebug()<<"\n\n"<<repertoire.path();
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.mp3");

    /*!
     *  Parcours le répertoire src/musique à la recherche de mp3
     */
    foreach(QFileInfo fileInfo, repertoire.entryInfoList())
    {
        qDebug()<<fileInfo.fileName();
        Phonon::MediaSource source(fileInfo.absoluteFilePath());
        sources.append(source);
    }
    qDebug()<<"\n\n";
}

/*!
 *  \fn void MusiqueWidget::stateChanged(Phonon::State newState, Phonon::State)
 *  \brief Action lors du chagement d'état du lecteur audio
 *
 *  \param[in] newState nouvel état du lecteur
 */
void MusiqueWidget::stateChanged(Phonon::State newState, Phonon::State){
    switch (newState) {
    case Phonon::ErrorState:
        if (mediaObject->errorType() == Phonon::FatalError)
            QMessageBox::warning(this, tr("Fatal Error"), mediaObject->errorString());
        else
            QMessageBox::warning(this, tr("Error"), mediaObject->errorString());
        break;
    case Phonon::PlayingState:
        ui->playButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
        break;
    case Phonon::StoppedState:
        ui->stopButton->setEnabled(false);
        ui->playButton->setEnabled(true);
        break;
    default:
        ;
    }
}

/*!
 *  \fn void MusiqueWidget::metaStateChanged(Phonon::State newState, Phonon::State)
 *  \brief Action lors du chagement d'état du media en cours
 */
void MusiqueWidget::metaStateChanged(Phonon::State newState, Phonon::State)
{
    /*!
     *  Si le media contient un erreur : afficher un dialog d'erreur
     */
    if (newState == Phonon::ErrorState) {
        QMessageBox::warning(this, tr("Error opening files"),
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
void MusiqueWidget::aboutToFinish()
{
    int index = sources.indexOf(mediaObject->currentSource()) + 1;
    if (sources.size() > index) {
        mediaObject->enqueue(sources.at(index));
    }
}

/*!
 *  \fn void MusiqueWidget::play()
 *  \brief lancement de la lecture
 *
 *  Si la playlist n'a pas été initialiser il faut lui donner les sources
 */
void MusiqueWidget::play(){
    if(mediaObject->state() != Phonon::PlayingState && mediaObject->state() != Phonon::PausedState){
        if(initPlaylist){
            metaInformationResolver->setCurrentSource(sources.at(0));
            mediaObject->setCurrentSource(sources.at(0));
            initPlaylist = false;
        }
        mediaObject->play();
    }
    if(mediaObject->state() == Phonon::PausedState) mediaObject->play();
}

/*!
 *  \fn void MusiqueWidget::stop()
 *  \brief arrete de la lecture
 */
void MusiqueWidget::stop()
{
    if(mediaObject->state() == Phonon::PlayingState) mediaObject->stop();
}

/*!
 *  \fn void MusiqueWidget::pause()
 *  \brief met en pause de la lecture
 */
void MusiqueWidget::pause()
{
    if(mediaObject->state() == Phonon::PlayingState) mediaObject->pause();
}


/*!
 *  \fn void MusiqueWidget::playExterne()
 *  \brief Passage au mode lecture externe
 */
void MusiqueWidget::playExterne(){
    mediaObject->stop();
    this->hide();
}

/*!
 *  \fn void MusiqueWidget::playInterne()
 *  \brief Passage au mode lecture Interne
 */
void MusiqueWidget::playInterne(){
    /*
        TODO :
        Création de la playlist
        Lancement de la playList
    */
    this->show();
}

/*!
 *  \fn void MusiqueWidget::playCDRom()
 *  \brief Passage au mode lecture CDRom
 */
void MusiqueWidget::playCDRom(){
    /*
        TODO:
        Lecture depuis un CD-rom
    */
    this->show();
}

/*!
 *  \fn void MusiqueWidget::setRandom(bool r)
 *  \brief Passage au mode lecture Aléatoire ou Normal
 *
 *  \param[in] r Nouvel état de la lecture aléatoire
 */
void MusiqueWidget::setRandom(bool r){
    random = r;
    /*
        TODO :
        Création d'une playlist
        Lecture aléatoire
    */
}


/*!
 *  \fn void MusiqueWidget::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entre les différents widgets de l'application.
 */
void MusiqueWidget::creerConnect(){
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
    connect(ui->playButton,SIGNAL(clicked()),this,SLOT(play()));
    connect(ui->stopButton,SIGNAL(clicked()),mediaObject,SLOT(stop()));
}

/*!
 *  \fn MusiqueWidget::~MusiqueWidget()
 *  \brief Destructeur de la classe MusiqueWidget
 */
MusiqueWidget::~MusiqueWidget() {
    delete ui;
}

