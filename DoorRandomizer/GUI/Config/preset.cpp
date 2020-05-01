#include "preset.h"

#include "time.h"

#include <QJsonArray>
#include <QStringList>
#include <QDebug>

Preset::Preset() {
    for(int i=0;i<5;++i) {
        for(int j=0;j<4;++j) {
            weights[i][j] = 25;
        }
    }
    input_iso = "";
    output_iso = "";
    seed = static_cast<int32_t>(time(nullptr));
    starting_pickups = 1;
    additional_settings.skip_frigate = true;
    additional_settings.skip_crater = false;
    additional_settings.skip_hudmemos = false;
    additional_settings.fix_flaaghra_music = false;
    additional_settings.trilogy_iso = "";
    additional_settings.stagger_suit_damage = false;
    additional_settings.varia_heat_protection = false;
    additional_settings.powerbomb_lockpick = false;

    initialize_doors();
}

int* DoorWeights::operator[](const int index) {
    switch(index) {
        case 0:
        return tallon_overworld;
        case 1:
        return chozo_ruins;
        case 2:
        return magmoor_caverns;
        case 3:
        return phendrana_drifts;
        case 4:
        return phazon_mines;
        default:
        return nullptr;
    }
}

void Preset::serialize(QJsonObject &json) {
    json["mpdrp_version"] = MPDRP_VERSION;

    json["input_iso"] = input_iso;
    json["output_iso"] = output_iso;

    json["seed"] = seed;

    QJsonObject weights_obj;
    weights.serialize(weights_obj);
    json["door_weights"] = weights_obj;

    json["starting_pickups"] = qint64(starting_pickups);

    QJsonObject settings_obj;
    additional_settings.serialize(settings_obj);
    json["patch_settings"] = settings_obj;

    QJsonArray excdoors_obj;
    for(auto world = excluded_doors.begin();world!=excluded_doors.end();++world) {
        int world_index = world-excluded_doors.begin();
        QJsonObject world_obj;
        for(auto area = world->begin();area!=world->end();++area) {
            QString room_name = area->first;
            vector<bool> room = area->second;
            QJsonArray room_obj;
            for(auto door = room.begin();door!=room.end();++door) {
                int door_index = door-room.begin();
                bool excluded = excluded_doors[world_index][room_name][door_index];
                room_obj.append(excluded);
            }
            world_obj[room_name] = room_obj;
        }
        excdoors_obj.append(world_obj);
    }
    json["excluded_doors"] = excdoors_obj;
}

void DoorWeights::serialize(QJsonObject &json) {
    QJsonArray tallon;
    QJsonArray chozo;
    QJsonArray magmoor;
    QJsonArray phendrana;
    QJsonArray phazon;
    for(int i=0;i<4;i++) {
        tallon.append(tallon_overworld[i]);
        chozo.append(chozo_ruins[i]);
        magmoor.append(magmoor_caverns[i]);
        phendrana.append(phendrana_drifts[i]);
        phazon.append(phazon_mines[i]);
    }
    json["tallon_overworld"] = tallon;
    json["chozo_ruins"] = chozo;
    json["magmoor_caverns"] = magmoor;
    json["phendrana_drifts"] = phendrana;
    json["phazon_mines"] = phazon;
}

void PatcherSettings::serialize(QJsonObject &json) {
    json["fix_flaaghra_music"] = fix_flaaghra_music;
    json["trilogy_iso"] = trilogy_iso;
    json["skip_frigate"] = skip_frigate;
    json["skip_crater"] = skip_crater;
    json["skip_hudmemos"] = skip_hudmemos;
    json["stagger_suit_damage"] = stagger_suit_damage;
    json["varia_heat_protection"] = varia_heat_protection;
    json["powerbomb_lockpick"] = powerbomb_lockpick;
}

int Preset::deserialize(const QJsonObject &json) {
    if(json.contains("mpdrp_version") && json["mpdrp_version"].isDouble()) {
        int mpdrp_version = json["mpdrp_version"].toInt();
        int check;
        switch(mpdrp_version) {
            case 3:
                if(json.contains("excluded_doors")&&json["excluded_doors"].isArray()) {
                    QJsonArray exc_doors = json["excluded_doors"].toArray();
                    for(auto world = exc_doors.begin();world!=exc_doors.end();++world) {
                        if(world->isObject()) {
                            QJsonObject area = world->toObject();
                            for(auto room = area.begin();room!=area.end();++room) {
                                if(room->isArray()) {
                                    QJsonArray place = room->toArray();
                                    for(auto door = place.begin();door!=place.end();++door) {
                                        if(door->isBool()) {
                                            World target_world = static_cast<World>(world.i);
                                            QString target_area = room.key();
                                            excluded_doors[target_world][target_area][door.i] = door->toBool();
                                        }
                                    }
                                }
                            }
                        } else {
                            return -2;
                        }
                    }
                } else {
                    return -2;
                }
                if(json.contains("seed")&&json["seed"].isDouble())
                    seed = static_cast<int32_t>(json["seed"].toInt());
                else
                    return -2;
                check = weights.deserialize(json,2);
                if(check!=0)
                    return check;
                if(json.contains("starting_pickups") && json["starting_pickups"].isDouble())
                    starting_pickups = static_cast<uint64_t>(json["starting_pickups"].toDouble());
                else
                    return -2;
                if(json.contains("input_iso") && json["input_iso"].isString())
                    input_iso = json["input_iso"].toString();
                else
                    input_iso = "";
                if(json.contains("output_iso") && json["output_iso"].isString())
                    output_iso = json["output_iso"].toString();
                else
                    output_iso = "";
                check = additional_settings.deserialize(json);
            break;
            case 2: {
                if(json.contains("seed")&&json["seed"].isDouble())
                    seed = static_cast<int32_t>(json["seed"].toInt());
                else
                    return -2;
                check = weights.deserialize(json,2);
                if(check!=0)
                    return check;
                if(json.contains("starting_pickups") && json["starting_pickups"].isDouble())
                    starting_pickups = static_cast<uint64_t>(json["starting_pickups"].toDouble());
                else
                    return -2;
                if(json.contains("input_iso") && json["input_iso"].isString())
                    input_iso = json["input_iso"].toString();
                else
                    input_iso = "";
                if(json.contains("output_iso") && json["output_iso"].isString())
                    output_iso = json["output_iso"].toString();
                else
                    output_iso = "";
                check = additional_settings.deserialize(json);
                break;
            }
            case 1: {
                if(json.contains("seed")&&json["seed"].isDouble())
                    seed = static_cast<int32_t>(json["seed"].toInt());
                else
                    return -2;
                check = weights.deserialize(json,1);
                if(check!=0)
                    return check;
                if(json.contains("skip_frigate") && json["skip_frigate"].isBool())
                    additional_settings.skip_frigate = json["skip_frigate"].toBool();
                else
                    return -2;
                if(json.contains("trilogy_path") && json["trilogy_path"].isString()) {
                    additional_settings.fix_flaaghra_music = json["trilogy_path"].toString() != "";
                    additional_settings.trilogy_iso = json["trilogy_path"].toString();
                } else {
                    additional_settings.fix_flaaghra_music = false;
                    additional_settings.trilogy_iso = "";
                }
                break;
            }
            default: {return -1;}
        }
        return check;
    } else {return -1;}
}

int DoorWeights::deserialize(const QJsonObject &json,const int version) {
    switch(version) {
        case 2:
            if(json.contains("door_weights")&&json["door_weights"].isObject()) {
                QJsonObject obj = json["door_weights"].toObject();
                for(auto zone = obj.begin();zone != obj.end(); ++zone) {
                    if(zone->isArray()) {
                        QJsonArray area = zone->toArray();
                        for(auto weight = area.begin();weight!=area.end();++weight) {
                            if(weight->isDouble()) {
                                if(zone.key() == "tallon_overworld") {
                                    tallon_overworld[weight.i] = weight->toInt();
                                } else if(zone.key() == "chozo_ruins") {
                                    chozo_ruins[weight.i] = weight->toInt();
                                } else if(zone.key() == "magmoor_caverns") {
                                    magmoor_caverns[weight.i] = weight->toInt();
                                } else if(zone.key() == "phendrana_drifts") {
                                    phendrana_drifts[weight.i] = weight->toInt();
                                } else if(zone.key() == "phazon_mines") {
                                    phazon_mines[weight.i] = weight->toInt();
                                } else {
                                    return -3;
                                }
                            } else {
                                return -2;
                            }
                        }
                    } else {
                        return -2;
                    }
                }
            } else
                return -2;
        break;
        case 1:
            if(json.contains("weights")&&json["weights"].isObject()) {
                QJsonObject obj = json["weights"].toObject();
                for(auto zone = obj.begin();zone != obj.end(); ++zone) {
                    if(zone->isArray()) {
                        QJsonArray area = zone->toArray();
                        for(auto weight = area.begin();weight!=area.end();++weight) {
                            if(weight->isDouble()) {
                                if(zone.key() == "Tallon Overworld") {
                                    tallon_overworld[weight.i] = weight->toInt();
                                } else if(zone.key() == "Chozo Ruins") {
                                    chozo_ruins[weight.i] = weight->toInt();
                                } else if(zone.key() == "Magmoor Caverns") {
                                    magmoor_caverns[weight.i] = weight->toInt();
                                } else if(zone.key() == "Phendrana Drifts") {
                                    phendrana_drifts[weight.i] = weight->toInt();
                                } else if(zone.key() == "Phazon Mines") {
                                    phazon_mines[weight.i] = weight->toInt();
                                } else {
                                    return -3;
                                }
                            } else {
                                return -2;
                            }
                        }
                    } else {
                        return -2;
                    }
                }
            } else {
                return -2;
            }
        break;
    }
    return 0;
}

int PatcherSettings::deserialize(const QJsonObject &json) {
    if(json.contains("patch_settings") && json["patch_settings"].isObject()) {
        QJsonObject obj = json["patch_settings"].toObject();
        if(obj.contains("fix_flaaghra_music") && obj["fix_flaaghra_music"].isBool()) {
            fix_flaaghra_music = obj["fix_flaaghra_music"].toBool();
            if(fix_flaaghra_music) {
                if(obj.contains("trilogy_iso") && obj["trilogy_iso"].isString())
                    trilogy_iso = obj["trilogy_iso"].toString();
                else
                  trilogy_iso = "";
            } else
                trilogy_iso = "";
        } else {
            fix_flaaghra_music = false;
            trilogy_iso = "";
        }
        if(obj.contains("skip_frigate") && obj["skip_frigate"].isBool() &&
           obj.contains("skip_crater") && obj["skip_crater"].isBool() &&
           obj.contains("skip_hudmemos") && obj["skip_hudmemos"].isBool() &&
           obj.contains("stagger_suit_damage") && obj["stagger_suit_damage"].isBool() &&
           obj.contains("varia_heat_protection") && obj["varia_heat_protection"].isBool())
        {
            skip_frigate = obj["skip_frigate"].toBool();
            skip_crater = obj["skip_crater"].toBool();
            skip_hudmemos = obj["skip_crater"].toBool();
            stagger_suit_damage = obj["stagger_suit_damage"].toBool();
            varia_heat_protection = obj["varia_heat_protection"].toBool();
            if(obj.contains("powerbomb_lockpick") && obj["powerbomb_lockpick"].isBool())
                powerbomb_lockpick = obj["powerbomb_lockpick"].toBool();
            else
                powerbomb_lockpick = false;
            return 0;
        } else
            return -2;
    } else
        return -2;
}

void Preset::initialize_doors() {
    excluded_doors = vector<RoomDoorList>();
    untouched_doors = vector<RoomDoorList>();

    RoomDoorList tallon;
    RoomDoorList chozo;
    RoomDoorList magmoor;
    RoomDoorList phendrana;
    RoomDoorList mines;

    chozo["Transport to Tallon Overworld North"] = {false};
    chozo["Ruins Entrance"] = {false,false};
    chozo["Main Plaza"] = {false,false,false,false,true,true};
    chozo["Ruined Fountain Access"] = {false,false};
    chozo["Ruined Shrine Access"] = {false,false};
    chozo["Nursery Access"] = {false,false};
    chozo["Plaza Access"] = {false,false};
    chozo["Ruined Fountain"] = {false,false,false};
    chozo["Ruined Shrine"] = {false,false};
    chozo["Eyon Tunnel"] = {false,false};
    chozo["Vault"] = {false,false};
    chozo["Training Chamber"] = {false,true};
    chozo["Arboretum Access"] = {false,false};
    chozo["Meditation Fountain"] = {false,false};
    chozo["Tower of Light Access"] = {false,false};
    chozo["Ruined Nursery"] = {false,false,false};
    chozo["Vault Access"] = {false,false};
    chozo["Training Chamber Access"] = {false,false};
    chozo["Arboretum"] = {false,false,false};
    chozo["Magma Pool"] = {false,false};
    chozo["Tower of Light"] = {false,true};
    chozo["Save Station 1"] = {false};
    chozo["North Atrium"] = {false,false};
    chozo["Transport to Magmoor Caverns North"] = {false,false,false};
    chozo["Sunchamber Lobby"] = {false,false};
    chozo["Gathering Hall Access"] = {false,false};
    chozo["Tower Chamber"] = {true};
    chozo["Ruined Gallery"] = {false,false,false};
    chozo["Sun Tower"] = {false,false};
    chozo["Transport Access North"] = {false,false};
    chozo["Sunchamber Access"] = {false,false};
    chozo["Gathering Hall"] = {false,false,false,false};
    chozo["Totem Access"] = {false,false};
    chozo["Map Station"] = {false};
    chozo["Sun Tower Access"] = {false,false};
    chozo["Hive Totem"] = {false,false};
    chozo["Sunchamber"] = {false,false};
    chozo["Watery Hall Access"] = {false,false};
    chozo["Save Station 2"] = {false};
    chozo["East Atrium"] = {false,false};
    chozo["Watery Hall"] = {false,false};
    chozo["Energy Core Access"] = {false,false};
    chozo["Dynamo Access"] = {false,false};
    chozo["Energy Core"] = {true,false,false};
    chozo["Dynamo"] = {false};
    chozo["Burn Dome Access"] = {false,true};
    chozo["West Furnace Access"] = {false,false};
    chozo["Burn Dome"] = {false};
    chozo["Furnace"] = {false,false,true};
    chozo["East Furnace Access"] = {false,false};
    chozo["Crossway Access West"] = {false,true};
    chozo["Hall of the Elders"] = {false,false,false,false,true};
    chozo["Crossway"] = {false,false,false};
    chozo["Reflecting Pool Access"] = {false,false};
    chozo["Elder Hall Access"] = {false,false};
    chozo["Crossway Access South"] = {false,false};
    chozo["Elder Chamber"] = {true};
    chozo["Reflecting Pool"] = {false,false,false,false};
    chozo["Save Station 3"] = {false,false};
    chozo["Transport Access South"] = {false,false};
    chozo["Antechamber"] = {false};
    chozo["Transport to Tallon Overworld East"] = {false};
    chozo["Transport to Tallon Overworld South"] = {false};
    chozo["Piston Hall"] = {true,true};
    phendrana["Transport to Magmoor Caverns West"] = {false};
    phendrana["Shoreline Entrance"] = {false,false};
    phendrana["Phendrana Shorelines"] = {false,false,false,false,false,false};
    phendrana["Temple Entryway"] = {false,false};
    phendrana["Save Station B"] = {false};
    phendrana["Ruins Entryway"] = {false,false};
    phendrana["Plaza Walkway"] = {false,false};
    phendrana["Ice Ruins Access"] = {false,false};
    phendrana["Chozo Ice Temple"] = {false,false};
    phendrana["Ice Ruins West"] = {false,false,false};
    phendrana["Ice Ruins East"] = {false,false};
    phendrana["Chapel Tunnel"] = {false,false};
    phendrana["Courtyard Entryway"] = {false,false};
    phendrana["Canyon Entryway"] = {false,false};
    phendrana["Chapel of the Elders"] = {false};
    phendrana["Ruined Courtyard"] = {false,false,false,false};
    phendrana["Phendrana Canyon"] = {false};
    phendrana["Save Station A"] = {false};
    phendrana["Specimen Storage"] = {false,false};
    phendrana["Quarantine Access"] = {false,false};
    phendrana["Research Entrance"] = {false,false,false};
    phendrana["North Quarantine Tunnel"] = {false,false};
    phendrana["Map Station"] = {false};
    phendrana["Hydra Lab Entryway"] = {false,false};
    phendrana["Quarantine Cave"] = {false,false,true};
    phendrana["Research Lab Hydra"] = {false,true};
    phendrana["South Quarantine Tunnel"] = {false,false};
    phendrana["Quarantine Monitor"] = {true};
    phendrana["Observatory Access"] = {false,true};
    phendrana["Transport to Magmoor Caverns South"] = {false,false};
    phendrana["Observatory"] = {false,false,false};
    phendrana["Transport Access"] = {false,false};
    phendrana["West Tower Entrance"] = {false,false};
    phendrana["Save Station D"] = {false};
    phendrana["Frozen Pike"] = {false,false,false,false};
    phendrana["West Tower "] = {false,false}; //todo: Door_index in all of these is 0?
    phendrana["Pike Access"] = {false,false};
    phendrana["Frost Cave Access"] = {false,false};
    phendrana["Hunter Cave Access"] = {false,false};
    phendrana["Control Tower"] = {false,false};
    phendrana["Research Core"] = {false,false};
    phendrana["Frost Cave"] = {false,false,false};
    phendrana["Hunter Cave"] = {false,false,false,false};
    phendrana["East Tower"] = {false,false};
    phendrana["Research Core Access"] = {false,true};
    phendrana["Save Station C"] = {false};
    phendrana["Upper Edge Tunnel"] = {false,false};
    phendrana["Lower Edge Tunnel"] = {false,false};
    phendrana["Chamber Access"] = {false,false};
    phendrana["Lake Tunnel"] = {false,false};
    phendrana["Aether Lab Entryway"] = {false,false};
    phendrana["Research Lab Aether"] = {false,true};
    phendrana["Phendrana's Edge"] = {false,false,false,true};
    phendrana["Gravity Chamber"] = {false,false};
    phendrana["Storage Cave"] = {false};
    phendrana["Security Cave"] = {true};
    tallon["Landing Site"] = {false,false,false,false,false};
    tallon["Gully"] = {false,false};
    tallon["Canyon Cavern"] = {false,false};
    tallon["Temple Hall"] = {false,false};
    tallon["Alcove"] = {false};
    tallon["Waterfall Cavern"] = {false,false};
    tallon["Tallon Canyon"] = {false,false,false,false};
    tallon["Temple Security Station"] = {false,false};
    tallon["Frigate Crash Site"] = {false,false,false};
    tallon["Transport Tunnel A"] = {false,false};
    tallon["Root Tunnel"] = {false,false};
    tallon["Temple Lobby"] = {false,false};
    tallon["Frigate Access Tunnel"] = {false,false};
    tallon["Overgrown Cavern"] = {false,false};
    tallon["Transport to Chozo Ruins West"] = {false};
    tallon["Root Cave"] = {false,false,false};
    tallon["Artifact Temple"] = {false};
    tallon["Main Ventilation Shaft Section C"] = {false,false};
    tallon["Main Ventilation Shaft Section B"] = {false,false,false,false};
    tallon["Main Ventilation Shaft Section A"] = {false,false};
    tallon["Transport Tunnel C"] = {false,false};
    tallon["Transport Tunnel B"] = {false,false};
    tallon["Arbor Chamber"] = {false};
    tallon["Transport to Chozo Ruins East"] = {false};
    tallon["Transport to Magmoor Caverns East"] = {false};
    tallon["Reactor Core"] = {true,false};
    tallon["Reactor Access"] = {true,true,false};
    tallon["Cargo Freight Lift to Deck Gamma"] = {true,false};
    tallon["Savestation"] = {false};
    tallon["Deck Beta Transit Hall"] = {false,false};
    tallon["Biohazard Containment"] = {false,false};
    tallon["Deck Beta Security Hall"] = {false,false};
    tallon["Biotech Research Area 1"] = {false,false};
    tallon["Deck Beta Conduit Hall"] = {false,false};
    tallon["Connection Elevator to Deck Beta"] = {false,false};
    tallon["Hydro Access Tunnel"] = {false,false};
    tallon["Great Tree Hall"] = {false,false,false,false,false};
    tallon["Great Tree Chamber"] = {false};
    tallon["Transport Tunnel D"] = {false,false};
    tallon["Life Grove Tunnel"] = {false,true};
    tallon["Transport Tunnel E"] = {false,false};
    tallon["Transport to Chozo Ruins South"] = {false};
    tallon["Life Grove"] = {true};
    tallon["Transport to Phazon Mines East"] = {false};
    mines["Transport to Tallon Overworld South"] = {false};
    mines["Quarry Access"] = {false,false};
    mines["Main Quarry"] = {false,false,false,false};
    mines["Waste Disposal"] = {false,false};
    mines["Save Station Mines A"] = {false};
    mines["Security Access A"] = {false,false};
    mines["Ore Processing"] = {false,false,false,false};
    mines["Mine Security Station"] = {false,true,false};
    mines["Research Access"] = {false,false};
    mines["Storage Depot B"] = {false};
    mines["Elevator Access A"] = {false,false};
    mines["Storage Depot A"] = {false};
    mines["Security Access B"] = {true,true};
    mines["Elite Research"] = {false,true};
    mines["Elevator A"] = {false,false};
    mines["Elite Control Access"] = {false,false};
    mines["Elite Control"] = {false,false,true};
    mines["Maintenance Tunnel"] = {false,false};
    mines["Ventilation Shaft"] = {false,true};
    mines["Phazon Processing Center"] = {false,false,false};
    mines["Omega Research"] = {false,false,true};
    mines["Transport Access"] = {false,false};
    mines["Processing Center Access"] = {false,false};
    mines["Map Station Mines"] = {false};
    mines["Dynamo Access"] = {false,true};
    mines["Transport to Magmoor Caverns South"] = {false};
    mines["Elite Quarters"] = {false,false};
    mines["Central Dynamo"] = {false,false,false};
    mines["Elite Quarters Access"] = {false,false};
    mines["Quarantine Access A"] = {false,false};
    mines["Save Station Mines B"] = {false};
    mines["Metroid Quarantine B"] = {false,false,false};
    mines["Metroid Quarantine A"] = {false,false};
    mines["Quarantine Access B"] = {false,false};
    mines["Save Station Mines C"] = {false};
    mines["Elevator Access B"] = {false,false};
    mines["Fungal Hall B"] = {false,false,false};
    mines["Elevator B"] = {false,false};
    mines["Missile Station Mines"] = {false};
    mines["Phazon Mining Tunnel"] = {false,false};
    mines["Fungal Hall Access"] = {false,false};
    mines["Fungal Hall A"] = {false,false};
    magmoor["Transport to Chozo Ruins North"] = {false};
    magmoor["Burning Trail"] = {false,false,false};
    magmoor["Lake Tunnel"] = {false,false};
    magmoor["Save Station Magmoor A"] = {false};
    magmoor["Lava Lake"] = {false,false};
    magmoor["Pit Tunnel"] = {false,false};
    magmoor["Triclops Pit"] = {false,false,false};
    magmoor["Monitor Tunnel"] = {false,false};
    magmoor["Storage Cavern"] = {false};
    magmoor["Monitor Station"] = {false,false,false,false};
    magmoor["Transport Tunnel A"] = {false,false};
    magmoor["Warrior Shrine"] = {false,true};
    magmoor["Shore Tunnel"] = {false,false};
    magmoor["Transport to Phendrana Drifts North"] = {false};
    magmoor["Fiery Shores"] = {false,false,true};
    magmoor["Transport Tunnel B"] = {false,false};
    magmoor["Transport to Tallon Overworld West"] = {false,false};
    magmoor["Twin Fires Tunnel"] = {false,false};
    magmoor["Twin Fires"] = {false,false};
    magmoor["North Core Tunnel"] = {false,false};
    magmoor["Geothermal Core"] = {false,false,false};
    magmoor["Plasma Processing"] = {false};
    magmoor["South Core Tunnel"] = {false,false};
    magmoor["Magmoor Workstation"] = {false,false,false};
    magmoor["Workstation Tunnel"] = {false,false};
    magmoor["Transport Tunnel C"] = {false,false};
    magmoor["Transport to Phazon Mines West"] = {false};
    magmoor["Transport to Phendrana Drifts South"] = {false,false};
    magmoor["Save Station Magmoor B"] = {false};


    excluded_doors.push_back(tallon);
    excluded_doors.push_back(chozo);
    excluded_doors.push_back(magmoor);
    excluded_doors.push_back(phendrana);
    excluded_doors.push_back(mines);

    untouched_doors.push_back(tallon);
    untouched_doors.push_back(chozo);
    untouched_doors.push_back(magmoor);
    untouched_doors.push_back(phendrana);
    untouched_doors.push_back(mines);
}
