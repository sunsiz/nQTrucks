TYPE=VIEW
query=select if((locate(\'.\',`ibp`.`TABLE_NAME`) = 0),\'InnoDB System\',replace(substring_index(`ibp`.`TABLE_NAME`,\'.\',1),\'`\',\'\')) AS `object_schema`,sum(if((`ibp`.`COMPRESSED_SIZE` = 0),16384,`ibp`.`COMPRESSED_SIZE`)) AS `allocated`,sum(`ibp`.`DATA_SIZE`) AS `data`,count(`ibp`.`PAGE_NUMBER`) AS `pages`,count(if((`ibp`.`IS_HASHED` = \'YES\'),1,0)) AS `pages_hashed`,count(if((`ibp`.`IS_OLD` = \'YES\'),1,0)) AS `pages_old`,round(ifnull((sum(`ibp`.`NUMBER_RECORDS`) / nullif(count(distinct `ibp`.`INDEX_NAME`),0)),0),0) AS `rows_cached` from `information_schema`.`innodb_buffer_page` `ibp` where (`ibp`.`TABLE_NAME` is not null) group by `object_schema` order by sum(if((`ibp`.`COMPRESSED_SIZE` = 0),16384,`ibp`.`COMPRESSED_SIZE`)) desc
md5=6e0bd592c7bd4abe24aa992c6c800fae
updatable=0
algorithm=1
definer_user=root
definer_host=localhost
suid=0
with_check_option=0
timestamp=2016-06-16 20:23:16
create-version=1
source=SELECT IF(LOCATE(\'.\', ibp.table_name) = 0, \'InnoDB System\', REPLACE(SUBSTRING_INDEX(ibp.table_name, \'.\', 1), \'`\', \'\')) AS object_schema,\n       SUM(IF(ibp.compressed_size = 0, 16384, compressed_size)) AS allocated,\n       SUM(ibp.data_size) AS data,\n       COUNT(ibp.page_number) AS pages,\n       COUNT(IF(ibp.is_hashed = \'YES\', 1, 0)) AS pages_hashed,\n       COUNT(IF(ibp.is_old = \'YES\', 1, 0)) AS pages_old,\n       ROUND(IFNULL(SUM(ibp.number_records)/NULLIF(COUNT(DISTINCT ibp.index_name), 0), 0)) AS rows_cached \n  FROM information_schema.innodb_buffer_page ibp \n WHERE table_name IS NOT NULL\n GROUP BY object_schema\n ORDER BY SUM(IF(ibp.compressed_size = 0, 16384, compressed_size)) DESC
client_cs_name=utf8
connection_cl_name=utf8_general_ci
view_body_utf8=select if((locate(\'.\',`ibp`.`TABLE_NAME`) = 0),\'InnoDB System\',replace(substring_index(`ibp`.`TABLE_NAME`,\'.\',1),\'`\',\'\')) AS `object_schema`,sum(if((`ibp`.`COMPRESSED_SIZE` = 0),16384,`ibp`.`COMPRESSED_SIZE`)) AS `allocated`,sum(`ibp`.`DATA_SIZE`) AS `data`,count(`ibp`.`PAGE_NUMBER`) AS `pages`,count(if((`ibp`.`IS_HASHED` = \'YES\'),1,0)) AS `pages_hashed`,count(if((`ibp`.`IS_OLD` = \'YES\'),1,0)) AS `pages_old`,round(ifnull((sum(`ibp`.`NUMBER_RECORDS`) / nullif(count(distinct `ibp`.`INDEX_NAME`),0)),0),0) AS `rows_cached` from `information_schema`.`innodb_buffer_page` `ibp` where (`ibp`.`TABLE_NAME` is not null) group by `object_schema` order by sum(if((`ibp`.`COMPRESSED_SIZE` = 0),16384,`ibp`.`COMPRESSED_SIZE`)) desc
