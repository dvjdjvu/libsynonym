#include "synonym.h"

synonym::synonym() {

    ////QT4 QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    
    db = QSqlDatabase::addDatabase("QPSQL");
}

synonym::~synonym() {
    
}

bool
synonym::connect(QString hostname, QString db_name, QString username, QString password) {

    this->hostname = hostname;
    this->db_name = db_name;
    this->username = username;
    this->password = password;
    
    db.setHostName(hostname);
    db.setDatabaseName(db_name);
    db.setUserName(username);
    db.setPassword(password);
    
    db.open();
    
    if (!db.isOpen()) {
        Debug(db.lastError());
        return false;
    }

    return true;
}


bool
synonym::loadInitialInterface(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath) {
    
    applyConfig(syn_dict_fullpath, ispell_dict_fullpath, ispell_affix_fullpath);
    
    do {
        if (!readDBMSDictionaryData()) {
            break;
        }

        if (!receiveTsConfiguration()) {
            break;
        }

        if (!writeConfig(syn_dict_fullpath, ispell_dict_fullpath, ispell_affix_fullpath)) {
            break;
        }
        
        return true;
        
    } while(0);
    
    return false;
}


bool 
synonym::readDBMSDictionaryData() {
    return receiveSpellDictionary() && receiveSynonymDictionary();
}

bool 
synonym::receiveSpellDictionary() {

    QFile file(Dict_config.dict_fullname);

    bool ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        QSqlQuery query(db);
        QString queryString;

        queryString = QString("CREATE TABLE IF NOT EXISTS speller_dictionary_files (blob oid, raw_dict_file text);");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT lo_unlink(blob) FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DELETE FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("INSERT INTO speller_dictionary_files VALUES(lo_import('%1'), NULL);").arg(Dict_config.dict_fullname_server);
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("UPDATE speller_dictionary_files SET "
                "raw_dict_file=convert_from(loread(lo_open(blob,262144),100000000),'UTF8');");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            QString fileContent;

            queryString = QString("SELECT raw_dict_file FROM speller_dictionary_files;");
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            while (query.next()) {
                fileContent = query.value(0).toString();
            }

            stream << fileContent;

            file.close();
        }
    }

    QFile file2(Dict_config.affix_fullname);

    ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        QSqlQuery query(db);
        QString queryString;

        queryString = QString("CREATE TABLE IF NOT EXISTS speller_dictionary_files (blob oid, raw_dict_file text);");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT lo_unlink(blob) FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DELETE FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("INSERT INTO speller_dictionary_files VALUES(lo_import('%1'), NULL);")
                .arg(Dict_config.affix_fullname_server);
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("UPDATE speller_dictionary_files SET "
                "raw_dict_file=convert_from(loread(lo_open(blob,262144),100000000),'UTF8');");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (file2.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file2);

            QString fileContent;

            queryString = QString("SELECT raw_dict_file FROM speller_dictionary_files;");
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            while (query.next()) {
                fileContent = query.value(0).toString();
            }

            stream << fileContent;

            file2.close();
        }
    }

    if (!ok) {
        return false;
    }

    return true;
}


bool 
synonym::receiveSynonymDictionary() {

    QFile file(Dict_config.syn_dict_fullname);

    bool ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        QSqlQuery query(db);
        QString queryString;

        queryString = QString("CREATE TABLE IF NOT EXISTS speller_dictionary_files (blob oid, raw_dict_file text);");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT lo_unlink(blob) FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DELETE FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("INSERT INTO speller_dictionary_files VALUES(lo_import('%1'), NULL);")
                .arg(Dict_config.syn_dict_fullname_server);
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("UPDATE speller_dictionary_files SET "
                "raw_dict_file=convert_from(loread(lo_open(blob,262144),100000000),'UTF8');");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (file.open(QIODevice::WriteOnly)) {
            QTextStream stream(&file);

            QString fileContent;

            queryString = QString("SELECT raw_dict_file FROM speller_dictionary_files;");
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            while (query.next()) {
                //qDebug() << __FILE__ << ":" << __LINE__  << query.value(0).toString(); // output everything
                fileContent = query.value(0).toString();
            }

            stream << fileContent;

            file.close();
        }
    }

    if (!ok) {
        return false;
    }

    return true;
}

void 
synonym::applyConfig(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath) {

    DBMS_config.search_debug_table_name = "text_seach_debug_table";

    Dict_config.dict_name     = "russian_ods_dict";
    Dict_config.syn_dict_name = "russian_ods_syn_dict";


    Dict_config.debug_configuration_name = "public.russian_ods";
    Dict_config.debug_base_config_name = "pg_catalog.russian";
    //    Dict_config.dict_debug_list = "russian_ods_syn_dict,russian_ods_dict,russian_stem";
    Dict_config.dict_debug_list = "ispell_filter,russian_ods_syn_dict,russian_ods_dict,russian_stem";
    //    Dict_config.dict_debug_list = "english_stem";

    //Dict_config.base_path = "/usr/local/pgsql/share/tsearch_data/";
    Dict_config.base_path = "./";

    // 09042014 fix this to be a tmpDir
    Dict_config.base_path = QDir::tempPath() + "/";

    //    QString path = "/tmp/";
    QString fullSynFilename = Dict_config.base_path + Dict_config.syn_dict_name + ".rules";
    QString fullDictFilename = Dict_config.base_path + Dict_config.dict_name + ".dict";
    QString fullAffixFilename = Dict_config.base_path + Dict_config.dict_name + ".affix";

    // внимание: назначение путей локального и серверного ЗДЕСЬ
    Dict_config.dict_fullname = fullDictFilename;
    Dict_config.affix_fullname = fullAffixFilename;
    Dict_config.syn_dict_fullname = fullSynFilename;

    // Dict_config.base_path_server = "/usr/share/postgresql/9.1/tsearch_data/";
    Dict_config.base_path_server = "/usr/share/postgresql/tsearch_data/";
    //Dict_config.base_path_server = "/usr/share/postgresql/9.4/tsearch_data/";

    Dict_config.syn_dict_fullname_server =
            Dict_config.base_path_server + Dict_config.syn_dict_name + ".rules";
    Dict_config.dict_fullname_server =
            Dict_config.base_path_server + Dict_config.dict_name + ".dict";
    Dict_config.affix_fullname_server =
            Dict_config.base_path_server + Dict_config.dict_name + ".affix";

    Dict_config.syn_dict_fullpath = syn_dict_fullpath;
    Dict_config.ispell_dict_fullpath = ispell_dict_fullpath;
    Dict_config.ispell_affix_fullpath = ispell_affix_fullpath;

}

bool 
synonym::receiveTsConfiguration() {
    bool ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        Dict_config.tsConfiguration.clear();

        QSqlQuery query(db);
        QString queryString;

        queryString = QString("DROP VIEW IF EXISTS ts_debug_view_3;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DROP VIEW IF EXISTS ts_debug_view_2;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DROP VIEW IF EXISTS ts_debug_view_1;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("CREATE VIEW ts_debug_view_1 AS SELECT mapdict,mapseqno,maptokentype FROM pg_ts_config,pg_ts_config_map WHERE cfgname='russian_ods' AND mapcfg=oid;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("CREATE VIEW ts_debug_view_2 AS SELECT dictname,mapseqno,maptokentype FROM ts_debug_view_1, pg_ts_dict WHERE mapdict=pg_ts_dict.oid;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("CREATE VIEW ts_debug_view_3 AS SELECT dictname, mapseqno, maptokentype, alias from ts_debug_view_2, ts_token_type('default') WHERE maptokentype=tokid;;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT * FROM ts_debug_view_3;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        while (query.next()) {
            QString key = query.value(3).toString();

            if (Dict_config.tsConfiguration.contains(key) == false) {
                QStringList* dicts = new QStringList;

                //int precedence = query.value(1).toInt();
                dicts->append(query.value(0).toString());

                Dict_config.tsConfiguration.insert(key, dicts);

            } else {

                QStringList* dicts = Dict_config.tsConfiguration[key];
                int precedence = query.value(1).toInt() - 1;

                //qDebug() << __FILE__ << ":" << __LINE__  << precedence;

                if (precedence < dicts->size()) {
                    dicts->insert(precedence, query.value(0).toString());
                } else {
                    dicts->append(query.value(0).toString());
                }
            }
        }
    }
    
    if (!ok) {
        return false;
    }

    return true;
}


bool
synonym::writeConfig(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath) {

    applyConfig(syn_dict_fullpath, ispell_dict_fullpath, ispell_affix_fullpath);

    bool ok = true;

    ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        QSqlQuery query(db);

        QString _query;


        if (ok) {
            ok = writeFixedConfigString();
        }
        if (ok) {
            ok = writeMovingConfigString();
        }

        //ALTER TEXT SEARCH CONFIGURATION public.russian_syn_test ALTER MAPPING FOR asciiword,word WITH my_synonyms,russian_stem;

        _query = QString("DROP TABLE IF EXISTS %1;")
                .arg(DBMS_config.search_debug_table_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
        }

        _query = QString("CREATE TABLE %1 (s_text text, s_vector tsvector, s_highlight text);")
                .arg(DBMS_config.search_debug_table_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (ok) {
            readRawDictionary();
        }

        if (ok) {
            readRawSpellDictionary();
        }

        if (ok) {
            configuration_status = true;
        }

        if (!ok) {
            return false;
        }


    } else {
        return false;
    }
    
    return true;
}


bool 
synonym::writeFixedConfigString() {

    bool ok = true;
    ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        QSqlQuery query(db);

        QString _query;

        // create extension

        _query = QString("CREATE EXTENSION IF NOT EXISTS dict_xsyn;");
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
        }
        /*
        _query = QString("CREATE EXTENSION IF NOT EXISTS dict_ispell_filter;");
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
        }
        */
        _query = QString("DROP TEXT SEARCH CONFIGURATION IF EXISTS %1 RESTRICT;").arg(Dict_config.debug_configuration_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
        }

        _query = QString("CREATE TEXT SEARCH CONFIGURATION %1 ( COPY=%2);")
                .arg(Dict_config.debug_configuration_name)
                .arg(Dict_config.debug_base_config_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("DROP TEXT SEARCH DICTIONARY IF EXISTS %1 CASCADE;").arg(Dict_config.syn_dict_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
        }

        _query = QString("CREATE TEXT SEARCH DICTIONARY %1 ( TEMPLATE=xsyn_template, RULES='%1', MATCHSYNONYMS=true, MATCHORIG=true, KEEPORIG=true, KEEPSYNONYMS=true);")
                .arg(Dict_config.syn_dict_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("DROP TEXT SEARCH DICTIONARY IF EXISTS %1 CASCADE;").arg(Dict_config.dict_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
        }

        _query = QString("CREATE TEXT SEARCH DICTIONARY %1 ( TEMPLATE=ispell, DictFile=%1, AffFile=%1, StopWords=russian );")
                .arg(Dict_config.dict_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

    }

    if (!ok) {
        return false;
    }

    return true;
}


bool 
synonym::writeMovingConfigString() {
    bool ok = true;
    ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {
        sendTsConfiguration();
    }

    return true;
}

bool
synonym::readRawDictionary() {

    receiveSynonymDictionary(); // transfer from server (only Synonym dict)

    QFile file(Dict_config.syn_dict_fullname);

    if (file.open(QIODevice::ReadWrite | QIODevice::Text) == true) {

        wordsListList.clear();
        wordDictionary.clear();

        QString dict;
        QString line = file.readLine();
        while (line.isEmpty() == false) {
            dict.append(line);

            QStringList list = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
            if (list.empty() == false) {
                wordsListList.append(list);

                {
                    for (QStringList::iterator word = list.begin(); word != list.end(); word++) {

                        QString key = *word;

                        if (key.isEmpty())
                            continue;

                        key = key.toLower();

                        if (wordDictionary.contains(key)) {
                            eDebug(QString("error: broken word structure ") + key);
                        }

                        wordDictionary.insert(key, &(wordsListList.last()));
                    }
                }
            }
            line = file.readLine();
        }
        file.close();
    } else {
        return false;
    }
    
    return true;
}

bool 
synonym::readRawSpellDictionary() {

    QFile file(Dict_config.dict_fullname);
    if (file.open(QIODevice::ReadWrite | QIODevice::Text) == true) {


        spellListList.clear();
        currentSpellDictionary.clear();
        currentRawSpellDictionary.clear();

        QString dict;
        QString line = file.readLine(); // skip the first line
        line = file.readLine();
        while (line.isEmpty() == false) {
            dict.append(line);

            if (line.isEmpty() == false) {


                QStringList raw_pair = line.split("/", QString::SkipEmptyParts);

                QString key = raw_pair.front();
                if (currentRawSpellDictionary.contains(key)) {
                    eDebug(QString("error: broken word structure ") + key);
                }

                currentRawSpellDictionary.insert(key, raw_pair.back().trimmed());
            }
            line = file.readLine();
        }

        file.close();

    } else {
        return false;
    }

    return true;
}

bool 
synonym::sendTsConfiguration() {

    bool ok = db.isOpen();

    if (!ok) {
        return false;
    }
    
    if (ok) {

        QMapIterator<QString, QStringList*> iterTsConfiguration(Dict_config.tsConfiguration);
        int rowCount = 0;
        while (iterTsConfiguration.hasNext()) {
            iterTsConfiguration.next();

            QString key = iterTsConfiguration.key();
            QString value = iterTsConfiguration.value()->join(",");;

            //qDebug() << __FILE__ << ":" << __LINE__  << "key:" << key << ", value:" << value;

            if (Dict_config.tsConfiguration.contains(key)) {
                QStringList* dicts = Dict_config.tsConfiguration[key];
                QStringList values = value.split(",");

                if (values.isEmpty() == false) {
                    dicts->clear();
                    (*dicts) << values;

                    QSqlQuery query(db);
                    QString _query;

                    _query = QString("ALTER TEXT SEARCH CONFIGURATION %1 ALTER MAPPING FOR "
                            " %2 "
                            "WITH %3;")
                            .arg(Dict_config.debug_configuration_name)
                            .arg(key)
                            .arg(value);
                            //.arg(Dict_config.dict_debug_list);

                    if (ok && query.exec(_query) == false) {
                        eDebug(query.lastError().text());
                        ok = false;
                    }
                }
            }
            rowCount++;
        }
    }
    
    if (!ok) {
        return false;
    }

    return true;
}

QString
synonym::searchTextSyn(QString searchString, QString Text) {

    /*
     * to_tsquery ожидает на вход уже обработанные данные, соответствующие типу tsquery (работает быстрее)
     * plainto_tsquery же предназначен для обработки «сырого» запроса
     */
    
    QString findString;
    
    QString search_string = searchString;
    QString search_text = Text;

    search_text.replace(QRegExp("[^\\w^\\s^\\-]"), " ");

    bool ok = db.isOpen();

    if (!ok) {
        return QString("[error]: db not openned!");
    }
    
    if (ok) {

        QSqlQuery query(db);

        QString _query;

        //_query = QString("SELECT ts_headline('%1','%2', plainto_tsquery('%1','%3'), 'HighlightAll=TRUE')")
        _query = QString("SELECT ts_headline('%1','%2', plainto_tsquery('%1','%3'), 'HighlightAll=TRUE')")
                .arg(Dict_config.debug_configuration_name)
                .arg(search_text)
                .arg(search_string);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
            return findString;
        }
        
        while (query.next()) {
            findString = query.value(0).toString();
        }
        
        if (query.first() == false) {
            return QString("");
        }
    }

    return findString;
}

QString
synonym::searchText(QString searchString, QString text) {

    /*
     * to_tsquery ожидает на вход уже обработанные данные, соответствующие типу tsquery (работает быстрее)
     * plainto_tsquery же предназначен для обработки «сырого» запроса
     */
    
    QString findString;
    
    QString search_string = searchString;
    QString search_text = text;

    search_text.replace(QRegExp("[^\\w^\\s^\\-]"), " ");

    bool ok = db.isOpen();

    if (!ok) {
        return QString("[error]: db not openned!");
    }
    
    if (ok) {
        QSqlQuery query(db);
        QString _query;

        _query = QString("select ts_headline('%1', plainto_tsquery('%2'), 'StartSel=<a>, StopSel=</a>');").arg(search_text).arg(search_string);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
            return findString;
        }
        
        while (query.next()) {
            findString = query.value(0).toString();
        }
        
        if (query.first() == false) {
            return QString("");
        }
        
    }
    return findString;
}
/*
QString
synonym::searchTextFull(QString searchString, QString text) {

    
    // to_tsquery ожидает на вход уже обработанные данные, соответствующие типу tsquery (работает быстрее)
    // plainto_tsquery же предназначен для обработки «сырого» запроса
     
    
    QString findString;
    
    QString search_string = searchString;
    QString search_text = text;


    search_text.replace(QRegExp("[^\\w^\\s^\\-]"), " ");

    bool ok = db.isOpen();

    if (!ok) {
        return QString("[error]: db not openned!");
    }
    
    if (ok) {
        QSqlQuery query(db);
        QString _query;

        _query = QString("DELETE FROM %1;").arg(DBMS_config.search_debug_table_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("INSERT INTO %1 VALUES ('%2', NULL);")
                .arg(DBMS_config.search_debug_table_name)
                .arg(search_text);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("UPDATE %1 SET s_vector=to_tsvector('%2',s_text);")
                .arg(DBMS_config.search_debug_table_name)
                .arg(Dict_config.debug_configuration_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("SELECT s_vector from %1;")
                .arg(DBMS_config.search_debug_table_name);
        if (ok && query.exec(_query) == false) {
            eDebug("error: values cannot be inserted");
            ok = false;
        }

        _query = QString("SELECT to_tsquery('%1','%2');")
                .arg(Dict_config.debug_configuration_name)
                .arg(search_string);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("SELECT ts_parse('default','%1');")
                .arg(search_string);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("SELECT * FROM %1 WHERE s_vector @@ to_tsquery('%2','%3');")
                .arg(DBMS_config.search_debug_table_name)
                .arg(Dict_config.debug_configuration_name)
                .arg(search_string);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (query.first() == false) {
            return QString("");
        }
        
        _query = QString("UPDATE %1 SET s_highlight=NULL;")
                .arg(DBMS_config.search_debug_table_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }


        _query = QString("UPDATE %1 SET s_highlight=ts_headline('%2',s_text,to_tsquery('%2','%3'),'HighlightAll=TRUE');")
                .arg(DBMS_config.search_debug_table_name)
                .arg(Dict_config.debug_configuration_name)
                .arg(search_string);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        _query = QString("SELECT s_highlight FROM %1;")
                .arg(DBMS_config.search_debug_table_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }
        
        while (query.next()) {
            findString = query.value(0).toString();
        }
    }

    return findString;
}
*/

QString
synonym::searchTextFull(QString searchString, QString text) {

    
    // to_tsquery ожидает на вход уже обработанные данные, соответствующие типу tsquery (работает быстрее)
    // plainto_tsquery же предназначен для обработки «сырого» запроса
     
    
    QString findString;
    
    QString search_string = searchString;
    QString search_text = text;


    search_text.replace(QRegExp("[^\\w^\\s^\\-]"), " ");

    bool ok = db.isOpen();

    if (!ok) {
        return QString("[error]: db not openned!");
    }
    
    if (ok) {
        QSqlQuery query(db);
        QString _query;

        _query = QString("select * from searchTextFull('%1', '%2', '%3', '%4');")
                .arg(search_string)
                .arg(search_text)
                .arg(DBMS_config.search_debug_table_name)
                .arg(Dict_config.debug_configuration_name);
        if (ok && query.exec(_query) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }
        
        while (query.next()) {
            findString = query.value(0).toString();
        }
    }

    return findString;
}

void 
synonym::updateDicts(QString syn_dict_fullpath, QString ispell_dict_fullpath, QString ispell_affix_fullpath) {
    
    applyConfig(syn_dict_fullpath, ispell_dict_fullpath, ispell_affix_fullpath);

    if ( Dict_config.ispell_dict_fullpath.isEmpty() == false ||
         Dict_config.ispell_affix_fullpath.isEmpty() == false ) {
        applyDictToDBMS();
    }
    
    if (Dict_config.syn_dict_fullpath.isEmpty() == false) {
        applySynDictToDBMS();
    }
}

bool 
synonym::updateSearchConfig(QString search_config) {
    bool ok = db.isOpen();

    if (ok) {

        QSqlQuery query(db);
        QString _query;
        
        QFile file(search_config);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text) == true) {
            
            QTextStream in(&file);
            QString line;
            
            while (!in.atEnd()) {
                line = in.readLine(); //read one line at a time
                _query = line;
                if (query.exec(_query) == false) {
                    eDebug(query.lastError().text());
                }
            }
            
            file.close();

        } else {
            eDebug(file.errorString());
            return false;
        }
    }
    
    return true;
}

bool 
synonym::applyDictToDBMS() {

    bool ok = true;

    if (Dict_config.ispell_dict_fullpath.isEmpty() ||
            Dict_config.ispell_affix_fullpath.isEmpty()) {
        ok = false;
        eDebug("error: filename not specified");
    }

    if (QFile::exists(Dict_config.dict_fullname) == true && QFile::exists(Dict_config.ispell_dict_fullpath)) {
        if (QFile::remove(Dict_config.dict_fullname) == false) {
            ok = false;
            eDebug("error: permission issue");

        } else if (QFile::copy(Dict_config.ispell_dict_fullpath, Dict_config.dict_fullname) == false) {
            ok = false;
            eDebug("error: permission issue");
        }
    }

    if (QFile::exists(Dict_config.affix_fullname) == true && QFile::exists(Dict_config.ispell_affix_fullpath)) {
        if (QFile::remove(Dict_config.affix_fullname) == false) {
            ok = false;
            eDebug("error: permission issue");

        } else if (QFile::copy(Dict_config.ispell_affix_fullpath, Dict_config.affix_fullname) == false) {
            ok = false;
            eDebug("error: permission issue");
        }
    }

    do {
        if (ok) {
            ok = db.isOpen();
            if (!ok) {
                break;
            }
        }

        sendSpellDictionary();

        if (ok) {
            QSqlQuery query(db);
            QString _query;

            _query = QString("ALTER TEXT SEARCH DICTIONARY %1 ( DictFile=%1, AffFile=%1 );").arg(Dict_config.dict_name);
            if (ok && query.exec(_query) == false) {
                eDebug("error: text search dictionary could not be created, stopping");
                ok = false;
            }

            ok = writeMovingConfigString();

        } else {
            break;
        }

        if (!ok) {
            break;
        }
    
        return true;
    } while (0);
    
    return false;
}


bool 
synonym::sendSpellDictionary() {
    QFile file(Dict_config.dict_fullname);
    bool ok = db.isOpen();

    if (ok) {
        QSqlQuery query(db);
        QString queryString;

        queryString = QString("CREATE TABLE IF NOT EXISTS speller_dictionary_files (blob oid, raw_dict_file text);");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT lo_unlink(blob) FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DELETE FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString fileContent;

            while (!stream.atEnd()) {
                fileContent.append(stream.readLine());
                fileContent.append("\r\n");
            }

            queryString = QString("INSERT INTO speller_dictionary_files VALUES(lo_creat(-1), '%1');")
                    .arg(fileContent);
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            queryString = QString("SELECT lowrite( "
                    " lo_open(blob, 131072), "
                    "convert_to(raw_dict_file, 'UTF-8') ) "
                    "FROM speller_dictionary_files;");
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            queryString = QString("SELECT lo_export(blob,'%1') FROM speller_dictionary_files;")
                    .arg(Dict_config.dict_fullname_server);
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }
        }
    }

    QFile file2(Dict_config.affix_fullname);
    ok = db.isOpen();

    if (ok) {
        QSqlQuery query(db);
        QString queryString;

        queryString = QString("CREATE TABLE IF NOT EXISTS speller_dictionary_files (blob oid, raw_dict_file text);");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT lo_unlink(blob) FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DELETE FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (file2.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file2);
            QString fileContent;

            while (!stream.atEnd()) {
                fileContent.append(stream.readLine());
                fileContent.append("\r\n");
            }

            queryString = QString("INSERT INTO speller_dictionary_files VALUES(lo_creat(-1), '%1');")
                    .arg(fileContent);
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            queryString = QString("SELECT lowrite( "
                    " lo_open(blob, 131072), "
                    "convert_to(raw_dict_file, 'UTF-8') ) "
                    "FROM speller_dictionary_files;");
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            queryString = QString("SELECT lo_export(blob,'%1') FROM speller_dictionary_files;")
                    .arg(Dict_config.affix_fullname_server);
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }
        }
    }

    if (!ok) {
        return false;
    }

    return true;
}


bool
synonym::applySynDictToDBMS() {
    bool ok = true;

    if (Dict_config.syn_dict_fullpath.isEmpty()) {
        ok = false;
        eDebug("error: filename not specified");
    }

    if (QFile::exists(Dict_config.syn_dict_fullname) == true && QFile::exists(Dict_config.syn_dict_fullpath)) {
        if (QFile::remove(Dict_config.syn_dict_fullname) == false) {
            ok = false;
            eDebug("error: permission issue");

        } else if (QFile::copy(Dict_config.syn_dict_fullpath, Dict_config.syn_dict_fullname) == false) {
            ok = false;
            eDebug("error: permission issue");
        }
    }

    sendSynonymDictionary();

    do {
        if (ok) {
            ok = db.isOpen();
            if (!ok) {
                break;
            }
        }

        if (ok) {
            QSqlQuery query(db);
            QString _query;

            _query = QString("ALTER TEXT SEARCH DICTIONARY %1 ( RULES=%1 );")
                    .arg(Dict_config.syn_dict_name);
            if (ok && query.exec(_query) == false) {
                eDebug("error: text search dictionary could not be created, stopping");
                ok = false;
            }

            ok = writeMovingConfigString();
            //sendTsConfiguration();

            if (ok && query.exec(_query) == false) {
                eDebug("error: text search configuration could not be altered");
                ok = false;
            }
        } else {
            break;
        }

        if (!ok) {
            break;
        }
    
        return true;
    } while (0);
    
    return false;
}


bool 
synonym::sendSynonymDictionary() {
    QFile file(Dict_config.syn_dict_fullname);
    bool ok = db.isOpen();

    if (ok) {
        QSqlQuery query(db);
        QString queryString;

        queryString = QString("CREATE TABLE IF NOT EXISTS speller_dictionary_files (blob oid, raw_dict_file text);");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("SELECT lo_unlink(blob) FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        queryString = QString("DELETE FROM speller_dictionary_files;");
        if (ok && query.exec(queryString) == false) {
            eDebug(query.lastError().text());
            ok = false;
        }

        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);
            QString fileContent;

            while (!stream.atEnd()) {
                fileContent.append(stream.readLine());
                fileContent.append("\r\n");
            }

            queryString = QString("INSERT INTO speller_dictionary_files VALUES(lo_creat(-1), '%1');")
                    .arg(fileContent);
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            queryString = QString("SELECT lowrite( "
                    " lo_open(blob, 131072), "
                    "convert_to(raw_dict_file, 'UTF-8') ) "
                    "FROM speller_dictionary_files;");
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }

            queryString = QString("SELECT lo_export(blob,'%1') FROM speller_dictionary_files;")
                    .arg(Dict_config.syn_dict_fullname_server);
            if (ok && query.exec(queryString) == false) {
                eDebug(query.lastError().text());
                ok = false;
            }
        }
    }

    if (!ok) {
        return false;
    }

    return true;
}
