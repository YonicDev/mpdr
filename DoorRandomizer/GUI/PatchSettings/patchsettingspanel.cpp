#include "patchsettingspanel.h"
#include "ui_patchsettingspanel.h"

PatchSettingsPanel::PatchSettingsPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatchSettingsPanel)
{
    ui->setupUi(this);
    buttongroupHeatDamage = new QButtonGroup(this);
    buttongroupHeatDamage->addButton(ui->radioHeatDamageAny);
    buttongroupHeatDamage->addButton(ui->radioHeatDamageVaria);
    buttongroupProtection = new QButtonGroup(this);
    buttongroupProtection->addButton(ui->radioStaggerProtection);
    buttongroupProtection->addButton(ui->radioStackProtection);
}

PatchSettingsPanel::~PatchSettingsPanel()
{
    delete buttongroupHeatDamage;
    delete buttongroupProtection;
    delete ui;
}

void PatchSettingsPanel::on_checkBox_4_stateChanged(int state)
{
    ui->lineFlaaghra->setEnabled(state>=1);
}
