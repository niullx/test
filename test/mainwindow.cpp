#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    display();
    getLocalIP();
    getAdapterInfoWithQt();
    getUpTime();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getLocalIP()
{
    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
    foreach(QHostAddress ipItem,ipList) //遍历ipList
    {
        if(ipItem.protocol() == QAbstractSocket::IPv4Protocol && ipItem != QHostAddress::Null
                && ipItem != QHostAddress::LocalHost && ipItem.toString().left(3) == "192" || ipItem.toString().left(3) == "127")
        {
            qDebug()<<ipItem.toString();
        }
    }
}

void MainWindow::getAdapterInfoWithQt()
{
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces(); //获取所有的网络接口
    foreach(QNetworkInterface interfaceItem, interfaceList)
    {
        if(interfaceItem.flags().testFlag(QNetworkInterface::IsUp)
                &&interfaceItem.flags().testFlag(QNetworkInterface::IsRunning)
                &&interfaceItem.flags().testFlag(QNetworkInterface::CanBroadcast)
                &&interfaceItem.flags().testFlag(QNetworkInterface::CanMulticast)
                &&!interfaceItem.flags().testFlag(QNetworkInterface::IsLoopBack)
                &&!interfaceItem.humanReadableName().contains("VMware"))
        {
            QList<QNetworkAddressEntry> addressEntryList=interfaceItem.addressEntries();
            foreach(QNetworkAddressEntry addressEntryItem, addressEntryList)
            {
                if(addressEntryItem.ip().protocol()==QAbstractSocket::IPv4Protocol)
                {
                    qDebug()<<"------------------------------------------------------------";
                    qDebug()<<"Adapter Name:"<<interfaceItem.name();//本机网络连接名
                    qDebug()<<"Adapter Address:"<<interfaceItem.hardwareAddress(); //MAC地址
                    qDebug()<<"IP Address:"<<addressEntryItem.ip().toString();
                    qDebug()<<"IP Mask:"<<addressEntryItem.netmask().toString();
                    qDebug()<<"Broadcast:"<<addressEntryItem.broadcast().toString(); //子网掩码
                    ui->lineEdit_versions->setText(interfaceItem.name());
                    ui->lineEdit_ip->setText(addressEntryItem.ip().toString());
                    getDataTime();
                    getMachineName();
                    getCpuRate();
                    getMemUsage();
                    Get_cpuinfo(cpufile);
                    Get_meminfo(memfile);
                }
            }
        }
    }
}

void MainWindow::getDataTime() //获取当前时间
{
    QDateTime date_time = QDateTime::currentDateTime();
    QString date = date_time.toString("MM.dd  hh:mm:ss");
    qDebug()<<"time:"<<date;

}

void MainWindow::getMachineName() //获取设备名称
{
    QString machineName = QHostInfo::localHostName();
    qDebug()<<"name: "<< machineName;
}

void MainWindow::Get_cpuinfo(QString path) //获取CPU使用率
{
    //获取文件路径及名称
    QStringList cpulist;
    QStringList temlist;
    QByteArray array;
    if(path.isEmpty() == false)//判断路径是否为空
    {
        QFile file(path);//指定文件路径
        bool isOk = file.open(QIODevice::ReadOnly);//检查文件打开情况
        if(isOk == true)
        {
            array = file.readAll();//读文件 内容放到字节组
            cpulist = QString(array).split('\n');       //通过“\n”将整个文件内容按字符串进行存储
        }
        else
        {
            qDebug()<<"文件打开错误"<<"\n"<<path;
            return ;
        }
        file.close();//文件关闭
    }

     for(int i=0;i<3;i++)
     {
           temlist = cpulist.at(i).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
           if(i==0)//计算CPU使用率
           {
             cpu_usage=100*(temlist.at(0).toFloat()+temlist.at(1).toFloat()+temlist.at(2).toFloat())/(temlist.at(0).toFloat()+temlist.at(1).toFloat()+temlist.at(2).toFloat()+temlist.at(3).toFloat());
             ui->lineEdit_usage->setText(QString("%1").arg(cpu_usage)+"%");
             qDebug()<<"cpu使用率: "<<QString("%1").arg(cpu_usage)<<"%";
           }
     }

}

void MainWindow::Get_meminfo(QString path) //获取内存使用率
{
    QStringList memlist;
    QStringList tem1,tem2;
    QByteArray array;
    if(path.isEmpty() == false)//判断路径是否为空
    {
        QFile file(path);//指定文件路径
        bool isOk = file.open(QIODevice::ReadOnly);//检查文件打开情况
        if(isOk == true)
        {
            array = file.readAll();//读文件 内容放到字节组
            memlist = QString(array).split('\n');        //通过“\n”将整个文件内容按字符串进行存储
        }
        file.close();//文件关闭
    }

   //---计算内存使用率
    tem1 = memlist.at(0).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
    tem2 = memlist.at(1).split(QRegExp("\\s+"), QString::SkipEmptyParts);//按一个或多个空格分割
    mem_usage=(1-(tem2.at(1).toFloat()/tem1.at(1).toFloat()))*100;
    ui->lineEdit_memory->setText(QString("%1").arg(mem_usage)+"%");
    qDebug()<<"内存使用率: "<<QString("%1").arg(mem_usage)<<"%";
}

bool MainWindow::getMemUsage()
{
    QProcess process;
    process.start("free -m"); //使用free完成获取
    process.waitForFinished();
    process.readLine();
    QString str = process.readLine();
    str.replace("\n","");
    str.replace(QRegExp("( ){1,}")," ");//将连续空格替换为单个空格 用于分割
    auto lst = str.split(" ");
    qDebug()<<"1--lst"<<lst;
    qDebug()<<"1--lst.size()"<<lst.size();
    if(lst.size() > 6)
    {
        qDebug("mem total:%.0lfMB free:%.0lfMB",lst[1].toDouble(),lst[6].toDouble());
        return true;
    }
    return false;
}

bool MainWindow::getCpuRate() //获取CPU速率
{
    QProcess process;
    process.start("cat /proc/stat");
    process.waitForFinished();
    QString str = process.readLine();
    str.replace("\n","");
    str.replace(QRegExp("( ){1,}")," ");
    auto lst = str.split(" ");
    qDebug()<<"lst"<<lst;
    qDebug()<<"lst.size()"<<lst.size();
    if(lst.size() > 3)
    {
        double use = lst[1].toDouble() + lst[2].toDouble() + lst[3].toDouble();
        double total = 0;
        for(int i = 1;i < lst.size();++i)
            total += lst[i].toDouble();
        if(total - m_cpu_total > 0)
        {
            qDebug("cpu rate:%.2lf%%",(use - m_cpu_use) / (total - m_cpu_total) * 100.0);
            m_cpu_total = total;
            m_cpu_use = use;
            return true;
        }
    }
    return false;
}

void MainWindow::getUpTime() //获取上机时间及系统负载
{
    QProcess process;
    process.start("cat /proc/uptime");
    process.waitForFinished();
    QString str = process.readLine();
    str.replace("\n","");
    str.replace(QRegExp("( ){1,}")," ");//将连续空格替换为单个空格 用于分割
    auto lst = str.split(" ");
    ui->lineEdit_up_time->setText(lst[0] + "sec");
    qDebug()<<"2--lst.size()"<<lst.size();
    float lst0 = lst[0].toFloat(); //将字符串转化为数字
    float lst1 = lst[1].toFloat();
    float load = (1-lst1/lst0);
    QString str_load = QString::number(load); //将数字转化为字符串
    ui->lineEdit_system_load->setText(str_load);
    qDebug()<<"load: "<<load;
    qDebug()<<"uptime: "<<lst[0] <<"free: "<<lst[1];

    return ;
}

void MainWindow::display()
{
    ui->lineEdit_versions->setText("ubuntu ...");
    ui->lineEdit_system_load->setText("0.10");
    ui->lineEdit_up_time->setText("41 sec");
    ui->lineEdit_memory->setText("8%");
    ui->lineEdit_ip->setText("123.456.789");
    ui->lineEdit_cpu->setText("19");
    ui->lineEdit_usage->setText("10%");

}







