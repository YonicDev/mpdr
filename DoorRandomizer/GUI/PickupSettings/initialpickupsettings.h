#ifndef INITIALPICKUPSETTINGS_H
#define INITIALPICKUPSETTINGS_H

#include <QDialog>

namespace Ui {
class InitialPickupSettings;
}

class InitialPickupSettings : public QDialog
{
    Q_OBJECT

public:
    explicit InitialPickupSettings(QWidget *parent = nullptr);
    ~InitialPickupSettings();

private:
    Ui::InitialPickupSettings *ui;
};

#endif // INITIALPICKUPSETTINGS_H
