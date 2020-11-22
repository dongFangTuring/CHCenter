/********************************************************************************
** Form generated from reading UI file 'chsettingform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHSETTINGFORM_H
#define UI_CHSETTINGFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CHSettingForm
{
public:
    QGridLayout *gridLayout;
    QGroupBox *MsgGB;
    QVBoxLayout *verticalLayout_5;
    QPushButton *clearBTN;
    QTextBrowser *TerminalBox;
    QGroupBox *ATcmdGB;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *ATcmdInput;
    QPushButton *sendATcmdBTN;
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *BaudrateGB;
    QVBoxLayout *verticalLayout;
    QRadioButton *BR9600BTN;
    QRadioButton *BR115200BTN;
    QRadioButton *BR460800BTN;
    QRadioButton *BR921600BTN;
    QPushButton *BRSetBTN;
    QGroupBox *QuickGB;
    QVBoxLayout *verticalLayout_3;
    QPushButton *InfoBTN;
    QPushButton *Mode6AxisBTN;
    QPushButton *Mode9AxisBTN;
    QPushButton *StopStreamBTN;
    QPushButton *RestartBTN;
    QGroupBox *FrameRateGB;
    QGridLayout *gridLayout_2;
    QPushButton *FRSetBTN;
    QRadioButton *FR25BTN;
    QRadioButton *FR50BTN;
    QRadioButton *FR100BTN;
    QRadioButton *FR1BTN;
    QRadioButton *FR0BTN;
    QGroupBox *DataProtocolGB;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *IDCheckBox;
    QCheckBox *AccCheckBox;
    QCheckBox *GyroCheckBox;
    QCheckBox *MagCheckBox;
    QCheckBox *EulerCheckBox;
    QCheckBox *QuatCheckBox;
    QFrame *line;
    QCheckBox *IMUSOLCheckBox;
    QPushButton *ProtocolSetBTN;
    QGroupBox *IDSettingGB;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpinBox *IDInput;
    QPushButton *IDSetBTN;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_5;
    QSpinBox *GWIDInput;
    QPushButton *GWIDSetBTN;

    void setupUi(QWidget *CHSettingForm)
    {
        if (CHSettingForm->objectName().isEmpty())
            CHSettingForm->setObjectName(QString::fromUtf8("CHSettingForm"));
        CHSettingForm->resize(620, 881);
        QFont font;
        font.setFamily(QString::fromUtf8("Times New Roman"));
        font.setPointSize(10);
        CHSettingForm->setFont(font);
        gridLayout = new QGridLayout(CHSettingForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(7);
        gridLayout->setVerticalSpacing(5);
        MsgGB = new QGroupBox(CHSettingForm);
        MsgGB->setObjectName(QString::fromUtf8("MsgGB"));
        verticalLayout_5 = new QVBoxLayout(MsgGB);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        clearBTN = new QPushButton(MsgGB);
        clearBTN->setObjectName(QString::fromUtf8("clearBTN"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(clearBTN->sizePolicy().hasHeightForWidth());
        clearBTN->setSizePolicy(sizePolicy);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/clear_text.png"), QSize(), QIcon::Normal, QIcon::Off);
        clearBTN->setIcon(icon);
        clearBTN->setIconSize(QSize(30, 20));

        verticalLayout_5->addWidget(clearBTN);

        TerminalBox = new QTextBrowser(MsgGB);
        TerminalBox->setObjectName(QString::fromUtf8("TerminalBox"));
        TerminalBox->setMinimumSize(QSize(0, 200));
        TerminalBox->setOverwriteMode(true);

        verticalLayout_5->addWidget(TerminalBox);


        gridLayout->addWidget(MsgGB, 5, 0, 1, 1);

        ATcmdGB = new QGroupBox(CHSettingForm);
        ATcmdGB->setObjectName(QString::fromUtf8("ATcmdGB"));
        horizontalLayout_2 = new QHBoxLayout(ATcmdGB);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ATcmdInput = new QLineEdit(ATcmdGB);
        ATcmdInput->setObjectName(QString::fromUtf8("ATcmdInput"));
        ATcmdInput->setMinimumSize(QSize(0, 20));

        horizontalLayout_2->addWidget(ATcmdInput);

        sendATcmdBTN = new QPushButton(ATcmdGB);
        sendATcmdBTN->setObjectName(QString::fromUtf8("sendATcmdBTN"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/send.png"), QSize(), QIcon::Normal, QIcon::Off);
        sendATcmdBTN->setIcon(icon1);
        sendATcmdBTN->setIconSize(QSize(30, 20));

        horizontalLayout_2->addWidget(sendATcmdBTN);


        gridLayout->addWidget(ATcmdGB, 4, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        BaudrateGB = new QGroupBox(CHSettingForm);
        BaudrateGB->setObjectName(QString::fromUtf8("BaudrateGB"));
        verticalLayout = new QVBoxLayout(BaudrateGB);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        BR9600BTN = new QRadioButton(BaudrateGB);
        BR9600BTN->setObjectName(QString::fromUtf8("BR9600BTN"));

        verticalLayout->addWidget(BR9600BTN);

        BR115200BTN = new QRadioButton(BaudrateGB);
        BR115200BTN->setObjectName(QString::fromUtf8("BR115200BTN"));

        verticalLayout->addWidget(BR115200BTN);

        BR460800BTN = new QRadioButton(BaudrateGB);
        BR460800BTN->setObjectName(QString::fromUtf8("BR460800BTN"));

        verticalLayout->addWidget(BR460800BTN);

        BR921600BTN = new QRadioButton(BaudrateGB);
        BR921600BTN->setObjectName(QString::fromUtf8("BR921600BTN"));

        verticalLayout->addWidget(BR921600BTN);

        BRSetBTN = new QPushButton(BaudrateGB);
        BRSetBTN->setObjectName(QString::fromUtf8("BRSetBTN"));
        BRSetBTN->setIcon(icon1);
        BRSetBTN->setIconSize(QSize(20, 20));

        verticalLayout->addWidget(BRSetBTN);


        horizontalLayout_4->addWidget(BaudrateGB);

        QuickGB = new QGroupBox(CHSettingForm);
        QuickGB->setObjectName(QString::fromUtf8("QuickGB"));
        verticalLayout_3 = new QVBoxLayout(QuickGB);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        InfoBTN = new QPushButton(QuickGB);
        InfoBTN->setObjectName(QString::fromUtf8("InfoBTN"));
        InfoBTN->setMinimumSize(QSize(100, 0));

        verticalLayout_3->addWidget(InfoBTN);

        Mode6AxisBTN = new QPushButton(QuickGB);
        Mode6AxisBTN->setObjectName(QString::fromUtf8("Mode6AxisBTN"));
        Mode6AxisBTN->setMinimumSize(QSize(100, 0));

        verticalLayout_3->addWidget(Mode6AxisBTN);

        Mode9AxisBTN = new QPushButton(QuickGB);
        Mode9AxisBTN->setObjectName(QString::fromUtf8("Mode9AxisBTN"));
        Mode9AxisBTN->setMinimumSize(QSize(100, 0));

        verticalLayout_3->addWidget(Mode9AxisBTN);

        StopStreamBTN = new QPushButton(QuickGB);
        StopStreamBTN->setObjectName(QString::fromUtf8("StopStreamBTN"));
        StopStreamBTN->setMinimumSize(QSize(100, 0));

        verticalLayout_3->addWidget(StopStreamBTN);

        RestartBTN = new QPushButton(QuickGB);
        RestartBTN->setObjectName(QString::fromUtf8("RestartBTN"));
        RestartBTN->setMinimumSize(QSize(100, 0));

        verticalLayout_3->addWidget(RestartBTN);


        horizontalLayout_4->addWidget(QuickGB);

        FrameRateGB = new QGroupBox(CHSettingForm);
        FrameRateGB->setObjectName(QString::fromUtf8("FrameRateGB"));
        gridLayout_2 = new QGridLayout(FrameRateGB);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        FRSetBTN = new QPushButton(FrameRateGB);
        FRSetBTN->setObjectName(QString::fromUtf8("FRSetBTN"));
        FRSetBTN->setIcon(icon1);

        gridLayout_2->addWidget(FRSetBTN, 8, 0, 1, 1);

        FR25BTN = new QRadioButton(FrameRateGB);
        FR25BTN->setObjectName(QString::fromUtf8("FR25BTN"));

        gridLayout_2->addWidget(FR25BTN, 5, 0, 1, 1);

        FR50BTN = new QRadioButton(FrameRateGB);
        FR50BTN->setObjectName(QString::fromUtf8("FR50BTN"));

        gridLayout_2->addWidget(FR50BTN, 6, 0, 1, 1);

        FR100BTN = new QRadioButton(FrameRateGB);
        FR100BTN->setObjectName(QString::fromUtf8("FR100BTN"));

        gridLayout_2->addWidget(FR100BTN, 7, 0, 1, 1);

        FR1BTN = new QRadioButton(FrameRateGB);
        FR1BTN->setObjectName(QString::fromUtf8("FR1BTN"));

        gridLayout_2->addWidget(FR1BTN, 4, 0, 1, 1);

        FR0BTN = new QRadioButton(FrameRateGB);
        FR0BTN->setObjectName(QString::fromUtf8("FR0BTN"));

        gridLayout_2->addWidget(FR0BTN, 3, 0, 1, 1);


        horizontalLayout_4->addWidget(FrameRateGB);

        DataProtocolGB = new QGroupBox(CHSettingForm);
        DataProtocolGB->setObjectName(QString::fromUtf8("DataProtocolGB"));
        verticalLayout_2 = new QVBoxLayout(DataProtocolGB);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        IDCheckBox = new QCheckBox(DataProtocolGB);
        IDCheckBox->setObjectName(QString::fromUtf8("IDCheckBox"));

        verticalLayout_2->addWidget(IDCheckBox);

        AccCheckBox = new QCheckBox(DataProtocolGB);
        AccCheckBox->setObjectName(QString::fromUtf8("AccCheckBox"));

        verticalLayout_2->addWidget(AccCheckBox);

        GyroCheckBox = new QCheckBox(DataProtocolGB);
        GyroCheckBox->setObjectName(QString::fromUtf8("GyroCheckBox"));

        verticalLayout_2->addWidget(GyroCheckBox);

        MagCheckBox = new QCheckBox(DataProtocolGB);
        MagCheckBox->setObjectName(QString::fromUtf8("MagCheckBox"));

        verticalLayout_2->addWidget(MagCheckBox);

        EulerCheckBox = new QCheckBox(DataProtocolGB);
        EulerCheckBox->setObjectName(QString::fromUtf8("EulerCheckBox"));

        verticalLayout_2->addWidget(EulerCheckBox);

        QuatCheckBox = new QCheckBox(DataProtocolGB);
        QuatCheckBox->setObjectName(QString::fromUtf8("QuatCheckBox"));

        verticalLayout_2->addWidget(QuatCheckBox);

        line = new QFrame(DataProtocolGB);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        IMUSOLCheckBox = new QCheckBox(DataProtocolGB);
        IMUSOLCheckBox->setObjectName(QString::fromUtf8("IMUSOLCheckBox"));

        verticalLayout_2->addWidget(IMUSOLCheckBox);

        ProtocolSetBTN = new QPushButton(DataProtocolGB);
        ProtocolSetBTN->setObjectName(QString::fromUtf8("ProtocolSetBTN"));
        ProtocolSetBTN->setIcon(icon1);

        verticalLayout_2->addWidget(ProtocolSetBTN);


        horizontalLayout_4->addWidget(DataProtocolGB);

        IDSettingGB = new QGroupBox(CHSettingForm);
        IDSettingGB->setObjectName(QString::fromUtf8("IDSettingGB"));
        verticalLayout_4 = new QVBoxLayout(IDSettingGB);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label = new QLabel(IDSettingGB);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_4->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        IDInput = new QSpinBox(IDSettingGB);
        IDInput->setObjectName(QString::fromUtf8("IDInput"));

        horizontalLayout->addWidget(IDInput);

        IDSetBTN = new QPushButton(IDSettingGB);
        IDSetBTN->setObjectName(QString::fromUtf8("IDSetBTN"));
        IDSetBTN->setIcon(icon1);

        horizontalLayout->addWidget(IDSetBTN);


        verticalLayout_4->addLayout(horizontalLayout);

        label_2 = new QLabel(IDSettingGB);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout_4->addWidget(label_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        GWIDInput = new QSpinBox(IDSettingGB);
        GWIDInput->setObjectName(QString::fromUtf8("GWIDInput"));

        horizontalLayout_5->addWidget(GWIDInput);

        GWIDSetBTN = new QPushButton(IDSettingGB);
        GWIDSetBTN->setObjectName(QString::fromUtf8("GWIDSetBTN"));
        GWIDSetBTN->setIcon(icon1);

        horizontalLayout_5->addWidget(GWIDSetBTN);


        verticalLayout_4->addLayout(horizontalLayout_5);


        horizontalLayout_4->addWidget(IDSettingGB);


        gridLayout->addLayout(horizontalLayout_4, 1, 0, 1, 1);


        retranslateUi(CHSettingForm);

        QMetaObject::connectSlotsByName(CHSettingForm);
    } // setupUi

    void retranslateUi(QWidget *CHSettingForm)
    {
        CHSettingForm->setWindowTitle(QCoreApplication::translate("CHSettingForm", "Form", nullptr));
        MsgGB->setTitle(QCoreApplication::translate("CHSettingForm", "Received Message", nullptr));
        clearBTN->setText(QString());
        ATcmdGB->setTitle(QCoreApplication::translate("CHSettingForm", "AT Command", nullptr));
        sendATcmdBTN->setText(QString());
        BaudrateGB->setTitle(QCoreApplication::translate("CHSettingForm", "Baud", nullptr));
        BR9600BTN->setText(QCoreApplication::translate("CHSettingForm", "9600", nullptr));
        BR115200BTN->setText(QCoreApplication::translate("CHSettingForm", "115200 (Default)", nullptr));
        BR460800BTN->setText(QCoreApplication::translate("CHSettingForm", "460800", nullptr));
        BR921600BTN->setText(QCoreApplication::translate("CHSettingForm", "921600", nullptr));
        BRSetBTN->setText(QString());
        QuickGB->setTitle(QCoreApplication::translate("CHSettingForm", "Quick Function", nullptr));
        InfoBTN->setText(QCoreApplication::translate("CHSettingForm", "INFO", nullptr));
        Mode6AxisBTN->setText(QCoreApplication::translate("CHSettingForm", "6 axis mode", nullptr));
        Mode9AxisBTN->setText(QCoreApplication::translate("CHSettingForm", "9 axis mode", nullptr));
        StopStreamBTN->setText(QCoreApplication::translate("CHSettingForm", "Stop Output", nullptr));
        RestartBTN->setText(QCoreApplication::translate("CHSettingForm", "Restart", nullptr));
        FrameRateGB->setTitle(QCoreApplication::translate("CHSettingForm", "Frame Rate", nullptr));
        FRSetBTN->setText(QString());
        FR25BTN->setText(QCoreApplication::translate("CHSettingForm", "25 Hz", nullptr));
        FR50BTN->setText(QCoreApplication::translate("CHSettingForm", "50 Hz", nullptr));
        FR100BTN->setText(QCoreApplication::translate("CHSettingForm", "100 Hz", nullptr));
        FR1BTN->setText(QCoreApplication::translate("CHSettingForm", "1 Hz", nullptr));
        FR0BTN->setText(QCoreApplication::translate("CHSettingForm", "0 Hz", nullptr));
        DataProtocolGB->setTitle(QCoreApplication::translate("CHSettingForm", "Data Protocol", nullptr));
        IDCheckBox->setText(QCoreApplication::translate("CHSettingForm", "ID", nullptr));
        AccCheckBox->setText(QCoreApplication::translate("CHSettingForm", "Accelerometer (g)", nullptr));
        GyroCheckBox->setText(QCoreApplication::translate("CHSettingForm", "Gyroscope (\302\260/s)", nullptr));
        MagCheckBox->setText(QCoreApplication::translate("CHSettingForm", "Magnetometer (\316\274T)", nullptr));
        EulerCheckBox->setText(QCoreApplication::translate("CHSettingForm", "Euler Angle", nullptr));
        QuatCheckBox->setText(QCoreApplication::translate("CHSettingForm", "Quaternion", nullptr));
        IMUSOLCheckBox->setText(QCoreApplication::translate("CHSettingForm", "IMUSOL (All)", nullptr));
        ProtocolSetBTN->setText(QString());
        IDSettingGB->setTitle(QCoreApplication::translate("CHSettingForm", "ID Setting", nullptr));
        label->setText(QCoreApplication::translate("CHSettingForm", "Node ID (ID) : ", nullptr));
        IDSetBTN->setText(QString());
        label_2->setText(QCoreApplication::translate("CHSettingForm", "Net ID (GWID) : ", nullptr));
        GWIDSetBTN->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CHSettingForm: public Ui_CHSettingForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHSETTINGFORM_H
