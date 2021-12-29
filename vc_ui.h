#ifndef VC_UI_H
#define VC_UI_H

#include <QWidget>
#include <QFileDialog>
#include <QGrypho.hpp>
#include <QMessageBox>
#include <minvc_core.h>

namespace Ui {
class VC_ui;
}

class VC_ui : public QWidget
{
    Q_OBJECT

public:
    explicit VC_ui(QWidget *parent = nullptr);
    ~VC_ui();

private slots:

    void on_inputButton_clicked();

    void on_outputButton_clicked();

    void on_showButton_clicked();

    void on_startButton_clicked();

private:
    Ui::VC_ui *ui;
    QGrypho drawer;
    minVC_core coveree;
    unsigned char status;
    const QString statuses[4] = {"-", "\\", "|", "/"};
    VCMode getVCMode();
};


#endif // VC_UI_H
