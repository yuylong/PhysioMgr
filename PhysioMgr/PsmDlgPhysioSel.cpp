#include "PsmDlgPhysioSel.h"
#include "ui_PsmDlgPhysioSel.h"

PsmDlgPhysioSel::PsmDlgPhysioSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPhysioSel)
{
    ui->setupUi(this);
}

PsmDlgPhysioSel::~PsmDlgPhysioSel()
{
    delete ui;
}
