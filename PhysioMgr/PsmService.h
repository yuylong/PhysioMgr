#ifndef PSMSERVICE_H
#define PSMSERVICE_H

#include <QString>
#include <QtSql>
#include <QLabel>
#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QMainWindow>
#include "PsmDatabase.h"

struct PsmSrvDepartment {
    QString id;
    QString name;
};

class PsmService
{
protected:
    QString configfile;
    PsmDatabase database;
    QMainWindow *parent;

public:
    PsmService();

    QString getConfigFile() const;
    void setConfigFile(const QString &configfile);
    QMainWindow *getParentWindow() const;
    void setParentWindow(QMainWindow *window);
    const PsmDatabase *getDatabase() const;
    PsmDatabase *getDatabase();

    bool init(QWidget *window = NULL);

    bool readSelectedDepartment(QTableWidget *tbl, PsmSrvDepartment *dep);
    QString readSelectedDepartmentId(QTableWidget *tbl);
    void refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertNewDepartment(const PsmSrvDepartment &dep, QWidget *window = NULL);
    void updateDepartment(const PsmSrvDepartment &dep, QWidget *window = NULL);
    void deleteDepartment(const QString &depid, QWidget *window = NULL);

    void refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);

private:
    int getTableSelectedRow(QTableWidget *tbl);
};

#endif // PSMSERVICE_H
