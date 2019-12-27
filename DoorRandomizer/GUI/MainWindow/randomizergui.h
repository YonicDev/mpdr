#ifndef RANDOMIZERGUI_H
#define RANDOMIZERGUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class RandomizerGUI; }
QT_END_NAMESPACE

class RandomizerGUI : public QMainWindow
{
    Q_OBJECT

public:
    RandomizerGUI(QWidget *parent = nullptr);
    ~RandomizerGUI();

private slots:

    void on_pickupsButton_clicked();

    void on_buttonBrowseInput_clicked();

    void on_buttonBrowseOutput_clicked();

    void on_buttonPatches_clicked();

private:
    Ui::RandomizerGUI *ui;
};
#endif // RANDOMIZERGUI_H
