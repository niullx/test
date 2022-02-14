#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QStringList>
#include <QString>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QDebug>
#include <QDateTime>
#include <QHostInfo>
#include <QProcess>
#include <QFile>
#include <QFileDialog>


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
    void display();
    void getLocalIP();
    void getAdapterInfoWithQt();
    void getDataTime();
    void getMachineName();
    bool getCpuRate();
    bool getMemUsage();

    void getUpTime();
    void Get_cpuinfo(QString path);
    void Get_meminfo(QString path);

public:
    int showfile=0;
private:
    QString cpufile="/proc/stat";
    QString memfile="/proc/meminfo";
    float cpu_usage;
    float mem_usage;

    double m_cpu_total = 0;
    double m_cpu_use = 0;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
