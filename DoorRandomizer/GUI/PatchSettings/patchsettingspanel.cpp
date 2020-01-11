#include "patchsettingspanel.h"
#include "ui_patchsettingspanel.h"

#include <QFileDialog>

PatchSettingsPanel::PatchSettingsPanel(QWidget *parent, Preset *main_preset) :
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

    preset = main_preset;
    set_checkbox(ui->checkBox_skipfrigate,preset->additional_settings.skip_frigate);
    set_checkbox(ui->checkBox_skipcrater,preset->additional_settings.skip_crater);
    set_checkbox(ui->checkBox_skiphudmemos,preset->additional_settings.skip_hudmemos);
    set_checkbox(ui->checkBox_fixflaaghra,preset->additional_settings.fix_flaaghra_music);
    if(preset->additional_settings.fix_flaaghra_music)
        ui->lineFlaaghra->setText(preset->additional_settings.trilogy_iso);
    ui->radioHeatDamageVaria->setChecked(preset->additional_settings.varia_heat_protection);
    ui->radioHeatDamageAny->setChecked(!preset->additional_settings.varia_heat_protection);
    ui->radioStaggerProtection->setChecked(preset->additional_settings.stagger_suit_damage);
    ui->radioStackProtection->setChecked(!preset->additional_settings.stagger_suit_damage);
}

void PatchSettingsPanel::set_checkbox(QCheckBox *checkbox,bool condition) {
    Qt::CheckState state = condition? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    checkbox->setCheckState(state);
}

PatchSettingsPanel::~PatchSettingsPanel()
{
    delete buttongroupHeatDamage;
    delete buttongroupProtection;
    delete ui;
}

void PatchSettingsPanel::on_buttonBox_accepted()
{
    preset->additional_settings.skip_hudmemos = ui->checkBox_skiphudmemos->isChecked();
    preset->additional_settings.skip_frigate = ui->checkBox_skipfrigate->isChecked();
    preset->additional_settings.skip_crater = ui->checkBox_skipcrater->isChecked();
    preset->additional_settings.fix_flaaghra_music = ui->checkBox_fixflaaghra->isChecked();
    preset->additional_settings.trilogy_iso = preset->additional_settings.fix_flaaghra_music?
                ui->lineFlaaghra->text():"";
    preset->additional_settings.varia_heat_protection = ui->radioHeatDamageVaria->isChecked();
    preset->additional_settings.stagger_suit_damage = ui->radioStaggerProtection->isChecked();

}

void PatchSettingsPanel::on_checkBox_fixflaaghra_stateChanged(int state)
{
    ui->lineFlaaghra->setEnabled(state>=1);
    ui->browseButton->setEnabled(state>=1);
}

void PatchSettingsPanel::on_browseButton_clicked()
{
    QString input = QFileDialog::getOpenFileName(this,tr("Select a Metroid Prime NTSC 0-00/0-02  disc image"),"",tr("Standard disk images (*.iso *.gcm)"),nullptr,QFileDialog::ReadOnly);
    if(!input.isNull())
        ui->lineFlaaghra->setText(input);
}
