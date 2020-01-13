#include "doormapicon.h"

#include <QBrush>
#include <QPen>
#include <QCursor>

DoorMapIcon::DoorMapIcon(DoorExcluder *parent, World world, int roomID, int doorID, QPoint map_position, bool is_front, DoorOrientation orientation, bool force_exclude) {
    int map_x, map_y;

    if(orientation == DoorOrientation::Horizontal) {
        map_x = map_position.x()-DOOR_LONG/2;
        if(is_front) {
            map_y = map_position.y();
        } else {
            map_y = map_position.y()-DOOR_SHORT;
        }
    } else if(orientation == DoorOrientation::Vertical) {
        map_y = map_position.y()-DOOR_LONG/2;
        if(is_front) {
            map_x = map_position.x();
        } else {
            map_x = map_position.x()-DOOR_SHORT;
        }
    } else {
        map_x = map_position.x()-DOOR_LONG/2;
        map_y = map_position.y()-DOOR_LONG/2;
    }

    doorExcluder = parent;

    m_World = world;
    m_areaID = roomID;
    m_doorID = doorID;

    setX(map_x);
    setY(map_y);

    setPen(QPen(QColor(0xFF,0xFF,0xFF)));
    setBrush(QBrush(QColor(0x03b813)));

    m_orientation = orientation;

    QRectF door_rect;
    if(m_orientation == DoorOrientation::Horizontal) {
        door_rect = QRectF(0,0,DOOR_LONG,DOOR_SHORT);
    } else if(m_orientation == DoorOrientation::Vertical) {
        door_rect = QRectF(0,0,DOOR_SHORT,DOOR_LONG);
    } else if(m_orientation == DoorOrientation::Floor) {
        door_rect = QRectF(0,0,DOOR_LONG,DOOR_LONG);
    }
    setRect(door_rect);
    setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
    if(force_exclude || m_orientation == DoorOrientation::Floor) {
        m_forceExclude = true;
        exclude(true);
    }
}

World DoorMapIcon::world() { return m_World; }
int DoorMapIcon::areaID() { return m_areaID; }
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
    QColor fill = excluded?QColor(0x731717):QColor(0x03b813);
    setBrush(QBrush(fill));
    update(this->rect());
}
