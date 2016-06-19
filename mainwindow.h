#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QDateTime>
#include <QPixmap>
#include <QScreen>
#include <QWindow>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnBrowse_clicked();
    void on_btnStart_clicked();
    void timer_timeout();

private:
    Ui::MainWindow *ui;
    QTimer timer;
    bool recording = false;
    int counter = 0;
    int totalsize = 0;
    QStringList list;
    QDateTime start;
    QDateTime end;
    const QString dateformat = "yyyy-MM-dd_HH-mm-ss";
};

#endif // MAINWINDOW_H
