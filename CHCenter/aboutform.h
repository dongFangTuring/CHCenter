#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QWidget>
#include <QDesktopServices>
#include <QUrl>

namespace Ui {
class AboutForm;
}

class AboutForm : public QWidget
{
    Q_OBJECT

public:
    explicit AboutForm(QWidget *parent = nullptr);
    ~AboutForm();

    void setLabelVersion(QString sf_version);


private slots:
    void on_LabelWeb_linkActivated(const QString &link);

    void on_LabelWeb2_linkActivated(const QString &link);

private:
    Ui::AboutForm *ui;
};

#endif // ABOUTFORM_H
