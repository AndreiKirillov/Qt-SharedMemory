#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <thread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _child_process = std::make_unique<MyProcess>();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_btn_clicked()
{
    if(_child_process->state() == QProcess::NotRunning)
    {
        _child_process->start("./Child");
    }
    else
        if(_child_process->state() == QProcess::Running)
        {
            if(_child_process->sendEvent(EventType::start))
                QMessageBox::about(this, "Внимание", "Событие отправилось");
            else
                QMessageBox::about(this, "Внимание", "Не удалось выполнить");
        }
}

void MainWindow::on_stop_btn_clicked()
{
    if(_child_process->state() == QProcess::Running)
    {
        _child_process->close();
        if(_child_process->state() == QProcess::NotRunning)
        {
            qDebug() << "Process is closed";
        }
    }
    else
        qDebug() << "Process is NOT running";

}

void MainWindow::on_read_btn_clicked() // чтение cout дочернего процесса
{
    _child_process->read();
}

void MainWindow::on_send_btn_clicked()   // отправка сообщения через разделяемую память
{
    if(_child_process->sendMessage("test message to shared memory"))
    {
        if(_child_process->sendEvent(EventType::message))
            QMessageBox::about(this, "Внимание", "Дочерний процесс прочитал сообщение в разделяемой памяти");
        else
            QMessageBox::about(this, "Внимание", "Дочерний процесс НЕ смог прочитать сообщение");
    }
    else
        QMessageBox::about(this, "Внимание", "Не удалось записать сообщение в разделяемую память");
}

