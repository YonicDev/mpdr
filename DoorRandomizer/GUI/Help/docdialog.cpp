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

    ui->versionSelector->addItems(engine->customFilters());
    ui->versionSelector->setCurrentText(engine->currentFilter());
    connect(ui->versionSelector,SIGNAL(currentTextChanged(QString)),this,SLOT(changeVersion(QString)));

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

    int version_number_index = engine->currentFilter().length() - engine->currentFilter().lastIndexOf("v")-1;
    QString version_number = engine->currentFilter().right(version_number_index);

    unprocessed_url.insert(unprocessed_url.lastIndexOf("/doc/")+5,version_number+"/");
    QString processed_url = "file:///"+unprocessed_url.replace(0,domain_name.length(),QCoreApplication::applicationDirPath());
    ui->browser->setUrl(QUrl(processed_url));
}

void DocDialog::changeVersion(QString new_ver) {
    engine->setCurrentFilter(new_ver);

    int version_number_index = new_ver.length() - new_ver.lastIndexOf("v")-1;
    QString version_number = new_ver.right(version_number_index);

    if(version_number == "0.1")
        ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/"+version_number+"/index.html"));
    else
        ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/"+version_number+"/en/index.html"));
}

