#include "doormapicon.h"

#include <QBrush>
#include <QPen>
#include <QCursor>

DoorMapIcon::DoorMapIcon(DoorExcluder *parent, World world, QString roomName, int doorID, QPoint map_position, bool is_front, DoorOrientation orientation, bool force_exclude) {
    int map_x, map_y;
    switch(orientation) {
        case Horizontal:
            map_x = map_position.x()-DOOR_LONG/2;
            if(is_front)
                map_y = map_position.y();
            else
                map_y = map_position.y()-DOOR_SHORT;
        break;
        case Vertical:
            map_y = map_position.y()-DOOR_LONG/2;
            if(is_front)
                map_x = map_position.x();
            else
                map_x = map_position.x()-DOOR_SHORT;
        break;
        case Floor:
            map_y = map_position.y()-DOOR_LONG/2;
            if(is_front)
                map_x = map_position.x()-DOOR_LONG;
            else
                map_x = map_position.x();
        break;
        case Centered:
            map_x = map_position.x()-DOOR_LONG/2;
            map_y = map_position.y()-DOOR_LONG/2;
        break;
    }

    doorExcluder = parent;

    m_World = world;
    m_areaName = roomName;
    m_doorID = doorID;

    setX(map_x);
    setY(map_y);

    setPen(QPen(QColor(0xFF,0xFF,0xFF)));
    setBrush(QBrush(QColor(0x03b813)));

    m_orientation = orientation;

    QRectF door_rect;
    if(m_orientation == Horizontal) {
        door_rect = QRectF(0,0,DOOR_LONG,DOOR_SHORT);
    } else if(m_orientation == Vertical) {
        door_rect = QRectF(0,0,DOOR_SHORT,DOOR_LONG);
    } else if(m_orientation == Floor || m_orientation == Centered) {
        door_rect = QRectF(0,0,DOOR_LONG,DOOR_LONG);
    }
    setRect(door_rect);
    setCursor(QCursor(Qt::PointingHandCursor));
    if(force_exclude || m_orientation == Floor) {
        m_forceExclude = true;
        exclude(true);
    }
}

DoorMapIcon::DoorMapIcon(DoorExcluder *parent, World world, QString area_name,int door_id, DoorPoint point, bool force_exclude)
    :DoorMapIcon(parent,world,area_name,door_id,point.position,point.is_front,point.orientation,force_exclude) {};

World DoorMapIcon::world() { return m_World; }
QString DoorMapIcon::areaName() { return m_areaName; }
int DoorMapIcon::doorID() { return m_doorID; }

void DoorMapIcon::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    doorExcluder->loadDoorDetails(this);
}

void DoorMapIcon::select(bool selected) {
    QColor border = selected?QColor(0,0,0xFF):QColor(0xFF,0xFF,0xFF);
    setPen(QPen(border));
    update(this->rect());
}

bool DoorMapIcon::isExcluded() {
    return m_Excluded;
}
bool DoorMapIcon::canBeExcluded() {
    return !m_forceExclude;
}

void DoorMapIcon::exclude(bool excluded) {
    m_Excluded = excluded;
    doorExcluder->excludeDoor(m_World,m_areaName,m_doorID,m_Excluded);

    QColor fill = excluded?QColor(0x731717):QColor(0x03b813);
    setBrush(QBrush(fill));
    update(this->rect());
}
