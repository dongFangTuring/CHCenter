#include "threedform.h"
#include "ui_threedform.h"

ThreeDForm::ThreeDForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThreeDForm)
{
    ui->setupUi(this);

    timer=new QTimer(this);
    connect(timer, SIGNAL(timeout(void)), this, SLOT(objectReplot(void)));
    timer->setInterval(30);

    obj_filepath="objModel/jet.obj";
    m_cam_scale[0]=100;
    m_cam_scale[1]=45;
    m_cam_scale[2]=225;
    initView();
}



ThreeDForm::~ThreeDForm()
{
    stopThreeDPlot();
    delete ui;
}
int ThreeDForm::initView()
{

    view = new Qt3DExtras::Qt3DWindow();

    // put Qt3DWindow into a container in order to make it possible
    // to handle the view inside a widget

    container = createWindowContainer(view,this);
    container->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    ui->ThreeDLayout->addWidget(container);

    view->defaultFrameGraph()->setClearColor(QColor(30,30,30));

    //build rootEntity first
    rootEntity = new Qt3DCore::QEntity;
    customObj = new Qt3DCore::QEntity(rootEntity);

    //set transformation
    objTransform = new Qt3DCore::QTransform();
    objTransform->setScale(5.0f);
    objTransform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

    //set obj material
    objMaterial = new Qt3DExtras::QPhongMaterial();
    objMaterial->setDiffuse(QColor(100, 100, 100));
    objMaterial->setAmbient(QColor(255, 255,255));
    objMaterial->setSpecular(QColor(255,255,255));
    objMaterial->setShininess(150.0f);

    //set obj material
    customObjMesh = new Qt3DRender::QMesh();

    //add .obj file
    int rst=loadobj(obj_filepath);
    if(rst==-1){
        //        QMessageBox msgBox;
        //        msgBox.setText(tr("No file exists or wrong path!"));
        //        msgBox.setWindowTitle(tr("Error"));
        //        msgBox.exec();
    }

    //load .obj file to customObj
    customObj->addComponent(customObjMesh);
    customObj->addComponent(objMaterial);
    customObj->addComponent(objTransform);

    camera = view->camera();
    camera->lens()->setPerspectiveProjection(60.0f, 16.0f/9.0f, 1.0f,700.0f);
    camera->setPosition(QVector3D(-70.0f, 70.0f, -70.0f));
    camera->setViewCenter(QVector3D(0, 0, 0));

    //draw axes
    drawLine({ 0, 0, 0 }, { 1000, 0, 0 }, Qt::green, rootEntity); // Y
    drawLine({ 0, 0, 0 }, { 0, 1000, 0 }, QColor(30, 160, 250), rootEntity); //blue: Z
    drawLine({ 0, 0, 0 }, { 0, 0, 1000 }, Qt::red, rootEntity); // X


    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1.2f);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(QVector3D(60, 500, 40.0f));
    lightEntity->addComponent(lightTransform);

    //this is for mouse control
    //    Qt3DExtras::QOrbitCameraController *camController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    //    camController->setCamera(camera);
    //    camController->setLinearSpeed(100.0);
    //    camController->setLookSpeed(100.0);
    camera->viewAll();
    view->setRootEntity(rootEntity);


    ui->SliderZoom->setRange(1,200);
    ui->SliderUpDown->setRange(-89,89);
    ui->SliderLeftRight->setRange(0,360);

    ui->SliderZoom->setValue(m_cam_scale[0]);
    ui->SliderUpDown->setValue(m_cam_scale[1]);
    ui->SliderLeftRight->setValue(m_cam_scale[2]);

    on_SliderZoom_sliderMoved(m_cam_scale[0]);
    on_SliderUpDown_sliderMoved(m_cam_scale[1]);
    on_SliderLeftRight_sliderMoved(m_cam_scale[2]);
    return 0;
}

int ThreeDForm::loadobj(QString file)
{
    // check if path exists and if yes: Is it a file and no directory?
    bool fileExists = QFileInfo::exists(file) && QFileInfo(file).isFile();
    if(fileExists!=1)
        return -1;
    QUrl url =QUrl::fromLocalFile(file);

    //load obj mesh
    customObjMesh->setMeshName("myPlane");
    customObjMesh->setSource(url);

    return 0;
}

void ThreeDForm::drawLine(const QVector3D& start, const QVector3D& end, const QColor& color, Qt3DCore::QEntity *_rootEntity)
{
    auto *geometry = new Qt3DRender::QGeometry(_rootEntity);

    // position vertices (start and end)
    QByteArray bufferBytes;
    bufferBytes.resize(3 * 2 * sizeof(float)); // start.x, start.y, start.end + end.x, end.y, end.z
    float *positions = reinterpret_cast<float*>(bufferBytes.data());
    *positions++ = start.x();
    *positions++ = start.y();
    *positions++ = start.z();
    *positions++ = end.x();
    *positions++ = end.y();
    *positions++ = end.z();

    auto *buf = new Qt3DRender::QBuffer(geometry);
    buf->setData(bufferBytes);

    auto *positionAttribute = new Qt3DRender::QAttribute(geometry);
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(buf);
    positionAttribute->setByteStride(3 * sizeof(float));
    positionAttribute->setCount(2);
    geometry->addAttribute(positionAttribute); // We add the vertices in the geometry

    // connectivity between vertices
    QByteArray indexBytes;
    indexBytes.resize(2 * sizeof(uint)); // start to end
    uint *indices = reinterpret_cast<uint*>(indexBytes.data());
    *indices++ = 0;
    *indices++ = 1;

    auto *indexBuffer = new Qt3DRender::QBuffer(geometry);
    indexBuffer->setData(indexBytes);

    auto *indexAttribute = new Qt3DRender::QAttribute(geometry);
    indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(2);
    geometry->addAttribute(indexAttribute); // We add the indices linking the points in the geometry

    // mesh
    auto *line = new Qt3DRender::QGeometryRenderer(_rootEntity);
    line->setGeometry(geometry);
    line->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    auto *material = new Qt3DExtras::QPhongMaterial(_rootEntity);
    material->setAmbient(color);

    // entity
    auto *lineEntity = new Qt3DCore::QEntity(_rootEntity);
    lineEntity->addComponent(line);
    lineEntity->addComponent(material);
}

void ThreeDForm::objectReplot()
{
    objTransform->setRotation(QQuaternion::fromEulerAngles(m_euler[1],m_euler[2],m_euler[0]));
    ui->LabelXnumber->setText(QString::number(m_euler[0],'f',2));
    ui->LabelYnumber->setText(QString::number(m_euler[1],'f',2));
    ui->LabelZnumber->setText(QString::number(m_euler[2],'f',2));
}

void ThreeDForm::startThreeDPlot()
{
    timer->start();

}

void ThreeDForm::stopThreeDPlot()
{
    if(timer->isActive()){
        timer->stop();
    }
}

void ThreeDForm::on_BNTLoad_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
                                                tr("Open .obj model"),
                                                QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).at(0),
                                                "*.obj");


    qDebug()<<file;
    if(QFile(file).exists()){
        obj_filepath=file;
    }
}


void ThreeDForm::getIMUData(receive_imusol_packet_t imu_data)
{
    mutex_writing.lock();

    for (int i=0; i<3; i++) {
        m_euler[i]=imu_data.eul[i];
    }
    mutex_writing.unlock();
}



void ThreeDForm::on_BTNPosReset_clicked()
{
    m_cam_scale[0]=100;
    m_cam_scale[1]=45;
    m_cam_scale[2]=225;

    ui->SliderZoom->setValue(m_cam_scale[0]);
    ui->SliderUpDown->setValue(m_cam_scale[1]);
    ui->SliderLeftRight->setValue(m_cam_scale[2]);

    on_SliderZoom_sliderMoved(m_cam_scale[0]);
    on_SliderUpDown_sliderMoved(m_cam_scale[1]);
    on_SliderLeftRight_sliderMoved(m_cam_scale[2]);

}
void ThreeDForm::on_SliderZoom_sliderMoved(int position)
{
    float pos=float(position);
    QVector3D camPos = camera->position();
    camPos.normalize();
    camPos = camPos*pos;
    camera->setPosition(camPos);
    m_cam_scale[0]=position;
}
void ThreeDForm::on_SliderUpDown_sliderMoved(int theta)
{
    QVector3D camPos = camera->position();
    float r=sqrt(pow((camPos.x()),2)+pow((camPos.y()),2)+pow((camPos.z()),2));
    camPos.setY(r*sin(theta*3.14/180));
    float xz=r*cos(theta*3.14/180);
    camPos.setX(xz*sin(ui->SliderLeftRight->value()*3.14/180));
    camPos.setZ(xz*cos(ui->SliderLeftRight->value()*3.14/180));

    camera->setPosition(camPos);
    camera->setUpVector(QVector3D(0,1,0));

    m_cam_scale[1]=theta;

}
void ThreeDForm::on_SliderLeftRight_sliderMoved(int theta)
{
    QVector3D camPos = camera->position();
    float r=sqrt((camPos.x())*(camPos.x())+(camPos.z())*(camPos.z()));
    camPos.setX(r*sin(theta*   3.14   /180   ));
    camPos.setZ(r*cos(theta*   3.14   /180   ));

    camera->setPosition(camPos);
    camera->setUpVector(QVector3D(0,1,0));

    m_cam_scale[2]=theta;
}

