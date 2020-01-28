#ifndef DOCDIALOG_H
#define DOCDIALOG_H

#include <QDialog>
#include <QHelpEngine>

namespace Ui {
class DocDialog;
}

class DocDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DocDialog(QWidget *parent = nullptr);
    ~DocDialog();
private slots:
    void processUrl(QUrl);
    void changeVersion(QString);
    void onBrowserRefresh();
    void on_home_button_clicked();

    void on_previous_button_clicked();

    void on_forward_button_clicked();

private:
    Ui::DocDialog *ui;
    QHelpEngine *engine;
    void homePage(QString version_number);
};

#endif // DOCDIALOG_H
