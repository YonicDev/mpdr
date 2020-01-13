#ifndef RANDOMIZERGUI_H
#define RANDOMIZERGUI_H

#include <QMainWindow>
#include <QThread>
#include <QJsonObject>
#ifdef WIN32
#include <QtWinExtras/QWinTaskbarButton>
#include <QtWinExtras/QWinTaskbarProgress>
#include <QShowEvent>
#endif

#include "../Config/preset.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RandomizerGUI; }
QT_END_NAMESPACE

class RandomizerGUI : public QMainWindow
{
    Q_OBJECT

public:
    RandomizerGUI(QWidget *parent = nullptr);
    ~RandomizerGUI();
    void log(QString type,QString message);
    void set_progress(int percentage);
    void setup_thread();

public slots:
    void process_message(QJsonObject data);

private slots:

    void on_pickupsButton_clicked();

    void on_buttonBrowseInput_clicked();

    void on_buttonBrowseOutput_clicked();

    void on_buttonPatches_clicked();

    void on_buttonGenerateSeed_clicked();

    void on_buttonUseTime_clicked();

    void on_actionOpen_preset_triggered();

    void on_actionSave_preset_triggered();

    void on_buttonRandomize_clicked();

    void on_actionAbout_MPDR_triggered();

    void on_actionContents_triggered();

    void on_buttonDoors_clicked();

private:
    Ui::RandomizerGUI *ui;
    Preset *preset;

    QThread worker_thread;
    #ifdef WIN32
    QWinTaskbarButton *taskbar_button;
    QWinTaskbarProgress *taskbar_progress;
    #endif

    void load_from_preset();
    void save_to_preset();

#ifdef WIN32
protected:
    void showEvent(QShowEvent *event) override;
#endif

signals:
    void run_thread(Preset *preset);
};
#endif // RANDOMIZERGUI_H
