#ifndef PRESET_H
#define PRESET_H

#include <string>
#include <stdint.h>

#include <QJsonObject>

#define MPDRP_VERSION 2

using namespace std;

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
    void serialize(QJsonObject &json);
    int deserialize(const QJsonObject &json);
};

#endif // PRESET_H

