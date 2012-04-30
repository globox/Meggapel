#ifndef OFMAGASINDIALOG_H
#define OFMAGASINDIALOG_H

/*!
 *  \file ofmagasindialog.h
 *  \brief Fenetre de gestion des ouvertures et fermetures du magasin
 */

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "ofinterfacewidget.h"
#include "tools.h"

class OFExceptionnelleDialog;

namespace Ui {
class OFMagasinDialog;
}

/*! \class OFMagasinDialog
 * \brief classe representant la gestion des ouvertures et fermetures du magasin
 *
 *  La classe créer une fenetre de gestion des ouvertures et fermetures du magasin
 */
class OFMagasinDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OFMagasinDialog(QWidget *parent = 0);
    ~OFMagasinDialog();
    
private:
    Ui::OFMagasinDialog *ui;    /*!< Interface graphique généer avec Qt Designer */
    void setOF();
    void creerConnect();

private slots:
    void copieDay();
    void save();
    void openExceptionnelle();

signals:
    void nouveauxHoraires();
};

#endif // OFMAGASINDIALOG_H
