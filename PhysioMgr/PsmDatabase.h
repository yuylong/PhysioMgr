/*-
 * GNU GENERAL PUBLIC LICENSE, version 3
 * See LICENSE file for detail.
 *
 * Author: Yulong Yu, Jun 29th, 2017
 * Copyright(c) 2015-2017 Yulong Yu. All rights reserved.
 */

#ifndef PSMDATABASE_H
#define PSMDATABASE_H

#include <QString>
#include <QList>
#include <QtSql>
#include <QtXml>
#include <QTableWidget>


class PsmDatabase
{
protected:
    QString host;
    QString username;
    QString password;
    QString schema;
    bool isStarted;

    static QString connectname;

public:
    PsmDatabase();

    QString getHost() const;
    QString getUsername() const;
    QString getPassword() const;
    QString getSchema() const;
    bool isConnectionConfigured() const;
    void configConnection(const QString &host, const QString &username,
                          const QString &password, const QString &schema);
    void configConnectionFromDom(const QDomElement &docelem);
    void configConnectionFromFile(const QString &path);

    bool startDatabase();
    void stopDatabase();
    bool isDatabaseStarted() const;

    QSqlQuery getQuery();
    void fillTableWidget(QTableWidget *tblwdg, QSqlQuery *query,
                         const QList<int> &colmap, const QList<int> &datamap = QList<int>());
};

#endif // PSMDATABASE_H
