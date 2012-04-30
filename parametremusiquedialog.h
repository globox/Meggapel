#ifndef PARAMETREMUSIQUEDIALOG_H
#define PARAMETREMUSIQUEDIALOG_H

/*!
 *  \file parametremusiquedialog.h
 *  \brief Fenetre permettant de controler les options du lecteur audio
 */

#include <QDialog>
#include <QFile>
#include <QTextStream>

#include "tools.h"

namespace Ui {
class ParametreMusiqueDialog;
}

/*! \class ParametreMusiqueDialog
 * \brief classe representant la fenetre permettant de controler les options du lecteur audio
 *
 *  La classe créer une fenetre permettant de controler les options du lecteur audio
 */
class ParametreMusiqueDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ParametreMusiqueDialog(QWidget *parent = 0);
    ~ParametreMusiqueDialog();
    static QString getHoraires();
    
private:
    Ui::ParametreMusiqueDialog *ui;     /*!< Interface graphique généer avec Qt Designer */
    void creerConnect();
    void setHoraires(QStringList horaires);
    void setSources();

private slots:
    void sourcesChanged();
    void save();
    void setRandom(bool random);

signals :
    void externeMusique();
    void interneMusique();
    void cdromMusique();
    void randomChanged(bool random);
    void parametreChanges();
};

#endif // PARAMETREMUSIQUEDIALOG_H
