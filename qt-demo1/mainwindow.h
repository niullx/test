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
//#include <QFileDialog>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QtGlobal>
#include <QDate>
#include <QTimer>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QSettings>    //ini读写


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

    void getNetworkInfo();
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


public:
    QSerialPort *serial;            //全局的串口对象
    QDateTime curDateTime;          //当前时间
    qint32 rec_buf_len;             //接收累计长度
    qint32 send_buf_len;            //发送累计长度

    QTimer * sysTimer;              //系统定时器
    QTimer * uartRecDataTimer;      //串口接收定时器
    QElapsedTimer  * fTimeCounter;    //串口计时器


private slots:

    //uart
    QString sendUartData(QString data,bool isHex,bool hasTimerStamp,bool isAT);
    void uartRec_timeout () ;      //定时溢出处理槽函数
    void SysStateDeal();
    bool openTextByIODevice(const QString &aFileName);
    bool saveTextByIODevice(const QString &aFileName);
    void ReadData();
    void GetAveriablePort();
    void on_overTimeRecEdit_returnPressed();
    void PortConfigureInit();
    void updateMainStyle(QString style);
    void insertDataToPlain();  //将串口数据填入面板
    void IniParamInit();
    bool SaveUartParam();
    void MenuBarInit();
    void receiveFont(QFont font);   //接收传递过来的字体

    //事件槽
    void on_openSerialButton_clicked();
    void on_sendDataButton_clicked();
    void on_clearSendButton_clicked();
    void on_clearRecButton_clicked();
    void on_saveDataButton_clicked();
    void on_readLogButton_clicked();
    void on_refreshPortButton_clicked();
    void sendButtonClick(QString str,bool isChecked);
    void on_paramSaveButton_clicked();

private:
//    letterFormWindow *letterFormUi = NULL;  //字体窗口
    QByteArray uart_rec_ss;            //串口接收数据
    qreal uartRecOvertimeCount;     //串口接收超时计数
    QSettings *configIni;           //配置文件
    bool isCmdPlainExpand;          //命令面板窗口是否展开

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
