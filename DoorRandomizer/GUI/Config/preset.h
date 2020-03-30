#ifndef PRESET_H
#define PRESET_H

#include <string>
#include <stdint.h>
#include <map>
#include <vector>

#include <QJsonObject>

#define MPDRP_VERSION 3

using namespace std;

typedef map<QString,vector<bool>> RoomDoorList;

enum World {
    TallonOverworld,
    ChozoRuins,
    MagmoorCaverns,
    PhendranaDrifts,
    PhazonMines
};

class DoorWeights final {
public:
    int tallon_overworld[4];
    int chozo_ruins[4];
    int magmoor_caverns[4];
    int phendrana_drifts[4];
    int phazon_mines[4];
    void serialize(QJsonObject &json);
    int deserialize(const QJsonObject &json,const int version);
    int* operator[](const int index);
};

class PatcherSettings final {
public:
    bool skip_hudmemos;
    bool skip_frigate;
    bool skip_crater;
    bool fix_flaaghra_music;
    QString trilogy_iso;
    bool stagger_suit_damage;
    bool varia_heat_protection;
    bool powerbomb_lockpick;
    void serialize(QJsonObject &json);
    int deserialize(const QJsonObject &json);
    PatcherSettings operator=(const PatcherSettings settings);
};

class Preset final {
public:
    Preset();
    QString input_iso;
    QString output_iso;
    int32_t seed;
    DoorWeights weights;
    uint64_t starting_pickups;
    PatcherSettings additional_settings;
    vector<RoomDoorList> excluded_doors;
    vector<RoomDoorList> untouched_doors;

    void serialize(QJsonObject &json);
    int deserialize(const QJsonObject &json);
private:
    void initialize_doors();
};

#endif // PRESET_H

