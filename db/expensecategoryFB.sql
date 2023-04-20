CREATE TABLE expensecategory			
(
expensecategoryid				INT NOT NULL PRIMARY KEY,
expensecategoryname				VARCHAR(16) NOT NULL UNIQUE,
expensecategorydesc				VARCHAR(100)
);

COMMIT;

CREATE GENERATOR GEN_PK_EXPENSECATEGORY;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_EXPENSECATEGORY FOR EXPENSECATEGORY
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.expensecategoryid IS NULL) THEN 
    NEW.expensecategoryid = GEN_ID(GEN_PK_EXPENSECATEGORY, 1);
END^

SET TERM ; ^

COMMIT;


INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('£APA',NULL);
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('FOOD',NULL);
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('PALIWO',NULL);
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('BIURO','Wyposarzenie biura');
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('REKLAMA',NULL);
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('KAMIL',NULL);
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('AUTO','Eksploatacja/Naprawa aut');
INSERT INTO expensecategory(expensecategoryname,expensecategorydesc) VALUES('KOMP','Sprzet komputerowy');

COMMIT;