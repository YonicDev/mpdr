#ifndef INITIALPICKUPSETTINGS_H
#define INITIALPICKUPSETTINGS_H

#include <QDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <preset.h>

namespace Ui {
class InitialPickupSettings;
}

class InitialPickupSettings : public QDialog
{
    Q_OBJECT


public:
    explicit InitialPickupSettings(QWidget *parent = nullptr, Preset *main_preset = nullptr);
    ~InitialPickupSettings() override;
    void set_bitfield(uint64_t value);

private slots:
    void on_buttonBox_accepted();
    void write_to_bitfield();

private:
    Ui::InitialPickupSettings *ui;
    uint64_t bitfield;
    Preset *preset;

    void read_from_bitfield();
    void set_checkbox(QCheckBox *field, uint64_t value);
    void set_numericfield(QSpinBox *field, uint64_t value);
};

#endif // INITIALPICKUPSETTINGS_H
