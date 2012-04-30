/*!
 *  \file enregistrementmessagedialog.cpp
 */
#include "enregistrementmessagedialog.h"
#include "ui_enregistrementmessagedialog.h"

#include <QStyle>

/*!
 *  \class EnregistrementMessageDialog
 *
 *  \bug La classe n'implement pas encore d'enregistreur
 *
 *  \fn EnregistrementMessageDialog::EnregistrementMessageDialog(QWidget *parent) :
 *  \brief constructeur de la classe EnregistrementMessageDialog
 *  \param[in] parent Parent de l'application (toujours la MainWindow)
 *
 */
EnregistrementMessageDialog::EnregistrementMessageDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::EnregistrementMessageDialog)
{

    ui->setupUi(this);
    titre = "";
    changeTitre();
    addIconButton();
    changeStateRecordeZone(false);
    creerConnect();
}

/*!
 *  \fn EnregistrementMessageDialog::~EnregistrementMessageDialog()
 *  \brief destructeur de la classe EnregistrementMessageDialog
 *
 */
EnregistrementMessageDialog::~EnregistrementMessageDialog()
{
    delete ui;
}

/*!
 *  \fn void EnregistrementMessageDialog::changeStateRecordeZone(bool state)
 *  \brief Change l'état de la zone d'enregistrement
 *  \param state nouvel état de la zone centrale
 *
 *  lorsque le dialog ets charger il n y a pas de message par defaut
 *  il faut en choisir un pour empecher l'utilisateur de faire des betise
 *  on rend inutilisable la zone d'enregistrement
 */
bool EnregistrementMessageDialog::changeStateRecordeZone(bool state)
{
    if(state == true && titre == "")
    {
        QString message = tr("Le titre de votre message n'est pas valide.");
        message += "\n" + tr("Veuillez en mettre un valide s'il vous plait.");
        QMessageBox::critical(this, "Erreur", message );
        return false;
    }
    ui->playButton->setEnabled(state);
    ui->saveButton->setEnabled(state);
    ui->supprimerButton->setEnabled(state);
    //TODO enable zone enregistrement

    return true;
}

/*!
 *  \fn void EnregistrementMessageDialog::creerConnect()
 *  \brief Créer les connection entre les différent composants
 *
 */
void EnregistrementMessageDialog::creerConnect()
{
    connect(ui->promotionRadio,SIGNAL(clicked()),this,SLOT(setPromotion()));
    connect(ui->programmesRadio,SIGNAL(clicked()),this,SLOT(setProgrammes()));
    connect(ui->pointAppelRadio,SIGNAL(clicked()),this,SLOT(setPointAppel()));
    connect(ui->jingleRadio,SIGNAL(clicked()),this,SLOT(setJingle()));
    connect(ui->ouvertureRadio,SIGNAL(clicked()),this,SLOT(setOuverture()));
    connect(ui->fermetureRadio,SIGNAL(clicked()),this,SLOT(setFermeture()));

    connect(ui->changeNomButton,SIGNAL(clicked()),this,SLOT(changeTitre()));
}

/*!
 *  \fn void GestionMessageDialog::addIconButton()
 *  \brief Complete l'interface créer avec Qt Designer
 *
 *  Rajouter des icone au bouton
 */
void EnregistrementMessageDialog::addIconButton()
{
    QStyle *style = QApplication::style();
    ui->saveButton->setIcon(style->standardIcon(QStyle::SP_DialogApplyButton));
    ui->playButton->setIcon(style->standardIcon(QStyle::SP_MediaPlay));
    ui->supprimerButton->setIcon(style->standardIcon(QStyle::SP_TrashIcon));
}

/*!
 *  \fn void EnregistrementMessageDialog::changeTitre()
 *  \brief ouvre une boite de dialog pour changer le titre du message
 *
 */
void EnregistrementMessageDialog::changeTitre()
{
    QRegExp rx("^[a-zA-Z]+[a-zA-Z0-9 _-]*[a-zA-Z0-9]+$");
    bool ok = false;
    bool isValid = false;
    QString title = "";
    while(title == "" || !isValid){
        title = QInputDialog::getText(this, "Changement de titre", "Entrer le titre de votre message :",
                                      QLineEdit::Normal, QString(), &ok);
        if(ok == false)break;
        isValid = rx.exactMatch(title);
        if(! isValid)
        {
            QString message = tr("Le titre de votre message ne doit comporter que les lettres :");
            message += "\n\t'a-z'   'A-Z'   '0-9'   '   '   '-'   '_'\n \n";
            message += tr("Le titre du message ne peut pas commencer ou finir par");
            message += "\n\t'  '        '-'        '_'";
            QMessageBox::critical(this, "Erreur changement de titre", message);
        }
    }
    if(ok == false)return;

    titre = title;
    updateLabelTitre();
}

/*!
 *  \fn void EnregistrementMessageDialog::updateLabelTitre()
 *  \brief met a jour le label sur UI
 *
 */
void EnregistrementMessageDialog::updateLabelTitre()
{
    ui->titreLabel->setText(titre);
}

/*!
 *  \fn void EnregistrementMessageDialog::unCheckAll()
 *  \brief Décheck tous les boutons radio
 *
 */
void EnregistrementMessageDialog::unCheckAll()
{
    ui->programmesRadio->setChecked(false);
    ui->pointAppelRadio->setChecked(false);
    ui->ouvertureRadio->setChecked(false);
    ui->fermetureRadio->setChecked(false);
    ui->promotionRadio->setChecked(false);
    ui->jingleRadio->setChecked(false);
}

/*!
 *  \fn void EnregistrementMessageDialog::setPromotion()
 *  \brief met l'interface a Promotion
 *
 */
void EnregistrementMessageDialog::setPromotion()
{
    unCheckAll();
    if(! changeStateRecordeZone(true))return;
    ui->promotionRadio->setChecked(true);
    path = "src/promotion";
}

/*!
 *  \fn void EnregistrementMessageDialog::setOuverture()
 *  \brief met l'interface a Ouverture
 *
 */
void EnregistrementMessageDialog::setOuverture()
{
    unCheckAll();
    if(! changeStateRecordeZone(true))return;
    ui->ouvertureRadio->setChecked(true);
    path = "src/ouverture";
}

/*!
 *  \fn void EnregistrementMessageDialog::setFermeture()
 *  \brief met l'interface a fermeture
 *
 */
void EnregistrementMessageDialog::setFermeture()
{
    unCheckAll();
    if(! changeStateRecordeZone(true))return;
    ui->fermetureRadio->setChecked(true);
    path = "src/fermeture";
}

/*!
 *  \fn void EnregistrementMessageDialog::setJingle()
 *  \brief met l'interface a jingle
 *
 */
void EnregistrementMessageDialog::setJingle()
{
    unCheckAll();
    if(! changeStateRecordeZone(true))return;
    ui->jingleRadio->setChecked(true);
    path = "src/jingle";
}

/*!
 *  \fn void EnregistrementMessageDialog::setPointAppel()
 *  \brief met l'interface a Point Appel
 *
 */
void EnregistrementMessageDialog::setPointAppel()
{
    unCheckAll();
    if(! changeStateRecordeZone(true))return;
    ui->pointAppelRadio->setChecked(true);
    path = "src/pointAppel";
}

/*!
 *  \fn void EnregistrementMessageDialog::setProgrammes()
 *  \brief met l'interface a Programmes
 *
 */
void EnregistrementMessageDialog::setProgrammes()
{
    unCheckAll();
    if(! changeStateRecordeZone(true))return;
    ui->programmesRadio->setChecked(true);
    path = "src/programmes";
}
