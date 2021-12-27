#include "vc_ui.h"
#include "ui_vc_ui.h"

VC_ui::VC_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VC_ui)
{
    ui->setupUi(this);
    drawer.setWindowTitle("Схема вершинного покрытия");
}

VC_ui::~VC_ui()
{
    delete ui;
}

void VC_ui::on_inputButton_clicked()
{
    ui->inputPath->setText(QFileDialog::getOpenFileName(this, tr("Открыть граф"), "~", tr("Файлы графов (*.cgf *.tgf *.dot *.*)")));
}


void VC_ui::on_outputButton_clicked()
{
    ui->outputPath->setText(QFileDialog::getSaveFileName(this, tr("Сохранить результат"), "~", tr("Текстовый документ (*.txt *.*)")));
}

void VC_ui::on_showButton_clicked()
{
    drawer.show();
}

void VC_ui::on_startButton_clicked()
{
    coveree = minVC_core(ui->inputPath->text().toStdString(), getVCMode());
    if(coveree.nonsense()){
        QMessageBox::warning(this, "Эй", "Не надо этого делать. Серьезно, есть более интересные способы тратить время.");
    }
    drawer.drawGraph(coveree.graph());
    QString time = QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(coveree.time()).count()) + " мс";
    drawer.text = "время: " + time + "\nразмер покрытия: " + QString::number(coveree.cover().size());
    ui->statusLabel->setText(time);
}

VCMode VC_ui::getVCMode()
{
    switch (ui->modeCombo->currentIndex()) {
    case 0:
        return VCMode::Exhaustive;
    case 1:
        return VCMode::LogNCombK;
    case 2:
        return VCMode::CliqueOpts;
    default:
        return VCMode::Exhaustive;
    }
}
