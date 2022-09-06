#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QDialog>

extern "C" {
#include "../back/calc.h"
}

namespace Ui {
class plotWindow;
}

class plotWindow : public QDialog
{
    Q_OBJECT

public:
    explicit plotWindow(QWidget *parent = nullptr);
    ~plotWindow();
    QString input1;
    double x_min_saved, x_max_saved, y_min_saved, y_max_saved;
    void plot(char *expression, double x_min, double x_max, double y_min, double y_max);
private slots:
    void on_plotButton_clicked();
private:
    Ui::plotWindow *ui;
};

#endif // PLOTWINDOW_H
