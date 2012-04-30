#ifndef INFODUJOURDIALOG_H
#define INFODUJOURDIALOG_H

/*!
 *  \file infodujour.h
 *  \brief affiche le saint du jour
 */

#include <QDialog>
#include <QDate>
#include <QFile>
#include <QTextStream>
#include "tools.h"

namespace Ui {
class InfoDuJourDialog;
}

/*! \class InfoDuJourDialog
 * \brief classe representant la fenetre qui affiche le saint du jour
 */
class InfoDuJourDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InfoDuJourDialog(QWidget *parent = 0);
    ~InfoDuJourDialog();
    
private:
    Ui::InfoDuJourDialog *ui; /*!< Interface graphique généer avec Qt Designer */
    void setAffichage();
};

#endif // INFODUJOURDIALOG_H
