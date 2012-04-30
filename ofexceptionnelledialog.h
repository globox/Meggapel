#ifndef OFEXCEPTIONNELLEDIALOG_H
#define OFEXCEPTIONNELLEDIALOG_H

/*!
 *  \file ofexceptionnelledialog.h
 *  \brief Fenetre d'ouverture exceptionnelle
 */

#include <QDialog>
#include <QDate>
#include <QLocale>
#include <QCalendarWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QModelIndex>

#include "tools.h"

class QStandardItem;
class QStandardItemModel;

namespace Ui {
class OFExceptionnelleDialog;
}

/*! \class OFExceptionnelleDialog
 * \brief classe representant la fenetre d'ouverture exceptionnelle
 *
 *  La classe créer une fenetre permetant d'ajouter des ouverture exceptionnelle
 */
class OFExceptionnelleDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OFExceptionnelleDialog(QWidget *parent = 0);
    ~OFExceptionnelleDialog();
    
private:
    Ui::OFExceptionnelleDialog *ui; /*!< Interface graphique généer avec Qt Designer */
    void creerConnect();
    void updateList();
    QStandardItemModel *model;  /*!< modele de la liste des ouverture déjà crée */

private slots:
    void ajouterException();
    void supprimerException();
    void afficherException(QModelIndex index);
};

#endif // OFEXCEPTIONNELLEDIALOG_H
