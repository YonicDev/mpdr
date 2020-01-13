#ifndef DOOREXCLUDER_H
#define DOOREXCLUDER_H

#include <QDialog>
#include <QGraphicsView>

#include <preset.h>

class DoorMapIcon;

namespace Ui {
class DoorExcluder;
}

class DoorExcluder : public QDialog
{
    Q_OBJECT

public:
    DoorExcluder(QWidget *parent = nullptr,Preset *main_preset = nullptr);
    ~DoorExcluder();
    void loadDoorDetails(DoorMapIcon *icon);

private slots:
    void on_checkBox_clicked(bool checked);

private:
    Ui::DoorExcluder *ui;
    DoorMapIcon *selected_icon = nullptr;
    Preset *preset;

    void initializeMap(QGraphicsView *map,QPixmap image);
};

#endif // DOOREXCLUDER_H
