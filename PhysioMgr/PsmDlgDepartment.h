#ifndef PSMDLGDEPARTMENT_H
#define PSMDLGDEPARTMENT_H

#include <QDialog>

namespace Ui {
class PsmDlgDepartment;
}

class PsmDlgDepartment : public QDialog
{
    Q_OBJECT

public:
    explicit PsmDlgDepartment(QWidget *parent = 0);
    ~PsmDlgDepartment();

private:
    Ui::PsmDlgDepartment *ui;
};

#endif // PSMDLGDEPARTMENT_H
