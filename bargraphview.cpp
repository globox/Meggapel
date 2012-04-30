/*!
 *  \file bargraphview.cpp
 */
#include "bargraphview.h"
#include <QBrush>
#include <QPen>
#include <QDebug>

/*!
 *  \class BarGraphView
 *  \fn BarGraphView::BarGraphView(QWidget *parent)
 *  \brief constructeur de la classe BarGraphView
 *  \param parent Parent de la classe
 */
BarGraphView::BarGraphView(QWidget *parent) :
    QWidget(parent)
{
    shift = 20;
    title ="Statistique";
    initPaint();
    bornes.append(10);
    bornes.append(3);
    bornes.append(9);
    bornes.append(7);
    bornes.append(4);
    bornes.append(8);
}

/*!
 *  \fn void BarGraphView::setTitle(QString titre)
 *  \brief Met a jour le titre d'un graph
 *  \param titre nouveau titre du graph
 *
 */
void BarGraphView::setTitle(QString titre)
{
    title = titre;
    update();
}

/*!
 *  \fn void BarGraphView::setBornes(QList<int> lst)
 *  \brief Met a jour les bornes d'un graph
 *  \param lst nouvelles valeurs pour les bornes
 *
 */
void BarGraphView::setBornes(QList<int> lst)
{
    bornes = lst;
    update();
}

/*!
 *  \fn void BarGraphView::initPaint()
 *  \brief Initialise les composants pour "peindre"
 *
 */
void BarGraphView::initPaint()
{
    colors.append(Qt::red);
    colors.append(Qt::green);
    colors.append(Qt::blue);
    colors.append(Qt::magenta);
    colors.append(Qt::yellow);
    colors.append(Qt::cyan);

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    pen = new QPen(Qt::black, 0,
                   Qt::PenStyle(Qt::SolidLine),
                   Qt::PenCapStyle(Qt::SquareCap),
                   Qt::PenJoinStyle(Qt::BevelJoin));

    brushStyle = Qt::BrushStyle( Qt::SolidPattern);
}

/*!
 *  \fn void BarGraphView::initVar()
 *  \brief initialise les variable du graph
 *
 */
void BarGraphView::initVar()
{
    maxStat=0;
    foreach(int value, bornes)
        if (value > maxStat)
            maxStat = value;
    statWidth   = (width() - 2*shift) / (maxStat + 2) ;
    int nbBornes = 0;
    ( bornes.size() > 4 ? nbBornes = bornes.size() : nbBornes = 4);
    statHeight  = (height()- 2*shift) / (nbBornes + 1);
    spaceHeight = statHeight / nbBornes;
}

/*!
 *  \fn void BarGraphView::drawStat(QPainter &painter)
 *  \brief "Peint" les barres du graph
 *  \param painter outile pour "Peindre" le graph
 *
 */
void BarGraphView::drawStat(QPainter &painter)
{
    painter.setPen(*pen);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QString str;
    int x1 = (shift + 1);
    int y1;
    int borneSize;
    QFont font = painter.font();
    font.setPixelSize(10);
    painter.setFont(font);
    for(int i=0; i<bornes.size(); i++)
    {
        brush = new QBrush(colors.at(i%colors.size()), brushStyle);
        painter.setBrush(*brush);
        y1 = (shift + 1) + i*spaceHeight + i*statHeight;
        if(bornes.at(i) != 0)
        {
            borneSize = bornes.at(i)*statWidth;
            painter.drawRect(x1, y1, borneSize, statHeight);
            painter.drawText(x1+borneSize+10,y1,shift,statHeight,Qt::AlignCenter,str.setNum(bornes.at(i)));
        }
        else
        {
            painter.drawText(0,y1,200,statHeight,Qt::AlignCenter,
                             tr("Aucune donnée pour cette borne"));
        }
        painter.drawText(0,y1,shift,statHeight,Qt::AlignCenter,str.setNum(i+1));
    }
}

/*!
 *  \fn void BarGraphView::drawStat(QPainter &painter)
 *  \brief "Peint" les composants du graph
 *  \param painter outile pour "Peindre" le graph
 *
 */
void BarGraphView::drawContexte(QPainter &painter)
{
    painter.drawText(0,0,width(),shift,Qt::AlignCenter,title);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);

    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
    painter.drawLine(shift,      shift- 10,         shift, height()-shift);
    painter.drawLine(shift, height()-shift, width()-shift, height()-shift);

    QString str;
    int index = 0;
    int pas = 0;
    (maxStat > 10 ? pas =maxStat /5 : pas = 1 );
    for(int i=shift; i<(width()-shift); i+= pas*statWidth)
    {
        painter.drawLine(i,height()-10,i,height()-shift);
        painter.drawText(i+4,height()-5,str.setNum(index));
        index += pas;
    }
}

/*!
 *  \fn void BarGraphView::paintEvent(QPaintEvent*)
 *  \brief "Peint le graph a l'arrivé d'un événement"
 *
 */
void BarGraphView::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    initVar();
    drawStat(painter);
    drawContexte(painter);
}
