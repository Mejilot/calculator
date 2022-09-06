#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "qcustomplot.h"

#define OPERAND_MISS 4
#define OPERATOR_MISS 5

plotWindow::plotWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plotWindow)
{
    ui->setupUi(this);
    ui->plot->xAxis->setLabel("x");
    ui->plot->yAxis->setLabel("y");
    ui->plot->xAxis->setRange(-10, 10);
    ui->plot->yAxis->setRange(-10, 10);
}

plotWindow::~plotWindow()
{
    delete ui;
}

void plotWindow::on_plotButton_clicked()
{
    // generate some data:
    bool is_double1, is_double2, is_double3, is_double4;
    double x_min = ui->xMin->text().toDouble(&is_double1);
    double x_max = ui->xMax->text().toDouble(&is_double2);
    double y_min = ui->yMin->text().toDouble(&is_double3);
    double y_max = ui->yMax->text().toDouble(&is_double4);
    QString input = ui->inputLine->text();
    QByteArray ba = input.toLocal8Bit();
    char *expression = ba.data();

    int error_code = 0;
    QString error = "";
    double res;
    calculator(expression, 0, &res, &error_code);
    if (error_code == 1) {
        error = "Incorrect expression!!!";
    } else if (error_code == 2) {
        error = "Semicolon missed";
    } else if (error_code == OPERAND_MISS) {
        error = "You missed an operand!!!";
    } else if (error_code == OPERATOR_MISS) {
        error = "You missed an operator!!!";
    } else {
        if (!is_double1 || !is_double2 || !is_double3 || !is_double4) {
            error = "Enter only real numbers!";
            plot(expression, x_min_saved, x_max_saved, y_min_saved, y_max_saved);
        } else if (x_min >= x_max || y_min >= y_max) {
            error = "Min values should be less than max values";
            plot(expression, x_min_saved, x_max_saved, y_min_saved, y_max_saved);
        } else if (x_min < -1000000 || y_min < -1000000 || x_max > 1000000 || y_max > 1000000) {
            error = "x value and the domain of function should be from -1000000 to 1000000";
            plot(expression, x_min_saved, x_max_saved, y_min_saved, y_max_saved);
        } else {
            x_min_saved = x_min;
            x_max_saved = x_max;
            y_min_saved = y_min;
            y_max_saved = y_max;
            plot(expression, x_min, x_max, y_min, y_max);
        }
    }
    ui->errorLine->setText(error);
}

void plotWindow::plot(char *expression, double x_min, double x_max, double y_min, double y_max) {
    double step = (x_max - x_min) / 100000;
    int error_code;
    QVector<double> x, y;
    for (double i=x_min; i<x_max; i += step)
    {
      double x_val = i;
      double res;
      calculator(expression, x_val, &res, &error_code);
      if (res < y_min || res > y_max) {
          continue;
      }
      x.push_back(x_val);
      y.push_back(res);
    }

    QCustomPlot *customPlot = ui->plot;
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);

    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    customPlot->xAxis->setRange(x_min, x_max);
    customPlot->yAxis->setRange(y_min, y_max);
    customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDot, Qt::red, Qt::white, 7));
    customPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    customPlot->setInteraction(QCP::iRangeZoom,true);
    customPlot->setInteraction(QCP::iRangeDrag, true);
    customPlot->replot();
}
