#ifndef DOCDIALOG_H
#define DOCDIALOG_H

#include <QDialog>
#include <QHelpEngine>
#include <QCoreApplication>

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

    #if defined(WIN32) || defined(Q_OS_MACOS)
    const QString dir_path = QCoreApplication::applicationDirPath();
    #else
    const QString local_path = QCoreApplication::applicationDirPath();
    const QString appimage_path = QCoreApplication::applicationDirPath() + "/../share/mpdr";
    const QString install_path = "/usr/share/mpdr";
    QString used_path;
    #endif

    void homePage();
    void loadDocs(bool critical,QString window_text);
};

#endif // DOCDIALOG_H
