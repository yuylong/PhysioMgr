#include "PsmDlgDepartSel.h"
#include "ui_PsmDlgDepartSel.h"

PsmDlgDepartSel::PsmDlgDepartSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDepartSel)
{
    ui->setupUi(this);
}

PsmDlgDepartSel::~PsmDlgDepartSel()
{
    delete ui;
}
