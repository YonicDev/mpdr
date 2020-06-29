#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include "global.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString logo_path = "://resources/img/";
    QPixmap logo(logo_path.append("mpdr_logo.png"));

    QString about_text = QString("<html><head/>"
                                 "<body><p>MPDR version %1<br/>"
                                 "MIT License YonicStudios 2020</p>"
                                 "<p>Using randomprime version %2<br/>"
                                 "MIT License aprilwade (syncathetic) 2020</p>"
                                 "<p><a href=\"https://github.com/YonicDev/mpdr-qt\"><span style=\" text-decoration: underline; color:#007af4;\">GitHub</span></a>"
                                 " | <a href=\"https://discord.gg/9Wxb37\"><span style=\" text-decoration: underline; color:#007af4;\">Modding Discord</span></a>"
                                 " | <a href=\"http://discord.gg/WWGcay6\"><span style=\" text-decoration: underline; color:#007af4;\">Randomizer Discord</span></a></p>"
                                 "</body></html>").arg(MPDR_VERSION).arg(RANDOMPRIME_VERSION);

    ui->label->setText(about_text);

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
