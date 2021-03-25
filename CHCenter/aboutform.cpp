#include "aboutform.h"
#include "ui_aboutform.h"

AboutForm::AboutForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutForm)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("About"));

    ui->LabelWeb->setText("<a href=\"https://hipnuc.com\">https://hipnuc.com</a>");
    ui->LabelWeb->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->LabelWeb2->setText("<a href=\"https://hipnuc.com/datahub.html\">https://hipnuc.com/datahub.html</a>");
    ui->LabelWeb2->setTextInteractionFlags(Qt::TextBrowserInteraction);
}

AboutForm::~AboutForm()
{
    delete ui;
}

void AboutForm::setLabelVersion(QString sf_version)
{
    ui->Label_Version->setText(sf_version);
}

void AboutForm::on_LabelWeb_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("https://hipnuc.com"));
}

void AboutForm::on_LabelWeb2_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("https://hipnuc.com/datahub.html"));
}
