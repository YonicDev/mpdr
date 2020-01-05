#include "randomizer.h"

#include <QJsonDocument>
#include <thread>
#include <QDebug>

typedef void(*callback_t)(void *,const char *);
extern "C" void randomprime_patch_iso(const char *config, void *cb_data, callback_t cb);

template<typename T>
void call_wrapper(void *ptr, const char*data) {
    (*static_cast<T*>(ptr))(data);
}

void Randomizer::randomize(Preset *config) {
    QJsonObject obj;
    config->serialize(obj);

    QJsonDocument doc(obj);

    QByteArray ba = doc.toJson(QJsonDocument::Indented);

    char *parsed_config = new char[ba.size()+1];
    strcpy(parsed_config,ba.data());

    qDebug() << parsed_config;

    Randomizer *worker = this;

    auto prime_callback = [worker](const char* data) {
        QJsonDocument data_json = QJsonDocument::fromJson(QString(data).toUtf8());
        QJsonObject data_obj = data_json.object();

        worker->emit send_message(data_obj);
    };

    randomprime_patch_iso(parsed_config,(void *)&prime_callback,call_wrapper<decltype(prime_callback)>);
    parsed_config[0] = '\0';
    delete[] parsed_config;

}
