#ifndef DOOREXCLUDER_H
#define DOOREXCLUDER_H

#include <QDialog>
#include <QGraphicsView>

#include <preset.h>

typedef std::map<QString,std::vector<bool>> RoomDoorList;

enum DoorOrientation {
    Horizontal,
    Vertical,
    Floor,
    Centered
};

enum DoorDirection {
    Up,
    Down,
    Left,
    Right,
    Top,
    Bottom,
    Center
};

struct DoorPoint {
    QPoint position;
    bool is_front;
    DoorOrientation orientation;
    DoorPoint(QPoint p,DoorDirection dir) {
        position = p;
        is_front = dir%2;
        orientation = DoorOrientation(dir/2);
    }
};

typedef std::map<QString,std::vector<DoorPoint>> RoomPointsList;

class DoorMapIcon;

namespace Ui {
class DoorExcluder;
}

enum World {
    TallonOverworld,
    ChozoRuins,
    MagmoorCaverns,
    PhendranaDrifts,
    PhazonMines
};

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
    Preset *preset = nullptr;
    vector<RoomDoorList> *excluded_doors;

    void showEvent(QShowEvent *) override;

    void initializeMap(QGraphicsView *map,QPixmap image);
    void initializeExcludedDoors();
    void addIcons(QGraphicsView *map,World world);
};

#endif // DOOREXCLUDER_H
