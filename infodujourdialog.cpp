/*!
 *  \file infodujourdialog.cpp
 *  \headerfile infodujourdialog.h
 */
#include "infodujourdialog.h"
#include "ui_infodujourdialog.h"

/*!
 *  \fn InfoDuJourDialog::InfoDuJourDialog(QWidget *parent)
 *  \brief Constructeur de la classe InfoDuJourDialog
 *  \param[in]  parent Parent de l'application soit la fenêtre principale
 */
InfoDuJourDialog::InfoDuJourDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::InfoDuJourDialog)
{
    ui->setupUi(this);
    setAffichage();
}

/*!
 *  \fn InfoDuJourDialog::~InfoDuJourDialog()
 *  \brief Destructeur de la classe InfoDuJourDialog
 */
InfoDuJourDialog::~InfoDuJourDialog()
{
    delete ui;
}

/*!
 *  \fn void InfoDuJourDialog::setAffichage()
 *  \brief affiche le Saint du jour
 */
void InfoDuJourDialog::setAffichage(){
    int numToday = QDate::currentDate().dayOfYear();
    if(QDate::isLeapYear(QDate::currentDate().year()) == false)numToday ++;

    QFile fichier(Tools::absolutePathFile("src/infoDuJour.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        int line = 1;
        while(!flux.atEnd() && line != numToday){
            flux.readLine();
            line ++;
        }
        QString temp = flux.readLine();
        ui->saintLabel->setText(temp);
        fichier.close();
    }
}
