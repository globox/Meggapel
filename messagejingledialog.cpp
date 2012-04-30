/*!
 *  \file messagejingledialog.cpp
 */
#include "messagejingledialog.h"
#include "ui_messagejingledialog.h"
#include <QComboBox>

/*!
 *  \class MessageJingleDialog
 *  \fn MessageJingleDialog::MessageJingleDialog(QWidget *parent) :
 *  \brief constructeur de la classe MessageJingleDialog
 *  \param[in] parent Parent de la fenetre (toujours la MainWindow)
 */
MessageJingleDialog::MessageJingleDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::MessageJingleDialog)
{
    ui->setupUi(this);
    creerList();
    fullComboBox();
    load();
    creerConnect();
}

/*!
 *  \fn MessageJingleDialog::~MessageJingleDialog()
 *  \brief destructeur de la classe
 *
 */
MessageJingleDialog::~MessageJingleDialog()
{
    delete ui;
}

/*!
 *  \fn MessageJingleDialog::fullComboBox()
 *  \brief remplit les combo box avec le contenu du dossier
 *
 */
void MessageJingleDialog::fullComboBox()
{
    QDir repertoire = Tools::cd("src/jingle");
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.wav");

    for(int i=0; i<comboBoxList.size();i++)
        comboBoxList.at(i)->addItem(tr("aucun message"));

    /*!
     *  Parcours le répertoire src/fermeture à la recherche de wav
     */
    QString name;
    foreach(QFileInfo fileInfo, repertoire.entryInfoList())
    {
        name = fileInfo.fileName().split(".").at(0);
        for(int i=0; i<comboBoxList.size();i++)
            comboBoxList.at(i)->addItem(name);
    }
}

/*!
 *  \fn MessageJingleDialog::creerList()
 *  \brief  Créer une liste comprenant les combo box.
 *
 *  Créer une liste comprenant les combo box de la fenetre de facon a ce que ce soit plus facil a utiliser.
 */
void MessageJingleDialog::creerList()
{
    comboBoxList.append(ui->ouvertureComboBox);
    comboBoxList.append(ui->femetureComboBox);
    comboBoxList.append(ui->promotionComboBox);
    comboBoxList.append(ui->programmesComboBox);
    comboBoxList.append(ui->envoiComboBox);
}

/*!
 *  \fn QStringList MessageJingleDialog::getValue()
 *  \brief retourne l'état de chacun des champs
 *  \return QStringList état des champs sous forme de string
 *
 */
QStringList MessageJingleDialog::getValue()
{
    QStringList values;
    QString value;
    for(int i=0; i<comboBoxList.size();i++)
    {
        if(comboBoxList.at(i)->currentIndex() != 0)
            value = Tools::prepare(comboBoxList.at(i)->currentText());
        else
            value = "";
        values.append(value);
    }
    return values;
}

/*!
 *  \fn void MessageJingleDialog::creerConnect()
 *  \brief Creer les connection
 *
 */
void MessageJingleDialog::creerConnect()
{
    connect(ui->saveButton,SIGNAL(clicked()),this,SLOT(save()));
    connect(ui->closeSaveButton,SIGNAL(clicked()),this,SLOT(save()));
}

/*!
 *  \fn void MessageJingleDialog::save()
 *  \brief sauvegarde l'etat des champs
 *
 *  sauvegarde l'état des champs dans un fichier de sauvegarde
 */
void MessageJingleDialog::save()
{
    QFile fichier(Tools::absolutePathFile("src/jingle.txt"));
    if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QStringList values = getValue();
        QTextStream flux(&fichier);
        foreach(QString value, values)
            flux << value << "\n";
        fichier.close();
    }
    emit jingleChange();
}

/*!
 *  \fn void MessageJingleDialog::load()
 *  \brief charge l'état des champs
 *
 *  charge l'état des champs situé dans le fichier de sauvegarde
 */
void MessageJingleDialog::load()
{
    QFile fichier(Tools::absolutePathFile("src/jingle.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        QStringList values;
        for(int i=0; i<5 ;i++)
            values.append(flux.readLine());
        setValue(values);
        fichier.close();
    }
}

/*!
 *  \fn void MessageJingleDialog::setValue(QStringList values)
 *  \brief assigne un état au différent champs
 *  \param[in] values état des champs
 *
 *  assigne pour chacun des champs le svaleur situé dans values
 *  chaque String correspond a un message
 */
void MessageJingleDialog::setValue(QStringList values)
{
    int index = 0;
    QString echapName;
    bool find;
    foreach(QString value, values)
    {
        find = false;
        if(value != "")
        {
            echapName = Tools::unPrepare(value);
            for(int i=1; i<comboBoxList.size();i++)
            {
                comboBoxList.at(index)->setCurrentIndex(i);
                if(comboBoxList.at(index)->currentText() == echapName){
                    find = true;
                    break;
                }
            }
            if(!find) comboBoxList.at(index)->setCurrentIndex(0);
        }
        index ++;
    }
}

