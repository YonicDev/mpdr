#include "initialpickupsettings.h"
#include "ui_initialpickupsettings.h"

InitialPickupSettings::InitialPickupSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitialPickupSettings)
{
    ui->setupUi(this);
}

InitialPickupSettings::~InitialPickupSettings()
{
    delete ui;
}
