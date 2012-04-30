#ifndef MESSAGEPROGRAMMESDIALOG_H
#define MESSAGEPROGRAMMESDIALOG_H

/*!
 *  \file messageprogrammesdialog.h
 */

#include <QDialog>
#include <QDir>
#include <QList>
#include <QTime>
#include <QMessageBox>
#include <QModelIndex>
#include "tools.h"

class QStandardItemModel;
class AjoutMessageProgrammeDialog;

namespace Ui {
class MessageProgrammesDialog;
}

/*!
 *  \class MessageProgrammesDialog
 *  \brief Fenetre permettant la consultation et la modification des messages programmes
 */
class MessageProgrammesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit MessageProgrammesDialog(QWidget *parent = 0);
    ~MessageProgrammesDialog();
    
private:
    Ui::MessageProgrammesDialog *ui;    /*!< Interface graphique généré avec Qt Desgner*/
    void creerConnect();
    QStandardItemModel *modele;     /*!< Contenu du tableua centrale*/
    void fullTable(QStringList value);
    void configView();
    QString getValueFromRow(int row);

private slots:
    void creerMessage();
    void loadTable(bool modification = false);
    void checkRow(QModelIndex index);
    void supprimerSelect();
    void supprimerAll();
    void modifierSelect();

signals:
    void messageChanged();
};

#endif // MESSAGEPROGRAMMESDIALOG_H
