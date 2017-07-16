#include "PsmDlgHospiPhysio.h"
#include "ui_PsmDlgHospiPhysio.h"

PsmDlgHospiPhysio::PsmDlgHospiPhysio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgHospiPhysio)
{
    ui->setupUi(this);
}

PsmDlgHospiPhysio::~PsmDlgHospiPhysio()
{
    delete ui;
}
