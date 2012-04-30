/*!
 *  \file ofinterfacewidget.cpp
 *  \headerfile ofinterfacewidget.h
 */
#include "ofinterfacewidget.h"
#include "ui_ofinterfacewidget.h"

/*!
 *  \fn OFInterfaceWidget::OFInterfaceWidget(QWidget *parent)
 *  \brief Constructeur de la classe OFExceptionnelleDialog
 *  \param[in]  parent Parent de l'application
 */
OFInterfaceWidget::OFInterfaceWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::OFInterfaceWidget)
{
    ui->setupUi(this);
    ui->HoraireGroupBox->hide();
    creerConnect();
}

/*!
 *  \fn OFInterfaceWidget::~OFInterfaceWidget()
 *  \brief Destructeur de la classe OFInterfaceWidget
 */
OFInterfaceWidget::~OFInterfaceWidget()
{
    delete ui;
}

/*!
 *  \fn void OFInterfaceWidget::copieFrom(OFInterfaceWidget *autre)
 *  \brief Copie un OFInterfaceWidget vers un autre
 *  \param[in] autre -Source de la copie
 */
void OFInterfaceWidget::copieFrom(OFInterfaceWidget *autre){
    if(autre->isFerme())
        setFerme();
    else{
        setOuvert();
        setValue(autre->getValue());
    }
}

/*!
 *  \fn bool OFInterfaceWidget::isFerme()
 *  \brief renvoi si le magasin est fermé ou non
 *  \return bool état du magasin [true = fermé][false = ouvert]
 */
bool OFInterfaceWidget::isFerme(){ return ui->FermeRadio->isChecked();}
/*!
 *  \fn bool OFInterfaceWidget::isContinue()
 *  \brief renvoi si le magasin est fermé ou non le midi
 *  \return bool état du magasin [true = ouvert][false = fermé]
 */
bool OFInterfaceWidget::isContinue(){return ui->OuvertMidiRadio->isChecked();}

/*!
 *  \fn QStringList OFInterfaceWidget::getValue()
 *  \brief Crée un liste comprenant la valeur de chacun des champs
 *  \return QStringList Valeur de tous les champs
 *
 *  Inspecte chacun des champs afin d'écrire leur valeur =état dans la QStringList
 */
QStringList OFInterfaceWidget::getValue(){
    QStringList arg;
    QString value;

    if(isFerme()){
        arg.append("");
    } else {
        if(isContinue()){
            arg.append(value.setNum(ui->OHeureSpin->value()));
            arg.append(value.setNum(ui->OMinuteSpin->value()));
            for(int i=0; i<4 ; i++) arg.append("0");
            arg.append(value.setNum(ui->FHeureSpin->value()));
            arg.append(value.setNum(ui->FMinuteSpin->value()));
        }else{
            arg.append(value.setNum(ui->OMatinHeureSpin->value()));
            arg.append(value.setNum(ui->OMatinMinuteSpin->value()));
            arg.append(value.setNum(ui->FMatinHeureSpin->value()));
            arg.append(value.setNum(ui->FMatinMinuteSpin->value()));
            arg.append(value.setNum(ui->OApremHeureSpin->value()));
            arg.append(value.setNum(ui->OApremMinuteSpin->value()));
            arg.append(value.setNum(ui->FApremHeureSpin->value()));
            arg.append(value.setNum(ui->FApremMinuteSpin->value()));
        }
    }
    return arg;
}

/*!
 *  \fn bool OFInterfaceWidget::isValid()
 *  \brief Vérifie si les donnés enregistrer ne sont pas contradictoire
 *  \param[in] jour nom du jour actuelle
 */
bool OFInterfaceWidget::isValid(QString jour)
{
    if(ui->FermeRadio->isChecked())return true;
    if(ui->OuvertMidiRadio->isChecked())
    {
        QTime ouverture(ui->OHeureSpin->value(),ui->OMinuteSpin->value());
        QTime fermeture(ui->FHeureSpin->value(),ui->FMinuteSpin->value());
        if(Tools::compareTime(ouverture,fermeture) == -1)return true;
    }
    else
    {
        QTime ouvertureMatin(ui->OMatinHeureSpin->value(),ui->OMatinMinuteSpin->value());
        QTime fermetureMatin(ui->FMatinHeureSpin->value(),ui->FMatinMinuteSpin->value());
        QTime ouvertureAprem(ui->OApremHeureSpin->value(),ui->OApremMinuteSpin->value());
        QTime fermetureAprem(ui->FApremHeureSpin->value(),ui->OApremMinuteSpin->value());
        if(     (Tools::compareTime(ouvertureMatin,fermetureMatin) == -1)&&
                (Tools::compareTime(fermetureMatin, ouvertureAprem) == -1)&&
                (Tools::compareTime(ouvertureAprem,fermetureAprem) == -1)) return true;
    }
    QMessageBox::warning(this,tr("Erreur : ") + jour,
                         tr("Il y a une incohérence dans les horaires de ")+jour+
                         tr(", impossible d'enregistrer"),
                         QMessageBox::Ok);
    return false;
}

/*!
 *  \fn void OFInterfaceWidget::setValue(QStringList arg)
 *  \brief assigne des valeur a partir d'un QStringList
 *  \param[in] arg List comprennant les valeur de chacun des champs
 *
 *  Parse la liste et assigne dans l'ordre chaque valeur a un composant
 */
void OFInterfaceWidget::setValue(QStringList arg)
{
    switch(arg.length()){
    case 8 :
        setOuvert();
        if(arg.at(2) == arg.at(4) && arg.at(3) == arg.at(5))
        { /*
           * Le magasin est en continue
           */
            setContinue();
            ui->OHeureSpin->setValue(arg.at(0).toInt());
            ui->OMinuteSpin->setValue(arg.at(1).toInt());
            ui->FHeureSpin->setValue(arg.at(6).toInt());
            ui->FMinuteSpin->setValue(arg.at(7).toInt());
            break;
        }
        else
        { /*
           * Le magasin est fermé le midi
           */
            setCoupure();
            ui->OMatinHeureSpin->setValue(arg.at(0).toInt());
            ui->OMatinMinuteSpin->setValue(arg.at(1).toInt());
            ui->FMatinHeureSpin->setValue(arg.at(2).toInt());
            ui->FMatinMinuteSpin->setValue(arg.at(3).toInt());
            ui->OApremHeureSpin->setValue(arg.at(4).toInt());
            ui->OApremMinuteSpin->setValue(arg.at(5).toInt());
            ui->FApremHeureSpin->setValue(arg.at(6).toInt());
            ui->FApremMinuteSpin->setValue(arg.at(7).toInt());
            break;
        }
    default:
        /*
         * Le magasin est fermé
         */
        setFerme();
        break;
    }
}

/*!
 *  \fn void OFInterfaceWidget::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entre les différents widgets de l'application.
 */
void OFInterfaceWidget::creerConnect()
{
    connect(ui->OuvertMidiRadio,SIGNAL(clicked()),this,SLOT(setContinue()));
    connect(ui->FermeMidiRadio,SIGNAL(clicked()),this,SLOT(setCoupure()));
    connect(ui->OuvertRadio,SIGNAL(clicked()),this,SLOT(setOuvert()));
    connect(ui->FermeRadio,SIGNAL(clicked()),this,SLOT(setFerme()));
}

/*!
 *  \fn void OFInterfaceWidget::setContinue()
 *  \brief  Modifie l'état du widget : affichage continue
 *
 *   La journée est en continue on affiche seulement
 *   l'interface "horaires"
 */
void OFInterfaceWidget::setContinue()
{
    ui->HorairesMatinGroupBox->hide();
    ui->HoraitesApremGroupBox->hide();
    ui->HoraireGroupBox->show();
    ui->OuvertRadio->setChecked(true);
    ui->FermeRadio->setChecked(false);
    ui->OuvertMidiRadio->setChecked(true);
    ui->FermeMidiRadio->setChecked(false);
}

/*!
 *  \fn void OFInterfaceWidget::setCoupure()
 *  \brief  Modifie l'état du widget : affichage coupure
 *
 *   La journée n'est pas en continue
 *   on affihce l interface "horaires matin" "horaires aprem"
 */
void OFInterfaceWidget::setCoupure()
{
    ui->HoraireGroupBox->hide();
    ui->HorairesMatinGroupBox->show();
    ui->HoraitesApremGroupBox->show();
    ui->OuvertRadio->setChecked(true);
    ui->FermeRadio->setChecked(false);
    ui->OuvertMidiRadio->setChecked(false);
    ui->FermeMidiRadio->setChecked(true);
}

/*!
 *  \fn void OFInterfaceWidget::setFerme()
 *  \brief  Modifie l'état du widget : affichage fermé
 *
 *   Le magasin est fermer on cache toutes les interfaces
 */
void OFInterfaceWidget::setFerme()
{
    ui->HoraireGroupBox->hide();
    ui->HorairesMatinGroupBox->hide();
    ui->HoraitesApremGroupBox->hide();
    ui->MidiGroupBox->hide();
    ui->OuvertRadio->setChecked(false);
    ui->FermeRadio->setChecked(true);
}

/*!
 *  \fn void OFInterfaceWidget::setsetOuvertFerme()
 *  \brief  Modifie l'état du widget : affichage ouvert
 *
 *   Le magasin est fermer on affiche toutes les interfaces
 */
void OFInterfaceWidget::setOuvert()
{
    ui->MidiGroupBox->show();
    if(ui->OuvertMidiRadio->isChecked())
        setContinue();
    else
        setCoupure();
}
