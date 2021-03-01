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
    movingwindow_timer.setInterval(1000/60);
    connect(&movingwindow_timer, SIGNAL(timeout(void)), this, SLOT(updateMovingWindow(void)), Qt::DirectConnection);
    //movingwindow_timer.start();

    //create default axes first, so we can add series later.
    axisX = new QValueAxis;
    axisY = new QValueAxis;


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

    }
    else if (type=="gyr"){

        this->setWindowTitle(tr("Angular Velocity Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        valueRange[0]=-2000;
        valueRange[1]=2000;
        m_chart->setTitle(tr("Angular Velocity (°/s)"));

    }
    else if (type=="mag"){

        this->setWindowTitle(tr("Magnetic Field Chart"));

        addSeries(point_X, "X");
        addSeries(point_Y, "Y");
        addSeries(point_Z, "Z");
        valueRange[0]=-80;
        valueRange[1]=80;
        m_chart->setTitle(tr("Magnetic Field (μT)"));
    }
    else if (type=="eul"){

        this->setWindowTitle(tr("Eular Angles Chart"));

        addSeries(point_X, "Roll");
        addSeries(point_Y, "Pitch");
        addSeries(point_Z, "Yaw");
        valueRange[0]=-180;
        valueRange[1]=180;
        m_chart->setTitle(tr("Euler Angles (°)"));

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

    }



    //the max and min of Y axis
    valueRange[0]=valueRange[0]*2;
    valueRange[1]=valueRange[1]*2;

    sample_counter=0;

    point_W.reserve(50000);
    point_X.reserve(50000);
    point_Y.reserve(50000);
    point_Z.reserve(50000);


    axisX->setLabelFormat("%d");
    axisX->setTitleText("Samples");

    axisY->setTickAnchor(0);
    axisY->setTickCount(11);
    axisY->setMinorTickCount(1);
    axisY->setTitleText("Value");

    //axisY->setTickType(QValueAxis::TicksDynamic);
    //axisY->setTickInterval(10);

    m_chart->legend()->setVisible(true);  //設定圖例可見


    connectMarkers();  //將曲線與圖例連線起來，可以勾選進行顯示與隱藏

    //sync max and min value to cuschart class
    m_chartView = new CusChartView(m_chart);
    m_chartView->valueRange[0]=valueRange[0];
    m_chartView->valueRange[1]=valueRange[1];
    m_chartView->max_sample_number=max_sample_number;

    m_chartView->setRenderHints(QPainter::Antialiasing);
    m_chartView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
    //m_chartView->setCacheMode(QGraphicsView::CacheBackground);
    m_chartView->setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing);
    //m_chart->setCacheMode(QGraphicsItem::ItemCoordinateCache,QSize(2000,2000));

    //m_chartView->setRubberBand(QChartView::RectangleRubberBand);  //整體縮放

    ui->LayoutChart->addWidget(m_chartView);


    init();

}

ChartWindow::~ChartWindow()
{
}

void ChartWindow::updateLineData(float *array){

    m_chartView->setFocus();
    if(this->isVisible()){
        //        if(!movingwindow_timer.isActive())
        //            movingwindow_timer.start();
        if(sample_counter>=max_sample_number){
            sample_counter=0;
        }

        else{
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

        updateMovingWindow();
    }
    else{
        //        if(movingwindow_timer.isActive())
        //            movingwindow_timer.stop();
    }


}
void ChartWindow::updateMovingWindow()
{
    if(this->isVisible()){
        //m_chartV->setUpdatesEnabled(false);

        //定位拉條
        //        qreal cur_x_min = axisX->min();//目前X軸顯示區間的最小x值
        //        qreal cur_x_max = axisX->max();//目前X軸顯示區間的最大x值
        //        qreal cur_y_min = axisY->min();
        //        qreal cur_y_max = axisY->max();

        //        qreal distance_x=abs(cur_x_max-cur_x_min);
        //        qreal distance_y=abs(cur_y_max-cur_y_min);
        //        QPointF view_center=QPointF((cur_x_max+cur_x_min)/2,(cur_y_max+cur_y_min)/2);
        //        axisX->setTitleText(tr("%1 Sample").arg(round(distance_x)));


        //        ui->SliderSample->setRange(round(0+distance_x/2),round(max_sample_number-distance_x/2));
        //        ui->SliderValue->setRange(round((valueRange[0]+distance_y/2)*100),round((valueRange[1]-distance_y/2)*100));

        //        ui->SliderSample->setValue(view_center.x());
        //        ui->SliderValue->setValue(round(-view_center.y()*100));


        //for following mode
        if(ui->BTNFollowingMode->isChecked()){

            qreal cur_x_min = axisX->min();//目前X軸顯示區間的最小x值
            qreal cur_x_max = axisX->max();//目前X軸顯示區間的最大x值
            qreal distance_x=abs(cur_x_max-cur_x_min);

            QList<QPointF> wSizePoints_X, wSizePoints_Y, wSizePoints_Z, wSizePoints_W;// a window size number of points.

            if(sample_counter>=distance_x){
                if(m_type=="quat"){
                    wSizePoints_W=point_W.mid(sample_counter-(distance_x+1), distance_x+1) ;
                }
                wSizePoints_X=point_X.mid(sample_counter-(distance_x+1), distance_x+1) ;
                wSizePoints_Y=point_Y.mid(sample_counter-(distance_x+1), distance_x+1) ;
                wSizePoints_Z=point_Z.mid(sample_counter-(distance_x+1), distance_x+1) ;
            }
            else {
                if(m_type=="quat"){
                    wSizePoints_W=point_W.mid(0, sample_counter) ;
                }
                wSizePoints_X=point_X.mid(0, sample_counter) ;
                wSizePoints_Y=point_Y.mid(0, sample_counter) ;
                wSizePoints_Z=point_Z.mid(0, sample_counter) ;
            }


            //renumbering X axis of each point from 1 to zoom scale
            for(int i =0; i<wSizePoints_X.length();i++){
                if(m_type=="quat")
                    wSizePoints_W.replace(i,QPointF(wSizePoints_W.at(i).x()+1-sample_counter+distance_x, wSizePoints_W.at(i).y()));
                wSizePoints_X.replace(i,QPointF(wSizePoints_X.at(i).x()+1-sample_counter+distance_x, wSizePoints_X.at(i).y()));
                wSizePoints_Y.replace(i,QPointF(wSizePoints_Y.at(i).x()+1-sample_counter+distance_x, wSizePoints_Y.at(i).y()));
                wSizePoints_Z.replace(i,QPointF(wSizePoints_Z.at(i).x()+1-sample_counter+distance_x, wSizePoints_Z.at(i).y()));

            }

            if(m_type=="quat"){
                m_serieslist.at(0)->replace(wSizePoints_W);
                m_serieslist.at(1)->replace(wSizePoints_X);
                m_serieslist.at(2)->replace(wSizePoints_Y);
                m_serieslist.at(3)->replace(wSizePoints_Z);
            }
            else{
                m_serieslist.at(0)->replace(wSizePoints_X);
                m_serieslist.at(1)->replace(wSizePoints_Y);
                m_serieslist.at(2)->replace(wSizePoints_Z);
            }

            axisX->setRange(0,distance_x);

            if(m_chartView->zoom_mode!=2)
                m_chartView->zoom_mode=2;  //zoom at the newest
        }
        //for free mode
        else{

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
            if(m_chartView->zoom_mode!=1)
                m_chartView->zoom_mode=1;  //zoom at the cursor pos
        }



    }

}



void ChartWindow::init()
{
    //default following mode
    ui->BTNFollowingMode->setChecked(true);

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
}


void ChartWindow::addSeries(QList<QPointF> &data, QString legend_title)  //用於新增曲線
{

    QLineSeries *series = new QLineSeries();
    series->setUseOpenGL(true);
    if(legend_title=="X" || legend_title=="Roll"){
        series->setPen(QPen(QColor(171,34,29), 1));
    }
    else if(legend_title=="Y" || legend_title=="Pitch"){

        series->setPen(QPen(QColor(13,139,77), 1));
    }
    else if(legend_title=="Z"  || legend_title=="Yaw"){

        series->setPen(QPen(QColor(65,83,175), 1));
    }
    else if(legend_title=="W"){
        series->setColor(Qt::gray);
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



//void ChartWindow::on_SliderSample_valueChanged(int value)
//{


//    qreal cur_x_min = axisX->min();//目前X軸顯示區間的最小x值
//    qreal cur_x_max = axisX->max();//目前X軸顯示區間的最大x值


//    qreal distance_x=abs(cur_x_max-cur_x_min);

//    axisX->setRange(value-distance_x/2,value+distance_x/2);

//    if(axisX->min()<0){
//        qreal error = 0-cur_x_min;
//        axisX->setRange(cur_x_min+error,cur_x_max+error);
//    }
//    if(axisX->max()>max_sample_number){
//        qreal error = cur_x_max-max_sample_number;
//        axisX->setRange(cur_x_min-error,cur_x_max-error);
//    }

//}

//void ChartWindow::on_BTNSampleZoomIn_clicked()
//{
//    m_chartView->zoom(0,0,0);

//}

//void ChartWindow::on_BTNSampleZoomOut_clicked()
//{
//    m_chartView->zoom(1,0,0);
//}

//void ChartWindow::on_SliderValue_valueChanged(int value)
//{
//    float f_value=float(-value)/100.0f;


//    qreal cur_y_min = axisY->min();
//    qreal cur_y_max = axisY->max();

//    qreal distance_y=abs(cur_y_max-cur_y_min);
//    axisY->setRange(f_value-distance_y/2,f_value+distance_y/2);

//    if(axisY->min()<valueRange[0]){
//        qreal error = valueRange[0]-cur_y_min;
//        axisY->setRange(cur_y_min+error,cur_y_max+error);
//    }
//    if(axisY->max()>valueRange[1]){
//        qreal error = cur_y_max-valueRange[1];
//        axisY->setRange(cur_y_min-error,cur_y_max-error);
//    }
//}

//void ChartWindow::on_BTNValueZoomIn_clicked()
//{
//    m_chartView->zoom(0,1,0);
//    //qDebug()<<"zoomin";
//}

//void ChartWindow::on_BTNValueZoomOut_clicked()
//{
//    m_chartView->zoom(1,1,0);
//    //qDebug()<<"zoomout";
//}
//void ChartWindow::on_BNTValueReset_clicked()
//{
//    qreal cur_y_min = axisY->min();
//    qreal cur_y_max = axisY->max();
//    qreal distance_y=abs(cur_y_max-cur_y_min);

//    axisY->setRange(0-distance_y/2,0+distance_y/2);

//}



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

    static uchar scale_level=5;
    static ushort x_scales[15]={50,100,200,500,1000,2000,5000,7500,10000,12500,15000,17500,20000,30000,50000};

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

    if(x_y==0){ //x
        if(in_out==0){  //zoom in
            if(scale_level>0){
                scale_level--;

                if(mode==0){//zoom at center
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                }
                else if (mode==1){ //zoom at cursur position
                    qreal ratio=qreal(cursor_pos.x())/qreal(this->width());
                    int target_data=round(cur_x_min+distance_x*ratio);
                    qreal new_distance_x=x_scales[scale_level];
                    int view_centerX=round(target_data-new_distance_x*(ratio-0.5));

                    view_center.setX(view_centerX);

                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                }
                else if(mode==2){//zoom at the newest
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(0,x_scales[scale_level]);
                }

                //old method 0.8x
                //            if(distance_x>100){
                //                if(mode==0){//zoom at center
                //                    qreal new_distance_x=distance_x*0.8;
                //                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                //                }
                //                else if (mode==1){ //zoom at cursur position


                //                    qreal ratio=qreal(cursor_pos.x())/qreal(this->width());
                //                    int target_data=round(cur_x_min+distance_x*ratio);
                //                    qreal new_distance_x=distance_x*0.8;
                //                    int view_centerX=round(target_data-new_distance_x*(ratio-0.5));

                //                    view_center.setX(view_centerX);

                //                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                //                }
                //                else if(mode==2){//zoom at the newest
                //                    qreal new_distance_x=distance_x*0.8;
                //                    axisX->setRange(cur_x_max-new_distance_x,cur_x_max);
                //                }
            }
        }


        else if(in_out==1){  //zoom out
            if(scale_level<14){
                scale_level++;

                if(mode==0){//zoom at center
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                }
                else if (mode==1){ //zoom at cursur position
                    if(distance_x<max_sample_number){
                        qreal ratio=qreal(cursor_pos.x())/qreal(this->width());
                        int target_data=round(cur_x_min+distance_x*ratio);
                        qreal new_distance_x=x_scales[scale_level];
                        int view_centerX=round(target_data-new_distance_x*(ratio-0.5));

                        view_center.setX(view_centerX);
                        axisX->setRange(view_center.x()-new_distance_x/2,view_center.x()+new_distance_x/2);
                    }

                }
                else if(mode==2){//zoom at the newest
                    qreal new_distance_x=x_scales[scale_level];
                    axisX->setRange(0,x_scales[scale_level]);
                }

            }
        }
        qDebug()<<x_scales[scale_level];
        if(axisX->min()<0){
            axisX->setRange(0,x_scales[scale_level]);
        }
        if(axisX->max()>max_sample_number){
            axisX->setRange(axisX->min(),max_sample_number);
        }


    }
    else if(x_y==1){ //y

        if(in_out==0){ //zoom in
            if(!(distance_y<0.01))
                axisY->setRange(view_center.y()-distance_y/2/1.5,view_center.y()+distance_y/2/1.5);
            else{
                axisY->setRange(view_center.y()-0.005,view_center.y()+0.005);
            }
        }
        else if(in_out==1){ //zoom out
            axisY->setRange(view_center.y()-distance_y/2*1.5,view_center.y()+distance_y/2*1.5);

            if(axisY->min()<valueRange[0]){
                axisY->setRange(valueRange[0],axisY->max());
            }
            if(axisY->max()>valueRange[1]){
                axisY->setRange(axisY->min(),valueRange[1]);
            }

        }

    }


    this->update();


}

void CusChartView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    {
        QApplication::setOverrideCursor(QCursor(Qt::SizeAllCursor));
        m_lastMousePos = event->pos();
        event->accept();
    }

    QChartView::mousePressEvent(event);
}

void CusChartView::mouseMoveEvent(QMouseEvent *event)
{
    // pan the chart with a left mouse drag
    if (event->buttons() & Qt::LeftButton)
    {

        //螢幕左上角為0
        //move Y
        int move_y=m_lastMousePos.y()-event->pos().y();

        this->chart()->scroll(0,-move_y);

        //static cast is faster, father to child
        QValueAxis *axisY = (QValueAxis*)(this->chart()->axisY());

        if(axisY->min()<valueRange[0]){
            qreal error = valueRange[0]-axisY->min();
            axisY->setRange(axisY->min()+error,axisY->max()+error);
        }
        if(axisY->max()>valueRange[1]){
            qreal error = axisY->max()-valueRange[1];
            axisY->setRange(axisY->min()-error,axisY->max()-error);
        }

        //move X
        if(zoom_mode==1){
            int move_x=m_lastMousePos.x()-event->pos().x();
            this->chart()->scroll(move_x,0);
            QValueAxis *axisX= (QValueAxis*)(this->chart()->axisX());

            if(axisX->min()<0){
                qreal error = 0-axisX->min();
                axisX->setRange(axisX->min()+error,axisX->max()+error);
            }
            if(axisX->max()>max_sample_number){
                qreal error = axisX->max()-max_sample_number;
                axisX->setRange(axisX->min()-error,axisX->max()-error);
            }
        }
        else if(zoom_mode==2){
            //pass
        }
        m_lastMousePos = event->pos();

        event->accept();
    }

    QChartView::mouseMoveEvent(event);
}

void CusChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    {
        QApplication::restoreOverrideCursor();
        event->accept();
    }
    QChartView::mouseReleaseEvent(event);
}

void CusChartView::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta();
    cursor_pos=event->pos();
    //qDebug()<<cursor_pos;

    if(event->buttons() & Qt::RightButton || key_ctrl_pressed==true){
        if(numDegrees.y()>0)  //while right btn is pressed, zoom X
            zoom(0,0,zoom_mode);      //mode 1=free mode
        else if(numDegrees.y()<0)
            zoom(1,0,zoom_mode);

    }

    else{ //zoom Y
        if(numDegrees.y()>0)
            zoom(0,1,zoom_mode);
        else if(numDegrees.y()<0)
            zoom(1,1,zoom_mode);

    }
    event->accept();

    QChartView::wheelEvent(event);
}

void CusChartView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() & Qt::Key_Control){
        key_ctrl_pressed=true;
    }


    event->accept();
    QChartView::keyPressEvent(event);
}

void CusChartView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() & Qt::Key_Control){
        key_ctrl_pressed=false;
    }
    event->accept();
    QChartView::keyReleaseEvent(event);
}

