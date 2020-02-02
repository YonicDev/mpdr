#include "docdialog.h"
#include "ui_docdialog.h"

#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QWebEngineHistory>
#include <QFileInfo>
#include <QMessageBox>

DocDialog::DocDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocDialog)
{

    const QString LATEST_VERSION = "0.2";

    language = QLocale::system().name();
    language.truncate(language.lastIndexOf("_"));

    QString doc_path = QCoreApplication::applicationDirPath() + "/doc/doc_collection_"+language+".qhc";

    QFileInfo trans_check(doc_path);
    if(trans_check.exists() && trans_check.isFile())
        engine = new QHelpEngine(doc_path,this);
    else {
        language = "en";
        engine = new QHelpEngine(QCoreApplication::applicationDirPath() + "/doc/doc_collection_en.qhc",this);
        QMessageBox::warning(this,tr("MPDR Documentation"),
            //: %1 = Already translated system language name.
            tr("No documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
    }

    #if FALLBACK_DOCUMENTATION
    // Reload the engine using english if docs for latest version aren't available
    if(engine->currentFilter() != "MPDR v"+LATEST_VERSION) {
        language = "en";
        engine = new QHelpEngine(QCoreApplication::applicationDirPath() + "/doc/doc_collection_en.qhc",this);
        QMessageBox::warning(this,tr("MPDR Documentation"),
            //: %1 = Already translated system language name.
            tr("The latest version of the documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
        engine->setupData();
    }
    #endif

    ui->setupUi(this);

    ui->versionSelector->addItems(engine->customFilters());
    ui->versionSelector->setCurrentText(engine->currentFilter());
    connect(ui->versionSelector,SIGNAL(currentTextChanged(QString)),this,SLOT(changeVersion(QString)));

    ui->tabber->addTab(engine->contentWidget(),tr("Content","table-of-contents"));
    ui->tabber->addTab(engine->indexWidget(),tr("Index","keyword-index"));
    connect(engine->contentWidget(),SIGNAL(linkActivated(QUrl)),this,SLOT(processUrl(QUrl)));

    ui->browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/"+LATEST_VERSION+"/"+LATEST_VERSION+"/"+language+"/index.html"));
    connect(ui->browser,SIGNAL(urlChanged(QUrl)),this,SLOT(onBrowserRefresh()));
}

DocDialog::~DocDialog()
{
    delete ui;
}

void DocDialog::processUrl(QUrl url) {

    // Target url: file:///path/to/doc/#{version: taken from filter}/#{revision: taken from namespace}/#{language}/file.html

    QString unprocessed_url = url.toString();

    #if WIN32
        QString processed_url = "file:///";
    #else
        QString processed_url = "file://"
    #endif

    int version_number_index = engine->currentFilter().length() - engine->currentFilter().lastIndexOf("v")-1;
    QString version_number = engine->currentFilter().right(version_number_index);

    if(version_number!="0.1") {
        QRegularExpression revision_regex("/doc/([0-9]\\.[0-9](?:\\.[0-9])?)");
        QString revision_number = revision_regex.match(unprocessed_url).captured(1);

        QRegularExpression file_regex("/doc/[0-9]\\.[0-9]\\.?[0-9]?/[a-z]{2}/(\\S+)");
        QString filename = file_regex.match(unprocessed_url).captured(1);

        processed_url += QCoreApplication::applicationDirPath() + "/doc/" + version_number + "/" + revision_number + "/" + language + "/" + filename;
    } else {
        QRegularExpression file_regex("/doc/(\\S+)");
        QString filename = file_regex.match(unprocessed_url).captured(1);

        processed_url += QCoreApplication::applicationDirPath() + "/doc/" + version_number + "/" + filename;
    }

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
