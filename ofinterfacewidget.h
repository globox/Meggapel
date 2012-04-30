#ifndef OFINTERFACEWIDGET_H
#define OFINTERFACEWIDGET_H

/*!
 *  \file ofinterfacewidget.h
 *  \brief interface de chacun des tables de OFMagasinDialog
 */

#include <QWidget>
#include "tools.h"
#include <QMessageBox>

namespace Ui {
class OFInterfaceWidget;
}

/*! \class OFInterfaceWidget
 * \brief classe representant l'interface de chacun des tables de OFMagasinDialog
 *
 *  La classe le widget contenu dans chacun des tables
 */
class OFInterfaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OFInterfaceWidget(QWidget *parent = 0);
    ~OFInterfaceWidget();
    void setValue(QStringList arg);
    void copieFrom(OFInterfaceWidget *autre);
    bool isFerme();
    bool isContinue();
    QStringList getValue();
    bool isValid(QString jour);

private:
    Ui::OFInterfaceWidget *ui;  /*!< Interface graphique généer avec Qt Designer */
    void creerConnect();

private slots:
    void setOuvert();
    void setFerme();
    void setCoupure();
    void setContinue();
};

#endif // OFINTERFACEWIDGET_H
