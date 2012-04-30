#ifndef STATISTICWIDGET_H
#define STATISTICWIDGET_H

#include <QWidget>

namespace Ui {
class StatisticWidget;
}

class StatisticWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatisticWidget(QWidget *parent = 0);
    ~StatisticWidget();
    
private:
    Ui::StatisticWidget *ui;

};

#endif // STATISTICWIDGET_H
