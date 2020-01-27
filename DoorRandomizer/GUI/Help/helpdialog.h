#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QHBoxLayout>
#include <QHelpEngine>

class HelpDialog : public QDialog
{
    Q_OBJECT
public:
    HelpDialog(QWidget *parent);
    ~HelpDialog();
private slots:
    void processUrl(QUrl);
private:
    QTabWidget *tabber;
    QWebEngineView *browser;
    QHBoxLayout *layout;
    QHelpEngine *engine;

    void setupUi();
};

#endif // HELPDIALOG_H
