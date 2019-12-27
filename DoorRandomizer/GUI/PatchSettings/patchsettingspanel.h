#ifndef PATCHSETTINGSPANEL_H
#define PATCHSETTINGSPANEL_H

#include <QDialog>
#include <QButtonGroup>

namespace Ui {
class PatchSettingsPanel;
}

class PatchSettingsPanel : public QDialog
{
    Q_OBJECT

public:
    explicit PatchSettingsPanel(QWidget *parent = nullptr);
    ~PatchSettingsPanel();

private slots:
    void on_checkBox_4_stateChanged(int arg1);

private:
    Ui::PatchSettingsPanel *ui;
    QButtonGroup *buttongroupHeatDamage;
    QButtonGroup *buttongroupProtection;
};

#endif // PATCHSETTINGSPANEL_H
