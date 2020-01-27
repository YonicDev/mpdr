#include "helpdialog.h"

#include <QCoreApplication>
#include <QHelpContentWidget>
#include <QHelpIndexWidget>

HelpDialog::HelpDialog(QWidget *parent) : QDialog(parent) { 

    engine = new QHelpEngine(QCoreApplication::applicationDirPath() + "/doc/doc_collection_en.qhc",this);
    engine->setupData();

    qDebug() << QCoreApplication::applicationDirPath()+ "/doc/doc_collection_en.qhc";

    setupUi();
}

HelpDialog::~HelpDialog() {

    delete browser;
    delete tabber;
    delete layout;
    delete engine;
}

void HelpDialog::setupUi() {

    this->resize(1024,768);

    this->setWindowTitle(tr("MPDR Documentation"));

    layout = new QHBoxLayout;

    tabber = new QTabWidget(this);
    tabber->setSizePolicy(QSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum));
    tabber->addTab(engine->contentWidget(),tr("Content"));
    tabber->addTab(engine->indexWidget(),tr("Index"));

    connect(engine->contentWidget(),SIGNAL(linkActivated(QUrl)),this,SLOT(processUrl(QUrl)));

    browser = new QWebEngineView(this);
    browser->setSizePolicy(QSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored));
    browser->setUrl(QUrl("file:///"+QCoreApplication::applicationDirPath()+"/doc/0.2/en/index.html"));
    browser->show();

    layout->addWidget(tabber);
    layout->addWidget(browser);
    setLayout(layout);
}

void HelpDialog::processUrl(QUrl url) {
    QString unprocessed_url = url.toString();
    QString domain_name = "qthelp://"+engine->namespaceName(QCoreApplication::applicationDirPath()+ "/doc/doc_collection_en.qhc");
    unprocessed_url.insert(unprocessed_url.lastIndexOf("/doc/")+5,domain_name.right(3)+"/");
    QString processed_url = "file:///"+unprocessed_url.replace(0,domain_name.length(),QCoreApplication::applicationDirPath());
    browser->setUrl(QUrl(processed_url));
}
