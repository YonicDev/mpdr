#include "docdialog.h"
#include "ui_docdialog.h"

#include <QHelpContentWidget>
#include <QHelpIndexWidget>

DocDialog::DocDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocDialog)
{

    engine = new QHelpEngine(QCoreApplication::applicationDirPath() + "/doc/doc_collection_en.qhc",this);
    engine->setupData();

    ui->setupUi(this);

    ui->tabber->addTab(engine->contentWidget(),tr("Content"));
    ui->tabber->addTab(engine->indexWidget(),tr("Index"));

    connect(engine->contentWidget(),SIGNAL(linkActivated(QUrl)),this,SLOT(processUrl(QUrl)));

    ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/0.2/en/index.html"));
}

DocDialog::~DocDialog()
{
    delete ui;
}

void DocDialog::processUrl(QUrl url) {
    QString unprocessed_url = url.toString();
    QString domain_name = "qthelp://"+engine->namespaceName(QCoreApplication::applicationDirPath()+ "/doc/doc_collection_en.qhc");
    unprocessed_url.insert(unprocessed_url.lastIndexOf("/doc/")+5,domain_name.right(3)+"/");
    QString processed_url = "file:///"+unprocessed_url.replace(0,domain_name.length(),QCoreApplication::applicationDirPath());
    ui->browser->setUrl(QUrl(processed_url));
}

