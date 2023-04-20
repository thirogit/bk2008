CREATE TABLE cowweightchngs			
(
changeid				INT NOT NULL PRIMARY KEY,
animalid				INT NOT NULL,
weight					INT,
changinguser				INT NOT NULL,
changedate				TIMESTAMP NOT NULL,

CONSTRAINT cowweightchngs_animalid_fk 	 FOREIGN KEY (animalid)     REFERENCES cattle(animalid),
CONSTRAINT cowweightchngs_users_fk 	 FOREIGN KEY (changinguser)     REFERENCES users(userid)

);

COMMIT;

CREATE GENERATOR GEN_PK_COWWEIGHTCHNGS;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_COWWEIGHTCHNGS FOR COWWEIGHTCHNGS
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.changeid IS NULL) THEN 
    NEW.changeid = GEN_IDGEN_PK_COWWEIGHTCHNGS, 1);
END^

SET TERM ; ^


COMMIT;