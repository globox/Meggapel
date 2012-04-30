/*!
 *  \file messagefermeturewidget.cpp
 *  \class MessageFermetureWidget
 */
#include "messagefermeturewidget.h"
#include "ui_messagefermeturewidget.h"

#include <QComboBox>
#include <QSpinBox>

/*!
 *  \fn MessageFermetureWidget::MessageFermetureWidget(QWidget *parent) :
 *  \brief constructeur de la class MessageFermetureWidget
 *  \param[in] fichier Chemin vers le fichier de sauvegarde
 *  \param[in] parent Parent de l'application (toujours MessageOF)
 *
 */
MessageFermetureWidget::MessageFermetureWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MessageFermetureWidget)
{
    ui->setupUi(this);
    creerLists();
}

/*!
 *  \fn MessageFermetureWidget::~MessageFermetureWidget()
 *  \brief destrcuteur de la classe
 *
 */
MessageFermetureWidget::~MessageFermetureWidget()
{
    delete ui;
}

/*!
 *  \fn QStringList MessageFermetureWidget::getValue()
 *  \brief retourne l'état de chacun des champs
 *  \return QStringList état des champs sous forme de string
 *
 */
QStringList MessageFermetureWidget::getValue()
{
    QStringList values;
    QString var, value;
    for(int i=0; i<messageCBoxList.size();i++)
    {
        if(messageCBoxList.at(i)->currentIndex() != 0)
        {
            value = Tools::prepare(messageCBoxList.at(i)->currentText()) + " ";
            value += var.setNum(timeSpinList.at(i)->value()) + " ";
            values.append(value);
        }
    }
    values += "-";
    return values;
}

/*!
 *  \fn void MessageFermetureWidget::save()
 *  \brief sauvegarde l'etat des champs
 *
 *  sauvegarde l'état des champs dans un fichier de sauvegarde
 */

void MessageFermetureWidget::save()
{
    QFile fichier(Tools::absolutePathFile(this->fichier));
    if(fichier.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        QStringList values = getValue();
        foreach(QString value, values)
            flux << value << "\n";
        fichier.close();
    }
}

/*!
 *  \fn void MessageFermetureWidget::setFichier(QString fichier)
 *  \brief assigne un chemin vers le fichier de sauvegarde
 *
 */
void MessageFermetureWidget::setFichier(QString f)
{
    fichier = f;
    fullMessageComboBox();
    load();
}

/*!
 *  \fn void MessageFermetureWidget::load()
 *  \brief charge l'état des champs
 *
 *  charge l'état des champs situé dans le fichier de sauvegarde
 */
void MessageFermetureWidget::load()
{
    QFile fichier(Tools::absolutePathFile(this->fichier));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        QStringList values;
        for(int i=0; i<6 ;i++)
            values.append(flux.readLine());
        setValue(values);
        fichier.close();
    }
}

/*!
 *  \fn void MessageFermetureWidget::setValue(QStringList values)
 *  \brief assigne un état au différent champs
 *  \param[in] values état des champs
 *
 *  assigne pour chacun des champs le svaleur situé dans values
 *  chaque String correspond a un message
 */
void MessageFermetureWidget::setValue(QStringList values)
{
    int index = 0;
    QStringList splitValues;
    QString echapName;
    bool exist;
    foreach(QString value, values)
    {
        splitValues = value.split(" ");
        if(splitValues.size() == 3)
        {
            exist = false;
            echapName = Tools::unPrepare(splitValues.at(0));
            for(int i=1; i  < messageCBoxList.size();i++){
                messageCBoxList.at(index)->setCurrentIndex(i);
                if(messageCBoxList.at(index)->currentText() == echapName)
                {
                    exist=true;
                    break;
                }
            }
            if(exist)
            {
                timeSpinList.at(index)->setValue(splitValues.at(1).toInt());
                index ++;
            }
        }
    }
}

/*!
 *  \fn void MessageFermetureWidget::fullMessageComboBox()
 *  \brief remplit tous les ComboBox de Message
 *
 *  remplis les comboBox de temps avec les différents son situé dans le dossier
 */
void MessageFermetureWidget::fullMessageComboBox()
{
    QDir repertoire = Tools::cd("src/fermeture");
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.wav");

    for(int i=0; i<messageCBoxList.size();i++)
        messageCBoxList.at(i)->addItem(tr("aucun message"));

    /*!
     *  Parcours le répertoire src/fermeture à la recherche de wav
     */
    QString name;
    foreach(QFileInfo fileInfo, repertoire.entryInfoList())
    {
        name = fileInfo.fileName().split(".").at(0);
        for(int i=0; i<messageCBoxList.size();i++)
            messageCBoxList.at(i)->addItem(name);
    }
}

/*!
 *  \fn void MessageOuvertureWidget::creerLists()
 *  \brief Créer des Listes afin de simplifier l'utiliation
 *
 *  Créer des listes remplis des différent comboBox message
 *  et des spinBox afin qu'il soit plus facil a manipuler
 *  par la suite
 */
void MessageFermetureWidget::creerLists()
{
    // Création de la liste des comboBox de mesages
    messageCBoxList.append(ui->message1CBox);
    messageCBoxList.append(ui->message2CBox);
    messageCBoxList.append(ui->message3CBox);
    messageCBoxList.append(ui->message4CBox);
    messageCBoxList.append(ui->message5CBox);
    messageCBoxList.append(ui->message6CBox);

    //Création de la liste des spinBox
    timeSpinList.append(ui->time1Spin);
    timeSpinList.append(ui->time2Spin);
    timeSpinList.append(ui->time3Spin);
    timeSpinList.append(ui->time4Spin);
    timeSpinList.append(ui->time5Spin);
    timeSpinList.append(ui->time6Spin);
}
