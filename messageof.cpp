/*!
 *  \file messageof.cpp
 *  \class MessageOF
 */

#include "messageof.h"
#include "ui_messageof.h"

/*!
 *  \fn MessageOF::MessageOF(QWidget *parent) :
 *  \brief constructeur de la classe MessageOF
 *
 *  param[in] parent Parent de l'application (toujours la MainWindow)
 */
MessageOF::MessageOF(QWidget *parent) :
    QDialog(parent), ui(new Ui::MessageOF)
{
    ui->setupUi(this);
    setFichier();
    creerConnect();
    setSame();
}

/*!
 *  \fn MessageOF::~MessageOF():
 *  \brief destructeur de la classe MessageOF
 *
 */
MessageOF::~MessageOF()
{
    delete ui;
}

/*!
 *  \fn void MessageOF::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entWriteOnlyre les différents widgets de l'application.
 */
void MessageOF::creerConnect()
{
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(saveValues()));
    connect(ui->closeSaveButton,SIGNAL(clicked()),this,SLOT(saveValues()));
    connect(ui->ouiRadio,SIGNAL(clicked()),this,SLOT(hideAffichage()));
    connect(ui->nonRadio,SIGNAL(clicked()),this,SLOT(showAffichage()));
}

/*!
 *  \fn void MessageOF::setFichier()
 *  \brief assigne un fichier de sauvegarde a chacune des tables.
 *
 */
void MessageOF::setFichier()
{
    ui->tabOuverture->setFichier("src/ouvertureMessage.txt");
    ui->tabFermeture->setFichier("src/fermetureMessage.txt");
    ui->tabOuvertureAprem->setFichier("src/ouvertureApremMessage.txt");
    ui->tabFermetureAprem->setFichier("src/fermetureApremMessage.txt");
}

/*!
 *  \fn void MessageOF::readSame()
 *  \brief vérifie si on utilise les meme musique le matin et l aprem
 *
 */
void MessageOF::setSame()
{
    QString value;
    QFile fichier(Tools::absolutePathFile("src/messageof.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        value = flux.readLine();
        fichier.close();
    }
    (value == "true" ? hideAffichage() : showAffichage());
    isSame = ui->ouiRadio->isChecked();
}

/*!
 *  \fn void MessageOF::saveValues()
 *  \brief lance la sauvegarde des valeurs
 *
 *  Appelle les fontions save() des classes
 *  MessageOuvertureWidget() et MessageFermetureWidget()
 */
void MessageOF::saveValues()
{
    ui->tabOuverture->save();
    ui->tabFermeture->save();
    ui->tabOuvertureAprem->save();
    ui->tabFermetureAprem->save();

    if(ui->ouiRadio->isChecked() != isSame)
    {
        QFile fichier(Tools::absolutePathFile("src/messageof.txt"));
        if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream flux(&fichier);
            (ui->ouiRadio->isChecked() ? flux<<"true" : flux<<"false");
            fichier.close();
        }
    }

    emit messagesChanges();
}

/*!
 *  \fn void MessageOF::hideAffichage()
 *  \brief rend inutilisable les onglet ouverture et fermeture apres-midi
 *
 */
void MessageOF::hideAffichage()
{
    ui->tabOuvertureAprem->setEnabled(false);
    ui->tabFermetureAprem->setEnabled(false);
    ui->ouiRadio->setChecked(true);
    ui->nonRadio->setChecked(false);
}

/*!
 *  \fn void MessageOF::hideAffichage()
 *  \brief rend utilisable les onglet ouverture et fermeture apres-midi
 *
 */
void MessageOF::showAffichage()
{
    ui->tabOuvertureAprem->setEnabled(true);
    ui->tabFermetureAprem->setEnabled(true);
    ui->ouiRadio->setChecked(false);
    ui->nonRadio->setChecked(true);
}
