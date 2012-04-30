#ifndef TOOLS_H
#define TOOLS_H

/*!
 *  \file tools.h
 *  \brief Outils static utile pour différente classe
 */

#include <QString>
#include <QDir>
#include <QTime>

/*! \class Tools
 * \brief classe representant les outils utiliser pour différentes classe
 *
 *  La classe propose différente fonction static évitant la redondance de code
 */
class Tools
{
public:
    static QString absolutePathFile(QString chemin);
    static QDir cd(QString chemin);
    static bool isNow(QTime time);
    static bool isSameTime(QTime A, QTime B);
    static int compareTime(QTime A, QTime B);
    static QTime* addTime(QTime A, int heure = 0, int minute = 0, int second = 0);
    static QString prepare(QString str);
    static QString prepareToCopy(QString str);
    static QString unPrepare(QString str);
};

#endif // TOOLS_H
