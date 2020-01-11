#ifndef PATCHSETTINGSPANEL_H
#define PATCHSETTINGSPANEL_H

#include <QDialog>
#include <QButtonGroup>
#include <QCheckBox>

#include <preset.h>

namespace Ui {
class PatchSettingsPanel;
}

class PatchSettingsPanel : public QDialog
{
    Q_OBJECT

public:
    explicit PatchSettingsPanel(QWidget *parent = nullptr, Preset *main_preset = nullptr);
    ~PatchSettingsPanel();

private slots:

    void on_buttonBox_accepted();

    void on_checkBox_fixflaaghra_stateChanged(int state);

    void on_browseButton_clicked();

private:
    Ui::PatchSettingsPanel *ui;
    QButtonGroup *buttongroupHeatDamage;
    QButtonGroup *buttongroupProtection;
    Preset *preset;

    void set_checkbox(QCheckBox *checkbox,bool condition);
};

#endif // PATCHSETTINGSPANEL_H
