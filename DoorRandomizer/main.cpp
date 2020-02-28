#include "randomizergui.h"

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator app_translator,qt_translator;

    QString qt_translations_path = QCoreApplication::applicationDirPath().append("/translations/");
    QString qt_trans_system_path = QLibraryInfo::location(QLibraryInfo::TranslationsPath).append("/");

    QString locale = QLocale::system().name();
    locale.truncate(locale.lastIndexOf("_"));

    #ifdef WIN32
    QString languagepacks_path = QCoreApplication::applicationDirPath().append("/LanguagePacks/");
    #else
    QString languagepacks_syspath = "/usr/share/mpdr/LanguagePacks/";
    QString languagepacks_path = QCoreApplication::applicationDirPath().append("/LanguagePacks/");
    #endif
    QString translation_file = QString("qtbase_%1.qm").arg(locale);
    QString alt_translation_file = QString("qt_%1.qm").arg(locale);

    if(qt_translator.load(qt_translations_path + translation_file)) {
        a.installTranslator(&qt_translator);
    } else if(qt_translator.load(qt_trans_system_path + translation_file)) {
        a.installTranslator(&qt_translator);
    } else if(qt_translator.load(qt_trans_system_path + alt_translation_file)) {
        a.installTranslator(&qt_translator);
    }
    #ifdef WIN32
    if(app_translator.load(languagepacks_path + QString("mpdr_%1.qm").arg(locale))) {
        a.installTranslator(&app_translator);
    }
    #else
    if(app_translator.load(languagepacks_syspath + QString("mpdr_%1.qm").arg(locale))) {
        a.installTranslator(&app_translator);
        qDebug() << "Translation files found on system.";
    } else if(app_translator.load(languagepacks_path + QString("mpdr_%1.qm").arg(locale))) {
        a.installTranslator(&app_translator);
        qDebug() << "Translation files found on build files.";
    }
    #endif
    RandomizerGUI w;
    w.show();
    return a.exec();
}
