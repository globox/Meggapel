#ifndef MESSAGEOF_H
#define MESSAGEOF_H

/*!
 *  \file messageof.h
 */

#include <QDialog>
#include <QFile>
#include <QTextStream>

#include "tools.h"

namespace Ui {
class MessageOF;
}

/*!
 *  \class MessageOF
 *  \brief classe permettant de modifier les message d'ouverture ou fermeture
 */
class MessageOF : public QDialog
{
    Q_OBJECT
    
public:
    explicit MessageOF(QWidget *parent = 0);
    ~MessageOF();
    
private:
    Ui::MessageOF *ui;
    void creerConnect();
    void setFichier();
    void setSame();
    bool isSame; /*!< on utilise les meme fichier le matin et l aprem ou non */

private slots:
    void saveValues();
    void showAffichage();
    void hideAffichage();

signals:
    void messagesChanges();
};

#endif // MESSAGEOF_H
