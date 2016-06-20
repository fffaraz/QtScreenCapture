#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
    QList<QByteArray> formats = QImageWriter::supportedImageFormats();
    for (int i = 0; i < formats.size(); ++i) ui->cmbFormat->insertItem(i, formats[i]);
    ui->cmbFormat->setCurrentText("jpeg");
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
        counter = totalsize = 0;
        list.clear();
        start = QDateTime::currentDateTime();
        timer.start(ui->spnInterval->text().toInt() * 1000);
        recording = true;
        timer_timeout();
    }
    else
    {
        timer.stop();
        timer_timeout();
        ui->lblStatus->clear();
        ui->groupBox->setEnabled(true);
        ui->btnStart->setText("Start");
        recording = false;
        end = QDateTime::currentDateTime();
        QFile file(ui->txtPath->text() + "/report_" + start.toString(dateformat) + ".txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write("Start: ");
        file.write(start.toString(dateformat).toUtf8());
        file.write("\nEnd  : ");
        file.write(end.toString(dateformat).toUtf8());
        file.write("\nLength: ");
        file.write(QString::number(start.secsTo(end)).toUtf8());
        //---
        file.write("\nPath: ");
        file.write(ui->txtPath->text().toUtf8());
        file.write("\nFormat: ");
        file.write(ui->cmbFormat->currentText().toUtf8());
        file.write("\nInterval: ");
        file.write(ui->spnInterval->text().toUtf8());
        file.write("\nSize: ");
        file.write(ui->spnSize->text().toUtf8());
        file.write("\nQuality: ");
        file.write(ui->spnQuality->text().toUtf8());
        //---
        file.write("\nNumber of Files: ");
        file.write(QString::number(counter).toUtf8());
        file.write("\nTotal Size: ");
        file.write(QString::number(totalsize / 1024).toUtf8());
        //---
        for (int i = 0; i < list.size(); ++i)
        {
            file.write("\n");
            file.write(list[i].toUtf8());
        }
        file.write("\n");
        file.close();
    }
}

void MainWindow::timer_timeout()
{
    counter++;
    QScreen *screen = QGuiApplication::primaryScreen();
    if (const QWindow *window = windowHandle()) screen = window->screen();
    if (!screen) return;
    QPixmap pixmap = screen->grabWindow(0);
    qDebug() << pixmap.size();
    if(ui->spnSize->text().toInt() != 100)
        pixmap = pixmap.scaled(pixmap.size() * (ui->spnSize->text().toInt() / 100.0), Qt::KeepAspectRatio);
    qDebug() << pixmap.size();
    QString file = QDateTime::currentDateTime().toString(dateformat + "_zzz");
    QString path = ui->txtPath->text() + "/" + file + "." + ui->cmbFormat->currentText();
    qDebug() << path;
    pixmap.save(path, ui->cmbFormat->currentText().toStdString().c_str(), ui->spnQuality->text().toInt());
    int size = QFile(path).size();
    totalsize += size;
    list.append(file + "\t" + QString::number(size / 1024));
    ui->lblStatus->setText(path);
}
