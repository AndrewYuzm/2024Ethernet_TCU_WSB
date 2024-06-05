#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Socket = new QTcpSocket;

    qchart = new QChart;

    //设置抗锯齿
    ui->mainChart->setRenderHint(QPainter::Antialiasing);

    //创建1条线
    ser0 = new QSplineSeries;
    // auto lineSeries = new QChart::QSplineSeries(this);
    // *ser0 << QPointF(0, 20) << QPointF(1, 12) << QPointF(2, 10) << QPointF(3, 13);

    // QLineSeries *ser1 = new QSplineSeries;
    // *ser1 << QPointF(0, 25) << QPointF(1, 19) << QPointF(2, 3) << QPointF(3, 15);
    //设置名字
    ser0->setName("CAN0");
    // ser1->setName("CAN1");

    // 给图表对象添加数据列
    qchart->addSeries(ser0);
    // qchart->addSeries(ser1);
    // 让图标根据数据列自动创建坐标轴
    // qchart->createDefaultAxes();
    // 将图标装进QChartView
    ui->mainChart->setChart(qchart);

    // CreatCharts();

    // 创建X轴，这里对齐方式选择AlignBottom，范围设置为(-10, 10)
    axisX = new QValueAxis{qchart};
    axisX->setRange(0, 3);
    // 创建Y轴，这里对齐方式选择AlignLeft，范围设置为(-10, 30)
    axisY = new QValueAxis{qchart};
    axisY->setRange(-2, 2);

    // 为图表添加坐标轴，并设置好对齐方式
    qchart->addAxis(axisX, Qt::AlignmentFlag::AlignBottom);
    qchart->addAxis(axisY, Qt::AlignmentFlag::AlignLeft);

    // 为数据绑定坐标轴
    ser0->attachAxis(axisX);
    ser0->attachAxis(axisY);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//创建表
void MainWindow::CreatCharts(){
    qchart = new QChart;

    //设置抗锯齿
    ui->mainChart->setRenderHint(QPainter::Antialiasing);

    //创建1条线
    ser0 = new QSplineSeries;
    // auto lineSeries = new QChart::QSplineSeries(this);
    // *ser0 << QPointF(0, 20) << QPointF(1, 12) << QPointF(2, 10) << QPointF(3, 13);

    // QLineSeries *ser1 = new QSplineSeries;
    // *ser1 << QPointF(0, 25) << QPointF(1, 19) << QPointF(2, 3) << QPointF(3, 15);
    //设置名字
    ser0->setName("CAN0");
    // ser1->setName("CAN1");

    // 给图表对象添加数据列
    qchart->addSeries(ser0);
    // qchart->addSeries(ser1);
    // 让图标根据数据列自动创建坐标轴
    qchart->createDefaultAxes();
    // 将图标装进QChartView
    ui->mainChart->setChart(qchart);

    // 创建坐标轴对象
    // 设置坐标轴参数
    // 将坐标轴添加到图表中
    // 将数据列与坐标轴绑定

    // 创建X轴，这里对齐方式选择AlignBottom，范围设置为(-10, 10)
    // auto *axisX = new QValueAxis{qchart};
    // axisX->setRange(0, 10);
    // 创建Y轴，这里对齐方式选择AlignLeft，范围设置为(-10, 30)
    // auto *axisY = new QValueAxis{qchart};
    // axisY->setRange(-10, 30);

    // 为图表添加坐标轴，并设置好对齐方式
    // qchart->addAxis(axisX, Qt::AlignmentFlag::AlignBottom);
    // qchart->addAxis(axisY, Qt::AlignmentFlag::AlignLeft);

    // 为数据绑定坐标轴
    // ser0->attachAxis(axisX);
    // ser0->attachAxis(axisY);
    // ser1->attachAxis(axisX);
    // ser1->attachAxis(axisY);
    // /* 定时200ms */
    // timer->start(200);
    // /* 信号槽连接 */
    // connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));
}
// void MainWindow::timerTimeOut(){

// }
// void MainWindow::receivedData(int value)
// {
//     /* 将数据添加到data中 */
//     data.append(value);

//     /* 当储存数据的个数大于最大值时，把第一个数据删除 */
//     while (data.size() > maxSize) {
//         /* 移除data中第一个数据 */
//         data.removeFirst();
//     }

//     /* 先清空 */
//     splineSeries->clear();

//     /* 计算x轴上的点与点之间显示的间距 */
//     int xSpace = maxX / (maxSize - 1);

//     /* 添加点，xSpace * i 表示第i个点的x轴的位置 */
//     for (int i = 0; i < data.size(); ++i) {
//         splineSeries->append(xSpace * i, data.at(i));
//     }
// }

void MainWindow::on_connectButton_clicked()
{
    //get IP addr and port
    QString IP = ui->IP_lineEdit->text();
    QString Port = ui->PortlineEdit_2->text();

    //connect to Server Host
    Socket->connectToHost(QHostAddress(IP),Port.toShort());
    connect(Socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    //check if successed
    connect(Socket,&QTcpSocket::connected,[this](){
        QMessageBox::information(this, "hint", "connected to Server");
    });

    //if lost connection
    connect(Socket,&QTcpSocket::disconnected,[this](){
        QMessageBox::information(this, "hint", "disconnected with Server or connection lost");
    });
}


void MainWindow::readData(){
    // QByteArray TCP_rev_data = Socket->readAll();
    // // 解析JSON并提取t和y值
    // // 假设JSON格式为：{"t": 1, "y": 2}

    // QJsonDocument doc = QJsonDocument::fromJson(TCP_rev_data);
    // QJsonObject obj = doc.object();
    // double t = obj["t"].toDouble();
    // double y = obj["y"].toDouble();

    // // char temp_txt[50];
    // // std::sprintf(temp_txt,"t=%f",t);
    // // QMessageBox::information(this, "hint",temp_txt);
    // ser0->append(t, y);
    // // qchart->addSeries(ser0);
    // // ser0->attachAxis(axisX);
    // // ser0->attachAxis(axisY);

    // // chart->axisX()->setMax(t); // 动态调整X轴

    QByteArray TCP_rev_data = Socket->readAll();

    // 使用换行符分割接收到的数据
    QList<QByteArray> jsonMessages = TCP_rev_data.split('\n');

    foreach (const QByteArray &jsonData, jsonMessages) {
        if (!jsonData.isEmpty()) {
            // 解析JSON并提取t和y值
            // 假设JSON格式为：{"t": 1, "y": 2}
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            QJsonObject obj = doc.object();
            double t = obj["t"].toDouble();
            double y = obj["y"].toDouble();
            //test
            if(t==0) ser0->clear();
            // 将解析后的数据添加到序列中
            ser0->append(t, y);
        }
    }
}


void MainWindow::on_Disc_pushButton_clicked()
{
    Socket->disconnectFromHost();
    //if disconnected
    connect(Socket,&QTcpSocket::disconnected,[this](){
        QMessageBox::information(this, "hint", "disconnected with Server");
    });
}

