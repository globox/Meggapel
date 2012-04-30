#ifndef ENVOIMESSAGEDIALOG_H
#define ENVOIMESSAGEDIALOG_H

/*!
 *  \fn envoimessagedialog.h
 */
#include <QDialog>
#include <QDir>

#include "tools.h"

namespace Ui {
class EnvoiMessageDialog;
}

/*!
 *  \class EnvoiMessageDialog
 *  \brief Classe permettant l'envoi non programmé d'un message
 */
class EnvoiMessageDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EnvoiMessageDialog(QWidget *parent = 0);
    ~EnvoiMessageDialog();
    
private slots:
    void send();

private:
    Ui::EnvoiMessageDialog *ui;
    void fullComboBox();
    void creerConnect();

signals:
    void playMessage(QString, int);
    void playMessage(QString, int, int);
};

#endif // ENVOIMESSAGEDIALOG_H
