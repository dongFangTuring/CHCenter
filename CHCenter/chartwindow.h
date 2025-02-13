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
#include <math.h>
#include <QCloseEvent>
#include <QShowEvent>
#include <array>

QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;                 //引入這兩個類而免於引入整個標頭檔案的方法
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE   //使用qtchart需要加入這條語句

QT_BEGIN_NAMESPACE
namespace Ui
{
class ChartWindow;
}
QT_END_NAMESPACE

class CusChartView: public QChartView
{
    Q_OBJECT

public:
    CusChartView(QChart* chart, QWidget *parent = nullptr);
    std::array<short,2> valueRange;

    float max_sample_number;
    uint sample_counter;

    int zoom_mode=0;
    bool isFreeMode=false;

    void zoom(bool in_out, bool x_y, int mode);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void sigUserWheelZoom();

private:
    QPoint m_lastMousePos;
    bool key_ctrl_pressed=false;
    QPoint cursor_pos;

    uchar scale_level=5;
    ushort x_scales[15]= {50,100,200,500,1000,2000,5000,7500,10000,12500,15000,17500,20000,30000,50000};
};



class ChartWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ChartWindow(QWidget *parent = nullptr, QString type="");
    ~ChartWindow() override;

    void setFrameRate(uint tmp){
        if(this->isVisible())
            framerate=tmp;
    }
    void updateLineData(float *);


public slots:
    //幾個操作資料的槽函式
    void addSeries(QList<QPointF> &data, QString legend_title);     //新增一條曲線
    void removeSeries();                            //移出一條曲線
    void connectMarkers();                   //連線圖線與圖例
    void disconnectMarkers();               //斷開圖線與圖例
    void handleMarkerClicked();           //佔擊圖例時的處理函式
protected:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void updateMovingWindow();

private:
    Ui::ChartWindow *ui;

    //Y and X axis range
    std::array<short,2> valueRange;

    const float max_sample_number=50000;
    //record the type of chart, such as quat, acc..
    QString m_type;

    QChart *m_chart=nullptr;     //圖表元件，可理解為畫筆，用它畫曲線
    CusChartView *m_chartView=nullptr;
    QList<QLineSeries *> m_serieslist;   //曲線列表

    QList<QPointF> point_X;
    QList<QPointF> point_Y;
    QList<QPointF> point_Z;
    QList<QPointF> point_W;
    QList<QPointF> point_RFLine;
    QList<QPointF> point_norm;

    uint framerate;
    uint sample_counter;

    QValueAxis *axisX=nullptr;
    QValueAxis *axisY=nullptr;

    void initChartParam();
};


#endif // CHARTWINDOW_H
