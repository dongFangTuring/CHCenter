#include "fftchartwindow.h"
#include "ui_fftchartwindow.h"

FFTChartWindow::FFTChartWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FFTChartWindow)
{
    ui->setupUi(this);
}

FFTChartWindow::~FFTChartWindow()
{
    delete ui;
}

void FFTChartWindow::get_fft_result(qint8 index, QVector<double> x_ticks, QVector<double> result)
{

}

void FFTChartWindow::initChartParam()
{
    m_chart = new QChart;
    m_chartView = new CusChartView(m_chart);

    //create default axes first, so we can add series later.
    axisX = new QValueAxis;
    axisY = new QValueAxis;

    m_chart->addAxis(axisX, Qt::AlignBottom);//將座標軸加到chart上，居下
    m_chart->addAxis(axisY, Qt::AlignLeft);//居左


    axisX->setLabelFormat("%d");
    axisX->setTitleText("Samples");

    //set axis tick
    axisY->setTickAnchor(0);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(1);
    axisY->setTitleText("Value");

    //sync max and min value to cuschart class
    m_chartView->valueRange=valueRange;
    m_chartView->max_sample_number = max_sample_number;

    //tune performance of opengl
    m_chartView->setRenderHints(QPainter::Antialiasing);
    m_chartView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    m_chartView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);

    ui->LayoutChart->addWidget(m_chartView);

    m_chartView->setFocus();

    //reset sample_counter
    sample_counter=0;

    //reset zoom
    axisX->setRange(0,1000);
    axisY->setRange(valueRange.at(0), (valueRange.at(1)));
}

void FFTChartWindow::initFFTParam()
{
    fft_thread = new QThread(this);
    fft = new fft_trans();
    fft->init_size(fft_cal_point);
    fft->moveToThread(fft_thread);
    qRegisterMetaType<QVector<double>>("QVector<double>");
    connect(fft_thread, SIGNAL(finished()), fft, SLOT(deleteLater()));
    connect(this, SIGNAL(start_fft_cal(qint8, qint32, QVector<double>)), fft, SLOT(fft_calculate(qint8, qint32, QVector<double>)));
    connect(fft, SIGNAL(fft_result(qint8, QVector<double>, QVector<double>)), this, SLOT(get_fft_result(qint8, QVector<double>, QVector<double>)));
    fft_thread->start();
}

void FFTChartWindow::startFFTCal()
{
    for(int32_t i = 0; i < graphsData.size(); i++)
    {
        emit start_fft_cal(i, fft_sample_frq, graphsData.at(i));
    }


}
