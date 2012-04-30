/*!
 *  \file parametremusiquedialog.h
 *  \headerfile ui_parametremusiquedialog.h
 */
#include "parametremusiquedialog.h"
#include "ui_parametremusiquedialog.h"


/*TODO*/
/*!
 *  \bug UI changement en mode externe <=> interne
 *       la fenetre ne doit pas bouger
 */

/*!
 *  \fn ParametreMusiqueDialog::ParametreMusiqueDialog(QWidget *parent)
 *  \brief Constructeur de la classe ParametreMusiqueDialog
 *  \param[in]  parent Parent de l'application
 */
ParametreMusiqueDialog::ParametreMusiqueDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ParametreMusiqueDialog)
{
    ui->setupUi(this);
    creerConnect();
    setSources();
}

/*!
 *  \fn ParametreMusiqueDialog::~ParametreMusiqueDialog()
 *  \brief Destructeur de la classe ParametreMusiqueDialog
 */
ParametreMusiqueDialog::~ParametreMusiqueDialog(){
    delete ui;
}

/*!
 *  \fn static QString ParametreMusiqueDialog::getHoraires()
 *  \brief Retourne les horaires de lancement de la musique
 *  \return QString contenu du fichier "src/lancementMusique.txt"
 */
QString ParametreMusiqueDialog::getHoraires(){
    QString horaires;
    QFile fichier(Tools::absolutePathFile("src/lancementMusique.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        horaires = flux.readLine();
        fichier.close();
    }
    return horaires;
}

/*!
 *  \fn void ParametreMusiqueDialog::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entre les différents widgets de l'application.
 */
void ParametreMusiqueDialog::creerConnect(){
    connect(ui->externeRadio,SIGNAL(clicked()),this,SLOT(sourcesChanged()));
    connect(ui->cdromRadio,SIGNAL(clicked()),this,SLOT(sourcesChanged()));
    connect(ui->interneRadio,SIGNAL(clicked()),this,SLOT(sourcesChanged()));
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->closeSaveButton,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->randomCheckBox,SIGNAL(clicked(bool)),this,SLOT(setRandom(bool)));
}

/*!
 *  \fn void ParametreMusiqueDialog::setHoraires(QStringList horaires)
 *  \brief initialise l'interface a partir d'une QStringList
 *  \param[in] horaires contenu du fichier "src/lancementMusique.txt"
 *
 *  Lit les horaires dans la liste et les assignent au différent spinBox
 *  et set l'état de la checkBox pour la lecture en random
 *
 */
void ParametreMusiqueDialog::setHoraires(QStringList horaires){
    ui->OmatinHeureSpin ->setValue(horaires.at(1).toInt());
    ui->OmatinMinuteSpin->setValue(horaires.at(2).toInt());
    ui->FmidiHeureSpin  ->setValue(horaires.at(3).toInt());
    ui->FmidiMinuteSpin ->setValue(horaires.at(4).toInt());
    ui->OmidiHeureSpin  ->setValue(horaires.at(5).toInt());
    ui->OmidiMinuteSpin ->setValue(horaires.at(6).toInt());
    ui->FsoirHeureSpin  ->setValue(horaires.at(7).toInt());
    ui->FsoirMinuteSpin ->setValue(horaires.at(8).toInt());

    if(horaires.at(9) == "true") ui->randomCheckBox->setChecked(true);
    else                      ui->randomCheckBox->setChecked(false);
}

/*!
 *  \fn void ParametreMusiqueDialog::setSources()
 *  \brief Lance l'initialisation des widget
 *
 *   Ouvre le fichier de sauvegarde "lancementMusique.txt
 *   et lance l'initialisation des widgets
 */
void ParametreMusiqueDialog::setSources(){
    QFile fichier(Tools::absolutePathFile("src/lancementMusique.txt"));
    ui->externeRadio->setChecked(false);
    ui->interneRadio->setChecked(false);
    ui->cdromRadio->setChecked(false);
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        QStringList lancement = flux.readLine().split(" ");
        fichier.close();
        if     (lancement.at(0) == "externe") ui->externeRadio->setChecked(true);
        else if(lancement.at(0) == "interne") ui->interneRadio->setChecked(true);
        else if(lancement.at(0) == "cdrom"  ) ui->cdromRadio->setChecked(true);
        if(lancement.size() == 10 && lancement.at(0) != "externe")
            setHoraires(lancement);
    }
    sourcesChanged();
}

/*!
 *  \fn void ParametreMusiqueDialog::sourcesChanged()
 *  \brief cache ou affiche la zone de musique interne
 */
void ParametreMusiqueDialog::sourcesChanged(){
    if(ui->externeRadio->isChecked())
        ui->lancementGroupBox->hide();
    else
        ui->lancementGroupBox->show();
}

/*!
 *  \fn void ParametreMusiqueDialog::save()
 *  \brief sauvegarde les valeurs des champs
 *
 *    sauvegarde les valeurs des champs dans le fichier "lancementMusique.txt"
 *    et emet les signaux corespondants au parametre voulu
 */
void ParametreMusiqueDialog::save(){
    QFile fichier(Tools::absolutePathFile("src/lancementMusique.txt"));
    if(fichier.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QTextStream flux(&fichier);
        QString str;
        QString var;
        if(ui->externeRadio->isChecked()){
            str = "externe"; emit externeMusique();
        }else{

            if (ui->interneRadio->isChecked()){ str = "interne"; emit interneMusique(); }
            else{ str = "cdrom"; emit cdromMusique();}

            str += " " + var.setNum(ui->OmatinHeureSpin ->value());
            str += " " + var.setNum(ui->OmatinMinuteSpin->value());
            str += " " + var.setNum(ui->FmidiHeureSpin  ->value());
            str += " " + var.setNum(ui->FmidiMinuteSpin ->value());
            str += " " + var.setNum(ui->OmidiHeureSpin  ->value());
            str += " " + var.setNum(ui->OmidiMinuteSpin ->value());
            str += " " + var.setNum(ui->FsoirHeureSpin  ->value());
            str += " " + var.setNum(ui->FsoirMinuteSpin ->value());

            if(ui->randomCheckBox->isChecked()){ str += " true"; setRandom(true);}
            else{ str += " false"; setRandom(false);}
        }
        flux << str;
        fichier.close();
    }
    emit parametreChanges();
}

/*!
 *  \fn void ParametreMusiqueDialog::setRandom(bool random)
 *  \brief Tansmet a la MainWindow que l'état du random a changé
 *  \param[in] random  Nouvel état du random
 *
 *  emet un signal au programme principale lui précisant que la lecture est
 *  en random ou non
 */
void ParametreMusiqueDialog::setRandom(bool random){
    emit randomChanged(random);
}
