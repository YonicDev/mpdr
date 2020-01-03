#ifndef RANDOMIZERGUI_H
#define RANDOMIZERGUI_H

#include <QMainWindow>

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

public slots:
    void set_starting_pickups(uint64_t);

private slots:

    void on_pickupsButton_clicked();

    void on_buttonBrowseInput_clicked();

    void on_buttonBrowseOutput_clicked();

    void on_buttonPatches_clicked();

    void on_buttonGenerateSeed_clicked();

    void on_buttonUseTime_clicked();

    void on_actionOpen_preset_triggered();

    void on_actionSave_preset_triggered();

private:
    Ui::RandomizerGUI *ui;
    Preset *preset;

    void load_from_preset();
    void save_to_preset();
};
#endif // RANDOMIZERGUI_H
