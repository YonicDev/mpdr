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

    loadDocs(false,
             //: %1 = Already translated system language name.
             tr("No documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));

    #if FALLBACK_DOCUMENTATION
    if(engine->currentFilter() != "MPDR v"+LATEST_VERSION) {
        QMessageBox::warning(this,tr("MPDR Documentation"),
            //: %1 = Already translated system language name.
            tr("The latest version of the documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
        language="en";
        loadDocs(true,tr("The English documentation could not be loaded!"));
    }
    #endif

    ui->setupUi(this);

    ui->versionSelector->addItems(engine->customFilters());
    ui->versionSelector->setCurrentText(engine->currentFilter());
    connect(ui->versionSelector,SIGNAL(currentTextChanged(QString)),this,SLOT(changeVersion(QString)));

    ui->tabber->addTab(engine->contentWidget(),tr("Content","table-of-contents"));
    ui->tabber->addTab(engine->indexWidget(),tr("Index","keyword-index"));
    connect(engine->contentWidget(),SIGNAL(linkActivated(QUrl)),this,SLOT(processUrl(QUrl)));
    connect(engine->indexWidget(),SIGNAL(linkActivated(QUrl,QString)),this,SLOT(processUrl(QUrl)));
    connect(ui->browser,SIGNAL(urlChanged(QUrl)),this,SLOT(onBrowserRefresh()));

    homePage();
}

DocDialog::~DocDialog()
{
    delete ui;
}

void DocDialog::loadDocs(bool critical,QString window_text) {
    #ifdef WIN32
    QString doc_path = QCoreApplication::applicationDirPath() + "/doc/doc_collection_"+language+".qhc";
    #else
    QString doc_path = local_path + "/doc/doc_collection_"+language+".qhc";
    #endif

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
    qDebug() << "Trying to load doc from local files at" << doc_path;
    if(trans_check.exists() && trans_check.isFile()) {
        engine = new QHelpEngine(doc_path,this);
        used_path = local_path;
        qDebug() << "Docs found in local files.";
    } else {
        doc_path = appimage_path + "/doc/doc_collection_"+language+".qhc";
        trans_check = QFileInfo(doc_path);
        qDebug() << "Not found in local files. Trying to load doc from AppImage at" << doc_path;
        if(trans_check.exists() && trans_check.isFile()) {
           used_path = appimage_path;
           engine = new QHelpEngine(doc_path,this);
           qDebug() << "Docs found in AppImage";
        } else {
            doc_path = install_path + "/doc/doc_collection_"+language+".qhc";
            trans_check = QFileInfo(doc_path);
            qDebug() << "Not found in AppImage. Trying to load doc from system files at" << doc_path;
            if(trans_check.exists() && trans_check.isFile()) {
                used_path = install_path;
                engine = new QHelpEngine(doc_path,this);
                trans_check = QFileInfo(doc_path);
                qDebug() << "Docs found in system images";
            } else {
                if(critical) {
                    QMessageBox::critical(this,tr("MPDR Documentation"),window_text);
                    delete this;
                } else {
                    QMessageBox::warning(this,tr("MPDR Documentation"),window_text);
                    language = "en";
                    loadDocs(true,tr("The English documentation could not be loaded!"));
                }
            }
        }
    }
    #endif
    if(engine!=nullptr) {
        engine->setupData();
    }
}

void DocDialog::processUrl(QUrl url) {

    // Target url: file:///path/to/doc/#{version: taken from filter}/#{revision: taken from namespace}/#{language}/file.html

    QString unprocessed_url = url.toString();

    #if WIN32
        QString processed_url = "file:///";
        QString target = QCoreApplication::applicationDirPath();
    #else
        QString processed_url = "file://";
    #endif

    int version_number_index = engine->currentFilter().length() - engine->currentFilter().lastIndexOf("v")-1;
    QString version_number = engine->currentFilter().right(version_number_index);

    if(version_number!="0.1") {
        QRegularExpression revision_regex("/doc/([0-9]\\.[0-9](?:\\.[0-9])?)");
        QString revision_number = revision_regex.match(unprocessed_url).captured(1);

        QRegularExpression file_regex("/doc/[0-9]\\.[0-9]\\.?[0-9]?/[a-z]{2}/(\\S+)");
        QString filename = file_regex.match(unprocessed_url).captured(1);
        processed_url += used_path + "/doc/" + version_number + "/" + revision_number + "/" + language + "/" + filename;
    } else {
        QRegularExpression file_regex("/doc/(\\S+)");
        QString filename = file_regex.match(unprocessed_url).captured(1);
        processed_url += used_path + "/doc/" + version_number + "/" + filename;
    }

    qDebug() << processed_url;

    ui->browser->setUrl(QUrl(processed_url));
    onBrowserRefresh();
}

void DocDialog::changeVersion(QString new_ver) {
    engine->setCurrentFilter(new_ver);

    int version_number_index = new_ver.length() - new_ver.lastIndexOf("v")-1;
    QString version_number = new_ver.right(version_number_index);

    homePage();
}

void DocDialog::homePage() {
    QString filter_text = engine->currentFilter();
    QString version_number = filter_text.right(filter_text.indexOf("v")-2);
    qDebug() << filter_text;
    #if WIN32
        QString schema = "file:///";
        QString target = QCoreApplication::applicationDirPath();
    #else
        QString schema = "file://";
    #endif
    if(version_number == "0.1")
        ui->browser->setUrl(QUrl(schema+used_path+"/doc/"+version_number+"/index.html"));
    else
        ui->browser->setUrl(QUrl(schema+used_path+"/doc/"+version_number+"/"+version_number+"/"+language+"/index.html"));
    qDebug() << schema+used_path+"/doc/"+version_number+"/"+version_number+"/"+language+"/index.html";
}


void DocDialog::on_home_button_clicked()
{
    homePage();
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
