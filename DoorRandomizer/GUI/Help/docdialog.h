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
private:
    Ui::DocDialog *ui;
    QHelpEngine *engine;
};

#endif // DOCDIALOG_H
