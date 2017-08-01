#include "PsmDlgDoctorSel.h"
#include "ui_PsmDlgDoctorSel.h"

#include <QKeyEvent>

PsmDlgDoctorSel::PsmDlgDoctorSel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PsmDlgDoctorSel)
{
    ui->setupUi(this);
    this->service = NULL;
    this->keyPressedInLe = false;

    this->installEventFilter(this);
    ui->leCond->setFocus();
}

PsmDlgDoctorSel::~PsmDlgDoctorSel()
{
    delete ui;
}

bool PsmDlgDoctorSel::eventFilter(QObject *obj, QEvent *event)
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

PsmService *PsmDlgDoctorSel::getService() const
{
    return this->service;
}

void PsmDlgDoctorSel::setService(PsmService *service)
{
    this->service = service;
}

bool PsmDlgDoctorSel::getSelectedDoctor(PsmSrvDoctor *doctor)
{
    if (this->service == NULL || doctor == NULL)
        return false;

    return this->service->readSelectedDoctor(ui->tableWidget, doctor);
}

void PsmDlgDoctorSel::on_pbRefresh_clicked()
{
    if (this->service == NULL)
        return;

    if (ui->leCond->text().isEmpty())
        this->service->refreshDoctorList(ui->lblCnt, ui->tableWidget, this);
    else
        this->service->searchDoctor(ui->leCond->text(), ui->lblCnt, ui->tableWidget, this);

    ui->leCond->selectAll();
    if ( ui->tableWidget->rowCount() > 0 ) {
        ui->tableWidget->selectRow(0);
        ui->tableWidget->setFocus();
    } else {
        ui->leCond->setFocus();
    }
}

void PsmDlgDoctorSel::on_leCond_returnPressed()
{
    this->keyPressedInLe = true;
    this->on_pbRefresh_clicked();
}

void PsmDlgDoctorSel::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    ui->tableWidget->selectRow(index.row());
    this->accept();
}
