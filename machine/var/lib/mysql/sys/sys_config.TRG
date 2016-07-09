TYPE=TRIGGERS
triggers='CREATE DEFINER=`root`@`localhost` TRIGGER sys_config_insert_set_user BEFORE INSERT on sys_config\n    FOR EACH ROW\nBEGIN\n    IF NEW.set_by IS NULL THEN\n        SET NEW.set_by = USER();\n    END IF;\nEND' 'CREATE DEFINER=`root`@`localhost` TRIGGER sys_config_update_set_user BEFORE UPDATE on sys_config\n    FOR EACH ROW\nBEGIN\n    IF NEW.set_by IS NULL THEN\n        SET NEW.set_by = USER();\n    END IF;\nEND'
sql_modes=1073741824 1073741824
definers='root@localhost' 'root@localhost'
client_cs_names='utf8' 'utf8'
connection_cl_names='utf8_general_ci' 'utf8_general_ci'
db_cl_names='utf8_general_ci' 'utf8_general_ci'
