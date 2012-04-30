/*!
 *  \file messagesprogrammeswidget.cpp
 */
#include "messagesprogrammeswidget.h"
#include "ui_messagesprogrammeswidget.h"

#include <QDebug>

/*!
 *  \class MessagesProgrammesWidget
 *  \fn MessagesProgrammesWidget::MessagesProgrammesWidget(QWidget *parent)
 *  \brief Constructeur de la classe MessagesProgrammesWidget
 *  \param[in] parent Parent de l'application (toujours la MainWindow)
 */
MessagesProgrammesWidget::MessagesProgrammesWidget(QWidget *parent) :
    QWidget(parent), ui(new Ui::MessagesProgrammesWidget)
{
    ui->setupUi(this);
}

/*!
 *  \fn MessagesProgrammesWidget::~MessagesProgrammesWidget()
 *  \brief Desctructeur de la class
 *
 */
MessagesProgrammesWidget::~MessagesProgrammesWidget()
{
    delete ui;
    programmation.clear();
}

/*!
 *  \fn void MessagesProgrammesWidget::creerProgrammation(QList<QTime *> horaires)
 *  \brief Créer la programmation à partir des horaires;
 *  \param[in] horaires Horaires du magasin
 *
 */
void MessagesProgrammesWidget::creerProgrammation(QList<QTime *> &horaires)
{
    QStringList messages;
    QFile fichier(Tools::absolutePathFile("src/programmes.txt"));
    if(fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream flux(&fichier);
        while(flux.atEnd() == false)
            messages.append(flux.readLine());
        fichier.close();
    }
    QStringList values;
    foreach(QString message, messages)
    {
        qDebug()<<"\n";
        values = message.split(" ");
        if(values.at(0) == "oui")
        {
            if(isForToday(values.at(2),values.at(3)))
            {
                if(values.at(4) == "heure") creerProgrammationHeure(values, horaires);
                else if(values.at(4) == "intervalle")creerProgrammationIntervalle(values, horaires);
                else if(values.at(4) == "frequence")creerProgrammationFrequence(values, horaires);
                else if(values.at(4) == "aleatoire")creerProgrammationAleatoire(values, horaires);
            }
        }
    }
}

/*!
 *  \fn void MessagesProgrammesWidget::creerProgrammationHeure(QStringList &values, QList<QTime *> &horaires)
 *  \brief créer une programmation de type Heure
 *  \param[in] values   Valeurs du message
 *  \param[in] horaires horaires du magasin
 *
 *  La fonction va vérifier que le message est bien pendant les heures d'ouverture si oui il va
 *  l'ajouter a la programmaiton
 */
void MessagesProgrammesWidget::creerProgrammationHeure(QStringList &values, QList<QTime *> &horaires)
{
    QTime time(values.at(5).toInt(),values.at(6).toInt());
    for(int i=0; i<horaires.size();i+=2)
    {
        if(time.secsTo(*horaires.at(i)) < 0 && time.secsTo(*horaires.at(i+1)) > 0)
        {
            programmation.insertMulti(time,values.at(1));
        }
    }
}

/*!
 *  \fn void MessagesProgrammesWidget::creerProgrammationAleatoire(QStringList &values, QList<QTime *> &horaires)
 *  \brief créer une programmation de type Aleatoire
 *  \param[in] values   Valeurs du message
 *  \param[in] horaires horaires du magasin
 *
 *  La fonction va calculer un nombe aleatoire entre 45 et 75 qu'il va ajouter en partant de l'heure
 *  d'ouverture.
 */
void MessagesProgrammesWidget::creerProgrammationAleatoire(QStringList &values, QList<QTime *> &horaires)
{
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    for(int i=0; i<horaires.size();i+=2)
    {
        time.setHMS(horaires.at(i)->hour(),horaires.at(i)->minute(),0);
        time = time.addSecs(((rand() % (30)) + 45) * 60);
        while(time.secsTo(*horaires.at(i+1)) > 0)
        {
            programmation.insertMulti(time,values.at(1));
            time = time.addSecs(((rand() % (30)) + 45) * 60);
        }
    }
}

/*!
 *  \fn void MessagesProgrammesWidget::creerProgrammationIntervalle(QStringList &values, QList<QTime *> &horaires)
 *  \brief créer une programmation de type Intervalle
 *  \param[in] values   Valeurs du message
 *  \param[in] horaires horaires du magasin
 *
 *  La fonction va calculer les horaires de programmation en fonction de l'intervalle donnée
 */
void MessagesProgrammesWidget::creerProgrammationIntervalle(QStringList &values, QList<QTime *> &horaires)
{
    QTime debut;
    debut.setHMS(values.at(5).toInt(),values.at(6).toInt(),0);
    QTime fin;
    fin.setHMS(values.at(7).toInt(),values.at(8).toInt(),0);
    QTime time;
    int seconds = values.at(9).toInt() * 60;
    for(int i=0; i<horaires.size();i+=2)
    {
        if(debut.secsTo(*horaires.at(i+1)) > 0)
        {
            if(debut.secsTo(*horaires.at(i)) > 0)
                time = horaires.at(i)->addSecs(seconds);
            else
                time = debut.addSecs(seconds);

            while(time.secsTo(fin) > 0 && time.secsTo(*horaires.at(i+1)) > 0)
            {
                programmation.insertMulti(time,values.at(1));
                time = time.addSecs(seconds);
            }
        }
    }
}

void MessagesProgrammesWidget::creerProgrammationFrequence(QStringList &values, QList<QTime *> &horaires)
{
    QTime debut;
    debut.setHMS(values.at(5).toInt(),values.at(6).toInt(),0);
    QTime fin;
    fin.setHMS(values.at(7).toInt(),values.at(8).toInt(),0);
    QTime time1, time2, time;
    int seconds = 0;

    for(int i=0; i<horaires.size();i+=2)
    {
        (debut.secsTo(*horaires.at(i)) > 0 ? time1 = *horaires.at(i) : time1 = debut);
        (fin.secsTo(*horaires.at(i+1)) > 0 ? time2 = fin : time2 = *horaires.at(i+1));
        seconds += time1.secsTo(time2);
    }
    seconds = seconds / (values.at(9).toInt()+1);

    int count = 0;
    for(int i=0; i<horaires.size();i+=2)
    {
        if(debut.secsTo(*horaires.at(i+1)) > 0)
        {
            if(debut.secsTo(*horaires.at(i)) > 0)
                time = horaires.at(i)->addSecs(seconds);
            else
                time = debut.addSecs(seconds);

            while(time.secsTo(fin) > 0 && time.secsTo(*horaires.at(i+1)) > 0)
            {
                programmation.insertMulti(time,values.at(1));
                time = time.addSecs(seconds);
                count ++;
            }
        }
    }
    if(horaires.size() == 4 && count < values.at(9).toInt())
    {
        programmation.insertMulti(horaires.at(1)->addSecs(-180),values.at(1));
        if((values.at(9).toInt() - count) % 2 == 0)
            programmation.insertMulti(horaires.at(3)->addSecs(-180),values.at(1));
    }
}

/*!
 *  \fn void MessagesProgrammesWidget::ordonnerProgrammation()
 *  \brief fait en sorte qu'il n y ai pas de message au même moment
 *  il faut au moins une minutes entre chaque message.
 */
void MessagesProgrammesWidget::ordonnerProgrammation()
{
    QList<QTime> problemKey;
    foreach(QTime key, programmation.uniqueKeys())
    {
        if(programmation.values(key).size() != 1)
        {
            problemKey.append(key);
        }
    }
    if(problemKey.size()!= 0)
    {
        foreach(QTime key, problemKey)
        {
            programmation.insert(key.addSecs(60),programmation.take(key));
        }
        ordonnerProgrammation();
    }
}

void MessagesProgrammesWidget::fullList()
{
    QMapIterator<QTime,QString> i(programmation);
    while(i.hasNext())
    {
        i.next();
    }
}

/*!
 *  \fn bool MessagesProgrammesWidget::isForToday(QString debut, QString fin)
 *  \brief vérifie si la programmation doit etre diffusé aujoruhdui
 *  \param[in] debut Date de début de la programmation
 *  \param[in] fin date de fin de la programmation
 */
bool MessagesProgrammesWidget::isForToday(QString debut, QString fin)
{
    QDate date;
    QStringList splitDate = debut.split("/");
    date.setYMD(splitDate.at(2).toInt(), splitDate.at(1).toInt(), splitDate.at(0).toInt());
    if(date.daysTo(QDate::currentDate()) > 0 )return false;
    splitDate = fin.split("/");
    date.setYMD(splitDate.at(2).toInt(), splitDate.at(1).toInt(), splitDate.at(0).toInt());
    if(date.daysTo(QDate::currentDate()) > 0 )return false;
    return true;
}

/*!
 *  \fn void MessagesProgrammesWidget::receiveHoraires(QList<QTime *> horaires)
 *  \brief Slot recevant les nouveau horaires du magasin
 *  \param[in] horaires Nouveau horaires du magasin
 *
 */
void MessagesProgrammesWidget::receiveHoraires(QList<QTime *> horaires)
{
    qDebug()<<"\n\n";
    programmation.clear();
    if(horaires.size() == 0)return;
    creerProgrammation(horaires);
    ordonnerProgrammation();
    fullList();
}
