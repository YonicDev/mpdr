#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString logo_path = "://resources/img/";
    QPixmap logo(logo_path.append("mpdr_logo.png"));

    ui->icon->setPixmap(logo.scaled(256,256,Qt::KeepAspectRatio,Qt::SmoothTransformation));

}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    this->close();
}
