#ifndef MUSIQUEVOLUMEDIALOG_H
#define MUSIQUEVOLUMEDIALOG_H

/*!
 *  \file musiquevolumedialog.h
 *  \brief Controle le volume audio
 */

#include <QDialog>
#include <phonon/AudioOutput>

class QTimer;

namespace Ui {
class MusiqueVolumeDialog;
}

/*! \class MusiqueVolumeDialog
 * \brief classe representant la fenetre de controle audio
 *
 *  La classe créer une boite de dialogue avec un slider de volume
 */
class MusiqueVolumeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MusiqueVolumeDialog(Phonon::AudioOutput* audio, QWidget *parent = 0);
    ~MusiqueVolumeDialog();

private slots:
    void changeVolume();

private:
    Ui::MusiqueVolumeDialog *ui;         /*!< Interface graphique généer avec Qt Designer */
    Phonon::AudioOutput *audioOutput;    /*!< sorti audio de l'application */
    Phonon::AudioOutput *fakeAudioOutput;/*!< sorti audio fictive utilisé pour travailler*/
    QTimer *timer;                       /*!< timer indiquant quand il faut assigner la nouvelle
                                          valeur a la sortie audio*/
};

#endif // MUSIQUEVOLUMEDIALOG_H
