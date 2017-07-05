#include "PsmDlgDoctor.h"
#include "ui_PsmDlgDoctor.h"

PsmDlgDoctor::PsmDlgDoctor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDoctor)
{
    ui->setupUi(this);
}

PsmDlgDoctor::~PsmDlgDoctor()
{
    delete ui;
}
