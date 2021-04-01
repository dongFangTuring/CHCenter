#ifndef FFTCHARTWINDOW_H
#define FFTCHARTWINDOW_H

#include "chartwindow.h"
#include "utilities/fft.h"

namespace Ui {
class FFTChartWindow;
}

class FFTChartWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FFTChartWindow(QWidget *parent = nullptr);
    ~FFTChartWindow() override;

signals:
    void start_fft_cal(qint8 index, qint32 sample_frq, QVector<double> data);

private slots:
    void get_fft_result(qint8 index, QVector<double> x_ticks, QVector<double> result);

private:
    Ui::FFTChartWindow *ui;

    QChart *m_chart=nullptr;
    CusChartView *m_chartView=nullptr;

    //Y and X axis range
    std::array<short,2> valueRange;

    const float max_sample_number=50000;
    //record the type of chart, such as quat, acc..
    QString m_type;

    QList<QLineSeries *> m_serieslist;   //曲線列表
    QList<QPointF> point_X;

    uint framerate;
    uint sample_counter;
    QValueAxis *axisX=nullptr;
    QValueAxis *axisY=nullptr;

    //fft params
    QVector<QVector<double>> graphsData;    //用于fft计算的数据，按曲线分组
    QThread *fft_thread = nullptr;
    fft_trans *fft = nullptr;

    QLineSeries fft_layer;
    int32_t inputCntPerSec = 0;
    int32_t estimated_sample_frq = 0;
    int32_t fft_sample_frq = 0;
    int32_t fft_cal_point;
    uint8_t needReplot = 0;
    int32_t last_fft_cal_point = 0;
    int32_t fft_cal_point_changed_tips = 0;

    void initChartParam();
    void initFFTParam();
    void startFFTCal();
};

#endif // FFTCHARTWINDOW_H
