#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <QThread>
#include <QJsonObject>

#include <preset.h>
#include <randomizergui.h>



class Randomizer : public QObject
{
    Q_OBJECT

public slots:
    void randomize(Preset *config);

signals:
    void send_message(QJsonObject data);
};

#endif // RANDOMIZER_H
