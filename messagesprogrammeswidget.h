#ifndef MESSAGESPROGRAMMESWIDGET_H
#define MESSAGESPROGRAMMESWIDGET_H

/*!
 *  \file messagesprogrammeswidget.h
 */
#include <QWidget>
#include <QList>
#include <QTime>
#include <QFile>
#include <QDate>
#include <QTextStream>
#include <QMap>

#include "tools.h"

namespace Ui {
class MessagesProgrammesWidget;
}

/*!
 *  \class MessagesProgrammesWidget
 *  \brief Classe permettant la progrmmation des messages durant la journée
 */
class MessagesProgrammesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessagesProgrammesWidget(QWidget *parent = 0);
    ~MessagesProgrammesWidget();
    
private:
    Ui::MessagesProgrammesWidget *ui;   /*!< Interface graphique génénré avec QT Designer */
    QMap<QTime,QString> programmation;  /*!< Programmtion de la journée */
    void creerProgrammation(QList<QTime *> &horaires);
    void creerProgrammationHeure(QStringList &values, QList<QTime *> &horaires);
    void creerProgrammationAleatoire(QStringList &values,QList<QTime *> &horaires);
    void creerProgrammationIntervalle(QStringList &values, QList<QTime *> &horaires);
    void creerProgrammationFrequence(QStringList &values, QList<QTime *> &horaires);
    void ordonnerProgrammation();
    void fullList();

    bool isForToday(QString debut, QString fin);
private slots:
    void receiveHoraires(QList<QTime*> horaires);
};

#endif // MESSAGESPROGRAMMESWIDGET_H
