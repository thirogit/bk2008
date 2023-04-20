CREATE TABLE cowstockchngs			
(
changeid				INT NOT NULL PRIMARY KEY,
animalid				INT NOT NULL,
stock					INT NOT NULL,
changinguser				INT NOT NULL,
changedate				TIMESTAMP NOT NULL,


CONSTRAINT cowstockchngs_stock_fk 	 FOREIGN KEY (stock)      REFERENCES stocks(stockid),
CONSTRAINT cowstockchngs_animalid_fk 	 FOREIGN KEY (animalid)     REFERENCES cattle(animalid),
CONSTRAINT cowstockchngs_users_fk 	 FOREIGN KEY (changinguser)     REFERENCES users(userid)

);

COMMIT;

CREATE GENERATOR GEN_PK_COWSTOCKCHNGS;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_COWSTOCKCHNGS FOR COWSTOCKCHNGS
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.changeid IS NULL) THEN 
    NEW.changeid = GEN_ID(GEN_PK_COWSTOCKCHNGS, 1);
END^

SET TERM ; ^


COMMIT;