#include "docdialog.h"
#include "ui_docdialog.h"

#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QWebEngineHistory>
#include <QFileInfo>

DocDialog::DocDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocDialog)
{

    language = QLocale::system().name();
    language.truncate(language.lastIndexOf("_"));

    QString doc_path = QCoreApplication::applicationDirPath() + "/doc/doc_collection_"+language+".qhc";

    QFileInfo trans_check(doc_path);
    if(trans_check.exists() && trans_check.isFile())
        engine = new QHelpEngine(doc_path,this);
    else {
        language = "en";
        engine = new QHelpEngine(QCoreApplication::applicationDirPath() + "/doc/doc_collection_en.qhc",this);
    }
    engine->setupData();

    ui->setupUi(this);

    ui->versionSelector->addItems(engine->customFilters());
    ui->versionSelector->setCurrentText(engine->currentFilter());
    connect(ui->versionSelector,SIGNAL(currentTextChanged(QString)),this,SLOT(changeVersion(QString)));

    ui->tabber->addTab(engine->contentWidget(),tr("Content"));
    ui->tabber->addTab(engine->indexWidget(),tr("Index"));
    connect(engine->contentWidget(),SIGNAL(linkActivated(QUrl)),this,SLOT(processUrl(QUrl)));

    ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/0.2/"+language+"/index.html"));
    connect(ui->browser,SIGNAL(urlChanged(QUrl)),this,SLOT(onBrowserRefresh()));
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
    onBrowserRefresh();
}

void DocDialog::changeVersion(QString new_ver) {
    engine->setCurrentFilter(new_ver);

    int version_number_index = new_ver.length() - new_ver.lastIndexOf("v")-1;
    QString version_number = new_ver.right(version_number_index);

    homePage(version_number);
}

void DocDialog::homePage(QString version_number) {
    if(version_number == "0.1")
        ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/"+version_number+"/index.html"));
    else
        ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/"+version_number+"/"+language+"/index.html"));
}


void DocDialog::on_home_button_clicked()
{
    int version_number_index = engine->currentFilter().length() - engine->currentFilter().lastIndexOf("v")-1;
    QString version_number = engine->currentFilter().right(version_number_index);

    homePage(version_number);
}

void DocDialog::onBrowserRefresh() {
    ui->previous_button->setEnabled(ui->browser->history()->canGoBack());
    ui->forward_button->setEnabled(ui->browser->history()->canGoForward());
}

void DocDialog::on_previous_button_clicked()
{
    ui->browser->back();
}

void DocDialog::on_forward_button_clicked()
{
    ui->browser->forward();
}
