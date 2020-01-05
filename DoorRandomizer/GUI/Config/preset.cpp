#include "preset.h"

#include "time.h"

#include <QJsonArray>

Preset::Preset() {
    for(int i=0;i<5;++i) {
        for(int j=0;j<4;++j) {
            weights[i][j] = 25;
        }
    }
    input_iso = "";
    output_iso = "";
    seed = static_cast<int32_t>(time(nullptr));
    starting_pickups = 0;
    additional_settings.skip_frigate = true;
    additional_settings.skip_crater = false;
    additional_settings.skip_hudmemos = false;
    additional_settings.fix_flaaghra_music = false;
    additional_settings.trilogy_iso = "";
    additional_settings.stagger_suit_damage = false;
    additional_settings.varia_heat_protection = false;
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
}

int Preset::deserialize(const QJsonObject &json) {
    if(json.contains("mpdrp_version") && json["mpdrp_version"].isDouble()) {
        if(json["mpdrp_version"].toInt() == 2) {
            if(json.contains("seed")&&json["seed"].isDouble())
                seed = static_cast<int32_t>(json["seed"].toInt());
            else
                return -2;
            int check = weights.deserialize(json);
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
            return check;
        } else if(json["mpdrp_version"].toInt() == 1) {
            //TODO: Handle backwards compatibility
            return -4;
        }
        return 0;
    } else {return -1;}
}

int DoorWeights::deserialize(const QJsonObject &json) {
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
        return 0;
    } else
        return -2;
}

int PatcherSettings::deserialize(const QJsonObject &json) {
    if(json.contains("patch_settings") && json["patch_settings"].isObject()) {
        QJsonObject obj = json["patch_settings"].toObject();
        if(obj.contains("fix_flaaghra_music") && obj["fix_flaaghra_music"].isBool()) {
            fix_flaaghra_music = obj["fix_flaaghra_music"].toBool();
            if(fix_flaaghra_music) {
                if(json.contains("trilogy_iso") && obj["trilogy_iso"].isString())
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

            return 0;
        } else
            return -2;
    } else
        return -2;
}
