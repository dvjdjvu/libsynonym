#ifndef __CONFIG_H
#define __CONFIG_H

#include <QString>
#include <QMap>

#define  Debug(str) qDebug() << __FILE__ << ":" << __LINE__ << ":"<< str;
#define eDebug(str) qDebug() << __FILE__ << ":" << __LINE__ << "[error]:" << str;
#define wDebug(str) qDebug() << __FILE__ << ":" << __LINE__ << "[warning]:" << str;
#define nDebug(str) qDebug() << __FILE__ << ":" << __LINE__ << "[notice]:" << str;

class Config
{
public:
    Config();

};


class DBMSConfig : public Config
{
public:
    DBMSConfig();

    QString hostname;
    QString username;
    QString password;
    QString db_name;

    QString search_debug_table_name;
};

class DictionaryConfig : public Config
{

public:
    DictionaryConfig();

    QString dict_name;

    QString syn_dict_name;

    QString debug_configuration_name;
    QString debug_base_config_name;
    QString dict_debug_list;


    QMap<QString,QStringList*> tsConfiguration;


    QString base_path;


    // ***************
    // полные имена для опциии загрузки/замены словарей из
    // альтернативного места на файловой системе клиента
    // **************

    QString ispell_dict_fullpath;
    QString ispell_affix_fullpath;
    QString syn_dict_fullpath;


    // ***************
    // ***************




    // new / currently under work
    // новое и добавленное, с целью наладить обмен файлами с сервером,
    // а так же старые имена используемые для локальных имен файлов словарей

    // секция намеренно отделена, чтобы подчеркнуть ее использование для обмена информацией с сервером,
    // однако, локальные файлы могут считываться/читаться клиентом для поддержания внутренних таблиц.
    // фактически, локальные файлы играют роль кэша информации.



    // локальные имена файлов
    QString dict_fullname;
    QString affix_fullname;
    QString syn_dict_fullname;

    // соответствующие имена на сервере
    QString dict_fullname_server;
    QString affix_fullname_server;
    QString syn_dict_fullname_server;

    QString base_path_server;

    // далее, см. applyConfig()

};

#endif // __CONFIG_H
