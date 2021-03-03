#ifndef FORM_DISPLAY_H
#define FORM_DISPLAY_H

#include <QWidget>

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
    void slt_kptl_recv(QByteArray &ba); /* on kptl frame recv */
private:
    Ui::Form_display *ui;
};

#endif // FORM_DISPLAY_H
