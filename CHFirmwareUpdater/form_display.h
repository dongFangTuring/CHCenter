#ifndef FORM_DISPLAY_H
#define FORM_DISPLAY_H

#include <QWidget>
#include "utilities/imu_parser.h"

namespace Ui {
class Form_display;
}

class Form_display : public QWidget
{
    Q_OBJECT

public:
    explicit Form_display(QWidget *parent = nullptr);
    ~Form_display();


public slots:
    void slt_kptl_payload_recv(QByteArray &ba); /* on kptl frame recv */
private:
    Ui::Form_display *ui;
    imu_parser *parser;
private slots:
    void slt_tmr(void);
};

#endif // FORM_DISPLAY_H
