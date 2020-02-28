#ifndef DOCDIALOG_H
#define DOCDIALOG_H

#include <QDialog>
#include <QHelpEngine>

#define FALLBACK_DOCUMENTATION 1

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
    QString language;

    #ifdef WIN32
    const QString dir_path = QCoreApplication::applicationDirPath();
    #else
    const QString dir_path = "../share/mpdr";
    const QString install_path = "/usr/share/mpdr";
    QString used_path;
    #endif

    void homePage(QString version_number);
};

#endif // DOCDIALOG_H
