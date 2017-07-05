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

struct PsmSrvPhysioItem {
    QString id;
    QString name;
    double price;
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

    /* Departments */
    bool readSelectedDepartment(QTableWidget *tbl, PsmSrvDepartment *dep);
    QString readSelectedDepartmentId(QTableWidget *tbl);
    void refreshDepartmentList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void searchDepartment(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertNewDepartment(const PsmSrvDepartment &dep, QWidget *window = NULL);
    void updateDepartment(const PsmSrvDepartment &dep, QWidget *window = NULL);
    void deleteDepartment(const QString &depid, QWidget *window = NULL);

    /* Physio Items */
    bool readSelectedPhysioItem(QTableWidget *tbl, PsmSrvPhysioItem *physio);
    QString readSelectedPhysioItemId(QTableWidget *tbl);
    void refreshPhysioItemList(QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void searchPhysioItem(const QString &srchstr, QLabel *lbl, QTableWidget *tbl, QWidget *window = NULL);
    void insertNewPhysioItem(const PsmSrvPhysioItem &physio, QWidget *window = NULL);
    void updatePhysioItem(const PsmSrvPhysioItem &physio, QWidget *window = NULL);
    void deletePhysioItem(const QString &physioid, QWidget *window = NULL);

private:
    int getTableSelectedRow(QTableWidget *tbl);
    QString readTableSelectedId(QTableWidget *tbl, int idx);
};

#endif // PSMSERVICE_H