/*!
 *  \file tools.cpp
 *  \headerfile tools.h
 */
#include "tools.h"

/*!
 *  \fn static QString Tools::absolutePathFile(QString chemin)
 *  \brief Retourne le chemin absolue vers un fichier quel que soit l'OS
 *  \param[in] chemin vers UNIX vers un fichier
 *  \return QString Chemin vers le fichier
 */
QString Tools::absolutePathFile(QString chemin){
    QStringList list = chemin.split("/");
    QDir current = QDir::current();
    for(int i=0; i < list.size() - 1; i++)
        current.cd(list.at(i));
    return current.absoluteFilePath(list.at(list.size()-1));
}

/*!
 *  \fn static QDir Tools::cd(QString chemin)
 *  \brief Retourne un QDir quel que soit l'OS
 *  \param[in] chemin vers UNIX vers un répertoire
 *  \return QDir répertoire souhaité
 */
QDir Tools::cd(QString chemin){
    QStringList list = chemin.split("/");
    QDir current = QDir::current();
    for(int i=0; i < list.size(); i++)
        current.cd(list.at(i));
    return current;
}

/*!
 *  \fn static QDir Tools::isNow(QTime time)
 *  \brief regarde si une date est mainteant
 *  \param[in] time date a comparé
 *  \return bool vrai ou faux
 */
bool Tools::isNow(QTime time)
{
    return isSameTime(time, QTime::currentTime());
}

/*!
 *  \fn static QDir Tools::isSameTime(QTime A, QTime B)
 *  \brief Retourne si deux dates sont égale
 *  \param[in] A temps à comparer
 *  \param[in] B temps à comparer
 *  \return bool vrai ou faux
 */
bool Tools::isSameTime(QTime A, QTime B)
{
    return ((A.hour()   == B.hour())&&
            (A.minute() == B.minute())&&
            (A.second() == B.second()));
}

/*!
 *  \fn static QDir Tools::compareTime(QTime A, QTime B)
 *  \brief compare si une heure A est avant ou apres ou egal a B
 *  \param[in] A temps à comparer
 *  \param[in] B temps à comparer
 *  \return -1 : A est avant B, 0 : A egal B, 1 A est après B
 */
int Tools::compareTime(QTime A, QTime B)
{
    if(isSameTime(A,B))return 0;
    if(A.hour() < B.hour()) return -1;
    else if(A.hour() == B.hour())
    {
        if(A.minute() < B.minute()) return -1;
        else if(A.minute() == B.minute())
        {
            if(A.second() < B.second()) return -1;
        }
    }
    return 1;
}

/*!
 *  \fn static QString Tools::prepareToCopy(QString str)
 *  \brief échappe les caractère d'une chaine pour une copie
 *  \param[in] str chaine à échapper
 *  \return QString chaine échappée
 */
QString Tools::prepareToCopy(QString str)
{
    /*!
     *  pour les QString a écrire dans un fichier on remplace les " " par des "#"
     *  étant donnée qu'apres on split sur des " ".
     */
    return str.replace(" ","_");
}


/*!
 *  \fn static QString Tools::prepare(QString str)
 *  \brief échappe les caractère d'une chaine pour un fichier conf
 *  \param[in] str chaine à échapper
 *  \return QString chaine échappée
 */
QString Tools::prepare(QString str)
{
    /*!
     *  pour les QString a écrire dans un fichier on remplace les " " par des "#"
     *  étant donnée qu'apres on split sur des " ".
     */
    return str.replace(" ","#");
}

/*!
 *  \fn static QString Tools::prepare(QString str)
 *  \brief redonne la chaine sans les échappement
 *  \param[in] str chaine à déséchapper
 *  \return QString chaine déséchapper
 */
QString Tools::unPrepare(QString str)
{
    return str.replace("#"," ");
}
