/*!
 *  \file envoimessagedialog.cpp;
 */
#include "envoimessagedialog.h"
#include "ui_envoimessagedialog.h"

#include <QDebug>

/*!
 *  \class EnvoiMessageDialog
 *  \fn EnvoiMessageDialog::EnvoiMessageDialog(QWidget *parent)
 *  \brief Constructeur de la classe EnvoiMessageDialog
 *  \param[in] parent Parent de la fenetre (toujours MainWindowm
 */
EnvoiMessageDialog::EnvoiMessageDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::EnvoiMessageDialog)
{
    ui->setupUi(this);
    fullComboBox();
    creerConnect();
}

/*!
 *  \fn EnvoiMessageDialog::~EnvoiMessageDialog()
 *  \brief Destructeur de la classe EnvoiMessageDialog
 *
 */
EnvoiMessageDialog::~EnvoiMessageDialog()
{
    delete ui;
}

/*!
 *  \fn void EnvoiMessageDialog::sendMessage()
 *  \brief envomi le message selectionner
 *
 */
void EnvoiMessageDialog::send()
{
    if(ui->messageCBox->currentIndex() == 0)  return;
    QString name = "src/programmes/"+ui->messageCBox->currentText()+".wav";
    int min = ui->minutesSpinBox->value();
    (min == 0 ? emit playMessage(name,3) : emit playMessage(name,min,3) );
}

/*!
 *  \fn void EnvoiMessageDialog::fullComboBox()
 *  \brief remplit la comboBox
 *
 */
void EnvoiMessageDialog::fullComboBox()
{
    QDir repertoire = Tools::cd("src/programmes");
    repertoire.setFilter(QDir::NoDotAndDotDot | QDir::Files);
    repertoire.setNameFilters(QStringList() << "*.wav");

    ui->messageCBox->addItem(tr("aucun message"));
    QString name;
    foreach(QFileInfo fileInfo, repertoire.entryInfoList())
    {
        name = fileInfo.fileName().split(".").at(0);
        ui->messageCBox->addItem(name);
    }
}

/*!
 *  \fn void EnvoiMessageDialog::creerConnect()
 *  \brief creer les connections entre les différents composants du widget
 *
 */
void EnvoiMessageDialog::creerConnect()
{
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(send()));
    connect(ui->closeSendButton,SIGNAL(clicked()),this,SLOT(send()));
}
