#ifndef BARGRAPHVIEW_H
#define BARGRAPHVIEW_H

#include <QWidget>
#include <QPainter>
#include <QList>

/*!
 *  \file bargraphview.h
 */

class QPen;
class QBrush;

/*!
 *  \class BarGraphView
 *  \brief "Peint un graph de statistique"
 *
 */
class BarGraphView : public QWidget
{
    Q_OBJECT
public:
    explicit BarGraphView(QWidget *parent = 0);
    void setTitle(QString titre);
    void setBornes(QList<int> lst);

private:
    void initPaint();
    void initVar();
    void drawStat(QPainter &painter);
    void drawContexte(QPainter &painter);
    QPen* pen;                  /*!< "Pinceau" pour "Peindre"*/
    QBrush* brush;              /*!< Outil pour "Peindre" */
    Qt::BrushStyle brushStyle;  /*!< Outil pour "Peindre" */
    QList<int> bornes;          /*!< Listes des bornes    */
    QList<QColor> colors;       /*!< Liste des couleurs   */
    QString title;              /*!< Titre du graoph      */
    int maxStat;                /*!< Stat le plus élevée  */
    int statWidth;              /*!< taille calculé d'un stat*/
    int statHeight;             /*!< hauteur calculé d'un stat */
    int spaceHeight;            /*!< taille d'un espace */
    int shift;                  /*!< taille des marge*/

protected:
    void paintEvent(QPaintEvent *);

};

#endif // BARGRAPHVIEW_H
