#include "chartwindow.h"
#include "ui_chartwindow.h"


ChartWindow::ChartWindow(QWidget *parent, QString type) :
    QWidget(parent),
    ui(new Ui::ChartWindow)

{
    ui->setupUi(this);
    m_type=type;

    this->setStyleSheet("background-color:#424242; color:white;");

    m_chart = new QChart;
    m_chartView = new CusChartView(m_chart);


    //create default axes first, so we can add series later.
    axisX = new QValueAxis;
    axisY = new QValueAxis;


    m_chart->addAxis(axisX, Qt::AlignBottom);//將座標軸加到chart上，居下
    m_chart->addAxis(axisY, Qt::AlignLeft);//居左


    if(type=="acc") {

        this->setWindowTitle(tr("Acceleration Chart"));

        addSeries(point_RFLine, "");
        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        addSeries(point_norm, "Norm");

        valueRange[0] = -16;
        valueRange[1] = 16;
        m_chart->setTitle(tr("Acceleration (G)"));

    } else if (type=="gyr") {

        this->setWindowTitle(tr("Angular Velocity Chart"));

        addSeries(point_RFLine, "");
        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        addSeries(point_norm, "Norm");

        valueRange[0] = -2000;
        valueRange[1] = 2000;
        m_chart->setTitle(tr("Angular Velocity (°/s)"));

    } else if (type=="mag") {

        this->setWindowTitle(tr("Magnetic Field Chart"));

        addSeries(point_RFLine, "");
        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        addSeries(point_norm, "Norm");

        valueRange[0] = -200;
        valueRange[1] = 200;
        m_chart->setTitle(tr("Magnetic Field (μT)"));
    } else if (type=="eul") {

        this->setWindowTitle(tr("Eular Angles Chart"));

        addSeries(point_RFLine, "");
        addSeries(point_X, "Roll");
        addSeries(point_Y, "Pitch");
        addSeries(point_Z, "Yaw");


        valueRange[0] = -180;
        valueRange[1] = 360;
        m_chart->setTitle(tr("Euler Angles (°)"));

    } else if (type=="quat") {

        this->setWindowTitle(tr("Quaternion Chart"));

        addSeries(point_RFLine, "");
        addSeries(point_W, "W");
        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");


        valueRange[0] = -1;
        valueRange[1] = 1;
        m_chart->setTitle(tr("Quaternion (Norm)"));

    }



    axisX->setLabelFormat("%d");
    axisX->setTitleText("Samples");

    axisY->setTickAnchor(0);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(1);
    axisY->setTitleText("Value");


    //sync max and min value to cuschart class

    m_chartView->valueRange[0]=valueRange[0];
    m_chartView->valueRange[1]=valueRange[1];
    m_chartView->max_sample_number=max_sample_number;

    //tune performance of opengl
    m_chartView->setRenderHints(QPainter::Antialiasing);
    m_chartView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    //m_chartView->setCacheMode(QGraphicsView::CacheBackground); //no effect to performance
    m_chartView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    //m_chart->setCacheMode(QGraphicsItem::ItemCoordinateCache,QSize(2000,2000)); //no effect to performance

    //m_chartView->setRubberBand(QChartView::RectangleRubberBand);  //整體縮放

    ui->LayoutChart->addWidget(m_chartView);


    m_chart->legend()->setVisible(true);  //設定圖例可見
    connectMarkers(); //將曲線與圖例連線起來，可以勾選進行顯示與隱藏
    m_chart->legend()->markers().first()->setVisible(false);//隱藏刷新線圖例

    init();

}

ChartWindow::~ChartWindow()
{
}

void ChartWindow::init()
{
    //default following mode
    m_chartView->isFreeMode=false;
    m_chartView->setFocus();


    //reset sample_counter
    sample_counter=0;

    //reset zoom
    axisX->setRange(0,1000);
    axisY->setRange(valueRange[0], valueRange[1]);

    //clear all lines
    point_W.clear();
    point_X.clear();
    point_Y.clear();
    point_Z.clear();
    point_norm.clear();
    point_RFLine.clear();

    //reserve ram spaces for points
    point_W.reserve(max_sample_number);
    point_X.reserve(max_sample_number);
    point_Y.reserve(max_sample_number);
    point_Z.reserve(max_sample_number);

    //pre-append data to 50000
    for(float i=0; i<max_sample_number; i++) {
        point_W.append(QPointF(i,0));
        point_X.append(QPointF(i,0));
        point_Y.append(QPointF(i,0));
        point_Z.append(QPointF(i,0));
        point_norm.append(QPointF(i,0));
    }

    //refresh line in free mode
    point_RFLine.append(QPointF(0,valueRange[0]));
    point_RFLine.append(QPointF(0,valueRange[1]));

}



void ChartWindow::updateLineData(float *array)
{


    if(this->isVisible()) {

        if(sample_counter>=max_sample_number) {
            sample_counter=0;
        }

        else {
            if(m_type=="quat") {
                point_W.replace(sample_counter, QPointF(sample_counter, array[0]));
                point_X.replace(sample_counter, QPointF(sample_counter, array[1]));
                point_Y.replace(sample_counter, QPointF(sample_counter, array[2]));
                point_Z.replace(sample_counter, QPointF(sample_counter, array[3]));

            } else {
                if(m_type=="acc"||m_type=="gyr"||m_type=="mag")
                    point_norm.replace(sample_counter, QPointF(sample_counter, sqrt(array[0]*array[0]+array[1]*array[1]+array[2]*array[2])));
                point_X.replace(sample_counter, QPointF(sample_counter, array[0]));
                point_Y.replace(sample_counter, QPointF(sample_counter, array[1]));
                point_Z.replace(sample_counter, QPointF(sample_counter, array[2]));
            }


            sample_counter++;
        }

        //dynamic change update frequency of chart
        uint update_interval;
        if(m_chartView->isFreeMode)
            update_interval=framerate/10;
        else
            update_interval=framerate/30;
        if(update_interval<1)
            update_interval=1;

        if(sample_counter%update_interval==0) {
            //move refresh line
            point_RFLine.replace(0,QPointF(sample_counter,valueRange[0]));
            point_RFLine.replace(1,QPointF(sample_counter,valueRange[1]));
            updateMovingWindow();
        }

    }



}
void ChartWindow::updateMovingWindow()
{
    if(this->isVisible()) {

        //for following mode
        if(m_chartView->isFreeMode==false) {

            qreal cur_x_min = axisX->min();//目前X軸顯示區間的最小x值
            qreal cur_x_max = axisX->max();//目前X軸顯示區間的最大x值
            qreal distance_x=abs(cur_x_max-cur_x_min);

            QList<QPointF> wSizePoints_X, wSizePoints_Y, wSizePoints_Z, wSizePoints_W, wSizePoints_norm;// a window size number of points.

            if(sample_counter>=distance_x) {
                if(m_type=="quat") {
                    wSizePoints_W=point_W.mid(sample_counter-(distance_x+1), distance_x+1) ;
                } else if(m_type=="acc"||m_type=="gyr"||m_type=="mag") {
                    wSizePoints_norm=point_norm.mid(sample_counter-(distance_x+1), distance_x+1) ;
                }

                wSizePoints_X=point_X.mid(sample_counter-(distance_x+1), distance_x+1) ;
                wSizePoints_Y=point_Y.mid(sample_counter-(distance_x+1), distance_x+1) ;
                wSizePoints_Z=point_Z.mid(sample_counter-(distance_x+1), distance_x+1) ;
            } else {
                if(m_type=="quat") {
                    wSizePoints_W=point_W.mid(0, sample_counter) ;
                } else if(m_type=="acc"||m_type=="gyr"||m_type=="mag") {
                    wSizePoints_norm=point_norm.mid(0, sample_counter);
                }
                wSizePoints_X=point_X.mid(0, sample_counter) ;
                wSizePoints_Y=point_Y.mid(0, sample_counter) ;
                wSizePoints_Z=point_Z.mid(0, sample_counter) ;
            }


            //renumbering X axis of each point from 1 to zoom scale
            for(int i =0; i<wSizePoints_X.length(); i++) {
                if(m_type=="quat")
                    wSizePoints_W.replace(i,QPointF(wSizePoints_W.at(i).x()+1-sample_counter+distance_x, wSizePoints_W.at(i).y()));
                else if(m_type=="acc"||m_type=="gyr"||m_type=="mag") {
                    wSizePoints_norm.replace(i,QPointF(wSizePoints_norm.at(i).x()+1-sample_counter+distance_x, wSizePoints_norm.at(i).y()));
                }

                wSizePoints_X.replace(i,QPointF(wSizePoints_X.at(i).x()+1-sample_counter+distance_x, wSizePoints_X.at(i).y()));
                wSizePoints_Y.replace(i,QPointF(wSizePoints_Y.at(i).x()+1-sample_counter+distance_x, wSizePoints_Y.at(i).y()));
                wSizePoints_Z.replace(i,QPointF(wSizePoints_Z.at(i).x()+1-sample_counter+distance_x, wSizePoints_Z.at(i).y()));

            }


            //replaced with new series
            if(m_type=="quat") {
                m_serieslist.at(1)->replace(wSizePoints_W);
                m_serieslist.at(2)->replace(wSizePoints_X);
                m_serieslist.at(3)->replace(wSizePoints_Y);
                m_serieslist.at(4)->replace(wSizePoints_Z);
            }

            else {
                m_serieslist.at(1)->replace(wSizePoints_X);
                m_serieslist.at(2)->replace(wSizePoints_Y);
                m_serieslist.at(3)->replace(wSizePoints_Z);

                if(m_type=="acc"||m_type=="gyr"||m_type=="mag")
                    m_serieslist.at(4)->replace(wSizePoints_norm);

            }


            //set range of X axis
            axisX->setRange(0,distance_x);

            if(m_chartView->zoom_mode!=2) {
                ui->LabelChartMode->setText(tr("Left Double Click: Follow(O)/FreeDrag mode"));
                m_chartView->zoom_mode=2;   //zoom at the newest
                m_serieslist.at(0)->setVisible(false); //hide refresh line
            }

        }
        //for free mode
        else {
            if(m_type=="quat") {
                m_serieslist.at(1)->replace(point_W);
                m_serieslist.at(2)->replace(point_X);
                m_serieslist.at(3)->replace(point_Y);
                m_serieslist.at(4)->replace(point_Z);
            } else {
                m_serieslist.at(1)->replace(point_X);
                m_serieslist.at(2)->replace(point_Y);
                m_serieslist.at(3)->replace(point_Z);

                if(m_type=="acc"||m_type=="gyr"||m_type=="mag")
                    m_serieslist.at(4)->replace(point_norm);

            }
            if(m_chartView->zoom_mode!=1) {
                qreal cur_x_min = axisX->min();//目前X軸顯示區間的最小x值
                qreal cur_x_max = axisX->max();//目前X軸顯示區間的最大x值
                qreal distance_x=abs(cur_x_max-cur_x_min);

                axisX->setRange(sample_counter-distance_x,sample_counter);

                if(sample_counter<distance_x) {
                    axisX->setRange(0,distance_x);
                }
                m_chartView->zoom_mode=1;  //zoom at the cursor pos
                ui->LabelChartMode->setText(tr("Left Double Click: Follow/FreeDrag(O) mode"));
            }

            m_serieslist.at(0)->replace(point_RFLine);
            m_serieslist.at(0)->setVisible(true);
            m_chart->legend()->markers().first()->setVisible(false);
        }



    }

}






void ChartWindow::addSeries(QList<QPointF> &data, QString legend_title)  //用於新增曲線
{

    QLineSeries *series = new QLineSeries();
    series->setUseOpenGL(true);

    m_serieslist.append(series);//將曲線加到曲線列表中進行管理
    series->setName(legend_title); //設定曲線對應的名字，用於圖例顯示
    series->append(data);  //將資料加到曲線中
    m_chart->addSeries(series);//將曲線增入chart中


    series->attachAxis(axisX);
    series->attachAxis(axisY);

    if(legend_title=="X" || legend_title=="Roll") {
        series->setPen(QPen(QColor(171,34,29), 1));
    } else if(legend_title=="Y" || legend_title=="Pitch") {

        series->setPen(QPen(QColor(13,139,77), 1));
    } else if(legend_title=="Z"  || legend_title=="Yaw") {

        series->setPen(QPen(QColor(65,83,175), 1));
    } else if(legend_title=="W") {
        series->setColor(Qt::gray);
    } else if(legend_title=="") {
        series->setPen(QPen(Qt::black, 1));
    } else if(legend_title=="Norm") {
        series->setPen(QPen(QColor(249,168,37), 1));
        auto norm_maker=m_chart->legend()->markers().last();
        norm_maker->series()->setVisible(false);
        norm_maker->setVisible(true);

        //Dim the marker, if series is not visible
        qreal alpha = 1.0;

        if (!norm_maker->series()->isVisible()) {
            alpha = 0.5;
        }

        QColor color;
        QBrush brush = norm_maker->labelBrush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        norm_maker->setLabelBrush(brush);

        brush = norm_maker->brush();
        color = brush.color();
        color.setAlphaF(alpha);
        brush.setColor(color);
        norm_maker->setBrush(brush);

        QPen pen = norm_maker->pen();
        color = pen.color();
        color.setAlphaF(alpha);
        pen.setColor(color);
        norm_maker->setPen(pen);
        this->update();
    }


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
    case QLegendMarker::LegendMarkerTypeXY: {

        // Toggle visibility of series
        marker->series()->setVisible(!marker->series()->isVisible());

        // Turn legend marker back to visible, since hiding series also hides the marker
        // and we don't want it to happen now.
        marker->setVisible(true);

        //Dim the marker, if series is not visible
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

        this->update();

        break;
    }
    default: {
        qDebug() << "Unknown marker type";
        break;

    }
    }
}





///CusChartView class
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

CusChartView::CusChartView(QChart* chart, QWidget *parent)
    : QChartView(chart, parent)
{
    setDragMode(QGraphicsView::NoDrag);
    this->setMouseTracking(true);
    this->setTransformationAnchor(QGraphicsView::NoAnchor);
}


void CusChartView::zoom(bool in_out, bool x_y, int mode)
{
    //static cast is faster, father to child
    QValueAxis *axisX= (QValueAxis*)(this->chart()->axisX());
    QValueAxis *axisY = (QValueAxis*)(this->chart()->axisY());

    qreal cur_x_min = axisX->min();//目前X軸顯示區間的最小x值
    qreal cur_x_max = axisX->max();//目前X軸顯示區間的最大x值
    qreal cur_y_min = axisY->min();
    qreal cur_y_max = axisY->max();

    qreal distance_x=abs(cur_x_max-cur_x_min);
    qreal distance_y=abs(cur_y_max-cur_y_min);


    QPointF view_center=QPointF(round((cur_x_max+cur_x_min)/2),(cur_y_max+cur_y_min)/2);

    if(x_y==0) { //x
        if(in_out==0) { //zoom in
            if(scale_level>0) {
                scale_level--;

                if(mode==0) { //zoom at center
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                } else if (mode==1) { //zoom at cursur position
                    qreal ratio=qreal(cursor_pos.x())/qreal(this->width());
                    int target_data=round(cur_x_min+distance_x*ratio);
                    qreal new_distance_x=x_scales[scale_level];
                    int view_centerX=round(target_data-new_distance_x*(ratio-0.5));

                    view_center.setX(view_centerX);

                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                } else if(mode==2) { //zoom at the newest
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(0,x_scales[scale_level]);
                }

            }
        }


        else if(in_out==1) { //zoom out
            if(scale_level<14) {
                scale_level++;

                if(mode==0) { //zoom at center
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                } else if (mode==1) { //zoom at cursur position
                    if(distance_x<max_sample_number) {
                        qreal ratio=qreal(cursor_pos.x())/qreal(this->width());
                        int target_data=round(cur_x_min+distance_x*ratio);
                        qreal new_distance_x=x_scales[scale_level];
                        int view_centerX=round(target_data-new_distance_x*(ratio-0.5));

                        view_center.setX(view_centerX);
                        axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                    }

                } else if(mode==2) { //zoom at the newest
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(0,x_scales[scale_level]);
                }

            }
        }

        if(axisX->min()<0) {
            axisX->setRange(0,x_scales[scale_level]);
        }
        if(axisX->max()>max_sample_number) {
            axisX->setRange(axisX->min(),max_sample_number);
        }


    } else if(x_y==1) { //y

        if(in_out==0) { //zoom in
            if(!(distance_y<0.01))
                axisY->setRange(view_center.y()-distance_y/2/2,view_center.y()+distance_y/2/2);
            else {
                axisY->setRange(view_center.y()-0.005,view_center.y()+0.005);
            }
        } else if(in_out==1) { //zoom out
            axisY->setRange(view_center.y()-distance_y/2*2,view_center.y()+distance_y/2*2);

            if(axisY->min()<valueRange[0]) {
                axisY->setRange(valueRange[0],axisY->max());
            }
            if(axisY->max()>valueRange[1]) {
                axisY->setRange(axisY->min(),valueRange[1]);
            }
        }
    }

    this->update();
}

void CusChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
        m_lastMousePos = event->pos();
        event->accept();
    }

    QChartView::mousePressEvent(event);
}

void CusChartView::mouseMoveEvent(QMouseEvent *event)
{
    // pan the chart with a left mouse drag
    if (event->buttons() & Qt::LeftButton) {

        //螢幕左上角為0
        //move Y
        int move_y=m_lastMousePos.y()-event->pos().y();

        this->chart()->scroll(0,-move_y);

        //static cast is faster, father to child
        QValueAxis *axisY = (QValueAxis*)(this->chart()->axisY());

        if(axisY->min()<valueRange[0]) {
            qreal error = valueRange[0]-axisY->min();
            axisY->setRange(axisY->min()+error,axisY->max()+error);
        }
        if(axisY->max()>valueRange[1]) {
            qreal error = axisY->max()-valueRange[1];
            axisY->setRange(axisY->min()-error,axisY->max()-error);
        }

        //move X
        if(zoom_mode==1) {
            int move_x=m_lastMousePos.x()-event->pos().x();
            this->chart()->scroll(move_x,0);
            QValueAxis *axisX= (QValueAxis*)(this->chart()->axisX());

            if(axisX->min()<0) {
                qreal error = 0-axisX->min();
                axisX->setRange(axisX->min()+error,axisX->max()+error);
            }
            if(axisX->max()>max_sample_number) {
                qreal error = axisX->max()-max_sample_number;
                axisX->setRange(axisX->min()-error,axisX->max()-error);
            }

        } else if(zoom_mode==2) {
            //pass
        }
        m_lastMousePos = event->pos();
        this->update();
        event->accept();

    }

    QChartView::mouseMoveEvent(event);
}

void CusChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        QApplication::restoreOverrideCursor();
        event->accept();
    }
    QChartView::mouseReleaseEvent(event);
}

void CusChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {

    } else if(event->button() == Qt::RightButton) {
        isFreeMode=!isFreeMode;
        event->accept();

    }
    QChartView::mouseDoubleClickEvent(event);
}

void CusChartView::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();
    cursor_pos = event->pos();
    //qDebug()<<cursor_pos;

    if(event->buttons() & Qt::RightButton || key_ctrl_pressed==true) {
        if(numDegrees.y() > 0)  //while right btn is pressed, zoom X
            zoom(0, 0, zoom_mode);      //mode 1=free mode
        else if(numDegrees.y()<0)
            zoom(1, 0, zoom_mode);

    }

    else { //zoom Y
        sigUserWheelZoom();
        if(numDegrees.y() > 0)
            zoom(0, 1, zoom_mode);
        else if(numDegrees.y() <0)
            zoom(1, 1, zoom_mode);

    }
    event->accept();

    QChartView::wheelEvent(event);
}

void CusChartView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() & Qt::Key_Control) {
        key_ctrl_pressed = true;
    }


    event->accept();
    QChartView::keyPressEvent(event);
}

void CusChartView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() & Qt::Key_Control) {
        key_ctrl_pressed = false;
    }
    event->accept();
    QChartView::keyReleaseEvent(event);
}

