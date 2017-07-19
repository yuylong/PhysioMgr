#include "PsmDlgPatientSel.h"
#include "ui_PsmDlgPatientSel.h"

PsmDlgPatientSel::PsmDlgPatientSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPatientSel)
{
    ui->setupUi(this);
}

PsmDlgPatientSel::~PsmDlgPatientSel()
{
    delete ui;
}
