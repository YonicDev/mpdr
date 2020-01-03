#include "randomizergui.h"
#include "./ui_randomizergui.h"

#include <random>

#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>

#include "./initialpickupsettings.h"
#include "./patchsettingspanel.h"

RandomizerGUI::RandomizerGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RandomizerGUI)
{
    ui->setupUi(this);
    preset = new Preset();
    load_from_preset();
}

RandomizerGUI::~RandomizerGUI()
{
    delete preset;
    delete ui;
}

void RandomizerGUI::save_to_preset() {
    auto zone_groups = ui->group_weights->children();
    for(auto zone = zone_groups.begin(); zone!=zone_groups.end();++zone) {
        QGroupBox *area = qobject_cast<QGroupBox*>(*zone);
        if(area) {
            auto area_widgets = area->children();
            for(auto widget=area_widgets.begin();widget!=area_widgets.end();++widget) {
                QSlider *slider = qobject_cast<QSlider*>(*widget);
                int i = zone_groups.indexOf(*zone);
                int j = area_widgets.indexOf(*widget);

                if(slider) {
                    preset->weights[i-1][j-1] = slider->value();
                }
            }
        }
    }
    preset->seed = static_cast<int32_t>(ui->seedField->value());
}

void RandomizerGUI::load_from_preset() {
    auto zone_groups = ui->group_weights->children();
    for(auto zone = zone_groups.begin(); zone!=zone_groups.end();++zone) {
        QGroupBox *area = qobject_cast<QGroupBox*>(*zone);
        if(area) {
            auto area_widgets = area->children();
            for(auto widget=area_widgets.begin();widget!=area_widgets.end();++widget) {
                QSlider *slider = qobject_cast<QSlider*>(*widget);
                int i = zone_groups.indexOf(*zone);
                int j = area_widgets.indexOf(*widget);

                if(slider) {
                    slider->setValue(preset->weights[i-1][j-1]);
                }
            }
        }
    }
    ui->seedField->setValue(static_cast<int32_t>(preset->seed));
}


void RandomizerGUI::on_pickupsButton_clicked()
{
    InitialPickupSettings *dialog = new InitialPickupSettings(this,preset);
    dialog->open();
}

void RandomizerGUI::on_buttonBrowseInput_clicked()
{
    ui->lineInputFile->setText(QFileDialog::getOpenFileName(this,tr("Select a Metroid Prime NTSC 0-00 disc image"),"",tr("Standard disk images (*.iso *.gcm)"),nullptr,QFileDialog::ReadOnly));
}

void RandomizerGUI::on_buttonBrowseOutput_clicked()
{
    ui->lineOutputFile->setText(QFileDialog::getSaveFileName(this,tr("Choose where to save the patched image"),"",tr("ISO disk image (*.iso)")));
}

void RandomizerGUI::on_buttonPatches_clicked()
{
    PatchSettingsPanel *dialog = new PatchSettingsPanel(this,preset);
    dialog->open();
}

void RandomizerGUI::on_buttonGenerateSeed_clicked()
{
    std::mt19937 rng(static_cast<uint32_t>(preset->seed));
    std::uniform_int_distribution<int32_t> distr;

    preset->seed = distr(rng);
    ui->seedField->setValue(preset->seed);
}

void RandomizerGUI::on_buttonUseTime_clicked()
{
    preset->seed = static_cast<int32_t>(time(nullptr));
    ui->seedField->setValue(preset->seed);
}

void RandomizerGUI::on_actionOpen_preset_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open a preset file..."),"",tr("Preset files (*.json)"),nullptr,QFileDialog::ReadOnly);
    if(path.isNull()) return;
    QFile preset_file(path);
    if(preset_file.open(QIODevice::ReadOnly)) {
        QByteArray data = preset_file.readAll();
        QJsonDocument preset_json = QJsonDocument::fromJson(data);
        int parse = preset->deserialize(preset_json.object());
        if(parse==0) {
            load_from_preset();
        } else {
            switch(parse) {
                case -1:
                    QMessageBox::critical(this,"Error","This is not a MPDR preset.");
                break;
                case -2:
                    QMessageBox::critical(this,"Error","Error parsing the MPDR preset.");
                break;
                case -3:
                    QMessageBox::critical(this,"Error","Data within the MPDR might be corrupt.");
                break;
                case -4:
                    QMessageBox::critical(this,"Error","MPDR presets of version 1 are currently unsupported.");
                break;
            }
        }
        preset_file.close();
    } else {
        QMessageBox::critical(this,"Error","Could not open preset.");
    }
}

void RandomizerGUI::on_actionSave_preset_triggered()
{
    save_to_preset();
    QString path = QFileDialog::getSaveFileName(this,tr("Save preset file to..."),"",tr("Preset files (*.json)"),nullptr);
    if(path.isNull()) return;
    QJsonObject preset_json;
    preset->serialize(preset_json);
    QFile preset_file(path);

    QJsonDocument doc(preset_json);
    if(preset_file.open(QIODevice::WriteOnly)) {
        preset_file.write(doc.toJson());
        preset_file.close();
    } else {
        qWarning("Could not open file");
        QMessageBox::critical(this,"Error","Could not save preset.");
    }

}
