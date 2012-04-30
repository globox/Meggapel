#ifndef MESSAGEFERMETUREWIDGET_H
#define MESSAGEFERMETUREWIDGET_H

/*!
 *  \file messagefermeturewidget.h
 */
#include <QWidget>
#include <QList>
#include <QDir>
#include <QFile>
#include <QTextStream>

#include "tools.h"

class QComboBox;
class QSpinBox;

namespace Ui {
class MessageFermetureWidget;
}

/*!
 *  \class MessageFermetureWidget
 *  \brief classe permettant de modifier les message d'ouverture
 */
class MessageFermetureWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageFermetureWidget(QWidget *parent = 0);
    ~MessageFermetureWidget();
    QStringList getValue();
    void save();
    void setFichier(QString f);
    
private:
    void setValue(QStringList values);
    void load();
    Ui::MessageFermetureWidget *ui;
    QString fichier; /*!< chemin vers le fichier de sauvegarde */
    void fullMessageComboBox();
    void creerLists();
    QList<QComboBox*> messageCBoxList;/*!< Liste des combobox de message*/
    QList<QSpinBox*> timeSpinList;/*!< Liste des spinbox de temps*/
};

#endif // MESSAGEFERMETUREWIDGET_H
