#ifndef GESTIONMESSAGEDIALOG_H
#define GESTIONMESSAGEDIALOG_H

/*!
 *  \file gestionmessagedialog.h
 *  \class GestionMessageDialog
 *  \brief Interface pour la fenetere de gestion des message
 */
#include <QDialog>
#include <QDir>
#include <QFile>

#include "tools.h"

class QStandardItemModel;
class QStyle;
class QListView;

namespace Ui {
class GestionMessageDialog;
}
/*!
 *  Cette classe permet de supprimer ou de changer l'état d'un fichier audio
 */
class GestionMessageDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GestionMessageDialog(QWidget *parent = 0);
    ~GestionMessageDialog();
    
private:
    Ui::GestionMessageDialog *ui; /*!< Interface graphique généré avec Qt Designer*/
    void creerConnect();
    void unCheckAll();
    void addIconButton();
    void loadAudioFile(QString dirPath, QStandardItemModel *modele, QListView *vue);
    void supprimer(QStandardItemModel *modele, bool actuel);
    void copie(QStandardItemModel *modele, bool leftToRight);
    QString currentPath;
    QStandardItemModel *modeleActuel;
    QStandardItemModel *modeleAncien;
    void updateList();

private slots:
    void copieLeftToRight();
    void copieRightToLeft();
    void supprimerActuel();
    void supprimerAncien();
    void setPromotion();
    void setPointAppel();
    void setOuverture();
    void setFermeture();
    void setProgrammes();
    void setJingle();
};

#endif // GESTIONMESSAGEDIALOG_H
