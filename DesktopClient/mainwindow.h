#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <memory>

#include "MyProcess.h"
#include "EventInterprocess.h"

#include <libSharedMemory.h>

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
    void on_start_btn_clicked();

    void on_stop_btn_clicked();

    void on_send_btn_clicked();

    void on_read_btn_clicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MyProcess> _child_process;

};


#endif // MAINWINDOW_H
