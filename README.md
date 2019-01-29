# libsynonym

1 Установка postgresql:

	sudo apt-get install postgresql-X postgresql-X-postgis-Y postgresql-X-postgis-scripts postgresql-client-x postgresql-contrib postgis
	
	Библиотека Qt для работы с postgres.
		sudo apt-get install libqt5sql5-psql	# Qt5 
		sudo apt-get install libqt4sql-psql		# Qt4
	
	Возможно потребуется перезапуск сервиса: sudo service postgresql stop && sudo service postgresql start

	Установить postgres нужный пароль:
		sudo -u postgres psql
		
	alter user postgres password '1';
	create database test_db owner postgres;
	
	Даем пользователю все права.
	ALTER USER postgres WITH SUPERUSER;
	
	sudo -u postgres psql -c 'create extension postgis;'
	
2 Настройка postgresql:

	Скопировать файлы из dicts в /usr/share/postgresql/X/tsearch_data/
		russian_ods_dict.affix 
		russian_ods_dict.dict
		russian_ods_syn_dict.rules

	Выставить права на файлы:
		sudo chmod 644 /usr/share/postgresql/X/tsearch_data/russian_ods* && sudo chown postgres /usr/share/postgresql/X/tsearch_data/russian_ods*

	Сделать ссылку /usr/share/postgresql/tsearch_data/ на /usr/share/postgresql/X/tsearch_data/
		ln -s /usr/share/postgresql/X/tsearch_data /usr/share/postgresql/tsearch_data
	
    Загрузить /conf/search_config.sql в базу.
		sudo -u postgres psql test_db < /conf/search_config.sql
