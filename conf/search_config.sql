CREATE EXTENSION IF NOT EXISTS dict_xsyn;
DROP TEXT SEARCH CONFIGURATION IF EXISTS public.russian_ods RESTRICT;
CREATE TEXT SEARCH CONFIGURATION public.russian_ods ( COPY=pg_catalog.russian);
DROP TEXT SEARCH DICTIONARY IF EXISTS russian_ods_syn_dict CASCADE;
CREATE TEXT SEARCH DICTIONARY russian_ods_syn_dict ( TEMPLATE=xsyn_template, RULES='russian_ods_syn_dict', MATCHSYNONYMS=true, MATCHORIG=true, KEEPORIG=true, KEEPSYNONYMS=true);
DROP TEXT SEARCH DICTIONARY IF EXISTS russian_ods_dict CASCADE;
CREATE TEXT SEARCH DICTIONARY russian_ods_dict ( TEMPLATE=ispell, DictFile=russian_ods_dict, AffFile=russian_ods_dict, StopWords=russian );
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  asciihword WITH russian_ods_syn_dict,russian_ods_dict,english_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  asciiword WITH russian_ods_syn_dict,russian_ods_dict,english_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  blank WITH russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  email WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  entity WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  file WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  float WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  host WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  hword WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  hword_asciipart WITH russian_ods_syn_dict,russian_ods_dict,english_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  hword_numpart WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  hword_part WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  int WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  numhword WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  numword WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  protocol WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  sfloat WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  tag WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  uint WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  url WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  url_path WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  version WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;
ALTER TEXT SEARCH CONFIGURATION public.russian_ods ALTER MAPPING FOR  word WITH russian_ods_syn_dict,russian_ods_dict,russian_stem;

--select * from searchTextFull('студент&алый', 'красный студент розовый', 'text_seach_debug_table', 'public.russian_ods');
--select * from searchTextFull('студент&алый', 'голубой студент страшный', 'text_seach_debug_table', 'public.russian_ods');

CREATE OR REPLACE FUNCTION searchTextFull(searchString varchar, text varchar, search_debug_table_name varchar, debug_configuration_name varchar) RETURNS varchar AS
$$
DECLARE
    sql varchar;
    ret varchar;
BEGIN
    sql:= 'DELETE FROM ' || search_debug_table_name;
    execute sql;

    sql := 'INSERT INTO ' || search_debug_table_name || ' VALUES (''' || text || ''', NULL)';
    execute sql;

    sql := 'UPDATE ' || search_debug_table_name || ' SET s_vector=to_tsvector (''' || debug_configuration_name || ''', s_text)';
    execute sql;

    --sql := 'SELECT s_vector from ' || search_debug_table_name;
    --execute sql;

    --sql := 'SELECT to_tsquery(''' || debug_configuration_name || ''',''' || searchString || ''')';
    --execute sql;

    --sql := 'SELECT ts_parse(''default'',''' || searchString || ''')';
    --execute sql;

    sql := 'SELECT * FROM ' || search_debug_table_name || ' WHERE s_vector @@ to_tsquery(''' || debug_configuration_name || ''',''' || searchString || ''')';
    execute sql INTO ret;
    IF ret <> '' THEN
    ELSE
        RETURN '';
    END IF;

    --sql := 'UPDATE ' || search_debug_table_name || ' SET s_highlight=NULL';
    --execute sql;

    sql := 'UPDATE ' || search_debug_table_name || ' SET s_highlight=ts_headline(''' || debug_configuration_name || E'\', s_text, to_tsquery(''' || debug_configuration_name || ''',''' || searchString || '''),''HighlightAll=TRUE'')';
    execute sql;

    sql:= 'SELECT s_highlight FROM ' || search_debug_table_name;
    execute sql INTO ret;

    RETURN ret;
END
$$ LANGUAGE plpgsql;



