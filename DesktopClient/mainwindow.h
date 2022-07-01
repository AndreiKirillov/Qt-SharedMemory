#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <memory>
#include <boost/process.hpp>

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

private:
    Ui::MainWindow *ui;
    std::unique_ptr<QProcess> _process;
    std::unique_ptr<boost::process::child> _child;

};
#endif // MAINWINDOW_H
