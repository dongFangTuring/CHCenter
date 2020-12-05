#ifndef THREEDFORM_H
#define THREEDFORM_H

#include <QWidget>
#include <Qt3DRender/QMesh>
#include <QDebug>
#include <Qt3DExtras>
#include <Qt3DCore>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DAnimation>
#include <QSizePolicy>
#include <QFileDialog>
#include <QTimer>
#include <QMessageBox>

#include "packet/packet.h"
#include "packet/imu_data_decode.h"

namespace Ui {
class ThreeDForm;
}

class ThreeDForm : public QWidget
{
    Q_OBJECT

public:
    explicit ThreeDForm(QWidget *parent = nullptr);
    ~ThreeDForm();
    int loadobj(QString file);

    void startThreeDPlot();
    void stopThreeDPlot();

private slots:
    void on_BNTLoad_clicked();

    void getIMUData(receive_imusol_packet_t);

    void on_BTNPosReset_clicked();

    void objectReplot();

    void on_SliderLeftRight_sliderMoved(int position);

    void on_SliderUpDown_sliderMoved(int theta);

    void on_SliderZoom_sliderMoved(int theta);

private:
    Ui::ThreeDForm *ui;
    QWidget *container;
    Qt3DExtras::Qt3DWindow *view;
    Qt3DCore::QTransform *objTransform;
    Qt3DRender::QCamera *camera;

    Qt3DCore::QEntity *customObj;
    Qt3DCore::QEntity *rootEntity;

    QTimer *timer;
    QMutex mutex_writing;

    QString obj_filepath;

    float m_euler[3]={}; //x, y, z

    void initView();
    void drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity);

    int m_cam_scale[3];  //zoom, up/down, left/right


};

#endif // THREEDFORM_H
