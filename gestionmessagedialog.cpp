/*!
 *  \file gestionmessagedialog.cpp
 */
#include "gestionmessagedialog.h"
#include "ui_gestionmessagedialog.h"
#include <QStyle>
#include <QStandardItemModel>
#include <QListView>

/*!
 *  \class GestionMessageDialog
 *  \fn GestionMessageDialog::GestionMessageDialog(QWidget *parent) :
 *  \param[in] parent Parent de la fenetre (toujours la MainWindow)
 *  \brief constructeur de la classe GestionMessageDialog
 *
 */
GestionMessageDialog::GestionMessageDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::GestionMessageDialog)
{
    ui->setupUi(this);
    addIconButton();
    modeleActuel = new QStandardItemModel();
    modeleAncien = new QStandardItemModel();
    creerConnect();
    setPromotion();
}

/*!
 *  \fn GestionMessageDialog::~GestionMessageDialog()
 *  \brief Destructeur de la classe GestionMessageDialog
 *
 */
GestionMessageDialog::~GestionMessageDialog()
{
    delete ui;
}

/*!
 *  \fn void GestionMessageDialog::creerConnect()
 *  \brief Creer les connections
 *
 *  Créer les connections entre les différents élements du widget
 */
void GestionMessageDialog::creerConnect()
{
    connect(ui->promotionRadio,SIGNAL(clicked()),this,SLOT(setPromotion()));
    connect(ui->programmesRadio,SIGNAL(clicked()),this,SLOT(setProgrammes()));
    connect(ui->pointAppelRadio,SIGNAL(clicked()),this,SLOT(setPointAppel()));
    connect(ui->jingleRadio,SIGNAL(clicked()),this,SLOT(setJingle()));
    connect(ui->ouvertureRadio,SIGNAL(clicked()),this,SLOT(setOuverture()));
    connect(ui->fermetureRadio,SIGNAL(clicked()),this,SLOT(setFermeture()));

    connect(ui->supActuelButton,SIGNAL(clicked()),this,SLOT(supprimerActuel()));
    connect(ui->supAncienButton,SIGNAL(clicked()),this,SLOT(supprimerAncien()));

    connect(ui->leftToRightButton,SIGNAL(clicked()),this,SLOT(copieLeftToRight()));
    connect(ui->rightToLeftButton,SIGNAL(clicked()),this,SLOT(copieRightToLeft()));
}

/*!
 *  \fn void GestionMessageDialog::addIconButton()
 *  \brief Complete l'interface créer avec Qt Designer
 *
 *  Rajouter des icone au bouton
 */
void GestionMessageDialog::addIconButton()
{
    QStyle *style = QApplication::style();
    ui->leftToRightButton->setIcon(style->standardIcon(QStyle::SP_ArrowRight));
    ui->rightToLeftButton->setIcon(style->standardIcon(QStyle::SP_ArrowLeft));
    ui->supActuelButton->setIcon(style->standardIcon(QStyle::SP_TrashIcon));
    ui->supAncienButton->setIcon(style->standardIcon(QStyle::SP_TrashIcon));
}

/*!
 *  \fn void GestionMessageDialog::loadAudioFile(QString dirPath)
 *  \brief charge les fichier contenu dans dirPath et "Ancien"dirPath
 *  \param[in] dirPath chemin vers le dossier a analyser
 *  \param[in] modele modele ou inserer mes donner
 *  \param[in] vue ou mettre en place le modele
 *
 */
void GestionMessageDialog::loadAudioFile(QString dirPath,
                                         QStandardItemModel *modele, QListView *vue)
{
    modele->clear();
    QDir repertoire = Tools::cd(dirPath);
    repertoire.setFilter(QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.wav");

    foreach(QFileInfo file, repertoire.entryInfoList())
        modele->appendRow(new QStandardItem(file.fileName()));
    QStandardItem *item;
    for(int i=0; i<modele->rowCount();i++)
    {
        item = modele->item(i);
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(QVariant(Qt::Unchecked), Qt::CheckStateRole);
    }
    vue->setModel(modele);
}

/*!
 *  \fn void GestionMessageDialog::supprimer(QStandardItemModel *modele, bool actuel)
 *  \brief  Supprime les fichier cocher dans un modele
 *  \param[in] modele modele ou il faut allez chercher les fichier cocher;
 *  \param[in] actuel indique si il faut cehrcher les fichiers "actuels" ou "Anciens"
 */

void GestionMessageDialog::supprimer(QStandardItemModel *modele, bool actuel)
{
    QStandardItem *item;
    QString path;
    for(int i=0; i< modele->rowCount(); i++)
    {
        item = modele->item(i);
        if(item->checkState())
        {
            (actuel ? path = currentPath +"/": path = currentPath +"Ancien/");
            path = Tools::absolutePathFile(path + item->text());
            QFile file(path);
            if(file.exists() && file.open(QIODevice::ReadWrite))
                file.remove();
        }
    }
    updateList();
}

/*!
 *  \fn void GestionMessageDialog::copie(QStandardItemModel *modele, bool leftToRight)
 *  \brief  Supprime les fichier cocher dans un modele
 *  \param[in] modele modele ou il faut allez chercher les fichier cocher;
 *  \param[in] leftToRight indique si la copie ce fait de "gauche a droite" ou de "droite a gauche"
 *
 */
void GestionMessageDialog::copie(QStandardItemModel *modele, bool leftToRight)
{
    QString sourcePath = currentPath;
    QString destPath = currentPath;
    (leftToRight? destPath +="Ancien" : sourcePath +="Ancien" );

    QStandardItem *item;
    for(int i=0; i< modele->rowCount(); i++)
    {
        item = modele->item(i);
        if(item->checkState())
        {
            sourcePath += "/" + item->text();
            destPath   += "/" + Tools::prepareToCopy(item->text());
            QFile file(sourcePath);
            if(file.exists() && file.open(QIODevice::ReadWrite))
                file.copy(destPath);
        }
    }
    (leftToRight? supprimerActuel() : supprimerAncien() );
    updateList();
}

/*!
 *  \fn GestionMessageDialog::updateList()
 *  \brief Appel la mise a jour des listes;
 *
 */
void GestionMessageDialog::updateList()
{
    if(ui->fermetureRadio->isChecked())setFermeture();
    if(ui->ouvertureRadio->isChecked())setOuverture();
    if(ui->pointAppelRadio->isChecked())setPointAppel();
    if(ui->promotionRadio->isChecked())setPromotion();
    if(ui->programmesRadio->isChecked())setProgrammes();
    if(ui->jingleRadio->isChecked())setJingle();
}

/*!
 *  \fn void GestionMessageDialog::unCheckAll()
 *  \brief Décheck tous les boutons radio
 *
 */
void GestionMessageDialog::unCheckAll()
{
    ui->programmesRadio->setChecked(false);
    ui->pointAppelRadio->setChecked(false);
    ui->ouvertureRadio->setChecked(false);
    ui->fermetureRadio->setChecked(false);
    ui->promotionRadio->setChecked(false);
    ui->jingleRadio->setChecked(false);
}

/*!
 *  \fn void GestionMessageDialog::setPromotion()
 *  \brief met l'interface a Promotion
 *
 */
void GestionMessageDialog::setPromotion()
{
    unCheckAll();
    ui->promotionRadio->setChecked(true);
    ui->actuelLabel->setText(tr("Promotions actuelles :"));
    ui->ancienLabel->setText(tr("Anciennes promotions :"));
    currentPath = "src/promotion";
    loadAudioFile(currentPath, modeleActuel, ui->actuelList);
    loadAudioFile(currentPath + "Ancien", modeleAncien, ui->ancienList);
}

/*!
 *  \fn void GestionMessageDialog::setOuverture()
 *  \brief met l'interface a Ouverture
 *
 */
void GestionMessageDialog::setOuverture()
{
    unCheckAll();
    ui->ouvertureRadio->setChecked(true);
    ui->actuelLabel->setText(tr("Messages de (pré)ouverture actuels :"));
    ui->ancienLabel->setText(tr("Anciens messages de (pré)ouverture :"));
    currentPath = "src/ouverture";
    loadAudioFile(currentPath, modeleActuel, ui->actuelList);
    loadAudioFile(currentPath + "Ancien", modeleAncien, ui->ancienList);
}

/*!
 *  \fn void GestionMessageDialog::setFermeture()
 *  \brief met l'interface a fermeture
 *
 */
void GestionMessageDialog::setFermeture()
{
    unCheckAll();
    ui->fermetureRadio->setChecked(true);
    ui->actuelLabel->setText(tr("Messages de (pré)fermeture actuels :"));
    ui->ancienLabel->setText(tr("Anciens messages de (pré)fermeture :"));
    currentPath = "src/fermeture";
    loadAudioFile(currentPath, modeleActuel, ui->actuelList);
    loadAudioFile(currentPath + "Ancien", modeleAncien, ui->ancienList);
}

/*!
 *  \fn void GestionMessageDialog::setJingle()
 *  \brief met l'interface a jingle
 *
 */
void GestionMessageDialog::setJingle()
{
    unCheckAll();
    ui->jingleRadio->setChecked(true);
    ui->actuelLabel->setText(tr("Messages jingle actuels :"));
    ui->ancienLabel->setText(tr("Anciens messages jingle :"));
    currentPath = "src/jingle";
    loadAudioFile(currentPath, modeleActuel, ui->actuelList);
    loadAudioFile(currentPath + "Ancien", modeleAncien, ui->ancienList);
}

/*!
 *  \fn void GestionMessageDialog::setPointAppel()
 *  \brief met l'interface a Point Appel
 *
 */
void GestionMessageDialog::setPointAppel()
{
    unCheckAll();
    ui->pointAppelRadio->setChecked(true);
    ui->actuelLabel->setText(tr("Messages de point appel actuels :"));
    ui->ancienLabel->setText(tr("Anciens messages de point Appel :"));
    currentPath = "src/pointAppel";
    loadAudioFile(currentPath, modeleActuel, ui->actuelList);
    loadAudioFile(currentPath + "Ancien", modeleAncien, ui->ancienList);
}

/*!
 *  \fn void GestionMessageDialog::setProgrammes()
 *  \brief met l'interface a Programmes
 *
 */
void GestionMessageDialog::setProgrammes()
{
    unCheckAll();
    ui->programmesRadio->setChecked(true);
    ui->actuelLabel->setText(tr("Messages Programmés actuels :"));
    ui->ancienLabel->setText(tr("Anciens messages programmés :"));
    currentPath = "src/programmes";
    loadAudioFile(currentPath, modeleActuel, ui->actuelList);
    loadAudioFile(currentPath + "Ancien", modeleAncien, ui->ancienList);
}

/*!
 *  \fn void GestionMessageDialog::supprimerAncien()
 *  \brief appel la suppression des message ancien
 *
 */
void GestionMessageDialog::supprimerAncien()
{
    supprimer(modeleAncien,false);
}

/*!
 *  \fn void GestionMessageDialog::supprimerActuel()
 *  \brief appel la suppression des message actuel
 *
 */
void GestionMessageDialog::supprimerActuel()
{
    supprimer(modeleActuel,true);
}

/*!
 *  \fn void GestionMessageDialog::copieRightToLeft()
 *  \brief appel la copie des messages ancien vers actuel
 *
 */
void GestionMessageDialog::copieRightToLeft()
{
    copie(modeleAncien,false);
}

/*!
 *  \fn void GestionMessageDialog::copieRightToLeft()
 *  \brief appel la copie des messages actuel vers ancien
 *
 */
void GestionMessageDialog::copieLeftToRight()
{
    copie(modeleActuel,true);
}

