#include "chartwindow.h"
#include "ui_chartwindow.h"

ChartWindow::ChartWindow(QWidget *parent, QString type) :
    QWidget(parent),
    ui(new Ui::ChartWindow)

{
    ui->setupUi(this);
    m_type=type;


    m_chart = new QChart;
    movingwindow_timer.setInterval(30);
    connect(&movingwindow_timer, SIGNAL(timeout(void)), this, SLOT(updateMovingWindow(void)));
    movingwindow_timer.start();

    //create default axes first, so we can add series later.
    axisX = new QValueAxis;
    axisY = new QValueAxis;

    //axisX->setTickCount(11);//this will cause lag

    m_chart->addAxis(axisX, Qt::AlignBottom);//將座標軸加到chart上，居下
    m_chart->addAxis(axisY, Qt::AlignLeft);//居左


    if(type=="acc"){

        this->setWindowTitle(tr("Acceleration Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        valueRange[0]=-8;
        valueRange[1]=8;
        m_chart->setTitle(tr("Acceleration (G)"));

        axisY->setTickCount(9);
        axisY->setMinorTickCount(1);
    }
    else if (type=="gyr"){

        this->setWindowTitle(tr("Angular Velecity Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        valueRange[0]=-2000;
        valueRange[1]=2000;
        m_chart->setTitle(tr("Angular Velecity (°/s)"));

        axisY->setTickCount(11);
        axisY->setMinorTickCount(1);

    }
    else if (type=="mag"){

        this->setWindowTitle(tr("Magnetic Field Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        valueRange[0]=-80;
        valueRange[1]=80;
        m_chart->setTitle(tr("Magnetic Field (μT)"));

        axisY->setTickCount(9);
        axisY->setMinorTickCount(1);
    }
    else if (type=="eul"){

        this->setWindowTitle(tr("Eular Angles Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        valueRange[0]=-180;
        valueRange[1]=180;
        m_chart->setTitle(tr("Euler Angles (°)"));

        axisY->setTickCount(19);
        axisY->setMinorTickCount(1);
    }
    else if (type=="quat"){

        this->setWindowTitle(tr("Quaternion Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        addSeries(point_W, "W");
        valueRange[0]=-1;
        valueRange[1]=1;
        m_chart->setTitle(tr("Quaternion (Norm)"));

        axisY->setTickCount(11);
        axisY->setMinorTickCount(1);
    }
    refresh_Line.clear();
    refresh_Line.append(QPoint(sample_counter-1,valueRange[0]));
    refresh_Line.append(QPoint(sample_counter-1,valueRange[1]));
    addSeries(refresh_Line,"Refresh Line");

    sample_counter=0;
    axisX->setLabelFormat("%d");

    //    axisX->setTitleVisible(false);
    //    axisX->setLabelsVisible(false);

    axisY->setRange(valueRange[0], valueRange[1]);

    m_chart->legend()->setVisible(true);  //設定圖例可見

    connectMarkers();  //將曲線與圖例連線起來，可以勾選進行顯示與隱藏


    m_chartView = new CusChartView(m_chart);
    //m_chartView->setRenderHint(QPainter::Antialiasing);
    //m_chartView->setRubberBand(QChartView::VerticalRubberBand);  //垂直縮放

    ui->LayoutChart->addWidget(m_chartView);

    ui->SliderSample->setRange(100,4000);

    ui->SliderSample->setPageStep(1000);
    ui->SliderSample->setTickInterval(100);
    ui->SliderSample->setSingleStep(100);
    ui->SliderSample->setValue(500);

    ui->SliderValue->setRange(1,10);
    ui->SliderValue->setSingleStep(1);
    ui->SliderValue->setValue(10);
}

ChartWindow::~ChartWindow()
{

}

void ChartWindow::updateChart(float *array){

    if(this->isVisible()){
        int max_sample_number=ui->SliderSample->value();

        if(sample_counter>=max_sample_number){
            sample_counter=0;
            refresh_Line.clear();
            refresh_Line.append(QPoint(sample_counter-1,valueRange[0]));
            refresh_Line.append(QPoint(sample_counter-1,valueRange[1]));
        }

        else{
            refresh_Line.clear();
            refresh_Line.append(QPoint(sample_counter+2,valueRange[0]));
            refresh_Line.append(QPoint(sample_counter+2,valueRange[1]));

            if(m_type=="quat"){
                if(point_X.count()>=max_sample_number){
                    point_W.replace(sample_counter , QPointF(sample_counter, array[0]));
                    point_X.replace(sample_counter , QPointF(sample_counter, array[1]));
                    point_Y.replace(sample_counter , QPointF(sample_counter, array[2]));
                    point_Z.replace(sample_counter , QPointF(sample_counter, array[3]));
                }
                else{
                    point_W.append(QPointF(sample_counter,array[0]));
                    point_X.append(QPointF(sample_counter,array[1]));
                    point_Y.append(QPointF(sample_counter,array[2]));
                    point_Z.append(QPointF(sample_counter,array[3]));
                }

            }
            else{
                if(point_X.count()>=max_sample_number){
                    point_X.replace(sample_counter , QPointF(sample_counter, array[0]));
                    point_Y.replace(sample_counter , QPointF(sample_counter, array[1]));
                    point_Z.replace(sample_counter , QPointF(sample_counter, array[2]));
                }
                else{
                    point_X.append(QPointF(sample_counter,array[0]));
                    point_Y.append(QPointF(sample_counter,array[1]));
                    point_Z.append(QPointF(sample_counter,array[2]));
                }


            }

            sample_counter++;
        }


        //        int max_sample_number=ui->SliderSample->value();

        //        if(sample_counter>=max_sample_number){
        //            sample_counter=0;

        //        }
        //        else{
        //            for(int i=0;i<m_serieslist.count();i++){

        //                if(m_serieslist.at(i)->count()>=max_sample_number){

        //                    m_serieslist.at(i)->replace(sample_counter , QPointF(sample_counter, array[i]));

        //                }
        //                else{
        //                    m_serieslist.at(i)->append(sample_counter,array[i]);
        //                }

        //            }

        //        }
        //        sample_counter++;
    }
    else{

    }
}
void ChartWindow::updateMovingWindow()
{
    if(this->isVisible()){
        m_chartView->setUpdatesEnabled(false);

        if(m_type=="quat"){
            m_serieslist.at(0)->replace(point_W);
            m_serieslist.at(1)->replace(point_X);
            m_serieslist.at(2)->replace(point_Y);
            m_serieslist.at(3)->replace(point_Z);
        }
        else{
            m_serieslist.at(0)->replace(point_X);
            m_serieslist.at(1)->replace(point_Y);
            m_serieslist.at(2)->replace(point_Z);

        }
        m_serieslist.last()->replace(refresh_Line);
        m_chartView->setUpdatesEnabled(true);
    }

}


void ChartWindow::init()
{
    sample_counter=0;
    point_W.clear();
    point_X.clear();
    point_Y.clear();
    point_Z.clear();
    refresh_Line.clear();
}


void ChartWindow::addSeries(QList<QPointF> &data, QString legend_title)  //用於新增曲線
{

    QLineSeries *series = new QLineSeries();
    series->setUseOpenGL(true);
    if(legend_title=="X"){
        series->setColor(QColor(171,34,29));
    }
    else if(legend_title=="Y"){
        series->setColor(QColor(13,139,77));
    }
    else if(legend_title=="Z"){
        series->setColor(QColor(65,83,175));
    }
    else if(legend_title=="W"){
        series->setColor(Qt::gray);
    }
    else if(legend_title=="Refresh Line"){
        series->setColor(Qt::black);
        series->setPen(QPen(Qt::black, 2));
    }


    m_serieslist.append(series);//將曲線加到曲線列表中進行管理
    series->setName(legend_title); //設定曲線對應的名字，用於圖例顯示
    series->append(data);  //將資料加到曲線中
    m_chart->addSeries(series);//將曲線增入chart中

    series->attachAxis(axisX);
    series->attachAxis(axisY);

}

void ChartWindow::removeSeries()
{
    // Remove last series from chart
    if (m_serieslist.count() > 0) {
        QLineSeries *series = m_serieslist.last();
        m_chart->removeSeries(series);
        m_serieslist.removeLast();
        delete series;
    }
}

void ChartWindow::connectMarkers()
{
    // Connect all markers to handler
    foreach(QLegendMarker* marker, m_chart->legend()->markers()) {
        // Disconnect possible existing connection to avoid multiple connections
        QObject::disconnect(marker, &QLegendMarker::clicked, this, &ChartWindow::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &ChartWindow::handleMarkerClicked);
    }
}

void ChartWindow::disconnectMarkers()
{
    foreach(QLegendMarker* marker, m_chart->legend()->markers()) {
        QObject::disconnect(marker, &QLegendMarker::clicked, this, &ChartWindow::handleMarkerClicked);
    }
}

void ChartWindow::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);

    switch (marker->type())

    {
    case QLegendMarker::LegendMarkerTypeXY:
    {

        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        // Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!marker->series()->isVisible()) {
            alpha = 0.5;
        }

        QColor color;
        QBrush brush = marker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setLabelBrush(brush);

        brush = marker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        marker->setBrush(brush);

        QPen pen = marker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        marker->setPen(pen);

        break;
    }
    default:
    {
        qDebug() << "Unknown marker type";
        break;

    }
    }
}



void ChartWindow::on_SliderSample_valueChanged(int value)
{
    value=value/100*100;
    ui->SliderSample->setValue(value);
    ui->LabelSampleNumber->setText(tr("%1 Samples").arg(value));

    axisX->setRange(0, value);

    init();

}

void ChartWindow::on_BTNSampleZoomIn_clicked()
{
    int cur_scale=ui->SliderSample->value()/1000*1000;
    if(cur_scale<=1000)
        cur_scale=100;
    else
        cur_scale-=1000;
    ui->SliderSample->setValue(cur_scale);
}

void ChartWindow::on_BTNSampleZoomOut_clicked()
{
    int cur_scale=ui->SliderSample->value()/1000*1000;
    if(cur_scale>=10000)
        cur_scale=10000;
    else
        cur_scale+=1000;
    ui->SliderSample->setValue(cur_scale);
}

void ChartWindow::on_SliderValue_valueChanged(int value)
{
    float f_cur_scale=float(value)/10;

    axisY->setRange(valueRange[0]*f_cur_scale,valueRange[1]*f_cur_scale);
}

void ChartWindow::on_BTNValueZoomIn_clicked()
{
    int cur_scale=ui->SliderValue->value();
    if(cur_scale<=1){
        cur_scale=1;
        on_SliderValue_valueChanged(cur_scale);
    }
    else
        cur_scale-=1;

    ui->SliderValue->setValue(cur_scale);

}

void ChartWindow::on_BTNValueZoomOut_clicked()
{
    int cur_scale=ui->SliderValue->value();
    if(cur_scale>=10){
        cur_scale=10;
        on_SliderValue_valueChanged(cur_scale);
    }
    else
        cur_scale+=1;

    ui->SliderValue->setValue(cur_scale);
}

CusChartView::CusChartView(QChart* chart, QWidget *parent)
    : QChartView(chart, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);
}

void CusChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QApplication::setOverrideCursor(QCursor(Qt::SizeVerCursor));
        m_lastMousePos = event->pos();
        event->accept();
    }

    QChartView::mousePressEvent(event);
}

void CusChartView::mouseMoveEvent(QMouseEvent *event)
{
    // pan the chart with a middle mouse drag
    if (event->buttons() & Qt::LeftButton)
    {
        QValueAxis *axisY = (QValueAxis*)(this->chart()->axisY());//static cast is faster, father to child
        qreal cur_x_min = axisY->min();//目前X軸顯示區間的最小x值
        qreal cur_x_max = axisY->max();

        float chart_height=cur_x_max-cur_x_min;

        float move=m_lastMousePos.y()-event->pos().y();
        float factor=chart_height/this->height();
        move=move*factor;


        this->chart()->axisY()->setRange(cur_x_min+move,cur_x_max+move);

        m_lastMousePos = event->pos();
        event->accept();
    }

    QChartView::mouseMoveEvent(event);
}

void CusChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QApplication::restoreOverrideCursor();
        event->accept();
    }
    QChartView::mouseReleaseEvent(event);
}
