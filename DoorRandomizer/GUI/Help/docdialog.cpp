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

    #ifdef WIN32
    QString doc_path = QCoreApplication::applicationDirPath() + "/doc/doc_collection_"+language+".qhc";
    #else
    QString doc_path = install_path + "/doc/doc_collection_"+language+".qhc";
    #endif
    qDebug() << "Trying to load doc from system files at" << doc_path;

    QFileInfo trans_check(doc_path);
    #ifdef WIN32
    if(trans_check.exists() && trans_check.isFile())
        engine = new QHelpEngine(doc_path,this);
    else {
        language = "en";
        engine = new QHelpEngine(dir_path + "/doc/doc_collection_en.qhc",this);
        QMessageBox::warning(this,tr("MPDR Documentation"),
            //: %1 = Already translated system language name.
            tr("No documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
    }
    #else
    if(trans_check.exists() && trans_check.isFile()) {
        engine = new QHelpEngine(doc_path,this);
        used_path = install_path;
        qDebug() << "Docs found on system files";
    } else {
        doc_path = dir_path + "/doc/doc_collection_"+language+".qhc";
        trans_check = QFileInfo(doc_path);
        qDebug() << "Not found. Trying to load doc from local app files at" << doc_path;
        if(trans_check.exists() && trans_check.isFile()) {
           used_path = dir_path;
           engine = new QHelpEngine(doc_path,this);
           qDebug() << "Docs found on local app files";
        } else {
            QMessageBox::warning(this,tr("MPDR Documentation"),
                //: %1 = Already translated system language name.
                tr("No documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
            language = "en";
            doc_path = install_path + "/doc/doc_collection_"+language+".qhc";
            trans_check = QFileInfo(doc_path);
            qDebug() << "Trying to load doc from system files at" << doc_path;
            if(trans_check.exists() && trans_check.isFile()) {
                used_path = install_path;
                engine = new QHelpEngine(used_path + "/doc/doc_collection_en.qhc",this);
            } else {
                doc_path = dir_path + "/doc/doc_collection_"+language+".qhc";
                trans_check = QFileInfo(doc_path);
                qDebug() << "Not found. Trying to load doc from local app files at" << doc_path;
                if(trans_check.exists() && trans_check.isFile()) {
                    used_path = dir_path;
                    engine = new QHelpEngine(used_path + "/doc/doc_collection_en.qhc",this);
                } else {
                    QMessageBox::critical(this,tr("MPDR Documentation"),tr("The documentation could not be loaded. Make sure the application is installed with make install!"));
                }
            }
        }
    }
    #endif

    #if FALLBACK_DOCUMENTATION
    // Reload the engine using english if docs for latest version aren't available
    #ifdef WIN32
        QString target = QCoreApplication::applicationDirPath();
    #else
        QString target = install_path;
    #endif
    qDebug() << engine->currentFilter();
    if(engine->currentFilter() != "MPDR v"+LATEST_VERSION) {
        language = "en";
        #ifndef WIN32
        qDebug() << "Trying to load English docs at" << target + "/doc/doc_collection_en.qhc";
        trans_check = QFileInfo(target+ "/doc/doc_collection_en.qhc");
        if(!(trans_check.exists() && trans_check.isFile())) {
            qDebug() << "Could not load at install path, trying with " << target + "/doc/doc_collection_en.qhc";
            target = dir_path;
            trans_check = QFileInfo(target+ "/doc/doc_collection_en.qhc");
            if(!(trans_check.exists() && trans_check.isFile())) {
                QMessageBox::critical(this,tr("MPDR Documentation"),tr("The documentation could not be loaded. Make sure the application is installed with make install!"));
            } else {
                used_path = dir_path;
            }
        } else {
            used_path = install_path;
        }
        #endif
        engine = new QHelpEngine(target + "/doc/doc_collection_en.qhc",this);
        #ifndef WIN32
        if(trans_check.exists() && trans_check.isFile()) {
        #endif
        QMessageBox::warning(this,tr("MPDR Documentation"),
            //: %1 = Already translated system language name.
            tr("The latest version of the documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
        #ifndef WIN32
        }
        #endif
        engine->setupData();
    }
    #else
    QString filter_text = engine->currentFilter();
    QString supported_version = filter_text.right(filter_text.indexOf("v")-2);
    #endif

    ui->setupUi(this);

    ui->versionSelector->addItems(engine->customFilters());
    ui->versionSelector->setCurrentText(engine->currentFilter());
    connect(ui->versionSelector,SIGNAL(currentTextChanged(QString)),this,SLOT(changeVersion(QString)));

    ui->tabber->addTab(engine->contentWidget(),tr("Content","table-of-contents"));
    ui->tabber->addTab(engine->indexWidget(),tr("Index","keyword-index"));
    connect(engine->contentWidget(),SIGNAL(linkActivated(QUrl)),this,SLOT(processUrl(QUrl)));
    connect(engine->indexWidget(),SIGNAL(linkActivated(QUrl,QString)),this,SLOT(processUrl(QUrl)));
    #if FALLBACK_DOCUMENTATION
    homePage(LATEST_VERSION);
    #else
    homePage(supported_version);
    #endif
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
        QString target = QCoreApplication::applicationDirPath();
    #else
        QString processed_url = "file://";
        QString target = QCoreApplication::applicationDirPath() + "/../share/mpdr";
    #endif

    int version_number_index = engine->currentFilter().length() - engine->currentFilter().lastIndexOf("v")-1;
    QString version_number = engine->currentFilter().right(version_number_index);

    if(version_number!="0.1") {
        QRegularExpression revision_regex("/doc/([0-9]\\.[0-9](?:\\.[0-9])?)");
        QString revision_number = revision_regex.match(unprocessed_url).captured(1);

        QRegularExpression file_regex("/doc/[0-9]\\.[0-9]\\.?[0-9]?/[a-z]{2}/(\\S+)");
        QString filename = file_regex.match(unprocessed_url).captured(1);
        processed_url += target + "/doc/" + version_number + "/" + revision_number + "/" + language + "/" + filename;
    } else {
        QRegularExpression file_regex("/doc/(\\S+)");
        QString filename = file_regex.match(unprocessed_url).captured(1);
        processed_url += target + "/doc/" + version_number + "/" + filename;
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
    #if WIN32
        QString schema = "file:///";
        QString target = QCoreApplication::applicationDirPath();
    #else
        QString schema = "file://";
        QString target = QCoreApplication::applicationDirPath() + "/../share/mpdr";
    #endif
    if(version_number == "0.1")
        ui->browser->setUrl(QUrl(schema+target+"/doc/"+version_number+"/index.html"));
    else
        ui->browser->setUrl(QUrl(schema+target+"/doc/"+version_number+"/"+version_number+"/"+language+"/index.html"));
    qDebug() << schema+target+"/doc/"+version_number+"/"+version_number+"/"+language+"/index.html";
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
