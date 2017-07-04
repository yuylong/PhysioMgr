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

    QString getDepartID() const;
    QString getDepartName() const;

    void setDepartID(const QString &id);
    void setDepartName(const QString &name);
    void lockDepartID();
    void unlockDepartID();

private:
    Ui::PsmDlgDepartment *ui;
};

#endif // PSMDLGDEPARTMENT_H
