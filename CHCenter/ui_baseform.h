/********************************************************************************
** Form generated from reading UI file 'baseform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BASEFORM_H
#define UI_BASEFORM_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BaseForm
{
public:
    QAction *actionTraditional_Chinese;
    QAction *actionSimplified_Chinese;
    QAction *actionEnglish;
    QAction *actionExit;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_10;
    QWidget *SideBar;
    QVBoxLayout *verticalLayout_3;
    QPushButton *BTNConnect;
    QListWidget *ListGWNode;
    QPushButton *SideBarBTN1;
    QPushButton *SideBarBTN2;
    QPushButton *SideBarBTN3;
    QPushButton *SideBarBTN4;
    QStackedWidget *stackedWidget;
    QWidget *PageData;
    QVBoxLayout *verticalLayout;
    QScrollArea *PageDataScroll;
    QWidget *PageDataScrollContents;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *DataTopBar;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_10;
    QLabel *LabelFrameRate;
    QLabel *LabelID;
    QSpacerItem *horizontalSpacer_6;
    QGroupBox *LabelGPAcc;
    QHBoxLayout *horizontalLayout_4;
    QFormLayout *formLayout;
    QLabel *label;
    QLabel *LabelAccX;
    QLabel *label_3;
    QLabel *LabelAccY;
    QLabel *LabelAccZ;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QGroupBox *LabelGPGyro;
    QHBoxLayout *horizontalLayout_5;
    QFormLayout *formLayout_2;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *LabelGyroY;
    QLabel *LabelGyroX;
    QLabel *LabelGyroZ;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_2;
    QGroupBox *LabelGPMag;
    QHBoxLayout *horizontalLayout_6;
    QFormLayout *formLayout_3;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *LabelMagX;
    QLabel *LabelMagY;
    QLabel *LabelMagZ;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_3;
    QGroupBox *LabelGPEuler;
    QHBoxLayout *horizontalLayout_7;
    QFormLayout *formLayout_4;
    QLabel *label_20;
    QLabel *label_22;
    QLabel *label_19;
    QLabel *LabelEulerX;
    QLabel *LabelEulerY;
    QLabel *LabelEulerZ;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_4;
    QGroupBox *LabelGPQuat;
    QGridLayout *gridLayout_5;
    QFormLayout *formLayout_5;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_21;
    QLabel *label_31;
    QLabel *LabelQuatW;
    QLabel *LabelQuatX;
    QLabel *LabelQuatY;
    QLabel *LabelQuatZ;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_5;
    QWidget *PageThreeDView;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_3;
    QRadioButton *radioButton;
    QWidget *PageLogger;
    QHBoxLayout *horizontalLayout_8;
    QComboBox *comboBox_2;
    QComboBox *comboBox;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QWidget *PageSetting;
    QVBoxLayout *verticalLayout_5;
    QScrollArea *PageSettingScroll;
    QWidget *PageSettingScrollContents;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *PageSettingWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *LabelStatusMsg;
    QMenuBar *menubar;
    QMenu *menuWindow;
    QMenu *menuLanguage;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *BaseForm)
    {
        if (BaseForm->objectName().isEmpty())
            BaseForm->setObjectName(QString::fromUtf8("BaseForm"));
        BaseForm->setWindowModality(Qt::NonModal);
        BaseForm->resize(1200, 600);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(BaseForm->sizePolicy().hasHeightForWidth());
        BaseForm->setSizePolicy(sizePolicy);
        BaseForm->setMinimumSize(QSize(1000, 600));
        BaseForm->setMouseTracking(true);
        BaseForm->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        BaseForm->setToolButtonStyle(Qt::ToolButtonIconOnly);
        BaseForm->setDockNestingEnabled(false);
        BaseForm->setDockOptions(QMainWindow::AnimatedDocks);
        BaseForm->setUnifiedTitleAndToolBarOnMac(true);
        actionTraditional_Chinese = new QAction(BaseForm);
        actionTraditional_Chinese->setObjectName(QString::fromUtf8("actionTraditional_Chinese"));
        actionSimplified_Chinese = new QAction(BaseForm);
        actionSimplified_Chinese->setObjectName(QString::fromUtf8("actionSimplified_Chinese"));
        actionEnglish = new QAction(BaseForm);
        actionEnglish->setObjectName(QString::fromUtf8("actionEnglish"));
        actionExit = new QAction(BaseForm);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(BaseForm);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout_4 = new QVBoxLayout(centralwidget);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(0);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        SideBar = new QWidget(centralwidget);
        SideBar->setObjectName(QString::fromUtf8("SideBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(SideBar->sizePolicy().hasHeightForWidth());
        SideBar->setSizePolicy(sizePolicy1);
        SideBar->setMinimumSize(QSize(300, 300));
        verticalLayout_3 = new QVBoxLayout(SideBar);
        verticalLayout_3->setSpacing(20);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(30, 20, 30, 20);
        BTNConnect = new QPushButton(SideBar);
        BTNConnect->setObjectName(QString::fromUtf8("BTNConnect"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(BTNConnect->sizePolicy().hasHeightForWidth());
        BTNConnect->setSizePolicy(sizePolicy2);
        BTNConnect->setMinimumSize(QSize(200, 80));
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setBold(true);
        font.setWeight(75);
        BTNConnect->setFont(font);
        BTNConnect->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font-size:20px;\n"
"background-color: #00ACC1; \n"
"color: #f1f1f1; \n"
"border: none;\n"
"border-radius:2px;\n"
"}\n"
"QPushButton:hover:!pressed{\n"
"background-color: #00ACC1;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: #008C9D;\n"
"}\n"
"QPushButton:disabled {\n"
"background-color:#dedede;\n"
"}"));
        BTNConnect->setFlat(true);

        verticalLayout_3->addWidget(BTNConnect);

        ListGWNode = new QListWidget(SideBar);
        ListGWNode->setObjectName(QString::fromUtf8("ListGWNode"));
        ListGWNode->setStyleSheet(QString::fromUtf8("QListWidget::item:selected{\n"
"background: white;\n"
"color:balck;\n"
"}"));
        ListGWNode->setTextElideMode(Qt::ElideMiddle);
        ListGWNode->setItemAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(ListGWNode);

        SideBarBTN1 = new QPushButton(SideBar);
        SideBarBTN1->setObjectName(QString::fromUtf8("SideBarBTN1"));
        sizePolicy2.setHeightForWidth(SideBarBTN1->sizePolicy().hasHeightForWidth());
        SideBarBTN1->setSizePolicy(sizePolicy2);
        SideBarBTN1->setMinimumSize(QSize(150, 60));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Times New Roman"));
        SideBarBTN1->setFont(font1);
        SideBarBTN1->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font-size:20px;\n"
"background-color: #30302E; \n"
"color: #f1f1f1; \n"
"border: none;\n"
"border-radius:2px;\n"
"}\n"
"QPushButton:hover:!pressed{\n"
"background-color: grey;\n"
"color: white;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: white;\n"
"color: black;\n"
"}\n"
"QPushButton:!enabled {\n"
"background-color:white;\n"
"color: black;\n"
"}\n"
""));

        verticalLayout_3->addWidget(SideBarBTN1);

        SideBarBTN2 = new QPushButton(SideBar);
        SideBarBTN2->setObjectName(QString::fromUtf8("SideBarBTN2"));
        sizePolicy2.setHeightForWidth(SideBarBTN2->sizePolicy().hasHeightForWidth());
        SideBarBTN2->setSizePolicy(sizePolicy2);
        SideBarBTN2->setMinimumSize(QSize(150, 60));
        SideBarBTN2->setFont(font1);
        SideBarBTN2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font-size:20px;\n"
"background-color: #30302E; \n"
"color: #f1f1f1; \n"
"border: none;\n"
"border-radius:2px;\n"
"}\n"
"QPushButton:hover:!pressed{\n"
"background-color: grey;\n"
"color: white;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: white;\n"
"color: black;\n"
"}\n"
"QPushButton:!enabled {\n"
"background-color:white;\n"
"color: black;\n"
"}\n"
""));

        verticalLayout_3->addWidget(SideBarBTN2);

        SideBarBTN3 = new QPushButton(SideBar);
        SideBarBTN3->setObjectName(QString::fromUtf8("SideBarBTN3"));
        sizePolicy2.setHeightForWidth(SideBarBTN3->sizePolicy().hasHeightForWidth());
        SideBarBTN3->setSizePolicy(sizePolicy2);
        SideBarBTN3->setMinimumSize(QSize(150, 60));
        SideBarBTN3->setFont(font1);
        SideBarBTN3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font-size:20px;\n"
"background-color: #30302E; \n"
"color: #f1f1f1; \n"
"border: none;\n"
"border-radius:2px;\n"
"}\n"
"QPushButton:hover:!pressed{\n"
"background-color: grey;\n"
"color: white;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: white;\n"
"color: black;\n"
"}\n"
"QPushButton:!enabled {\n"
"background-color:white;\n"
"color: black;\n"
"}\n"
""));

        verticalLayout_3->addWidget(SideBarBTN3);

        SideBarBTN4 = new QPushButton(SideBar);
        SideBarBTN4->setObjectName(QString::fromUtf8("SideBarBTN4"));
        sizePolicy2.setHeightForWidth(SideBarBTN4->sizePolicy().hasHeightForWidth());
        SideBarBTN4->setSizePolicy(sizePolicy2);
        SideBarBTN4->setMinimumSize(QSize(150, 60));
        SideBarBTN4->setFont(font1);
        SideBarBTN4->setCursor(QCursor(Qt::ArrowCursor));
        SideBarBTN4->setMouseTracking(false);
        SideBarBTN4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"font-size:20px;\n"
"background-color: #30302E; \n"
"color: #f1f1f1; \n"
"border: none;\n"
"border-radius:2px;\n"
"}\n"
"QPushButton:hover:!pressed{\n"
"background-color: grey;\n"
"color: white;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: white;\n"
"color: black;\n"
"}\n"
"QPushButton:!enabled {\n"
"background-color:white;\n"
"color: black;\n"
"}\n"
""));
        SideBarBTN4->setFlat(false);

        verticalLayout_3->addWidget(SideBarBTN4);


        horizontalLayout_10->addWidget(SideBar);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        PageData = new QWidget();
        PageData->setObjectName(QString::fromUtf8("PageData"));
        verticalLayout = new QVBoxLayout(PageData);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        PageDataScroll = new QScrollArea(PageData);
        PageDataScroll->setObjectName(QString::fromUtf8("PageDataScroll"));
        PageDataScroll->setWidgetResizable(true);
        PageDataScrollContents = new QWidget();
        PageDataScrollContents->setObjectName(QString::fromUtf8("PageDataScrollContents"));
        PageDataScrollContents->setGeometry(QRect(0, -284, 837, 786));
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(PageDataScrollContents->sizePolicy().hasHeightForWidth());
        PageDataScrollContents->setSizePolicy(sizePolicy3);
        verticalLayout_2 = new QVBoxLayout(PageDataScrollContents);
        verticalLayout_2->setSpacing(10);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        DataTopBar = new QHBoxLayout();
        DataTopBar->setObjectName(QString::fromUtf8("DataTopBar"));
        DataTopBar->setContentsMargins(-1, 10, -1, 10);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_10 = new QLabel(PageDataScrollContents);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(label_10->sizePolicy().hasHeightForWidth());
        label_10->setSizePolicy(sizePolicy4);
        QFont font2;
        font2.setFamily(QString::fromUtf8("Times New Roman"));
        font2.setPointSize(12);
        label_10->setFont(font2);

        horizontalLayout_2->addWidget(label_10);

        LabelFrameRate = new QLabel(PageDataScrollContents);
        LabelFrameRate->setObjectName(QString::fromUtf8("LabelFrameRate"));
        sizePolicy4.setHeightForWidth(LabelFrameRate->sizePolicy().hasHeightForWidth());
        LabelFrameRate->setSizePolicy(sizePolicy4);
        LabelFrameRate->setMinimumSize(QSize(0, 0));
        LabelFrameRate->setFont(font2);
        LabelFrameRate->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(LabelFrameRate);


        verticalLayout_6->addLayout(horizontalLayout_2);

        LabelID = new QLabel(PageDataScrollContents);
        LabelID->setObjectName(QString::fromUtf8("LabelID"));
        sizePolicy4.setHeightForWidth(LabelID->sizePolicy().hasHeightForWidth());
        LabelID->setSizePolicy(sizePolicy4);
        LabelID->setFont(font2);

        verticalLayout_6->addWidget(LabelID);


        DataTopBar->addLayout(verticalLayout_6);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

        DataTopBar->addItem(horizontalSpacer_6);


        verticalLayout_2->addLayout(DataTopBar);

        LabelGPAcc = new QGroupBox(PageDataScrollContents);
        LabelGPAcc->setObjectName(QString::fromUtf8("LabelGPAcc"));
        LabelGPAcc->setMinimumSize(QSize(100, 0));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Times New Roman"));
        font3.setPointSize(10);
        LabelGPAcc->setFont(font3);
        LabelGPAcc->setStyleSheet(QString::fromUtf8(""));
        LabelGPAcc->setFlat(true);
        horizontalLayout_4 = new QHBoxLayout(LabelGPAcc);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setHorizontalSpacing(0);
        formLayout->setVerticalSpacing(10);
        label = new QLabel(LabelGPAcc);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy5(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy5);

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        LabelAccX = new QLabel(LabelGPAcc);
        LabelAccX->setObjectName(QString::fromUtf8("LabelAccX"));
        LabelAccX->setMinimumSize(QSize(50, 0));
        LabelAccX->setMaximumSize(QSize(50, 16777215));
        LabelAccX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(0, QFormLayout::FieldRole, LabelAccX);

        label_3 = new QLabel(LabelGPAcc);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_3);

        LabelAccY = new QLabel(LabelGPAcc);
        LabelAccY->setObjectName(QString::fromUtf8("LabelAccY"));
        LabelAccY->setMinimumSize(QSize(50, 0));
        LabelAccY->setMaximumSize(QSize(50, 16777215));
        LabelAccY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(1, QFormLayout::FieldRole, LabelAccY);

        LabelAccZ = new QLabel(LabelGPAcc);
        LabelAccZ->setObjectName(QString::fromUtf8("LabelAccZ"));
        LabelAccZ->setMinimumSize(QSize(50, 0));
        LabelAccZ->setMaximumSize(QSize(50, 16777215));
        LabelAccZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout->setWidget(2, QFormLayout::FieldRole, LabelAccZ);

        label_2 = new QLabel(LabelGPAcc);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy6(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy6);

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);


        horizontalLayout_4->addLayout(formLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        pushButton = new QPushButton(LabelGPAcc);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy7(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy7);

        horizontalLayout_4->addWidget(pushButton);


        verticalLayout_2->addWidget(LabelGPAcc);

        LabelGPGyro = new QGroupBox(PageDataScrollContents);
        LabelGPGyro->setObjectName(QString::fromUtf8("LabelGPGyro"));
        LabelGPGyro->setFont(font3);
        LabelGPGyro->setFlat(true);
        horizontalLayout_5 = new QHBoxLayout(LabelGPGyro);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setHorizontalSpacing(0);
        formLayout_2->setVerticalSpacing(10);
        label_5 = new QLabel(LabelGPGyro);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy6.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy6);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_5);

        label_4 = new QLabel(LabelGPGyro);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_4);

        label_6 = new QLabel(LabelGPGyro);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        LabelGyroY = new QLabel(LabelGPGyro);
        LabelGyroY->setObjectName(QString::fromUtf8("LabelGyroY"));
        LabelGyroY->setMinimumSize(QSize(50, 0));
        LabelGyroY->setMaximumSize(QSize(50, 16777215));
        LabelGyroY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, LabelGyroY);

        LabelGyroX = new QLabel(LabelGPGyro);
        LabelGyroX->setObjectName(QString::fromUtf8("LabelGyroX"));
        LabelGyroX->setMinimumSize(QSize(50, 0));
        LabelGyroX->setMaximumSize(QSize(50, 16777215));
        LabelGyroX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, LabelGyroX);

        LabelGyroZ = new QLabel(LabelGPGyro);
        LabelGyroZ->setObjectName(QString::fromUtf8("LabelGyroZ"));
        LabelGyroZ->setMinimumSize(QSize(50, 0));
        LabelGyroZ->setMaximumSize(QSize(50, 16777215));
        LabelGyroZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, LabelGyroZ);


        horizontalLayout_5->addLayout(formLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_2);

        pushButton_2 = new QPushButton(LabelGPGyro);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        sizePolicy7.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy7);

        horizontalLayout_5->addWidget(pushButton_2);


        verticalLayout_2->addWidget(LabelGPGyro);

        LabelGPMag = new QGroupBox(PageDataScrollContents);
        LabelGPMag->setObjectName(QString::fromUtf8("LabelGPMag"));
        LabelGPMag->setFont(font3);
        LabelGPMag->setFlat(true);
        horizontalLayout_6 = new QHBoxLayout(LabelGPMag);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        formLayout_3 = new QFormLayout();
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        formLayout_3->setHorizontalSpacing(0);
        formLayout_3->setVerticalSpacing(10);
        label_9 = new QLabel(LabelGPMag);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        sizePolicy6.setHeightForWidth(label_9->sizePolicy().hasHeightForWidth());
        label_9->setSizePolicy(sizePolicy6);

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_9);

        label_8 = new QLabel(LabelGPMag);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_8);

        label_7 = new QLabel(LabelGPMag);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_7);

        LabelMagX = new QLabel(LabelGPMag);
        LabelMagX->setObjectName(QString::fromUtf8("LabelMagX"));
        LabelMagX->setMinimumSize(QSize(50, 0));
        LabelMagX->setMaximumSize(QSize(50, 16777215));
        LabelMagX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, LabelMagX);

        LabelMagY = new QLabel(LabelGPMag);
        LabelMagY->setObjectName(QString::fromUtf8("LabelMagY"));
        LabelMagY->setMinimumSize(QSize(50, 0));
        LabelMagY->setMaximumSize(QSize(50, 16777215));
        LabelMagY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, LabelMagY);

        LabelMagZ = new QLabel(LabelGPMag);
        LabelMagZ->setObjectName(QString::fromUtf8("LabelMagZ"));
        LabelMagZ->setMinimumSize(QSize(50, 0));
        LabelMagZ->setMaximumSize(QSize(50, 16777215));
        LabelMagZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_3->setWidget(2, QFormLayout::FieldRole, LabelMagZ);


        horizontalLayout_6->addLayout(formLayout_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        pushButton_3 = new QPushButton(LabelGPMag);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        sizePolicy7.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy7);

        horizontalLayout_6->addWidget(pushButton_3);


        verticalLayout_2->addWidget(LabelGPMag);

        LabelGPEuler = new QGroupBox(PageDataScrollContents);
        LabelGPEuler->setObjectName(QString::fromUtf8("LabelGPEuler"));
        LabelGPEuler->setFont(font3);
        LabelGPEuler->setFlat(true);
        horizontalLayout_7 = new QHBoxLayout(LabelGPEuler);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        formLayout_4 = new QFormLayout();
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setHorizontalSpacing(0);
        formLayout_4->setVerticalSpacing(10);
        label_20 = new QLabel(LabelGPEuler);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        sizePolicy6.setHeightForWidth(label_20->sizePolicy().hasHeightForWidth());
        label_20->setSizePolicy(sizePolicy6);

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_20);

        label_22 = new QLabel(LabelGPEuler);
        label_22->setObjectName(QString::fromUtf8("label_22"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_22);

        label_19 = new QLabel(LabelGPEuler);
        label_19->setObjectName(QString::fromUtf8("label_19"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, label_19);

        LabelEulerX = new QLabel(LabelGPEuler);
        LabelEulerX->setObjectName(QString::fromUtf8("LabelEulerX"));
        LabelEulerX->setMinimumSize(QSize(60, 0));
        LabelEulerX->setMaximumSize(QSize(60, 16777215));
        LabelEulerX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_4->setWidget(0, QFormLayout::FieldRole, LabelEulerX);

        LabelEulerY = new QLabel(LabelGPEuler);
        LabelEulerY->setObjectName(QString::fromUtf8("LabelEulerY"));
        LabelEulerY->setMinimumSize(QSize(60, 0));
        LabelEulerY->setMaximumSize(QSize(60, 16777215));
        LabelEulerY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, LabelEulerY);

        LabelEulerZ = new QLabel(LabelGPEuler);
        LabelEulerZ->setObjectName(QString::fromUtf8("LabelEulerZ"));
        LabelEulerZ->setMinimumSize(QSize(60, 0));
        LabelEulerZ->setMaximumSize(QSize(60, 16777215));
        LabelEulerZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, LabelEulerZ);


        horizontalLayout_7->addLayout(formLayout_4);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_4);

        pushButton_4 = new QPushButton(LabelGPEuler);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        sizePolicy7.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy7);

        horizontalLayout_7->addWidget(pushButton_4);


        verticalLayout_2->addWidget(LabelGPEuler);

        LabelGPQuat = new QGroupBox(PageDataScrollContents);
        LabelGPQuat->setObjectName(QString::fromUtf8("LabelGPQuat"));
        LabelGPQuat->setEnabled(true);
        LabelGPQuat->setFont(font3);
        LabelGPQuat->setFlat(true);
        gridLayout_5 = new QGridLayout(LabelGPQuat);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setHorizontalSpacing(0);
        gridLayout_5->setVerticalSpacing(10);
        formLayout_5 = new QFormLayout();
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        formLayout_5->setHorizontalSpacing(0);
        formLayout_5->setVerticalSpacing(10);
        label_26 = new QLabel(LabelGPQuat);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        formLayout_5->setWidget(1, QFormLayout::LabelRole, label_26);

        label_27 = new QLabel(LabelGPQuat);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_27);

        label_21 = new QLabel(LabelGPQuat);
        label_21->setObjectName(QString::fromUtf8("label_21"));

        formLayout_5->setWidget(2, QFormLayout::LabelRole, label_21);

        label_31 = new QLabel(LabelGPQuat);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        formLayout_5->setWidget(3, QFormLayout::LabelRole, label_31);

        LabelQuatW = new QLabel(LabelGPQuat);
        LabelQuatW->setObjectName(QString::fromUtf8("LabelQuatW"));
        sizePolicy5.setHeightForWidth(LabelQuatW->sizePolicy().hasHeightForWidth());
        LabelQuatW->setSizePolicy(sizePolicy5);
        LabelQuatW->setMinimumSize(QSize(50, 0));
        LabelQuatW->setMaximumSize(QSize(50, 16777215));
        LabelQuatW->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, LabelQuatW);

        LabelQuatX = new QLabel(LabelGPQuat);
        LabelQuatX->setObjectName(QString::fromUtf8("LabelQuatX"));
        LabelQuatX->setMinimumSize(QSize(50, 0));
        LabelQuatX->setMaximumSize(QSize(50, 16777215));
        LabelQuatX->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_5->setWidget(1, QFormLayout::FieldRole, LabelQuatX);

        LabelQuatY = new QLabel(LabelGPQuat);
        LabelQuatY->setObjectName(QString::fromUtf8("LabelQuatY"));
        LabelQuatY->setMinimumSize(QSize(50, 0));
        LabelQuatY->setMaximumSize(QSize(50, 16777215));
        LabelQuatY->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_5->setWidget(2, QFormLayout::FieldRole, LabelQuatY);

        LabelQuatZ = new QLabel(LabelGPQuat);
        LabelQuatZ->setObjectName(QString::fromUtf8("LabelQuatZ"));
        LabelQuatZ->setMinimumSize(QSize(50, 0));
        LabelQuatZ->setMaximumSize(QSize(50, 16777215));
        LabelQuatZ->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        formLayout_5->setWidget(3, QFormLayout::FieldRole, LabelQuatZ);


        gridLayout_5->addLayout(formLayout_5, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 0, 3, 1, 1);

        pushButton_5 = new QPushButton(LabelGPQuat);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        sizePolicy7.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy7);

        gridLayout_5->addWidget(pushButton_5, 0, 4, 1, 1);


        verticalLayout_2->addWidget(LabelGPQuat);

        PageDataScroll->setWidget(PageDataScrollContents);

        verticalLayout->addWidget(PageDataScroll);

        stackedWidget->addWidget(PageData);
        PageThreeDView = new QWidget();
        PageThreeDView->setObjectName(QString::fromUtf8("PageThreeDView"));
        horizontalLayout_3 = new QHBoxLayout(PageThreeDView);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        radioButton_2 = new QRadioButton(PageThreeDView);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));

        horizontalLayout_3->addWidget(radioButton_2);

        radioButton_3 = new QRadioButton(PageThreeDView);
        radioButton_3->setObjectName(QString::fromUtf8("radioButton_3"));

        horizontalLayout_3->addWidget(radioButton_3);

        radioButton = new QRadioButton(PageThreeDView);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));

        horizontalLayout_3->addWidget(radioButton);

        stackedWidget->addWidget(PageThreeDView);
        PageLogger = new QWidget();
        PageLogger->setObjectName(QString::fromUtf8("PageLogger"));
        horizontalLayout_8 = new QHBoxLayout(PageLogger);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        comboBox_2 = new QComboBox(PageLogger);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        horizontalLayout_8->addWidget(comboBox_2);

        comboBox = new QComboBox(PageLogger);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout_8->addWidget(comboBox);

        radioButton_4 = new QRadioButton(PageLogger);
        radioButton_4->setObjectName(QString::fromUtf8("radioButton_4"));

        horizontalLayout_8->addWidget(radioButton_4);

        radioButton_5 = new QRadioButton(PageLogger);
        radioButton_5->setObjectName(QString::fromUtf8("radioButton_5"));

        horizontalLayout_8->addWidget(radioButton_5);

        stackedWidget->addWidget(PageLogger);
        PageSetting = new QWidget();
        PageSetting->setObjectName(QString::fromUtf8("PageSetting"));
        verticalLayout_5 = new QVBoxLayout(PageSetting);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        PageSettingScroll = new QScrollArea(PageSetting);
        PageSettingScroll->setObjectName(QString::fromUtf8("PageSettingScroll"));
        PageSettingScroll->setWidgetResizable(true);
        PageSettingScrollContents = new QWidget();
        PageSettingScrollContents->setObjectName(QString::fromUtf8("PageSettingScrollContents"));
        PageSettingScrollContents->setGeometry(QRect(0, 0, 858, 502));
        verticalLayout_7 = new QVBoxLayout(PageSettingScrollContents);
        verticalLayout_7->setObjectName(QString::fromUtf8("verticalLayout_7"));
        PageSettingWidget = new QVBoxLayout();
        PageSettingWidget->setObjectName(QString::fromUtf8("PageSettingWidget"));

        verticalLayout_7->addLayout(PageSettingWidget);

        PageSettingScroll->setWidget(PageSettingScrollContents);

        verticalLayout_5->addWidget(PageSettingScroll);

        stackedWidget->addWidget(PageSetting);

        horizontalLayout_10->addWidget(stackedWidget);


        verticalLayout_4->addLayout(horizontalLayout_10);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        LabelStatusMsg = new QLabel(centralwidget);
        LabelStatusMsg->setObjectName(QString::fromUtf8("LabelStatusMsg"));
        LabelStatusMsg->setMinimumSize(QSize(200, 0));
        LabelStatusMsg->setFont(font3);
        LabelStatusMsg->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        horizontalLayout->addWidget(LabelStatusMsg);


        verticalLayout_4->addLayout(horizontalLayout);

        BaseForm->setCentralWidget(centralwidget);
        menubar = new QMenuBar(BaseForm);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1200, 26));
        menuWindow = new QMenu(menubar);
        menuWindow->setObjectName(QString::fromUtf8("menuWindow"));
        menuLanguage = new QMenu(menuWindow);
        menuLanguage->setObjectName(QString::fromUtf8("menuLanguage"));
        BaseForm->setMenuBar(menubar);
        statusbar = new QStatusBar(BaseForm);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QSizePolicy sizePolicy8(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(statusbar->sizePolicy().hasHeightForWidth());
        statusbar->setSizePolicy(sizePolicy8);
        BaseForm->setStatusBar(statusbar);

        menubar->addAction(menuWindow->menuAction());
        menuWindow->addAction(menuLanguage->menuAction());
        menuWindow->addAction(actionExit);
        menuLanguage->addAction(actionTraditional_Chinese);
        menuLanguage->addAction(actionSimplified_Chinese);
        menuLanguage->addAction(actionEnglish);

        retranslateUi(BaseForm);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(BaseForm);
    } // setupUi

    void retranslateUi(QMainWindow *BaseForm)
    {
        BaseForm->setWindowTitle(QCoreApplication::translate("BaseForm", "BaseForm", nullptr));
        actionTraditional_Chinese->setText(QCoreApplication::translate("BaseForm", "Traditional Chinese", nullptr));
        actionSimplified_Chinese->setText(QCoreApplication::translate("BaseForm", "Simplified Chinese", nullptr));
        actionEnglish->setText(QCoreApplication::translate("BaseForm", "English", nullptr));
        actionExit->setText(QCoreApplication::translate("BaseForm", "Exit", nullptr));
        BTNConnect->setText(QCoreApplication::translate("BaseForm", "Connect", nullptr));
        SideBarBTN1->setText(QCoreApplication::translate("BaseForm", "Data/Dashboard", nullptr));
        SideBarBTN2->setText(QCoreApplication::translate("BaseForm", "3D View", nullptr));
        SideBarBTN3->setText(QCoreApplication::translate("BaseForm", "CSV Logger", nullptr));
        SideBarBTN4->setText(QCoreApplication::translate("BaseForm", "Device Setting", nullptr));
        label_10->setText(QCoreApplication::translate("BaseForm", "Frame Rate : ", nullptr));
        LabelFrameRate->setText(QCoreApplication::translate("BaseForm", "0 Hz", nullptr));
        LabelID->setText(QCoreApplication::translate("BaseForm", "ID = 0", nullptr));
        LabelGPAcc->setTitle(QCoreApplication::translate("BaseForm", "Linear Accelaration (g)", nullptr));
        label->setText(QCoreApplication::translate("BaseForm", "X= ", nullptr));
        LabelAccX->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        label_3->setText(QCoreApplication::translate("BaseForm", "Y= ", nullptr));
        LabelAccY->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelAccZ->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        label_2->setText(QCoreApplication::translate("BaseForm", "Z= ", nullptr));
        pushButton->setText(QCoreApplication::translate("BaseForm", "PushButton", nullptr));
        LabelGPGyro->setTitle(QCoreApplication::translate("BaseForm", "Gyroscope (\302\260/s)", nullptr));
        label_5->setText(QCoreApplication::translate("BaseForm", "X=", nullptr));
        label_4->setText(QCoreApplication::translate("BaseForm", "Y=", nullptr));
        label_6->setText(QCoreApplication::translate("BaseForm", "Z=", nullptr));
        LabelGyroY->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelGyroX->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelGyroZ->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        pushButton_2->setText(QCoreApplication::translate("BaseForm", "PushButton", nullptr));
        LabelGPMag->setTitle(QCoreApplication::translate("BaseForm", "Magnetometer (\316\274T)", nullptr));
        label_9->setText(QCoreApplication::translate("BaseForm", "X=", nullptr));
        label_8->setText(QCoreApplication::translate("BaseForm", "Y=", nullptr));
        label_7->setText(QCoreApplication::translate("BaseForm", "Z=", nullptr));
        LabelMagX->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelMagY->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelMagZ->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        pushButton_3->setText(QCoreApplication::translate("BaseForm", "PushButton", nullptr));
        LabelGPEuler->setTitle(QCoreApplication::translate("BaseForm", "Euler Angle (\302\260)", nullptr));
        label_20->setText(QCoreApplication::translate("BaseForm", "X(Roll)=", nullptr));
        label_22->setText(QCoreApplication::translate("BaseForm", "Y(Pitch)=", nullptr));
        label_19->setText(QCoreApplication::translate("BaseForm", "Z(Yaw)=", nullptr));
        LabelEulerX->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelEulerY->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelEulerZ->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        pushButton_4->setText(QCoreApplication::translate("BaseForm", "PushButton", nullptr));
        LabelGPQuat->setTitle(QCoreApplication::translate("BaseForm", "Quaterinion", nullptr));
        label_26->setText(QCoreApplication::translate("BaseForm", "X=", nullptr));
        label_27->setText(QCoreApplication::translate("BaseForm", "W=", nullptr));
        label_21->setText(QCoreApplication::translate("BaseForm", "Y=", nullptr));
        label_31->setText(QCoreApplication::translate("BaseForm", "Z=", nullptr));
        LabelQuatW->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelQuatX->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelQuatY->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        LabelQuatZ->setText(QCoreApplication::translate("BaseForm", "0", nullptr));
        pushButton_5->setText(QCoreApplication::translate("BaseForm", "PushButton", nullptr));
        radioButton_2->setText(QCoreApplication::translate("BaseForm", "3D view", nullptr));
        radioButton_3->setText(QCoreApplication::translate("BaseForm", "3D view", nullptr));
        radioButton->setText(QCoreApplication::translate("BaseForm", "3D view", nullptr));
        radioButton_4->setText(QCoreApplication::translate("BaseForm", "RadioButton", nullptr));
        radioButton_5->setText(QCoreApplication::translate("BaseForm", "RadioButton", nullptr));
        LabelStatusMsg->setText(QCoreApplication::translate("BaseForm", "TextLabel", nullptr));
        menuWindow->setTitle(QCoreApplication::translate("BaseForm", "Window", nullptr));
        menuLanguage->setTitle(QCoreApplication::translate("BaseForm", "Language", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BaseForm: public Ui_BaseForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BASEFORM_H
