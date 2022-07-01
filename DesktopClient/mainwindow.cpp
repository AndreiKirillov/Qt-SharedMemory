#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), _child()
{
    ui->setupUi(this);
    _process = std::make_unique<QProcess>(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_btn_clicked()
{
    if(_process->state() == QProcess::NotRunning)
    {
        _process->start("gnome-terminal -e ./Child");
    }
    //_child =  std::make_unique<boost::process::child>("./Child");
}

void MainWindow::on_stop_btn_clicked()
{
    //_child->terminate();
    //if(!_child->running())
        //qDebug() << "Process is closed";

    if(_process->state() == QProcess::Running)
    {
        _process->close();
        if(_process->state() == QProcess::NotRunning)
        {
            qDebug() << "Process is closed";
        }
    }
    else
        qDebug() << "Process is NOT running";

}
