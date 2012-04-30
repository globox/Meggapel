#ifndef MESSAGEOUVERTUREWIDGET_H
#define MESSAGEOUVERTUREWIDGET_H

/*!
 *  \file messageouverture.h
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
class MessageOuvertureWidget;
}

/*!
 *  \class MessageOuvertureWidget
 *  \brief classe permettant de modifier les message d'ouverture
 */
class MessageOuvertureWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MessageOuvertureWidget(QWidget *parent = 0);
    ~MessageOuvertureWidget();
    QStringList getValue();
    void save();
    void setFichier(QString fichier);
private:
    void load();
    void setValue(QStringList values);
    Ui::MessageOuvertureWidget *ui;
    void fullTimeComboBox();
    void fullMessageComboBox();
    QString fichier; /*!< chemin vers le fichier de sauvegarde */
    void creerLists();
    QList<QComboBox*> timeCBoxList; /*!< Liste des combobox de temps*/
    QList<QComboBox*> messageCBoxList;/*!< Liste des combobox de message*/
    QList<QSpinBox*> timeSpinList;/*!< Liste des spinbox de temps*/
};

#endif // MESSAGEOUVERTUREWIDGET_H
