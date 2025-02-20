CREATE DATABASE `classicmangos` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE DATABASE `classiclogs` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE DATABASE `classiccharacters` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE DATABASE `classicrealmd` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE DATABASE `classicplayerbots` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

CREATE USER IF NOT EXISTS 'mangos'@'localhost' IDENTIFIED BY 'mangos';

GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, ALTER, LOCK TABLES, CREATE TEMPORARY TABLES ON `classicmangos`.* TO 'mangos'@'localhost';

GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, ALTER, LOCK TABLES, CREATE TEMPORARY TABLES ON `classiclogs`.* TO 'mangos'@'localhost';

GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, ALTER, LOCK TABLES, CREATE TEMPORARY TABLES ON `classiccharacters`.* TO 'mangos'@'localhost';

GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, ALTER, LOCK TABLES, CREATE TEMPORARY TABLES ON `classicrealmd`.* TO 'mangos'@'localhost';

GRANT SELECT, INSERT, UPDATE, DELETE, CREATE, DROP, ALTER, LOCK TABLES, CREATE TEMPORARY TABLES ON `classicplayerbots`.* TO 'mangos'@'localhost';
