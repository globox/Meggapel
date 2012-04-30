#ifndef ENREGISTREMENTMESSAGEDIALOG_H
#define ENREGISTREMENTMESSAGEDIALOG_H

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>

class QStyle;

namespace Ui {
class EnregistrementMessageDialog;
}

class EnregistrementMessageDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EnregistrementMessageDialog(QWidget *parent = 0);
    ~EnregistrementMessageDialog();
    
private:
    Ui::EnregistrementMessageDialog *ui;
    bool changeStateRecordeZone(bool state);
    QString path;
    QString titre;
    void creerConnect();
    void unCheckAll();
    void addIconButton();
    void updateLabelTitre();

private slots:
    void changeTitre();
    void setProgrammes();
    void setPromotion();
    void setOuverture();
    void setFermeture();
    void setPointAppel();
    void setJingle();
};

#endif // ENREGISTREMENTMESSAGEDIALOG_H
