#include "PsmDlgPatientSel.h"
#include "ui_PsmDlgPatientSel.h"

#include <QKeyEvent>

PsmDlgPatientSel::PsmDlgPatientSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgPatientSel)
{
    ui->setupUi(this);
    this->service = NULL;
    this->keyPressedInLe = false;

    this->installEventFilter(this);
    ui->leCond->setFocus();
}

PsmDlgPatientSel::~PsmDlgPatientSel()
{
    delete ui;
}

bool PsmDlgPatientSel::eventFilter(QObject *obj, QEvent *event)
{
    if ( obj == this ) {
        if ( event->type() == QEvent::KeyPress ) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if ( this->keyPressedInLe &&
                 (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) ) {
                this->keyPressedInLe = false;
                return true;
            }
        }
    }

    return QDialog::eventFilter(obj, event);
}

PsmService *PsmDlgPatientSel::getService() const
{
    return this->service;
}

void PsmDlgPatientSel::setService(PsmService *service)
{
    this->service = service;
}

void PsmDlgPatientSel::setAndRefreshCond(const QString &cond)
{
    ui->leCond->setText(cond);
    this->on_pbRefresh_clicked();
}

bool PsmDlgPatientSel::getSelectedPatient(PsmSrvPatient *patient)
{
    if (this->service == NULL || patient == NULL)
        return false;

    return this->service->readSelectedPatient(ui->tableWidget, patient);
}

void PsmDlgPatientSel::on_pbRefresh_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshPatientList(ui->lblCount, ui->tableWidget, this);
    else
        this->service->searchPatient(ui->leCond->text(), ui->lblCount, ui->tableWidget, this);

    ui->leCond->selectAll();
    if ( ui->tableWidget->rowCount() > 0 ) {
        ui->tableWidget->selectRow(0);
        ui->tableWidget->setFocus();
    } else {
        ui->leCond->setFocus();
    }
}

void PsmDlgPatientSel::on_leCond_returnPressed()
{
    this->keyPressedInLe = true;
    this->on_pbRefresh_clicked();
}

void PsmDlgPatientSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
