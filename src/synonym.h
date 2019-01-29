#ifndef SYNONYM_H
#define SYNONYM_H

#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "config.h"

#define SYN_POSTGRESQL_RECONNECT_COUNT 5

class synonym
{
    
public:
    synonym();
    ~synonym();
    
    QString hostname;
    QString db_name;
    QString username;
    QString password;
    
    /**
     * @brief Подключение к БД.
     * @param ip адрес БД.
     * @param Имя БД.
     * @param Пользователь БД.
     * @param Пароль.
     * @return @ref true успешно, @ref false неуспешно.
     */
    bool connect(QString hostname, QString db_name, QString username, QString password);
    
    /**
     * @brief Добавление синонима к слову.
     * @param Слово.
     * @param Новый синоним.
     * @return @ref true успешно, @ref false неуспешно.
     */
    bool updateSyn(QString word, QString syn);
    
    /**
     * @brief Поиск слов в тексте, включая по синонимам. Слова которые ищутся во фразе, перечисляются через разделители: & - и, | - или .
     * @param Слова которые ищем.
     * @param Текст поиска.
     * @return Исходная фраза поиска, где найденые слова обрамляются <b>Слово</b>.
     */
    QString searchTextSyn(QString searchString, QString Text);

    /**
     * @brief Поиск слов в тексте. Слова которые ищутся во фразе, перечисляются через разделители: & - и, | - или .
     * @param Слова которые ищем.
     * @param Текст поиска.
     * @return Исходная фраза поиска, где найденые слова обрамляются <b>Слово</b>.
     */
    QString searchText(QString searchString, QString Text);
    
    /**
     * @brief Полный поиск слов с условием в тексте. По синонимам, по условиям & | !. Слова которые ищутся во фразе, перечисляются через разделители: & - и, | - или .
     * @param Слова которые ищем.
     * @param Текст поиска.
     * @return Исходная фраза поиска, где найденые слова обрамляются <b>Слово</b>.
     */
    QString searchTextFull(QString searchString, QString Text);
    
    /**
     * Главные функции разметки базы.
     */
    
    /**
     * @brief Инициализация базы (разметка и настройка). Запускать всегда.
     * @param Путь до russian_ods_syn_dict.rules.
     * @param Путь до russian_ods_dict.dict.
     * @param Путь до russian_ods_dict.affix.
     * @return @ref true успешно, @ref false неуспешно.
     */
    bool loadInitialInterface(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath);
    
    /**
     * @brief Загрузка/обновление конфига поиска слов. Выполнять один раз.
     * @param Путь до search_config.sql.
     * @return @ref true успешно, @ref false неуспешно.
     */
    bool updateSearchConfig(QString search_config);
    
    /**
     * @brief Обновление словарей.
     * @param Путь до russian_ods_syn_dict.rules.
     * @param Путь до russian_ods_dict.dict.
     * @param Путь до russian_ods_dict.affix.
     */
    void updateDicts(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath);
    
private:
    DBMSConfig DBMS_config;
    DictionaryConfig Dict_config;
    QSqlDatabase db;
    
    void applyConfig(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath);
    
    bool readDBMSDictionaryData(); 

    bool receiveSpellDictionary();
    bool receiveSynonymDictionary();
    bool receiveTsConfiguration();

    bool writeConfig(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath);
    bool writeFixedConfigString();
    bool writeMovingConfigString();
    bool readRawDictionary();
    bool readRawSpellDictionary();
    
    bool sendTsConfiguration();
    
    bool applyDictToDBMS();
    bool sendSpellDictionary();
    bool applySynDictToDBMS();
    bool sendSynonymDictionary();
    
    
    bool configuration_status;
    
    QList< QStringList > spellListList;

    QMap<QString, QStringList* > currentSpellDictionary; // single Map
    QMap<QString, QStringList* > currentPrelimSpellDictionary; // single Map
    QMap<QString, QString> currentRawSpellDictionary; // single Map
    QMap<QString, QString> currentRawPreliminarySpellDictionary; // single Map
    
    // SYNONYM
    QMap<QString, QString> current_dictionary;
    QList< QStringList > wordsListList;
    QMap< QString, QStringList* >  wordDictionary;
};

#endif 
