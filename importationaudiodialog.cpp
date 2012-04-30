/*!
 * \file importationaudiodialog.cpp
 * \class ImportationAudioDialog
 */

#include "importationaudiodialog.h"
#include "ui_importationaudiodialog.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QFileSystemModel>

#include <QDebug>

/*!
 *  \fn ImportationAudioDialog::ImportationAudioDialog(QWidget *parent)
 *  \brief Constructeur de la classe ImportationAudioDialog
 *  \param[in]  parent Parent de l'application soit la fenêtre principale
 */
ImportationAudioDialog::ImportationAudioDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ImportationAudioDialog)
{
    ui->setupUi(this);
    creerConnect();
    ui->repertoireView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    modeleRepertoire = new QFileSystemModel ();
    creerArborescence();
}

/*!
 *  \fn ImportationAudioDialog::~ImportationAudioDialog()
 *  \brief Destructeur de la classe ImportationAudioDialog
 */
ImportationAudioDialog::~ImportationAudioDialog(){
    delete ui;
}

/*!
 *  \fn void ImportationAudioDialog::chercherAudio(QModelIndex index)
 *  \brief slot cherchant les fichiers audio dans le répertoire selectionné
 *  \param[in] index Emplacement de la sélection dans repertoireView
 *
 *  Paroucourt le repertoire selectionné dans repertoireView
 *  et en crée une liste remplis de fichier audio
 */
void ImportationAudioDialog::chercherAudio(QModelIndex index){
    modeleFichier = new QStandardItemModel();

    QString path = modeleRepertoire->filePath(index);
    QDir dir(Tools::absolutePathFile("src/.link_linux"));
    path = path.replace(dir.absolutePath(),"");
    QDir repertoire(path);
    repertoire.setFilter(QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.mp3" << "*.wav");

    foreach(QFileInfo file, repertoire.entryInfoList())
        modeleFichier->appendRow(new QStandardItem(file.fileName()));
    QStandardItem *item;
    /*
     *  rend chacune des lignes de la liste checkable
     */
    for(int i=0; i<modeleFichier->rowCount();i++){
        item = modeleFichier->item(i);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(QVariant(Qt::Unchecked), Qt::CheckStateRole);
    }
    ui->fichierView->setModel(modeleFichier);
    indexCourant = index;
}

/*!
 * \fn void ImportationAudioDialog::importAudio()
 * \brief Copie les fichiers
 *
 *  Copie les fichiers depuis la source jusque dans le dossier
 *  préciser dans "type de fichier"
 */
void ImportationAudioDialog::importAudio(){
    QStandardItem *item;
    QString path, originalFilePath, newFilePath;
    bool importBool = false;
    for(int i=0; i< modeleFichier->rowCount(); i++){
        item = modeleFichier->item(i);
        if(item->checkState()){
            /*
             *  Récuprère le chemin d'origine et crée le nouveau chemin
             */
            QDir dir = Tools::cd("src/" + getSelectType());
            path = modeleRepertoire->filePath(indexCourant);
            path = path.replace(dir.absolutePath(),"");
            originalFilePath = Tools::absolutePathFile(path +"/"+ item->text());
            newFilePath = Tools::absolutePathFile(dir.absolutePath() +"/"+
                                                  Tools::prepareToCopy(item->text()));

            /*
             * Copie du fichier
             */
            QFile file(originalFilePath);
            if(file.exists() && file.open(QIODevice::ReadWrite)){
                file.copy(newFilePath);
                importBool = true;
            }
            file.close();
        }
    }
    /*!
     *   une fois les imports effectué un message de confirmationd ecopie
     *   s'ouvre
     */
    if(importBool)
        QMessageBox::information(this,tr("Import de fichier"),
                                 tr("Tous les fichiers ont bien été importés"),
                                 QMessageBox::Ok);
}

/*!
 *  \fn void ImportationAudioDialog::creerConnect()
 *  \brief Créer les connections.
 *
 *  Créer les connections entre les différents widgets de l'application.
 */
void ImportationAudioDialog::creerConnect(){
    connect(ui->repertoireView,SIGNAL(clicked(QModelIndex)),this,SLOT(chercherAudio(QModelIndex)));
    connect(ui->importButton,SIGNAL(clicked()),this,SLOT(importAudio()));
}

/*!
 *  \fn void ImportationAudioDialog::creerArborescence()
 *  \brief Selectionne l'arborescence à créer en fonction de OS
 *
 *  Selon le system d'exploitation la navigation et la créeation de l'arborescence
 *  n'est pas pareil
 */
void ImportationAudioDialog::creerArborescence(){
#if defined(Q_WS_X11)/*! Q_WS_X11 = variable préprocesseur LINUX */
    arborescenceLinux();
#else /*! Q_WS_WIN32 = variable préprocesseur WINDOWS */
    arborescenceWin();
#endif
}

/*!
 *  \fn void ImportationAudioDialog::arborescenceLinux()
 *  \brief Créer l'arborescence sous linux
 *
 *  Créer l'arborescence des fichier sous linux à l'aide ce lien relatif
 *  les "raccourcis" sont situés dans src/.link_linux
 */
void ImportationAudioDialog::arborescenceLinux(){ 
    QDir dir = Tools::cd("src/.link_linux");
    modeleRepertoire->setRootPath(dir.absolutePath());
    modeleRepertoire->setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);
    ui->repertoireView->setModel(modeleRepertoire);
    QModelIndex idx = modeleRepertoire->index(dir.absolutePath());
    ui->repertoireView->setRootIndex(idx);
    for(int i=1;i<modeleRepertoire->columnCount();i++) ui->repertoireView->hideColumn(i);
}

/*!
 *  \fn void ImportationAudioDialog::arborescenceWin()
 *  \brief Créer l'arborescence sous Windows
 *
 *  Créer l'arborescence des fichier sous Windows
 */
void ImportationAudioDialog::arborescenceWin(){
    //TODO
}
/*!
 *  QString ImportationAudioDialog::getSelectType()
 *  \brief Retourne le type de fichier en import
 *  \return QString - nom du repertoire ou importer les fichiers
 *
 *  Le répertoire est choisi en fonction du bouton radio choisi
 */
QString ImportationAudioDialog::getSelectType(){
    /*
      Retourne le type de fichier selectionné
    */
    if(ui->ouvertureRadio->isChecked())  return "ouverture";
    if(ui->fermetureRadio->isChecked())  return "fermeture";
    if(ui->pointAppelRadio->isChecked()) return "pointAppel";
    if(ui->jingleRadio->isChecked())     return "jingle";
    if(ui->programmesRadio->isChecked()) return "programmes";
    if(ui->promotionRadio->isChecked())  return "promotion";
    return "musique";
}
