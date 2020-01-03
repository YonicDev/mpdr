#ifndef INITIALPICKUPSETTINGS_H
#define INITIALPICKUPSETTINGS_H

#include <QDialog>
#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>

namespace Ui {
class InitialPickupSettings;
}

class InitialPickupSettings : public QDialog
{
    Q_OBJECT


public:
    explicit InitialPickupSettings(QWidget *parent = nullptr, uint64_t starting_pickups = 0);
    ~InitialPickupSettings() override;
    void set_bitfield(uint64_t value);

signals:
    void closed_ok(uint64_t);

public slots:
    void write_to_bitfield();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::InitialPickupSettings *ui;
    uint64_t bitfield;

    void read_from_bitfield();
    void set_checkbox(QCheckBox *field, uint64_t value);
    void set_numericfield(QSpinBox *field, uint64_t value);
};

#endif // INITIALPICKUPSETTINGS_H
