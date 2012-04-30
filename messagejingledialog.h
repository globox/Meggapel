#ifndef MESSAGEJINGLEDIALOG_H
#define MESSAGEJINGLEDIALOG_H

/*!
 *  \file messagejingledialog.h
 */

#include <QDialog>
#include <QDir>
#include <QList>
#include <QTextStream>
#include <QFile>

#include "tools.h"

class QComboBox;

namespace Ui {
class MessageJingleDialog;
}

/*!
 *  \class MessageJingleDialog
 *  \brief Permet d'afficher la fenetre de gestion des jingles
 *
 */
class MessageJingleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MessageJingleDialog(QWidget *parent = 0);
    ~MessageJingleDialog();
    
private:
    Ui::MessageJingleDialog *ui; /*!< Interface graphique génénrer avec Qt Designer*/
    void fullComboBox();
    void creerList();
    QList<QComboBox*> comboBoxList; /*!< Liste des comboBox */
    QStringList getValue();
    void creerConnect();
    void setValue(QStringList values);
    void load();

private slots:
    void save();

signals:
    void jingleChange();
};

#endif // MESSAGEJINGLEDIALOG_H
