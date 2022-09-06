#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QRegularExpression>

extern "C" {
#include "../back/calc.h"
}


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_equal_clicked();
    void keyPressEvent(QKeyEvent *x);
    void ui_handler();
    void on_plotButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
