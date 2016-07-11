TYPE=VIEW
query=select if(isnull(`performance_schema`.`accounts`.`HOST`),\'background\',`performance_schema`.`accounts`.`HOST`) AS `host`,sum(`stmt`.`total`) AS `statements`,sum(`stmt`.`total_latency`) AS `statement_latency`,(sum(`stmt`.`total_latency`) / sum(`stmt`.`total`)) AS `statement_avg_latency`,sum(`stmt`.`full_scans`) AS `table_scans`,sum(`io`.`ios`) AS `file_ios`,sum(`io`.`io_latency`) AS `file_io_latency`,sum(`performance_schema`.`accounts`.`CURRENT_CONNECTIONS`) AS `current_connections`,sum(`performance_schema`.`accounts`.`TOTAL_CONNECTIONS`) AS `total_connections`,count(distinct `performance_schema`.`accounts`.`USER`) AS `unique_users` from ((`performance_schema`.`accounts` left join `sys`.`x$host_summary_by_statement_latency` `stmt` on((`performance_schema`.`accounts`.`HOST` = `stmt`.`host`))) left join `sys`.`x$host_summary_by_file_io` `io` on((`performance_schema`.`accounts`.`HOST` = `io`.`host`))) group by if(isnull(`performance_schema`.`accounts`.`HOST`),\'background\',`performance_schema`.`accounts`.`HOST`)
md5=f523ba9c134a551cdb2f0b2edbe807df
updatable=0
algorithm=1
definer_user=root
definer_host=localhost
suid=0
with_check_option=0
timestamp=2016-06-16 20:23:16
create-version=1
source=SELECT IF(accounts.host IS NULL, \'background\', accounts.host) AS host,\n       SUM(stmt.total) AS statements,\n       SUM(stmt.total_latency) AS statement_latency,\n       SUM(stmt.total_latency) / SUM(stmt.total) AS statement_avg_latency,\n       SUM(stmt.full_scans) AS table_scans,\n       SUM(io.ios) AS file_ios,\n       SUM(io.io_latency) AS file_io_latency,\n       SUM(accounts.current_connections) AS current_connections,\n       SUM(accounts.total_connections) AS total_connections,\n       COUNT(DISTINCT accounts.user) AS unique_users\n  FROM performance_schema.accounts\n  LEFT JOIN sys.x$host_summary_by_statement_latency AS stmt ON accounts.host = stmt.host\n  LEFT JOIN sys.x$host_summary_by_file_io AS io ON accounts.host = io.host\n GROUP BY IF(accounts.host IS NULL, \'background\', accounts.host)
client_cs_name=utf8
connection_cl_name=utf8_general_ci
view_body_utf8=select if(isnull(`performance_schema`.`accounts`.`HOST`),\'background\',`performance_schema`.`accounts`.`HOST`) AS `host`,sum(`stmt`.`total`) AS `statements`,sum(`stmt`.`total_latency`) AS `statement_latency`,(sum(`stmt`.`total_latency`) / sum(`stmt`.`total`)) AS `statement_avg_latency`,sum(`stmt`.`full_scans`) AS `table_scans`,sum(`io`.`ios`) AS `file_ios`,sum(`io`.`io_latency`) AS `file_io_latency`,sum(`performance_schema`.`accounts`.`CURRENT_CONNECTIONS`) AS `current_connections`,sum(`performance_schema`.`accounts`.`TOTAL_CONNECTIONS`) AS `total_connections`,count(distinct `performance_schema`.`accounts`.`USER`) AS `unique_users` from ((`performance_schema`.`accounts` left join `sys`.`x$host_summary_by_statement_latency` `stmt` on((`performance_schema`.`accounts`.`HOST` = `stmt`.`host`))) left join `sys`.`x$host_summary_by_file_io` `io` on((`performance_schema`.`accounts`.`HOST` = `io`.`host`))) group by if(isnull(`performance_schema`.`accounts`.`HOST`),\'background\',`performance_schema`.`accounts`.`HOST`)