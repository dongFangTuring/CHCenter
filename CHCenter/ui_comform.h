/********************************************************************************
** Form generated from reading UI file 'comform.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMFORM_H
#define UI_COMFORM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ComForm
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *ListPort;
    QVBoxLayout *verticalLayout;
    QPushButton *BTNPortRefresh;
    QPushButton *BTNOK;
    QPushButton *BTNCancle;
    QHBoxLayout *horizontalLayout_3;
    QLabel *LabelSelected;
    QComboBox *CBoxBaudrate;

    void setupUi(QWidget *ComForm)
    {
        if (ComForm->objectName().isEmpty())
            ComForm->setObjectName(QString::fromUtf8("ComForm"));
        ComForm->setWindowModality(Qt::NonModal);
        ComForm->resize(400, 200);
        ComForm->setMinimumSize(QSize(400, 200));
        ComForm->setMaximumSize(QSize(600, 200));
        ComForm->setContextMenuPolicy(Qt::ActionsContextMenu);
        ComForm->setAutoFillBackground(false);
        ComForm->setStyleSheet(QString::fromUtf8(""));
        horizontalLayout = new QHBoxLayout(ComForm);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        ListPort = new QListWidget(ComForm);
        ListPort->setObjectName(QString::fromUtf8("ListPort"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ListPort->sizePolicy().hasHeightForWidth());
        ListPort->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(ListPort);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        BTNPortRefresh = new QPushButton(ComForm);
        BTNPortRefresh->setObjectName(QString::fromUtf8("BTNPortRefresh"));
        BTNPortRefresh->setMinimumSize(QSize(80, 40));
        BTNPortRefresh->setMaximumSize(QSize(80, 40));
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
        BTNPortRefresh->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(BTNPortRefresh);

        BTNOK = new QPushButton(ComForm);
        BTNOK->setObjectName(QString::fromUtf8("BTNOK"));
        BTNOK->setMinimumSize(QSize(80, 40));
        BTNOK->setMaximumSize(QSize(80, 40));
        BTNOK->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        BTNOK->setIcon(icon1);
        BTNOK->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(BTNOK);

        BTNCancle = new QPushButton(ComForm);
        BTNCancle->setObjectName(QString::fromUtf8("BTNCancle"));
        BTNCancle->setMinimumSize(QSize(80, 40));
        BTNCancle->setMaximumSize(QSize(80, 40));
        BTNCancle->setStyleSheet(QString::fromUtf8("QPushButton {\n"
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
        BTNCancle->setIcon(icon2);
        BTNCancle->setIconSize(QSize(30, 30));

        verticalLayout->addWidget(BTNCancle);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        LabelSelected = new QLabel(ComForm);
        LabelSelected->setObjectName(QString::fromUtf8("LabelSelected"));

        horizontalLayout_3->addWidget(LabelSelected);

        CBoxBaudrate = new QComboBox(ComForm);
        CBoxBaudrate->setObjectName(QString::fromUtf8("CBoxBaudrate"));

        horizontalLayout_3->addWidget(CBoxBaudrate);


        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout->addLayout(verticalLayout_2);


        retranslateUi(ComForm);

        QMetaObject::connectSlotsByName(ComForm);
    } // setupUi

    void retranslateUi(QWidget *ComForm)
    {
        ComForm->setWindowTitle(QCoreApplication::translate("ComForm", "Selecting Port", nullptr));
        BTNPortRefresh->setText(QString());
        BTNOK->setText(QString());
        BTNCancle->setText(QString());
        LabelSelected->setText(QCoreApplication::translate("ComForm", "Selected :", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ComForm: public Ui_ComForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMFORM_H
