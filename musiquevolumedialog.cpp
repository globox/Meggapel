/*!
 *  \file musiquevolumedialog.cpp
 *  \headerfile musiquevolumedialog.h
 */

#include "musiquevolumedialog.h"
#include "ui_musiquevolumedialog.h"
#include <QTimer>

/*!
 *  \fn MusiqueVolumeDialog::MusiqueVolumeDialog(Phonon::AudioOutput *audio, QWidget *parent)
 *  \brief Constructeur de la classe ImportationAudioDialog
 *
 *  \param[in]  parent  Parent de l'application soit la fenêtre principale
 *  \param[in]  audio   Sortie audio du lecteur de musique
 *
 *  \bug Le volume slider ne marche plus
 *  \bug le volume slider se réinitialise au chargement de la fenetre
 */
MusiqueVolumeDialog::MusiqueVolumeDialog(Phonon::AudioOutput *audio, QWidget *parent) :
    QDialog(parent),  ui(new Ui::MusiqueVolumeDialog) , audioOutput(audio) {
    ui->setupUi(this);
    fakeAudioOutput = new Phonon::AudioOutput(this);
    fakeAudioOutput->setVolume(audioOutput->volume());
    /*
     *  Il y a un problème lorsque l'on glisse le volumeSlider le volume change trop souvent
     *  l'astuce est de créer un timer qui tick toutes les secondes qui assigne le volume
     *  à ce moment là, le client ne voit pas la différence ou juste avec un leger
     *  décalage.
     */
    timer = new QTimer(this);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(changeVolume()));
}

/*!
 *  \fn void MusiqueVolumeDialog::changeVolume()
 *  \brief assigne le volume de la sortie audio en fonction du Slider
 */
void MusiqueVolumeDialog::changeVolume(){
    audioOutput->setVolume(fakeAudioOutput->volume());
}

/*!
 *  \fn MusiqueVolumeDialog::~MusiqueVolumeDialog()
 *  \brief Destructeur de la classe MusiqueVolumeDialog.
 */
MusiqueVolumeDialog::~MusiqueVolumeDialog(){
    delete timer;
    delete fakeAudioOutput;
    delete ui;
}
