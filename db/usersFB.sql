CREATE TABLE users			
(
username				VARCHAR(16) NOT NULL UNIQUE,
userid					INT NOT NULL PRIMARY KEY,
fname					VARCHAR(24) NOT NULL,
lname					VARCHAR(32),
pass					VARCHAR(16),
lastlogin				TIMESTAMP,
privledges				INT NOT NULL
);

COMMIT;

CREATE GENERATOR GEN_PK_USERS;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_USERS FOR USERS
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.userid IS NULL) THEN 
    NEW.userid = GEN_ID(GEN_PK_USERS, 1);
END^

SET TERM ; ^

COMMIT;


INSERT INTO users(username,fname,lname,pass,lastlogin,privledges) VALUES('admin','admin','admin','secretpass',NULL,63);

COMMIT;