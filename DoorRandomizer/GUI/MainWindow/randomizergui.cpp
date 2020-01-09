#include "randomizergui.h"
#include "./ui_randomizergui.h"

#include <random>

#include <QFileDialog>
#include <QJsonDocument>
#include <QMessageBox>
#include <QDateTime>
#include <QProcess>

#include "./initialpickupsettings.h"
#include "./patchsettingspanel.h"
#include <randomizer.h>

RandomizerGUI::RandomizerGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RandomizerGUI)
{
    ui->setupUi(this);
    preset = new Preset();
    load_from_preset();
    QHeaderView *header = ui->logTable->horizontalHeader();
    header->setStretchLastSection(true);

    Randomizer *randomizer = new Randomizer;
    randomizer->moveToThread(&worker_thread);
    connect(&worker_thread,&QThread::finished,randomizer,&QObject::deleteLater,Qt::ConnectionType::QueuedConnection);
    connect(this,&RandomizerGUI::run_thread,randomizer,&Randomizer::randomize,Qt::ConnectionType::QueuedConnection);
    connect(randomizer,&Randomizer::send_message,this,&RandomizerGUI::process_message,Qt::ConnectionType::QueuedConnection);
}

RandomizerGUI::~RandomizerGUI()
{
    if(worker_thread.isRunning())
        worker_thread.terminate();
    delete preset;
    delete ui;
}

#ifdef WIN32
void RandomizerGUI::showEvent(QShowEvent *event) {
    taskbar_button = new QWinTaskbarButton(this);
    taskbar_button->setWindow(this->windowHandle());
    taskbar_progress = taskbar_button->progress();
}
#endif

void RandomizerGUI::save_to_preset() {
    auto zone_groups = ui->group_weights->children();
    for(int i=0;i<5;++i) {
        for (int j=0;j<4;++j) {
            const QString selector = QString("slider%1_%2").arg(i).arg(j);
            QSlider *slider = this->findChild<QSlider *>(selector);
            preset->weights[i][j] = slider->value();
        }
    }
    preset->seed = static_cast<int32_t>(ui->seedField->value());
    preset->input_iso = ui->lineInputFile->text();
    preset->output_iso = ui->lineOutputFile->text();
}

void RandomizerGUI::load_from_preset() {
    auto zone_groups = ui->group_weights->children();
    for(int i=0;i<5;++i) {
        for (int j=0;j<4;++j) {
            const QString selector = QString("slider%1_%2").arg(i).arg(j);
            QSlider *slider = this->findChild<QSlider *>(selector);
            slider->setValue(preset->weights[i][j]);
        }
    }
    ui->seedField->setValue(static_cast<int32_t>(preset->seed));
    ui->lineInputFile->setText(preset->input_iso);
    ui->lineOutputFile->setText(preset->output_iso);
}


void RandomizerGUI::on_pickupsButton_clicked()
{
    InitialPickupSettings *dialog = new InitialPickupSettings(this,preset);
    dialog->open();
}

void RandomizerGUI::on_buttonBrowseInput_clicked()
{
    ui->lineInputFile->setText(QFileDialog::getOpenFileName(this,tr("Select a Metroid Prime NTSC 0-00/0-02  disc image"),"",tr("Standard disk images (*.iso *.gcm)"),nullptr,QFileDialog::ReadOnly));
}

void RandomizerGUI::on_buttonBrowseOutput_clicked()
{
    ui->lineOutputFile->setText(QFileDialog::getSaveFileName(this,tr("Choose where to save the patched image"),"",tr("ISO disk image (*.iso)")));
}

void RandomizerGUI::on_buttonPatches_clicked()
{
    PatchSettingsPanel *dialog = new PatchSettingsPanel(this,preset);
    dialog->open();
}

void RandomizerGUI::on_buttonGenerateSeed_clicked()
{
    std::mt19937 rng(static_cast<uint32_t>(preset->seed));
    std::uniform_int_distribution<int32_t> distr;

    preset->seed = distr(rng);
    ui->seedField->setValue(preset->seed);
}

void RandomizerGUI::on_buttonUseTime_clicked()
{
    preset->seed = static_cast<int32_t>(time(nullptr));
    ui->seedField->setValue(preset->seed);
}

void RandomizerGUI::on_actionOpen_preset_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,tr("Open a preset file..."),"",tr("Preset files (*.json)"),nullptr,QFileDialog::ReadOnly);
    if(path.isNull()) return;
    QFile preset_file(path);
    if(preset_file.open(QIODevice::ReadOnly)) {
        QByteArray data = preset_file.readAll();
        QJsonDocument preset_json = QJsonDocument::fromJson(data);
        int parse = preset->deserialize(preset_json.object());
        if(parse==0) {
            load_from_preset();
        } else {
            switch(parse) {
                case -1:
                    QMessageBox::critical(this,tr("Error"),
                      //: This pops up when a JSON file doesn't match a MPDR preset format.
                      //~ Context Error message
                      tr("This is not a MPDR preset."));
                break;
                case -2:
                    QMessageBox::critical(this,tr("Error"),
                      //: This pops up when an MPDR preset has data that doesn't match what an MPDR preset of its version should have.
                      //~ Context Error message
                      tr("Error parsing the MPDR preset."));
                break;
                case -3:
                    QMessageBox::critical(this,tr("Error"),
                      //: This pops up when an MPDR preset has invalid data.
                      //~ Context Error message
                      tr("Data within the MPDR profile might be corrupt."));
                break;
                case -4:
                    QMessageBox::critical(this,tr("Error"),
                      //: This pops up when an MPDR preset is from the first version of MPDR, which is currently not supported.
                      //~ Context Error message
                      tr("MPDR presets of version 1 are currently unsupported."));
                break;
            }
        }
        preset_file.close();
    } else {
        QMessageBox::critical(this,tr("Error"),
           //: This pops up when a file could not be opened. It could be any kind of file, but we refer to it as a preset regardless.
           //~ Context Error message
           tr("Could not open preset."));
    }
}

void RandomizerGUI::on_actionSave_preset_triggered()
{
    save_to_preset();
    QString path = QFileDialog::getSaveFileName(this,tr("Save preset file to..."),"",tr("Preset files (*.json)"),nullptr);
    if(path.isNull()) return;
    QJsonObject preset_json;
    preset->serialize(preset_json);
    QFile preset_file(path);

    QJsonDocument doc(preset_json);
    if(preset_file.open(QIODevice::WriteOnly)) {
        preset_file.write(doc.toJson());
        preset_file.close();
    } else {
        QMessageBox::critical(this,tr("Error"),tr("Could not save preset."));
    }

}

void RandomizerGUI::log(QString type,QString message) {

    QString timestamp = QDateTime::currentDateTime().toString("HH:mm:ss");

    ui->logTable->insertRow(0);
    QTableWidgetItem *type_cell = new QTableWidgetItem(type);
    QTableWidgetItem *time_cell = new QTableWidgetItem(timestamp);
    QTableWidgetItem *message_cell = new QTableWidgetItem(message);

    type_cell->setFlags(type_cell->flags() ^ Qt::ItemIsEditable);
    message_cell->setFlags(message_cell->flags() ^ Qt::ItemIsEditable);

    ui->logTable->setVerticalHeaderItem(0,type_cell);

    ui->logTable->setItem(0,0,time_cell);
    ui->logTable->setItem(0,1,message_cell);
}

void RandomizerGUI::set_progress(int percentage) {
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(percentage);
    #ifdef WIN32
    taskbar_progress->setMaximum(100);
    taskbar_progress->setValue(percentage);
    #endif
}

void RandomizerGUI::on_buttonRandomize_clicked()
{

    save_to_preset();

    ui->buttonRandomize->setEnabled(false);
    ui->progressBar->setMaximum(0);

    #ifdef WIN32
    taskbar_progress->resume();
    taskbar_progress->setMaximum(0);
    taskbar_progress->show();
    #endif

    worker_thread.start();
    emit run_thread(preset);
}

void RandomizerGUI::process_message(QJsonObject data) {
    QString type = data["type"].toString();
    QString message = data["msg"].toString();
    if(type == "progress") {
        int progress = floor(data["percent"].toDouble());
        set_progress(progress);
    } if(type == "error") {
        if(worker_thread.isRunning())
            worker_thread.exit(-1);
        ui->buttonRandomize->setEnabled(true);
        #ifdef WIN32
            taskbar_progress->stop();
        #endif
        ui->progressBar->setMaximum(100);
        QMessageBox::critical(this,tr("Error"),tr("An error has occurred while patching:\n") + message);
        #ifdef WIN32
            taskbar_progress->reset();
            taskbar_progress->hide();
        #endif
    }
    if(type == "success") {
        if(worker_thread.isRunning())
            worker_thread.exit(0);
        ui->buttonRandomize->setEnabled(true);
        #ifdef WIN32
            taskbar_progress->reset();
            taskbar_progress->hide();
        #endif
        QMessageBox::information(this,tr("Done"),tr("Succesfully patched the game!"));
    }
    log(type,message);
}

void RandomizerGUI::on_actionAbout_MPDR_triggered()
{
    QMessageBox::about(this,tr("About MPDR"),"Metroid Prime Randomizer v0.1\nMIT License YonicStudios 2019");
}

void RandomizerGUI::on_actionContents_triggered()
{
    QProcess *process = new QProcess;
    QStringList args;
    args << QLatin1String("-collectionFile")
         << QCoreApplication::applicationDirPath().append("/doc/doc_collection.qhc")
         << QLatin1String("-enableRemoteControl");
    #ifdef WIN32
    QString command = QCoreApplication::applicationDirPath().append("/assistant")
    #else
    // Because Linux has to build from source,
    // we're assuming Qt Assistant is already installed
    const QString command = "assistant";
    #endif

    process->start(command,args);
    if(!process->waitForStarted())
        return;

}
