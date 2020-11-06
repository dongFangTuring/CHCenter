/********************************************************************************
** Form generated from reading UI file 'portform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PORTFORM_H
#define UI_PORTFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PortForm
{
public:
    QHBoxLayout *horizontalLayout;
    QComboBox *comboBox;
    QPushButton *BTNPortRefresh;
    QVBoxLayout *verticalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QDialog *PortForm)
    {
        if (PortForm->objectName().isEmpty())
            PortForm->setObjectName(QString::fromUtf8("PortForm"));
        PortForm->setWindowModality(Qt::WindowModal);
        PortForm->resize(422, 117);
        PortForm->setContextMenuPolicy(Qt::NoContextMenu);
        horizontalLayout = new QHBoxLayout(PortForm);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        comboBox = new QComboBox(PortForm);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMaximumSize(QSize(300, 40));
        comboBox->setAcceptDrops(false);
        comboBox->setStyleSheet(QString::fromUtf8("QComboBox {\n"
"background-color: #ebebeb;\n"
"color: black;\n"
"border: 2px solid grey;\n"
"border-radius: 3px;\n"
"}\n"
""));
        comboBox->setIconSize(QSize(20, 20));
        comboBox->setFrame(true);

        horizontalLayout->addWidget(comboBox);

        BTNPortRefresh = new QPushButton(PortForm);
        BTNPortRefresh->setObjectName(QString::fromUtf8("BTNPortRefresh"));
        BTNPortRefresh->setMinimumSize(QSize(40, 40));
        BTNPortRefresh->setMaximumSize(QSize(40, 40));
        BTNPortRefresh->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: white;\n"
"color: black;\n"
"border: 2px solid grey;\n"
"border-radius: 3px;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: grey;\n"
"}\n"
""));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/refreshing.png"), QSize(), QIcon::Normal, QIcon::Off);
        BTNPortRefresh->setIcon(icon);
        BTNPortRefresh->setFlat(false);

        horizontalLayout->addWidget(BTNPortRefresh);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        pushButton = new QPushButton(PortForm);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(80, 40));
        pushButton->setMaximumSize(QSize(80, 40));
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: white;\n"
"color: black;\n"
"border: 2px solid grey;\n"
"border-radius: 3px;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: grey;\n"
"}\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/ok.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);
        pushButton->setIconSize(QSize(30, 30));
        pushButton->setFlat(false);

        verticalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(PortForm);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(80, 40));
        pushButton_2->setMaximumSize(QSize(80, 40));
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"background-color: white;\n"
"color: black;\n"
"border: 2px solid grey;\n"
"border-radius: 3px;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: grey;\n"
"}\n"
""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/cancel.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton_2->setIcon(icon2);
        pushButton_2->setIconSize(QSize(30, 30));
        pushButton_2->setFlat(false);

        verticalLayout->addWidget(pushButton_2);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(PortForm);

        QMetaObject::connectSlotsByName(PortForm);
    } // setupUi

    void retranslateUi(QDialog *PortForm)
    {
        PortForm->setWindowTitle(QCoreApplication::translate("PortForm", "Dialog", nullptr));
        BTNPortRefresh->setText(QString());
        pushButton->setText(QString());
        pushButton_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class PortForm: public Ui_PortForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PORTFORM_H
