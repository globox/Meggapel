/*!
 *  \file ofmagasindialog.cpp
 *  \headerfile ofmagasindialog.h
 */
#include "ofmagasindialog.h"
#include "ui_ofmagasindialog.h"
#include "ofexceptionnelledialog.h"

/*!
 *  \fn OFMagasinDialog::OFMagasinDialog(QWidget *parent)
 *  \brief Constructeur de la classe OFMagasinDialog
 *  \param[in]  parent Parent de l'application
 */
OFMagasinDialog::OFMagasinDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OFMagasinDialog)
{
    ui->setupUi(this);
    setOF();
    creerConnect();
}

/*!
 *  \fn OFMagasinDialog::~OFMagasinDialog()
 *  \brief Destructeur de la classe OFMagasinDialog
 */
OFMagasinDialog::~OFMagasinDialog()
{
    delete ui;
}

/*!
 *  \fn void OFMagasinDialog::setOF()
 *  \brief initialise a l'aide d'un fichier les horaires de la semaines
 *
 *  Lit chaque ligne du fichier correspondant a un jour de la semaine
 *  si une ligne n'existe pas Qt la mettra comme ("")
 *  on considere donc que le magasin est fermé ce jour ci
 */
void OFMagasinDialog::setOF()
{
    QFile fichier(Tools::absolutePathFile("src/ouverture.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        ui->tabLundi->setValue(flux.readLine().split(" "));
        ui->tabMardi->setValue(flux.readLine().split(" "));
        ui->tabMercredi->setValue(flux.readLine().split(" "));
        ui->tabJeudi->setValue(flux.readLine().split(" "));
        ui->tabVendredi->setValue(flux.readLine().split(" "));
        ui->tabSamedi->setValue(flux.readLine().split(" "));
        ui->tabDimanche->setValue(flux.readLine().split(" "));
        fichier.close();
    }
}

/*!
 *  \fn void OFMagasinDialog::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entre les différents widgets de l'application.
 */
void OFMagasinDialog::creerConnect(){
    connect(ui->copieButton,SIGNAL(clicked()),this,SLOT(copieDay()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->saveCloseButton,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->exceptionButton,SIGNAL(clicked()),this,SLOT(openExceptionnelle()));
}

/*!
 *  \fn void OFMagasinDialog::copieDay()
 *  \brief Copie le jour actuel sur les autres jours
 *
 *  Copie le jour actuellement sélectionné sur les autres jours
 *  cochés en bas.
 */
void OFMagasinDialog::copieDay(){
    OFInterfaceWidget *interface = (OFInterfaceWidget *) ui->tabWidget->currentWidget();
    if(ui->lundiCheck   ->isChecked()) ui->tabLundi->copieFrom(interface);
    if(ui->mardiCheck   ->isChecked()) ui->tabMardi->copieFrom(interface);
    if(ui->mercrediCheck->isChecked()) ui->tabMercredi->copieFrom(interface);
    if(ui->jeudiCheck   ->isChecked()) ui->tabJeudi->copieFrom(interface);
    if(ui->vendrediCheck->isChecked()) ui->tabVendredi->copieFrom(interface);
    if(ui->samediCheck  ->isChecked()) ui->tabSamedi->copieFrom(interface);
    if(ui->dimancheCheck->isChecked()) ui->tabDimanche->copieFrom(interface);
}

/*!
 *  \fn void OFMagasinDialog::save()
 *  \brief Sauvegarde l'état de chacune des tables
 *
 *  Sauvegarde l'état de chacun des tables n écriant dans un fichier
 *  La valeur dans chacun des composant des tables
 */
void OFMagasinDialog::save(){
    bool valid = true ;
    valid &= ui->tabLundi->isValid(    tr("Lundi"));
    valid &= ui->tabMardi->isValid(    tr("Mardi"));
    valid &= ui->tabMercredi->isValid( tr("Mercredi"));
    valid &= ui->tabJeudi->isValid(    tr("Jeudi"));
    valid &= ui->tabVendredi->isValid( tr("Vendredi"));
    valid &= ui->tabSamedi->isValid(   tr("Samedi"));
    valid &= ui->tabDimanche->isValid( tr("Dimanche"));
    if(valid == false) return;

    QFile fichier(Tools::absolutePathFile("src/ouverture.txt"));
    if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        flux << ui->tabLundi->getValue().join(" ") << "\n";
        flux << ui->tabMardi->getValue().join(" ")<< "\n";
        flux << ui->tabMercredi->getValue().join(" ")<< "\n";
        flux << ui->tabJeudi->getValue().join(" ")<< "\n";
        flux << ui->tabVendredi->getValue().join(" ")<< "\n";
        flux << ui->tabSamedi->getValue().join(" ")<< "\n";
        flux << ui->tabDimanche->getValue().join(" ");
        fichier.close();
    }
    emit nouveauxHoraires();
}

/*!
 *  \fn void OFMagasinDialog::openExceptionnelle()
 *  \brief Ouvre la fenetre "ouverture exceptionnelle"
 *
 *  Crée la fenetre ouverture exceptionnelle puis l'affiche
 */
void OFMagasinDialog::openExceptionnelle(){
    OFExceptionnelleDialog *ofe = new OFExceptionnelleDialog(this);
    ofe->show();
}
