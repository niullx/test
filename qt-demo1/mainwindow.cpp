#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    display();
//    getLocalIP();
    getAdapterInfoWithQt();
    getDataTime();
    getMachineName();
    getCpuRate();
    getMemUsage();
    Get_cpuinfo(cpufile);
    Get_meminfo(memfile);
    getUpTime();
    getNetworkInfo();

    //-----------------
    //初始化菜单栏
//    MenuBarInit();

    //变量初始化
    uartRecOvertimeCount = 0.2; //起始等待时间0.2S
    rec_buf_len = 0;
    send_buf_len = 0;

    //设置uart接收缓冲超时定时器
    uartRecDataTimer = new QTimer();
    uartRecDataTimer->stop();
    uartRecDataTimer->setInterval(uartRecOvertimeCount*1000);                     //设置定时周期，单位：毫秒
    uartRecDataTimer->setSingleShot(true);                                        //设置为单次触发
    connect(uartRecDataTimer,SIGNAL(timeout()),this,SLOT(uartRec_timeout()));     //设置槽

    //串口接收持续超时计时器
    fTimeCounter = new QElapsedTimer();
    //系统定时器
    sysTimer = new QTimer();
    sysTimer->setInterval(1000); //1000ms
    sysTimer->setSingleShot(false);
    connect(sysTimer,SIGNAL(timeout()),this,SLOT(SysStateDeal()));
    sysTimer->start();
    PortConfigureInit();    //配置串口初始化
    GetAveriablePort();     //查找可用的串口
    IniParamInit(); //读取参数文件,配置参数


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
    ui->lineEdit_up_time->setText(lst[0] + " "+"sec");
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

void MainWindow::getNetworkInfo()
{
    // 获取所有网络接口的列表
        QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
      //获取可用串口列表
        QList<QSerialPortInfo> serial_list = QSerialPortInfo::availablePorts();
        // 遍历每一个网络接口
/*        foreach (QNetworkInterface interface, list)
        {
            // 接口名称
            qDebug() << "接口名称 Name: " << interface.name();
            // 硬件地址
            qDebug() << "硬件地址 HardwareAddress: " << interface.hardwareAddress();
            //可以通过它获取所有ip地址
            qDebug()<< "所有ip地址 Ip address"<<interface.allAddresses();
            // 获取IP地址条目列表，每个条目中包含一个IP地址，一个子网掩码和一个广播地址
            QList<QNetworkAddressEntry> entryList = interface.addressEntries();//addressEntries()可以返回一个QNetworkAddressEntry对象的列表
            //QNetworkAddressEntry类保存了一个网络支持的IP 以及该IP地址相关的子网掩码和广播地址
            // 遍历每一个IP地址条目
            foreach (QNetworkAddressEntry entry, entryList)
            {
                // IP地址
                qDebug() << "IP地址 IP Address: " << entry.ip().toString();
                // 子网掩码
                qDebug() << "子网掩码 Netmask: " << entry.netmask().toString();
                // 广播地址
                qDebug() << "广播地址 Broadcast: " << entry.broadcast().toString();
            }
        } */

        int serialsize = serial_list.size();
        qDebug()<<serial_list.size();
        for(int i=0;i<serialsize;i++)
        {
            QString flag = serial_list.at(i).description();
            qDebug()<<"串口 ："<<i<<flag;
        }

        foreach (const QSerialPortInfo &info, serial_list)
        {
            qDebug() << "Name : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();
            qDebug() << "Serial Number: " << info.serialNumber();
            qDebug() << "System Location: " << info.systemLocation();
        }
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



//------------------------------
//-----------------------------------
//-------------------------------------------


//菜单栏初始化配置
void MainWindow::MenuBarInit()
{
    QMenuBar *menuBar = ui->menuBar;
    QMenu *pFile = menuBar->addMenu("主题设置");

    QDir dir1("/qss/");
    QFileInfoList list;
    QStringList filters;
    if(dir1.exists())
    {
        dir1.setFilter(QDir::Files); //加过滤器，选择前缀为name的文件
        filters << "*qss";
        dir1.setNameFilters(filters);
        list = dir1.entryInfoList();
    }
    else
    {
        qDebug()<<"qss不在盘根目录";
        QDir dir2("qss/");
        if(dir2.exists())
        {
            dir2.setFilter(QDir::Files); //加过滤器，选择前缀为name的文件
            dir2.setNameFilters(filters);
            list = dir2.entryInfoList();
        }
        else
        {
            qDebug()<<"qss不在程序根目录";
        }
    }

    //对每个QSS文件的点击添加事件槽，触发更新主题
    qDebug()<<"文件数量:"<<QString::number(list.size());
    if(list.size()>1)
    {
        QFileInfo fileInfo;
        QList<QAction*> actions;
        foreach (fileInfo, list)
        {
            qDebug()<<fileInfo.fileName();
            actions.append(new QAction(fileInfo.fileName()));
        }
        for(QAction *ac:actions)
        {
            pFile->addActions(actions);
            connect(ac,&QAction::triggered,
                        [=] ()
                        {
                            updateMainStyle(ac->text());
                        }
                        );
        }
    }

    isCmdPlainExpand = true;

    //命令面板宽度设置
    QAction *cmdPanlAct = menuBar->addAction("命令面板开关");
    connect(cmdPanlAct,&QAction::triggered,
            [=] ()
            {
                isCmdPlainExpand = !isCmdPlainExpand;
                if(isCmdPlainExpand)
                {
/*                    ui->tableWidget->setVisible(true);*/
                    ui->horizontalLayout_12->setStretch(0,1);
                    ui->horizontalLayout_12->setStretch(1,6);
                    ui->horizontalLayout_12->setStretch(2,3);
                }
                else
                {
/*                    ui->tableWidget->setVisible(false);*/
                    ui->horizontalLayout_12->setStretch(0,1);
                    ui->horizontalLayout_12->setStretch(1,9);
                    ui->horizontalLayout_12->setStretch(2,0);
                }
            }
            );
/*
    //接收面板字体类型绑定
    QAction *letterPanlAct = menuBar->addAction("字体设置");
    connect(letterPanlAct,&QAction::triggered,
            [=] ()
            {
                if(letterFormUi == NULL)
                {
                    letterFormUi = new letterFormWindow;
                    connect(letterFormUi, SIGNAL(sendFont(QFont)), this, SLOT(receiveFont(QFont)));
                }
                letterFormUi->show();
            }
    );*/
}

//命令菜单按键发送数据
void MainWindow::sendButtonClick(QString command,bool isChecked)
{
    //未打开串口则不准发送
    if(ui->openSerialButton->text() == "打开串口")
    {
        QMessageBox::warning(NULL, "警告", "未打开可用串口，无法发送数据！\r\n\r\n");
        return;
    }

    //拼接数据
    QString recShowData = sendUartData(command,isChecked,
                 ui->timeZoneCheckBox->isChecked(),ui->changeLineCheckBox->isChecked());

    if(ui->timeZoneCheckBox->isChecked())
    {
        ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
        ui->uartReadPlain->insertPlainText(recShowData);
        ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
    }

    ui->TXLenLabel->setText(QString::number(send_buf_len)+"bytes");
}

//保存参数
bool MainWindow::SaveUartParam(void)
{
    if(configIni == NULL)
        return false;

    //串口配置相关

    //波特率
    configIni->setValue("uartParam/BaudRate",ui->rateBox->currentText());
    //数据位
    configIni->setValue("uartParam/DataBit",ui->dataBox->currentText());
    //奇偶校验位
    configIni->setValue("uartParam/Parity",ui->checkBox->currentText());
    //停止位
    configIni->setValue("uartParam/StopBit",ui->stopBox->currentText());
    //时间戳
    configIni->setValue("uartParam/timestamp",ui->timeZoneCheckBox->isChecked());
    //AT
    configIni->setValue("uartParam/AT",ui->changeLineCheckBox->isChecked());

    //HEX发送
    configIni->setValue("uartParam/HEXS",ui->checkBoxHexS->isChecked());
    //HEX显示
    configIni->setValue("uartParam/HEXR",ui->checkBoxHexR->isChecked());

    //命令面板相关
/*    qint32 rowNum = ui->tableWidget->rowCount();
    for(int i =0;i<rowNum;i++)
    {
        //保存命令内容
        auto cellWidget = (ui->tableWidget->cellWidget(i, 1));
        QLineEdit *lines =(QLineEdit*)cellWidget;
        QString cmdVal = "cmdParam/cmd";
        cmdVal.append(QString::number(i));
        configIni->setValue(cmdVal,lines->text());

        //保存是否选择HEX
        cellWidget = (ui->tableWidget->cellWidget(i, 0));
        QCheckBox *box =(QCheckBox*)cellWidget;
        cmdVal = "cmdParam/hex";
        cmdVal.append(QString::number(i));
        configIni->setValue(cmdVal,box->isChecked());
    }*/
    return true;
}

//根据配置文件初始化参数
void MainWindow::IniParamInit(void)
{
    if(QFile::exists("qss/param.ini"))
    {
       configIni = new QSettings("qss/param.ini", QSettings::IniFormat);
    }
    else if(QFile::exists("/qss/param.ini"))
    {
       configIni = new QSettings("/qss/param.ini", QSettings::IniFormat);
    }
    else
    {
       configIni = NULL;
       qDebug()<<"iniparaminit---------------------";
       return;
    }

    configIni->setIniCodec("UTF-8");

    //波特率
    QString  baudRate = configIni->value("uartParam/BaudRate").toString();
    ui->rateBox->setCurrentText(baudRate);

    //数据位
    QString  dataBit = configIni->value("uartParam/DataBit").toString();
    ui->dataBox->setCurrentText(dataBit);

    //奇偶校验位
    QString  parity = configIni->value("uartParam/Parity").toString();
    qDebug()<<"parity:"<<parity;
    ui->checkBox->setCurrentText(parity);

    //停止位
    QString  stopBit = configIni->value("uartParam/StopBit").toString();
    ui->stopBox->setCurrentText(stopBit);

    //时间戳
    bool  hasTimeStamp = configIni->value("uartParam/timestamp").toBool();
    ui->timeZoneCheckBox->setChecked(hasTimeStamp);

    //回车换行
    bool  hasAT = configIni->value("uartParam/AT").toBool();
    ui->changeLineCheckBox->setChecked(hasAT);

    //HEX发送
    bool  hexSend = configIni->value("uartParam/HEXS").toBool();
    ui->checkBoxHexS->setChecked(hexSend);

    //HEX显示
    bool  hexRec = configIni->value("uartParam/HEXR").toBool();
    ui->checkBoxHexR->setChecked(hexRec);
}


//配置串口初始化
void MainWindow::PortConfigureInit()
{
    //填入串口选项
    ui->rateBox->addItem("9600","9600");
    ui->rateBox->addItem("19200","19200");
    ui->rateBox->addItem("38400","38400");
    ui->rateBox->addItem("57600","57600");
    ui->rateBox->addItem("115200","115200");
    ui->rateBox->addItem("921600","921600");

    ui->dataBox->addItem("8",8);
    ui->dataBox->addItem("7",7);

    ui->checkBox->addItem("无校验",0);
    ui->checkBox->addItem("奇校验",1);
    ui->checkBox->addItem("偶校验",2);

    ui->stopBox->addItem("1位",1);
    ui->stopBox->addItem("2位",2);

    //设置时间输入框只允许使用数字
    ui->overTimeRecEdit->setValidator(new QRegExpValidator(QRegExp("^([0-9]{1,4}(.[0-9]{1,3})?)$")));
    ui->overTimeRecEdit->setText(QString::number(uartRecOvertimeCount));
}

//串口开关按钮
void MainWindow::on_openSerialButton_clicked()
{
    //尝试打开串口
    if(ui->openSerialButton->text() == tr("打开串口"))
    {
        if(ui->portBox->currentText() == "" )
        {
            QMessageBox::warning(NULL, "警告", "无可开启串口！\r\n\r\n");
            return;
        }

        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->portBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->rateBox->currentText().toInt());
        //设置数据位
        switch (ui->dataBox->currentData().toInt())
        {
            case 8:
                serial->setDataBits(QSerialPort::Data8);
                break;
            case 7:
                serial->setDataBits(QSerialPort::Data7);
                break;
            default:
                break;
        }
        //设置校验位
        switch (ui->checkBox->currentIndex())
        {
            case 0:
                serial->setParity(QSerialPort::NoParity);
                break;
            case 1:
                serial->setParity(QSerialPort::EvenParity);
                break;
            case 2:
                serial->setParity(QSerialPort::OddParity);
                break;
            default:
                break;
        }
        //设置停止位
        switch(ui->stopBox->currentIndex())
        {
            case 0:
                serial->setStopBits(QSerialPort::OneStop);
                break;
            case 1:
                serial->setStopBits(QSerialPort::TwoStop);
                break;
            default:
                break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl); //设置为无流控制

        //关闭设置菜单使能
        ui->portBox->setEnabled(false);
        ui->dataBox->setEnabled(false);
        ui->checkBox->setEnabled(false);
        ui->stopBox->setEnabled(false);
        ui->rateBox->setEnabled(false);
        ui->openSerialButton->setText("关闭串口");

        fTimeCounter->restart();

        //连接信号和槽函数，串口有数据可读时，调用ReadData()函数读取数据并处理。
        QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);

        ui->openSerialButton->setStyleSheet("background-color:red");
    }
    else
    {
        uartRecDataTimer->stop () ; //定时器停止

        if(serial->isOpen())       //原先串口打开，则关闭串口
        {
            serial->close();
        }

        //释放串口
        delete serial;
        serial = NULL;

        //恢复使能
        ui->portBox->setEnabled(true);
        ui->rateBox->setEnabled(true);
        ui->dataBox->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->stopBox->setEnabled(true);
        ui->openSerialButton->setText("打开串口");
        ui->openSerialButton->setStyleSheet("");
    }
}

//定时器触发打印串口数据
void MainWindow::uartRec_timeout()
{
    if(!uart_rec_ss.isEmpty())
    {
        ui->uartReadPlain->moveCursor(QTextCursor::End);            //光标移动到结尾
        insertDataToPlain();
        ui->uartReadPlain->moveCursor(QTextCursor::End);            //光标移动到结尾
        uart_rec_ss.clear();

        //定时器计时器重启
        fTimeCounter->restart();
        uartRecDataTimer->start();

        ui->RXLenLabel->setText(QString::number(rec_buf_len)+"bytes");
    }
}

//系统状态1S处理
void MainWindow:: SysStateDeal()
{
    if(uart_rec_ss.size() == 0)
    {
        if(fTimeCounter->elapsed()>=2000)
            fTimeCounter->restart();
    }
}

//填入接收数据到面板
void MainWindow::insertDataToPlain()
{
    curDateTime = QDateTime::currentDateTime();

    QString tempRecData = "\r\n";

    if(ui->timeZoneCheckBox->isChecked())
    {
        tempRecData.append(curDateTime.toString("[hh:mm:ss]")).append("R:");
        ui->uartReadPlain->insertPlainText(tempRecData);
        if(ui->checkBoxHexR->isChecked())
        {
            QString ss;
            for(int c :uart_rec_ss)
            {
                if(c>=0)
                {
                    ss += QString(" %1")
                            .arg(c, 2, 16, QChar('0'));
                }
                else
                {
                    ss += QString(" %1")
                            .arg(c+256, 2, 16, QChar('0'));
                }
            }
            ui->uartReadPlain->insertPlainText(ss);
        }
        else
        {
            ui->uartReadPlain->insertPlainText(uart_rec_ss);
        }
    }
    else
    {
        if(ui->checkBoxHexR->isChecked())
        {
            QString ss;
            for(int c :uart_rec_ss)
            {
                if(c>=0)
                {
                    ss += QString(" %1")
                            .arg(c, 2, 16, QChar('0'));
                }
                else
                {
                    ss += QString(" %1")
                            .arg(c+256, 2, 16, QChar('0'));
                }
            }
            ui->uartReadPlain->insertPlainText(ss);
        }
        else
        {
            ui->uartReadPlain->insertPlainText(uart_rec_ss);
        }
    }
    ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
}

//主界面按键发送串口数据
void MainWindow::on_sendDataButton_clicked()
{
    if(ui->openSerialButton->text() == "打开串口")
    {
        QMessageBox::warning(NULL, "警告", "未打开可用串口，无法发送数据！\r\n\r\n");
        return;
    }

    QString command = ui->uartWritePlain->toPlainText();

    //发送数据
    QString recShowData = sendUartData(command,ui->checkBoxHexS->isChecked(),
                 ui->timeZoneCheckBox->isChecked(),ui->changeLineCheckBox->isChecked());

    if(ui->timeZoneCheckBox->isChecked())
    {
        ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
        ui->uartReadPlain->insertPlainText(recShowData);
        ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
    }

    ui->TXLenLabel->setText(QString::number(send_buf_len)+"bytes");
}

//读取串口接收消息
void MainWindow::ReadData()
{
    //串口可读数据长度
    int byteLen = serial->bytesAvailable();
    if(byteLen <= 0)
        return;

    rec_buf_len += byteLen;
    uart_rec_ss.append(serial->readAll());  //读取数据

    //计时器超过最大间隔仍未填入数据，强制填入
    if(fTimeCounter->hasExpired(2000) && uart_rec_ss.size()>0)
    {
        fTimeCounter->restart();
        ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
        insertDataToPlain();
        ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾

        uart_rec_ss.clear();
        ui->RXLenLabel->setText(QString::number(rec_buf_len)+"bytes");
    }

    //定时器开始工作、定时器重启
    uartRecDataTimer->start();
}

//清除发送
void MainWindow::on_clearSendButton_clicked()
{
    ui->uartWritePlain->clear();
}

//清除接收
void MainWindow::on_clearRecButton_clicked()
{
    ui->uartReadPlain->clear();

    send_buf_len = 0;
    rec_buf_len = 0;
    ui->TXLenLabel->setText("0bytes");
    ui->RXLenLabel->setText("0bytes");
}

//超时间隔设置
void MainWindow::on_overTimeRecEdit_returnPressed()
{
    if(ui->overTimeRecEdit->text().toFloat()>60)
    {
        QMessageBox::warning(NULL,"警告","超时时间不要超过1分钟");
        ui->overTimeRecEdit->setText("0.1");
        return;
    }
    uartRecOvertimeCount = ui->overTimeRecEdit->text().toFloat();
    ui->uartReadPlain->insertPlainText("设置超时时间为："+QString::number(uartRecOvertimeCount*1000)+"ms");
    uartRecDataTimer->setInterval(uartRecOvertimeCount*1000);                       //设置定时周期，单位：毫秒

    fTimeCounter->restart();
    uartRecDataTimer->start();
}

//接收字体窗口
void MainWindow::receiveFont(QFont font)
{
    ui->uartReadPlain->setFont(font);
}

//保存文本
void MainWindow::on_saveDataButton_clicked()
{
    QString curPath = QDir::currentPath();      //获取系统当前目录
    QString dlgTitle = "另存为一个文件";           //对话框标题
    QString filter = "文本文件(*.txt);;所有文件(*.*);;h文件(*.h)";    //文件过滤器
    QString aFileName = QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (aFileName.isEmpty())
        return;
    saveTextByIODevice(aFileName);
}

//用IODevice方式保存文本文件
bool MainWindow::saveTextByIODevice(const QString &aFileName)
{
    QFile aFile(aFileName);
    //aFile.setFileName(aFileName);
    if (!aFile.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    QString str = ui->uartReadPlain->toPlainText();     //整个内容作为字符串
    QByteArray strBytes = str.toUtf8();                 //转换为字节数组
    //QByteArray  strBytes=str.toLocal8Bit();
    aFile.write(strBytes,strBytes.length());            //写入文件
    aFile.close();
    ui->uartReadPlain->clear();
    send_buf_len = 0;
    rec_buf_len = 0;
    ui->TXLenLabel->setText("0bytes");
    ui->RXLenLabel->setText("0bytes");

    QMessageBox::information(NULL, "提示", "保存完成\r\n");
    return true;
}

//用IODevice方式打开文本文件
bool MainWindow::openTextByIODevice(const QString &aFileName)
{
    QFile aFile(aFileName);
    if (!aFile.exists())                //文件不存在
        return false;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    ui->uartReadPlain->insertPlainText(QString(aFile.readAll()));
    aFile.close();
    return  true;
}

//打开文件
void MainWindow::on_readLogButton_clicked()
{
    QString curPath=QDir::currentPath();    //获取系统当前目录
    QString dlgTitle="打开文件";            //对话框标题
    QString filter="文本文件(*.txt);;程序文件(*.h *.cpp);;所有文件(*.*)"; //文件过滤器
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if (aFileName.isEmpty())
        return;
    openTextByIODevice(aFileName);
}

//刷新端口
void MainWindow::on_refreshPortButton_clicked()
{
    GetAveriablePort();
}

//查找可用串口,刷新串口信息
void MainWindow::GetAveriablePort()
{
     ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
     ui->uartReadPlain->insertPlainText("\r\n串口初始化：\r\n");

     //先清除所有串口列表
      ui->portBox->clear();

    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);

        if(serial.open(QIODevice::ReadWrite))
        {
            ui->uartReadPlain->insertPlainText("可用："+serial.portName()+"\r\n");
            ui->portBox->addItem(serial.portName());
            serial.close();
        }
        else
        {
            ui->uartReadPlain->insertPlainText("不可用："+serial.portName()+"\r\n");
        }
    }

    ui->uartReadPlain->moveCursor(QTextCursor::End);        //光标移动到结尾
}

//更新样式
void MainWindow::updateMainStyle(QString style)
{
    //QSS文件初始化界面样式
    QFile qss("qss/"+style);
    if(qss.open(QFile::ReadOnly))
    {
        qDebug()<<"qss load";
        QTextStream filetext(&qss);
        QString stylesheet = filetext.readAll();
        this->setStyleSheet(stylesheet);
        qss.close();
    }
    else
    {
        qDebug()<<"qss not load";
        qss.setFileName("/qss/"+style);
        if(qss.open(QFile::ReadOnly))
        {
            qDebug()<<"qss load";
            QTextStream filetext(&qss);
            QString stylesheet = filetext.readAll();
            this->setStyleSheet(stylesheet);
            qss.close();
        }
    }
}

//保存参数
void MainWindow::on_paramSaveButton_clicked()
{
    qDebug()<<"save param";
    if(SaveUartParam())
    {
        QMessageBox::information(NULL,"保存参数","保存成功");
    }
}


//发送串口数据
QString MainWindow::sendUartData(QString data,bool isHex,bool hasTimerStamp,bool isAT)
{
    curDateTime = QDateTime::currentDateTime();

    QString command = "";
    QString showBuff = "";

    command.append(data);

    if(isAT){
        command.append("\r\n");
    }

    if(hasTimerStamp){
//        QString tempStr = "\r\n"+curDateTime.toString("[yyyy-MM-dd hh:mm:ss]")+"S:";
        QString tempStr = "\r\n"+curDateTime.toString("[hh:mm:ss]")+"S:";
        showBuff = tempStr + command;
    }

    //HEX格式发送
    if(isHex){
        //将接收到的HEX格式字符串转为HEX格式原始数据
        QByteArray arr;

        //分析字符串格式中是否带空格
        for(int i = 0;i<data.size();i++)
        {
            if(data[i]==' ')
                continue;   //跳过

            int num  = data.mid(i,2).toUInt(nullptr,16);
            i++;

            arr.append(num);
        }

        send_buf_len += arr.length();
        //发送HEX格式原始数据
        serial->write(arr);

        //返回HEX格式字符串显示到串口接收面板
        return showBuff;
    }
    //非HEX格式发送
    else {
        send_buf_len += data.length();
        serial->write(command.toLatin1());
        return showBuff;
    }
}


