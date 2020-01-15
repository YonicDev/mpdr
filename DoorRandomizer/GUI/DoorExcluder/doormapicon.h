#ifndef DOORMAPICON_H
#define DOORMAPICON_H

#include <QGraphicsItem>
#include <QMetaObject>
#include <doorexcluder.h>

class DoorMapIcon : public QGraphicsRectItem
{
public:

    // is_front = The left most / upper most door in a door set
    DoorMapIcon(DoorExcluder *parent, World world, QString area_name,int door_id, QPoint map_position, bool is_front=true, DoorOrientation orientation=DoorOrientation::Horizontal, bool force_exclude=false);
    DoorMapIcon(DoorExcluder *parent, World world, QString area_name,int door_id, DoorPoint point, bool force_exclude=false);
    void getDoorInformation();
    void select(bool selected=true);
    void exclude(bool excluded);

    World world();
    QString areaName();
    int doorID();
    bool isExcluded();
    bool canBeExcluded();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    DoorExcluder *doorExcluder;

    DoorOrientation m_orientation;

    QString m_areaName;
    int m_doorID;
    World m_World;

    bool m_Excluded=false;
    bool m_forceExclude = false;

    const int DOOR_LONG = 20;
    const int DOOR_SHORT = 10;
};

#endif // DOORMAPICON_H
