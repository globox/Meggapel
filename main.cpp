/*!
 *  \file main.cpp
 */

#include <QtGui/QApplication>
#include <QTextCodec>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include "mainwindow.h"


/*!
 *  \fn int main(int argc, char *argv[])
 *  \return Le code de retour de l'execution de l'application
 *
 *  Lance l'application avec les modules parallèles indispensables
 */
int main(int argc, char *argv[])
{

    /*!
     *  Met en place le codec permettant le bon fonctionnement des accents dans les QString
     */
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    /*!
     *  Met en place le codec permettant le bon fonctionnement des accents dans les tr
     */
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));


    /*!
     *  Charge le fichier de langue "megappel_<LANGUE>" en fonction
     *  de la langue
     */
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("megappel_") + locale);
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    /*!
     *  Lance la fenêtre principale MainWindow
     */
//*
    QApplication a(argc, argv);
    a.installTranslator(&translator);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    MainWindow w;
    w.show();
    return a.exec();
//*/
/*
    QApplication a(argc, argv);
    a.installTranslator(&translator);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    MessageProgrammesDialog w;
    w.show();
    return a.exec();
*/
}
