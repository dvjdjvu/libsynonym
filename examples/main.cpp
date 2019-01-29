#include <QApplication>
#include <QTextCodec>

#include <synonym/synonym.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp866"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    ////QT4 QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    
    synonym syn;
    
    Debug(QString("connect"));
    Debug(syn.connect("127.0.0.1", "test_db", "postgres", "1"));
    
    Debug("loadInitialInterface()");
    if (!syn.loadInitialInterface("./dicts/russian_ods_syn_dict.rules", "./dicts/russian_ods_dict.dict", "./dicts/russian_ods_dict.affix")) {
        Debug("error: loadInitialInterface");
    }

    /*
    Debug("updateSearchConfig");
    if (!syn.updateSearchConfig("conf/search_config.sql")) {
        Debug("error: updateSearchConfig");
    }
    */
    Debug("searchText");
    struct timespec ts_start;
    struct timespec ts_stop;
    clock_gettime(CLOCK_REALTIME, &ts_start);
    
    
    /**
        Правила синтаксиса поиска.
        searchString Слова которые ищутся во фразе, перечисляются через разделители: & - и, | - или . 
        Text Фраза в которой происходит поиск.
    */

    //Debug(syn.searchTextSyn("магазин", "магазин лавка ряд много много магазинов"));
    //Debug(syn.searchTextSyn("студент&алый", "студент красный розовый пурпурный оранжевый розоватый красноватый алый оранжеватый"));
    //Debug(syn.searchTextSyn("студент", "студент красный розовый пурпурный оранжевый розоватый красноватый алый оранжеватый"));
    Debug(syn.searchTextSyn("студент", "студент красный розовый пурпурный оранжевый розоватый красноватый алый оранжеватый"));
    Debug(syn.searchTextFull("студент|алый", "студент красный розовый пурпурный оранжевый розоватый красноватый алый оранжеватый"));
    
    //Debug(start.elapsed());
    
    clock_gettime(CLOCK_REALTIME, &ts_stop);
    int sec = 1000000000*(ts_stop.tv_sec - ts_start.tv_sec)+(ts_stop.tv_nsec - ts_start.tv_nsec);
    printf("%s : %d : 0.%.9d секунд\n", __FILE__, __LINE__, sec);

    
    return 0;
}
