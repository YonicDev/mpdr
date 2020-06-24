#include "initialpickupsettings.h"
#include "ui_initialpickupsettings.h"
#include <stdio.h>

InitialPickupSettings::InitialPickupSettings(QWidget *parent, Preset *main_preset) :
    QDialog(parent),
    ui(new Ui::InitialPickupSettings)
{
    ui->setupUi(this);
    QList<QObject*> groups = this->children();
    preset = main_preset;

    set_bitfield(preset->starting_pickups);

    foreach(QObject* group,groups) {
        QGroupBox* box = qobject_cast<QGroupBox*>(group);
        if(box) {
            QList<QObject*> group_children = box->children();
            foreach(QObject* child,group_children) {
                QCheckBox *cb = qobject_cast<QCheckBox*>(child);
                QSpinBox *sb = qobject_cast<QSpinBox*>(child);
                if(cb) {
                    cb->connect(cb,SIGNAL(clicked()),this,SLOT(write_to_bitfield()));
                }
                if(sb) {
                    sb->connect(sb,SIGNAL(valueChanged(int)),this,SLOT(write_to_bitfield()));
                }
            }
        }
    }
}

InitialPickupSettings::~InitialPickupSettings()
{
    delete ui;
}

void InitialPickupSettings::set_bitfield(uint64_t value) {
    bitfield = value;
    read_from_bitfield();
}

void InitialPickupSettings::read_from_bitfield() {
    set_checkbox(ui->checkBox_VisorScan,bitfield>>0 &1);
    set_numericfield(ui->spinBox_Missiles,bitfield>>1 &0xff);
    set_numericfield(ui->spinBox_ETanks,bitfield>>9 & ((1 << 4)-1));
    set_numericfield(ui->spinBox_PowerBombs,bitfield>>13 & ((1 << 4)-1));
    set_checkbox(ui->checkBox_WaveBeam,bitfield>>17 &1);
    set_checkbox(ui->checkBox_IceBeam,bitfield>>18 &1);
    set_checkbox(ui->checkBox_PlasmaBeam,bitfield>>19 &1);
    set_checkbox(ui->checkBox_ChargeBeam,bitfield>>20 &1);
    set_checkbox(ui->checkBox_BallMorph,bitfield>>21 &1);
    set_checkbox(ui->checkBox_BallBombs,bitfield>>22 &1);
    set_checkbox(ui->checkBox_BallSpider,bitfield>>23 &1);
    set_checkbox(ui->checkBox_BallBoost,bitfield>>24 &1);
    set_checkbox(ui->checkBox_SuitVaria,bitfield>>25 &1);
    set_checkbox(ui->checkBox_SuitGravity,bitfield>>26 &1);
    set_checkbox(ui->checkBox_SuitPhazon,bitfield>>27 &1);
    set_checkbox(ui->checkBox_VisorThermal,bitfield>>28 &1);
    set_checkbox(ui->checkBox_VisorXRay,bitfield>>29 &1);
    set_checkbox(ui->checkBox_SpaceJump,bitfield>>30 &1);
    set_checkbox(ui->checkBox_GrappleBeam,bitfield>>31 &1);
    set_checkbox(ui->checkBox_SuperMissile,bitfield>>32 &1);
    set_checkbox(ui->checkBox_WaveBuster,bitfield>>33 &1);
    set_checkbox(ui->checkBox_IceSpreader,bitfield>>34 &1);
    set_checkbox(ui->checkBox_Flamethrower,bitfield>>35 &1);
    // TODO: Maybe add Phazon Beam, Combat Visor and Power suit
}

void InitialPickupSettings::set_checkbox(QCheckBox *field, uint64_t value) {
    Qt::CheckState check = value>0?Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    field->setCheckState(check);
}
void InitialPickupSettings::set_numericfield(QSpinBox *field, uint64_t value) {
    int val = static_cast<int>(value);
    field->setValue(val);
}

void InitialPickupSettings::write_to_bitfield() {
    bitfield =
            static_cast<uint64_t>(ui->checkBox_VisorScan->isChecked() << 0) |
            static_cast<uint64_t>(ui->spinBox_Missiles->value() << 1) |
            static_cast<uint64_t>(ui->spinBox_ETanks->value() << 9) |
            static_cast<uint64_t>(ui->spinBox_PowerBombs->value())<< 13 |
            static_cast<uint64_t>(ui->checkBox_WaveBeam->isChecked()) << 17 |
            static_cast<uint64_t>(ui->checkBox_IceBeam->isChecked()) << 18 |
            static_cast<uint64_t>(ui->checkBox_PlasmaBeam->isChecked()) << 19 |
            static_cast<uint64_t>(ui->checkBox_ChargeBeam->isChecked()) << 20 |
            static_cast<uint64_t>(ui->checkBox_BallMorph->isChecked()) << 21 |
            static_cast<uint64_t>(ui->checkBox_BallBombs->isChecked()) << 22 |
            static_cast<uint64_t>(ui->checkBox_BallSpider->isChecked()) << 23 |
            static_cast<uint64_t>(ui->checkBox_BallBoost->isChecked()) << 24 |
            static_cast<uint64_t>(ui->checkBox_SuitVaria->isChecked()) << 25 |
            static_cast<uint64_t>(ui->checkBox_SuitGravity->isChecked()) << 26 |
            static_cast<uint64_t>(ui->checkBox_SuitPhazon->isChecked()) << 27 |
            static_cast<uint64_t>(ui->checkBox_VisorThermal->isChecked()) << 28 |
            static_cast<uint64_t>(ui->checkBox_VisorXRay->isChecked()) << 29 |
            static_cast<uint64_t>(ui->checkBox_SpaceJump->isChecked()) << 30 |
            static_cast<uint64_t>(ui->checkBox_GrappleBeam->isChecked()) << 31 |
            static_cast<uint64_t>(ui->checkBox_SuperMissile->isChecked()) << 32 |
            static_cast<uint64_t>(ui->checkBox_WaveBuster->isChecked()) << 33 |
            static_cast<uint64_t>(ui->checkBox_IceSpreader->isChecked()) << 34 |
            static_cast<uint64_t>(ui->checkBox_Flamethrower->isChecked()) << 35;
            // TODO: Maybe add Phazon Beam, Combat Visor and Power suit
}

void InitialPickupSettings::on_buttonBox_accepted()
{
    preset->starting_pickups = bitfield;
}
