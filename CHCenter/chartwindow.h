#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QChartView>   //兩個基本模組

#include <QPointF>     //點類
#include <QList>         //列表
#include <QTimer> //定時器
#include <QTime>
#include <QDebug>

#include <QPen>
#include <QPainter>

#include <QThread>




QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;                 //引入這兩個類而免於引入整個標頭檔案的方法
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE   //使用qtchart需要加入這條語句

QT_BEGIN_NAMESPACE
namespace Ui { class ChartWindow; }
QT_END_NAMESPACE

class CusChartView: public QChartView
{
    Q_OBJECT

public:
    CusChartView(QChart* chart, QWidget *parent = 0);
    int valueRange[2];
    float max_sample_number;

    void zoom(bool in_out, bool x_y);

protected:

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QPoint m_lastMousePos;

};



class ChartWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWindow(QWidget *parent = nullptr, QString type="");
    ~ChartWindow();

    CusChartView *m_chartView;   //因為佈局時其它函式會訪問這個畫布，所以設為public
    void updateChart(float *);
    void init();



public slots:
    //	void handleTimeout();

    //幾個操作資料的槽函式
    void addSeries(QList<QPointF> &data, QString legend_title);     //新增一條曲線
    void removeSeries();                            //移出一條曲線
    void connectMarkers();                   //連線圖線與圖例
    void disconnectMarkers();               //斷開圖線與圖例
    void handleMarkerClicked();           //佔擊圖例時的處理函式

private slots:
    void updateMovingWindow();

    void on_SliderSample_valueChanged(int value);

    void on_BTNSampleZoomIn_clicked();

    void on_BTNSampleZoomOut_clicked();

    void on_SliderValue_valueChanged(int value);

    void on_BTNValueZoomIn_clicked();

    void on_BTNValueZoomOut_clicked();

    void on_BNTValueReset_clicked();

private:
    Ui::ChartWindow *ui;

    int valueRange[2];
    const float max_sample_number=50000;
    //紀錄圖表資料種類
    QString m_type;

    QList<QTime> timelist;
    QTimer movingwindow_timer;

    QChart * m_chart;     //圖表元件，可理解為畫筆，用它畫曲線
    QList<QLineSeries *> m_serieslist;   //曲線列表
    QList<QPointF> point_X;
    QList<QPointF> point_Y;
    QList<QPointF> point_Z;
    QList<QPointF> point_W;
    QList<QPointF> refresh_Line;


    uint sample_counter;

    QLineSeries *m_series;     //曲線指標
    QValueAxis *axisX;
    QValueAxis *axisY;

    QThread *m_thread;


};


#endif // CHARTWINDOW_H
