#include "docdialog.h"
#include "ui_docdialog.h"
#include "global.h"

#include <QHelpContentWidget>
#include <QHelpIndexWidget>
#include <QWebEngineHistory>
#include <QFileInfo>
#include <QMessageBox>

DocDialog::DocDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocDialog)
{

    language = QLocale::system().name();
    language.truncate(language.lastIndexOf("_"));

    loadDocs(false,
             //: %1 = Already translated system language name.
             tr("No documentation for your system language could not be found (%1). The English documentation will be shown.").arg(QLocale::system().nativeLanguageName()));
    if(engine->currentFilter() != QString("MPDR v")+MPDR_VERSION) {
        if(QMessageBox::warning(this,tr("MPDR Documentation"),
            //: %1 = Already translated system language name.
            tr("The documentation for your system language (%1) is outdated.\nWould you like to show the English documentation instead?").arg(QLocale::system().nativeLanguageName()),
            QMessageBox::Yes|QMessageBox::No,QMessageBox::No)) {
            language = "en";
            loadDocs(true,tr("The English documentation could not be loaded!"));
        }
    }

    ui->setupUi(this);
    ui->version_label->setText(engine->currentFilter());

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
    #if defined(Q_OS_WIN)
    QString doc_path = dir_path + "/doc/doc_collection_"+language+".qhc";
    #elif __APPLE__
    QString doc_path = dir_path + "/../Resources/doc/doc_collection_"+language+".qhc";
    #else
    QString doc_path = local_path + "/doc/doc_collection_"+language+".qhc";
    #endif

    QFileInfo trans_check(doc_path);
    qDebug() << doc_path;

    #if defined(Q_OS_WIN)
    if(trans_check.exists() && trans_check.isFile()) {
        engine = new QHelpEngine(doc_path,this);
    } else {
        if(critical) {
            QMessageBox::critical(this,tr("MPDR Documentation"),window_text);
            delete this;
        } else {
            language = "en";
            QMessageBox::warning(this,tr("MPDR Documentation"),window_text);
            loadDocs(true,tr("The English documentation could not be loaded!"));
        }
    }
    #elif defined(__APPLE__)
    qDebug() << "Trying to load doc from app files at" << doc_path;
    if(trans_check.exists() && trans_check.isFile()) {
        engine = new QHelpEngine(doc_path,this);
        qDebug() << "Docs found in app files.";
    } else {
        doc_path = dir_path + "/doc/doc_collection_"+language+".qhc";
        qDebug() << "Not found in local files. Trying to load doc from AppImage at" << doc_path;
        trans_check = QFileInfo(doc_path);
        if(trans_check.exists() && trans_check.isFile()) {
            engine = new QHelpEngine(doc_path,this);
            qDebug() << "Docs found in local files.";
        } else {
            if(critical) {
                QMessageBox::critical(this,tr("MPDR Documentation"),window_text);
                delete this;
            } else {
                language = "en";
                QMessageBox::warning(this,tr("MPDR Documentation"),window_text);
                loadDocs(true,tr("The English documentation could not be loaded!"));
            }
        }
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

    // Target url: file:///path/to/doc/#{language}/file.html

    QString unprocessed_url = url.toString();

    #if defined(WIN32)
        QString processed_url = "file:///";
        QString used_path = QCoreApplication::applicationDirPath();
    #elif defined(__APPLE__)
        QString processed_url = "file://";
        QString used_path = QCoreApplication::applicationDirPath().append("/../Resources");
    #else
        QString processed_url = "file://";
    #endif

    int version_number_index = engine->currentFilter().length() - engine->currentFilter().lastIndexOf("v")-1;
    QString version_number = engine->currentFilter().right(version_number_index);

    QRegularExpression file_regex("/doc/(\\S+)");
    QString filename = file_regex.match(unprocessed_url).captured(1);
    processed_url += used_path + "/doc/" + filename;

    qDebug() << processed_url;

    ui->browser->setUrl(QUrl(processed_url));
    onBrowserRefresh();
}

void DocDialog::homePage() {
    QString filter_text = engine->currentFilter();
    QString version_number = filter_text.right(filter_text.indexOf("v")-2);
    #if defined(Q_OS_WIN)
        QString schema = "file:///";
        QString used_path = QCoreApplication::applicationDirPath();
    #elif defined(__APPLE__)
        QString schema = "file://";
        QString used_path = QCoreApplication::applicationDirPath().append("/../Resources");
    #else
        QString schema = "file://";
    #endif
    QString target_url = schema+used_path+"/doc/"+language+"/index.html";
    ui->browser->setUrl(QUrl(target_url));
    qDebug() << target_url;
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
