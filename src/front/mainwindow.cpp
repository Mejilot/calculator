#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotwindow.h"

#define OPERAND_MISS 4
#define OPERATOR_MISS 5

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_equal->setAttribute(Qt::WA_MacShowFocusRect,0);
    ui->plotButton->setAttribute(Qt::WA_MacShowFocusRect,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double calculate(char* expression, double x, int *error_code)
{
        double res;
        calculator(expression, x, &res, error_code);
        return res;

}

void MainWindow::ui_handler() {
    QString input = ui->inputLine->text();
    input = input.trimmed();
    if (input.length() == 0) {
        ui->inputLine->setText("");
        return;
    }
    int error_code = 0;
    QByteArray ba = input.toLocal8Bit();
    char *expression = ba.data();
    QString x_string = ui->x_value->text();
    bool is_double;
    double x = x_string.toDouble(&is_double);
    QString error = "";
    if ((is_double && input.indexOf('x') != -1) || input.indexOf('x') == -1) {
        double res = calculate(expression, x, &error_code);
        if (error_code == 1) {
            error = "Incorrect expression!!!";
        } else if (error_code == 2) {
            error = "Semicolon missed";
        } else if (error_code == OPERAND_MISS) {
            error = "You missed an operand!!!";
        } else if (error_code == OPERATOR_MISS) {
            error = "You missed an operator!!!";
        } else {
            QString resAsString = QString::number(res, 'f', 7);\
            resAsString.remove( QRegularExpression("0+$") );
            resAsString.remove( QRegularExpression("\\.$") );
            ui->inputLine->setText(resAsString);
        }
    } else {
        error = "Enter correct x value";
    }
    ui->errorLine->setText(error);
}

void MainWindow::on_pushButton_equal_clicked() {
    ui_handler();
}

void MainWindow::keyPressEvent(QKeyEvent *x) {
    if (x->key() == 16777220) {
        ui_handler();
    }
}

void MainWindow::on_plotButton_clicked()
{
    plotWindow window;
    QString input = ui->inputLine->text();
    window.input1 = input;
    window.x_min_saved = -10.;
    window.y_min_saved = -10.;
    window.y_max_saved = 10.;
    window.x_max_saved = 10.;
    if (input.length() != 0) {
        QByteArray ba = input.toLocal8Bit();
        char *expression = ba.data();
        window.plot(expression, -10., 10., -10., 10.);
    }
    window.setModal(true);
    window.exec();
}

