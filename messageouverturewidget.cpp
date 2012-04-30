/*!
 *  \file messageouverturewidget.cpp
 *  \class MessageOuvertureWidget
 */
#include "messageouverturewidget.h"
#include "ui_messageouverturewidget.h"

#include <QComboBox>
#include <QSpinBox>

/*!
 *  \fn MessageOuvertureWidget::MessageOuvertureWidget(QWidget *parent) :
 *  \brief constructeur de la class MessageOuvertureWidget
 *  \param[in] parent Parent de l'application (toujours MessageOF)
 *  \param[in] fichier Chemin vers le fichier de sauvegarde
 *
 */
MessageOuvertureWidget::MessageOuvertureWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MessageOuvertureWidget),fichier("")
{
    ui->setupUi(this);
    creerLists();
}

/*!
 *  \fn MessageOuvertureWidget::~MessageOuvertureWidget()
 *  \brief destrcuteur de la classe
 *
 */
MessageOuvertureWidget::~MessageOuvertureWidget()
{
    delete ui;
}

/*!
 *  \fn QStringList MessageOuvertureWidget::getValue()
 *  \brief retourne l'état de chacun des champs
 *  \return QStringList état des champs sous forme de string
 *
 */
QStringList MessageOuvertureWidget::getValue()
{
    QStringList values;
    QString var, value;
    for(int i=0; i<timeCBoxList.size();i++)
    {
        if(messageCBoxList.at(i)->currentIndex() != 0)
        {
            value = Tools::prepare(messageCBoxList.at(i)->currentText()) + " ";
            value += var.setNum(timeSpinList.at(i)->value()) + " ";
            (timeCBoxList.at(i)->currentIndex()==0 ? value += "-" : value += "+");
            values.append(value);
        }
    }
    return values;
}

/*!
 *  \fn void MessageOuvertureWidget::save()
 *  \brief sauvegarde l'etat des champs
 *
 *  sauvegarde l'état des champs dans un fichier ouvertureMessage.txt
 */
void MessageOuvertureWidget::save()
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
 *  \fn void MessageOuvertureWidget::load()
 *  \brief charge l'état des champs
 *
 *  charge l'état des champs situé dans le fichier ouvertureMessage.txt
 */
void MessageOuvertureWidget::load()
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
 *  \fn void MessageOuvertureWidget::setValue(QStringList values)
 *  \brief assigne un état au différent champs
 *  \param[in] values état des champs
 *
 *  assigne pour chacun des champs le svaleur situé dans values
 *  chaque String correspond a un message
 */
void MessageOuvertureWidget::setValue(QStringList values)
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
                if(splitValues.at(2) == "-")
                    timeCBoxList.at(index)->setCurrentIndex(0);
                else
                    timeCBoxList.at(index)->setCurrentIndex(1);
                index ++;
            }
        }
    }
}

/*!
 *  \fn void MessageOuvertureWidget::setFichier(QString fichier)
 *  \brief assigne un chemin vers le fichier de sauvegarde
 *
 */
void MessageOuvertureWidget::setFichier(QString fichier)
{
    this->fichier = fichier;
    fullTimeComboBox();
    fullMessageComboBox();
    load();
}

/*!
 *  \fn void MessageOuvertureWidget::fullTimeComboBox()
 *  \brief remplit tous les ComboBox de temps
 *
 *  remplis les comboBox de temps par "minute avant/après ouverture"
 */
void MessageOuvertureWidget::fullTimeComboBox()
{
    for(int i=0; i<timeCBoxList.size();i++)
    {
        timeCBoxList.at(i)->addItem(tr("minutes avant ouverture"));
        timeCBoxList.at(i)->addItem(tr("minutes après ouverture"));
    }
}

/*!
 *  \fn void MessageOuvertureWidget::fullMessageComboBox()
 *  \brief remplit tous les ComboBox de Message
 *
 *  remplis les comboBox de temps avec les différents son situé dans le dossier
 */
void MessageOuvertureWidget::fullMessageComboBox()
{
    QDir repertoire = Tools::cd("src/ouverture");
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.wav");

    for(int i=0; i<messageCBoxList.size();i++)
        messageCBoxList.at(i)->addItem(tr("aucun message"));

    /*!
     *  Parcours le répertoire src/ouverture à la recherche de wav
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
 *  Créer des listes remplis des différent comboBox de temps
 *  ou message et des spinBox afin qu'il soit plus facil a manipuler
 *  par la suite
 */
void MessageOuvertureWidget::creerLists()
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

    // Création de la liste des comboBox de temps
    timeCBoxList.append(ui->time1CBox);
    timeCBoxList.append(ui->time2CBox);
    timeCBoxList.append(ui->time3CBox);
    timeCBoxList.append(ui->time4CBox);
    timeCBoxList.append(ui->time5CBox);
    timeCBoxList.append(ui->time6CBox);
}
