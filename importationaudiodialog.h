#ifndef IMPORTATIONAUDIODIALOG_H
#define IMPORTATIONAUDIODIALOG_H

/*!
 *  \file importationaudiodialog.h
 *  \brief Fenetre d'import audio
 */

#include <QDialog>
#include <QDir>
#include <QApplication>
#include <QModelIndex>
#include <QMessageBox>

class QStandardItemModel;
class QStandardItem;
class QItemSelectionModel;
class QFileSystemModel;

#include "tools.h"

namespace Ui {
class ImportationAudioDialog;
}

/*! \class ImportationAudioDialog
 * \brief classe representant la fenetre d'import audio
 *
 *  la classe créer un fenetre proposant a l'utilisateur d'importer
 *  différent type de fichier audio
 */
class ImportationAudioDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImportationAudioDialog(QWidget *parent = 0);
    ~ImportationAudioDialog();
    
private:
    Ui::ImportationAudioDialog *ui;     /*!< Interface graphique généer avec Qt Designer */
    QFileSystemModel *modeleRepertoire; /*!< Modele de l'arborescence  des répertoire */
    QStandardItemModel *modeleFichier;  /*!< Modele de la liste des fichiers audio */
    QModelIndex indexCourant;           /*!< Index courant dans l'arboresence des répertoire */

    void creerConnect();
    void creerArborescence();
    void arborescenceLinux();
    void arborescenceWin();
    QString getSelectType();

private slots:
    void chercherAudio(QModelIndex index);
    void importAudio();
};

#endif // IMPORTATIONAUDIODIALOG_H
