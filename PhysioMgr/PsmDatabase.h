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
#include <QtSql>


class PsmDatabase
{
protected:
    QString host;
    QString username;
    QString password;
    QString schema;
    bool isStarted;

public:
    PsmDatabase();

    QString getHost() const;
    QString getUsername() const;
    QString getPassword() const;
    QString getSchema() const;
    bool isConnectionConfigured() const;
    void configConnection(QString host, QString username, QString password, QString schema);
    void configConnectionFromFile(QString path);

    bool startDatabase();
    bool stopDatabase();
    bool isDatabaseStarted() const;
};

#endif // PSMDATABASE_H
