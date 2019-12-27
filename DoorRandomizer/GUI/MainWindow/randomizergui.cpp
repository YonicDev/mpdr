#pragma once
#include "randomizergui.h"

#include <QFileDialog>

#include "./ui_randomizergui.h"
#include "./initialpickupsettings.h"
#include "./patchsettingspanel.h"

RandomizerGUI::RandomizerGUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RandomizerGUI)
{
    ui->setupUi(this);
}

RandomizerGUI::~RandomizerGUI()
{
    delete ui;
}


void RandomizerGUI::on_pickupsButton_clicked()
{
    InitialPickupSettings *dialog = new InitialPickupSettings(this);
    dialog->show();
}

void RandomizerGUI::on_buttonBrowseInput_clicked()
{
    ui->lineInputFile->setText(QFileDialog::getOpenFileName(this,tr("Select a Metroid Prime NTSC 0-00 disc image"),"",tr("Standard disk images (*.iso *.gcm)"),nullptr,QFileDialog::ReadOnly));
}

void RandomizerGUI::on_buttonBrowseOutput_clicked()
{
    ui->lineOutputFile->setText(QFileDialog::getSaveFileName(this,tr("Choose where to save the patched image"),"",tr("ISO disk image (*.iso)")));
}

void RandomizerGUI::on_buttonPatches_clicked()
{
    PatchSettingsPanel *dialog = new PatchSettingsPanel(this);
    dialog->show();
}
