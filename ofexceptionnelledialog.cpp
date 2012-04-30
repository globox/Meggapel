/*!
 *  \file ofexceptionnelledialog.cpp
 *  \headerfile ofexceptionnelledialog.h
 */
#include "ofexceptionnelledialog.h"
#include "ui_ofexceptionnelledialog.h"

#include <QStandardItem>
#include <QStandardItemModel>

/*!
 *  \fn OFExceptionnelleDialog::OFExceptionnelleDialog(QWidget *parent)
 *  \brief Constructeur de la classe OFExceptionnelleDialog
 *  \param[in]  parent Parent de l'application
 */
OFExceptionnelleDialog::OFExceptionnelleDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::OFExceptionnelleDialog)
{
    ui->setupUi(this);
    model = new QStandardItemModel();
    QLocale local(QLocale::system().name());
    ui->date->setLocale(local);
    ui->date->calendarWidget()->setLocale(local);
    ui->date->setDate(QDate::currentDate());
    creerConnect();
    updateList();
}

/*!
 *  \fn OFExceptionnelleDialog::~OFExceptionnelleDialog()
 *  \brief Destructeur de la classe OFExceptionnelleDialog
 */
OFExceptionnelleDialog::~OFExceptionnelleDialog()
{
    delete ui;
    delete model;
}

/*!
 *  \fn void OFExceptionnelleDialog::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entre les différents widgets de l'application.
 */
void OFExceptionnelleDialog::creerConnect(){
    connect(ui->ajouterButton,SIGNAL(clicked()),this,SLOT(ajouterException()));
    connect(ui->supprimerButton,SIGNAL(clicked()),this,SLOT(supprimerException()));
    connect(ui->exceptionList,SIGNAL(clicked(QModelIndex)),
            this,SLOT(afficherException(QModelIndex)));
}

/*!
 *  \fn void OFExceptionnelleDialog::updateList()
 *  \brief met a jour la liste des exception
 *
 *  lit l integralité du repertoire, et ajoute tous les dd-mm-yyyy.txt
 */
void OFExceptionnelleDialog::updateList(){

    QDir repertoire = Tools::cd("src/exception");
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList()<<"*.txt");

    QString name;
    delete model;
    model = new QStandardItemModel();
    foreach(QFileInfo fileInfo, repertoire.entryInfoList()){
        name = fileInfo.fileName().split(".").at(0);
        model->appendRow(new QStandardItem(name.replace("-","/")));
    }
    ui->exceptionList->setModel(model);
    ui->exceptionList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

/*!
 *  \fn OFExceptionnelleDialog::ajouterException()()
 *  \brief ajoute une nouvelle date Exception
 *
 *  crée un fichier dans /src/exception/ contenant toutes les exceptions
 *  chaque jour est représenté par un fichier
 */
void OFExceptionnelleDialog::ajouterException(){
    QString str = ui->date->date().toString("dd-M-yyyy");
    QFile fichier(Tools::absolutePathFile("src/exception/"+ str +".txt"));
    if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        flux << ui->interfaceWidget->getValue().join(" ") << "\n";
        fichier.close();
    }
    updateList();
}

/*!
 *  \fn OFExceptionnelleDialog::supprimerException()()
 *  \brief supprime une date Exception
 *
 *  Supprime le fichier nommé comme la date selectionné
 *
 */
void OFExceptionnelleDialog::supprimerException(){
    QItemSelectionModel *model = ui->exceptionList->selectionModel();
    QString str = model->selectedIndexes().at(0).data().toString();
    str = "src/exception/" + str.replace("/","-") + ".txt";
    QFile::remove(Tools::absolutePathFile(str));
    updateList();
}

/*!
 *  \fn void OFExceptionnelleDialog::afficherException(QModelIndex index)
 *  \brief Affiche les horaires de l'exception sélectionné
 *  \param[in] index -Index de l'élément sélectionner dans la liste
 *
 *  Lit le fichier selectionner et remplis l'interface avec
 */
void OFExceptionnelleDialog::afficherException(QModelIndex index){
    QString name = index.data().toString().replace("/","-") + ".txt";
    QFile fichier(Tools::absolutePathFile("src/exception/" + name));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        ui->interfaceWidget->setValue(flux.readLine().split(" "));
        fichier.close();
    }
}
