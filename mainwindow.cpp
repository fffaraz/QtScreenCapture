#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    QFileDialog fd(this, "Save Directory", QDir::currentPath());
    fd.setFileMode(QFileDialog::Directory);
    fd.setOption(QFileDialog::ShowDirsOnly); //?
    if(!fd.exec()) return;
    ui->txtPath->setText(fd.directory().absolutePath());
}

void MainWindow::on_btnStart_clicked()
{
    if(!recording)
    {
        ui->groupBox->setEnabled(false);
        ui->btnStart->setText("Stop");
        timer.start(ui->spnInterval->text().toInt() * 1000);
        recording = true;
    }
    else
    {
        timer.stop();
        ui->lblStatus->clear();
        ui->groupBox->setEnabled(true);
        ui->btnStart->setText("Start");
        recording = false;
    }
}

void MainWindow::timer_timeout()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle()) screen = window->screen();
    if (!screen) return;
    QPixmap pixmap = screen->grabWindow(0);
    qDebug() << pixmap.size();
    pixmap = pixmap.scaled(pixmap.size() * (ui->spnSize->text().toInt() / 100.0), Qt::KeepAspectRatio);
    qDebug() << pixmap.size();
    QString path = ui->txtPath->text() + QDateTime::currentDateTime().toString("/yyyy-MM-dd_HH-mm-ss.") + ui->cmbFormat->currentText().toLower();
    qDebug() << path;
    pixmap.save(path, ui->cmbFormat->currentText().toStdString().c_str(), ui->spnQuality->text().toInt());
    ui->lblStatus->setText(path);
}
