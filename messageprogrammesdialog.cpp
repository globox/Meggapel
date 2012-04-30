/*!
 *  \file messageprogrammesdialog.cpp
 */
#include "messageprogrammesdialog.h"
#include "ui_messageprogrammesdialog.h"

#include <QStandardItemModel>
#include "ajoutmessageprogrammedialog.h"

/*!
 *  \class MessageProgrammesDialog
 *  \fn MessageProgrammesDialog::MessageProgrammesDialog(QWidget *parent)
 *  \brief constructeur de la classe MessageProgrammesDialog
 *  \param[in] parent Parent de l'application (toujorus la MainWindow)
 *
 */
MessageProgrammesDialog::MessageProgrammesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageProgrammesDialog)
{
    ui->setupUi(this);
    creerConnect();
    loadTable();
}

/*!
 *  \fn MessageProgrammesDialog::~MessageProgrammesDialog()
 *  \brief Destructeur de la classe
 *
 */
MessageProgrammesDialog::~MessageProgrammesDialog()
{
    delete ui;
    delete modele;
}

/*!
 *  \fn void MessageProgrammesDialog::creerConnect()
 *  \brief Créer les connections entre les différents composant du widget
 *
 */
void MessageProgrammesDialog::creerConnect()
{
    connect(ui->addMessageButton,SIGNAL(clicked()),this,SLOT(creerMessage()));
    connect(ui->messageTable,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(checkRow(QModelIndex)));
    connect(ui->supprimerButton,SIGNAL(clicked()),this,SLOT(supprimerSelect()));
    connect(ui->supprimerAllButton,SIGNAL(clicked()),this,SLOT(supprimerAll()));
    connect(ui->modifierButton,SIGNAL(clicked()),this,SLOT(modifierSelect()));
}

/*!
 *  \fn void MessageProgrammesDialog::fullTable(QStringList value)
 *  \brief Remplis le tableau centrale avec les valeur contenues dans value
 *  \param[in] value Liste de valeur correspondant a chacune des ligne du tableau
 *
 */
void MessageProgrammesDialog::fullTable(QStringList value)
{
    QStringList splitValue;
    QTime time;
    for(int i=0; i<value.size();i++)
    {
        /*!
         *  On découpe chacune des valeur sur les " " et on remplit les cases
         *  du tableau avec.
         */
        splitValue = value.at(i).split(" ");
        /* colonne selection */
        modele->setItem(i,0, new QStandardItem());
        modele->item(i,0)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        modele->item(i,0)->setData(QVariant(Qt::Unchecked), Qt::CheckStateRole);
        /* Colonne Date de début*/
        modele->setItem(i,1, new QStandardItem(splitValue.at(2)));
        /* Colonne Date de fin */
        modele->setItem(i,2, new QStandardItem(splitValue.at(3)));
        /* Colonne Message */
        modele->setItem(i,9, new QStandardItem(Tools::unPrepare(splitValue.at(1))));
        /* Colonne Heure de début  */
        modele->setItem(i,3, new QStandardItem());
        /* Colonne Heure de fin */
        modele->setItem(i,4, new QStandardItem());
        /* Colonne Frequence */
        modele->setItem(i,5, new QStandardItem());
        if(splitValue.at(4) == "frequence")
        {
            modele->item(i,5)->setText(splitValue.at(9));
            time.setHMS(splitValue.at(5).toInt(),splitValue.at(6).toInt(),0);
            modele->item(i,3)->setText(time.toString("hh:mm"));
            time.setHMS(splitValue.at(7).toInt(),splitValue.at(8).toInt(),0);
            modele->item(i,4)->setText(time.toString("hh:mm"));
        }
        /* Colonne Heure */
        modele->setItem(i,6, new QStandardItem());
        if(splitValue.at(4) == "heure")
        {
            time.setHMS(splitValue.at(5).toInt(),splitValue.at(6).toInt(),0);
            modele->item(i,6)->setText(time.toString("hh:mm"));
        }
        /* Colonne Intervalle */
        modele->setItem(i,7, new QStandardItem());
        if(splitValue.at(4) == "intervalle")
        {
            modele->item(i,7)->setText(splitValue.at(9));
            time.setHMS(splitValue.at(5).toInt(),splitValue.at(6).toInt(),0);
            modele->item(i,3)->setText(time.toString("hh:mm"));
            time.setHMS(splitValue.at(7).toInt(),splitValue.at(8).toInt(),0);
            modele->item(i,4)->setText(time.toString("hh:mm"));
        }
        /* Colonne Aléatoire */
        modele->setItem(i,8, new QStandardItem());
        modele->item(i,8)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        modele->item(i,8)->setData(QVariant(Qt::Unchecked), Qt::CheckStateRole);
        if(splitValue.at(4) == "aleatoire")
            modele->item(i,8)->setData(QVariant(Qt::Checked), Qt::CheckStateRole);
        /* Colonne Activé  */
        modele->setItem(i,10, new QStandardItem());
        modele->item(i,10)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        modele->item(i,10)->setData(QVariant(Qt::Unchecked), Qt::CheckStateRole);
        if(splitValue.at(0) == "oui")
            modele->item(i,10)->setData(QVariant(Qt::Checked), Qt::CheckStateRole);
    }
}

/*!
 *  \fn void MessageProgrammesDialog::configView()
 *  \brief parametre d'apparence du modèle et de la vue.
 *
 */
void MessageProgrammesDialog::configView()
{
    QStringList header;
    header<<"Sélection"<<"Début"<< "Fin"<<"H. début"<<"H. fin";
    header<<"Freq"<<"Heure"<<"Intervalle"<<"Aléatoire"<<"Message"<<"Activé";
    modele->setHorizontalHeaderLabels(header);
    ui->messageTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->messageTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->messageTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->messageTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    for(int i=0; i<modele->columnCount(); i++)
        for(int j=0; j<modele->rowCount(); j++)
            modele->item(j,i)->setTextAlignment(Qt::AlignHCenter);
}

/*!
 *  \fn QString MessageProgrammesDialog::getValueFromRow(int row)
 *  \brief Récupère les données du tableau et en fait une chaine de caractère
 *  \param[in] row Ligne du tableau à récuprerer
 *  \returns la valeur des champs de la ligne du tableau
 *
 */
QString MessageProgrammesDialog::getValueFromRow(int row)
{
    QString value = "";
    QString str;
    QTime time;
    (modele->item(row,10)->data(Qt::CheckStateRole) == QVariant(Qt::Checked) ?
                value +="oui " : value += "non ");
    value += Tools::prepare(modele->item(row,9)->text()) + " ";
    value += modele->item(row,1)->text() + " ";
    value += modele->item(row,2)->text() + " ";
    if(modele->item(row,8)->data(Qt::CheckStateRole) == QVariant(Qt::Checked))
        value += "aleatoire";
    else
    {
        str = modele->item(row,6)->text();
        if(str != "")
        {
            value +="heure "+ str.split(":").at(0) + " " + str.split(":").at(1);
        }
        else
        {
            if(modele->item(row,5)->text() != "")
                value += "frequence ";
            else
                value += "intervalle ";
            int h = modele->item(row,3)->text().split(":").at(0).toInt();
            int m = modele->item(row,3)->text().split(":").at(1).toInt();
            time.setHMS(h,m,0);
            value += time.toString("h m ");
            h = modele->item(row,4)->text().split(":").at(0).toInt();
            m = modele->item(row,4)->text().split(":").at(1).toInt();
            time.setHMS(h,m,0);
            value += time.toString("h m ");
            if(modele->item(row,5)->text() != "")
                value += modele->item(row,5)->text();
            else
                value += modele->item(row,7)->text();
        }
    }
    return value;
}

/*!
 *  \fn void MessageProgrammesDialog::loadTable(bool newMessage)
 *  \bief remplit le tableau en exploitant les données écrite dans le fichier programmes.txt
 *  \param[in] modification Indique si le chargement est du au chargement de la page ou a une modification
 *
 *  Si modification est vrai alors c'est qu'il y a eu du changement dans la programmation le
 *  programme envoi donc un signal afin de prévenir la fenetre principale.
 */
void MessageProgrammesDialog::loadTable(bool modification)
{
    if(modification) emit messageChanged();
    QStringList value;
    QFile fichier(Tools::absolutePathFile("src/programmes.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        while(flux.atEnd() == false)
            value.append(flux.readLine());
        fichier.close();
    }
    modele = new QStandardItemModel(value.size(),11,this);
    fullTable(value);
    configView();
    ui->messageTable->setModel(modele);
}

/*!
 *  \fn void MessageProgrammesDialog::checkRow(QModelIndex index)
 *  \brief change l'état "sélectionner ou non"  d'une ligne lorsque l on double clic dessus
 *  \param[in] index indique la ligne du tableau ou le double clic a eu lieu
 *
 */
void MessageProgrammesDialog::checkRow(QModelIndex index)
{
    QStandardItem *item = modele->item(index.row(),0);
    if(item->data(Qt::CheckStateRole) == QVariant(Qt::Checked))
        item->setData(QVariant(Qt::Unchecked), Qt::CheckStateRole);
    else
        item->setData(QVariant(Qt::Checked), Qt::CheckStateRole);
}

/*!
 *  \fn void MessageProgrammesDialog::supprimerSelect()
 *  \brief supprimer toutes les lignes cocher
 *
 */
void MessageProgrammesDialog::supprimerSelect()
{
    /*!
     *  Le logiciel cherche les ligne cocher
     */
    QList<int> rowSelect;
    for(int i=0; i<modele->rowCount(); i++)
        if(modele->item(i,0)->data(Qt::CheckStateRole) == QVariant(Qt::Checked))
            rowSelect.append(i);
    QStringList values;
    /*!
     *  puis récuprère les contenu du fichier programmes.txt
     */
    QFile fichier(Tools::absolutePathFile("src/programmes.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        while(flux.atEnd() == false)
            values.append(flux.readLine());
        fichier.close();
    }
    foreach(int row, rowSelect)
        values.removeAt(row);
    /*!
     *  pour ensuite restituer le contenu avec les ligne cocher en moins.
     */
    if(fichier.open(QIODevice::WriteOnly |QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        foreach(QString value, values)
            flux << value <<"\n";
        fichier.close();
    }
    /*!
     *  pius rappeler le chargement du tableau
     */
    loadTable();
}

/*!
 *  \fn void MessageProgrammesDialog::supprimerAll()
 *  \brief supprime tout le contenu du fichier de sauvegarde
 *
 *  supprime tout le contenu du fichier de sauvegarde puis redemande l'affichage du tableau
 */
void MessageProgrammesDialog::supprimerAll()
{
    QFile fichier(Tools::absolutePathFile("src/programmes.txt"));
    if(fichier.open(QIODevice::WriteOnly |QIODevice::Truncate | QIODevice::Text))
    {
        fichier.close();
    }
    loadTable();
}

/*!
 *  \fn void MessageProgrammesDialog::modifierSelect()
 *  \brief appele l'interface de création de message programmes afin de pouvoir modifier le mesage
 *  sélectionné
 *
 */
void MessageProgrammesDialog::modifierSelect()
{
    /*!
     *  Avant de lancer la modification la fenetre vérifie qu'il n y ai bien qu'une
     *  ligne de sélectionné sinon elle affiche un message d'erreur
     */
    QList<int> rowSelect;
    for(int i=0; i<modele->rowCount(); i++)
        if(modele->item(i,0)->data(Qt::CheckStateRole) == QVariant(Qt::Checked))
            rowSelect.append(i);
    if(rowSelect.size() == 0)
    {
        QMessageBox::warning(this,tr("Erreur de sélection"),
                             tr("Vous devez sélectionner un message si vous voulez pouvoir le modifier"),
                             QMessageBox::Ok);
        return;
    }
    if(rowSelect.size() > 1)
    {
        QMessageBox::warning(this,tr("Erreur de sélection"),
                             tr("Vous ne pouvez pas modifier plusieurs programmes à la fois")
                             +"\n"+tr("Veuillez sélectionner un seul programme"),
                             QMessageBox::Ok);
        return;
    }
    /*!
     *  Si tout est correction on appelle la fenetre de création
     */
    QString value = getValueFromRow(rowSelect.at(0));
    AjoutMessageProgrammeDialog *ajout = new AjoutMessageProgrammeDialog(value,rowSelect.at(0),this);
    ajout->show();
    connect(ajout,SIGNAL(newMessage(bool)),this,SLOT(loadTable(bool)));
}

/*!
 *  \fn void MessageProgrammesDialog::creerMessage()
 *  \brief Appelle la fenetre de création de message programmés
 *
 */
void MessageProgrammesDialog::creerMessage()
{
    AjoutMessageProgrammeDialog *ajout = new AjoutMessageProgrammeDialog(this);
    ajout->show();
    connect(ajout,SIGNAL(newMessage(bool)),this,SLOT(loadTable(bool)));
}
