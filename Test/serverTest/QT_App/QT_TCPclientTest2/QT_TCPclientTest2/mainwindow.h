#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChartView>
// QT_CHARTS_USE_NAMESPACE
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QSplineSeries>

#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include <QTcpSocket>
#include <QHostAddress>
#include <QMessagebox>

#include <QTimer>
#include <QDateTime>
#include <math.h>
#include <cstdio>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void CreatCharts();
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QChart *qchart;
    QLineSeries *series;
    QTcpSocket *Socket; //Socket Handle
    /* 接收数据接口 */
    void receivedData(int);
    void readData();

    /* 数据最大个数 */
    int maxSize;

    /* x轴上的最大值 */
    int maxX;

    /* y轴上的最大值 */
    int maxY;

    /* y轴 */
    QValueAxis *axisY;

    /* x轴 */
    QValueAxis *axisX;

    /* QList double类型容器 */
    QList<double> x_data;
    QList<double> y_data;

    /* QSplineSeries对象（曲线）*/
    QSplineSeries *splineSeries;
    QSplineSeries *ser0,*ser1;

    /* QChart图表 */
    QChart *chart;

    /* 图表视图 */
    QChartView *chartView;

    /* 定时器 */
    // QTimer *timer;
private slots:
    // void timerTimeOut();
    void on_connectButton_clicked();
    void on_Disc_pushButton_clicked();
};
#endif // MAINWINDOW_H
