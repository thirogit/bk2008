CREATE TABLE rules
(
ruleid  				INT  NOT NULL PRIMARY KEY,
rulename				VARCHAR(64) NOT NULL,	
buylimweight				INT NOT NULL,
buypriceoverweightxy			NUMERIC(15,2) NOT NULL,	
buypriceunderweightxy			NUMERIC(15,2) NOT NULL,	
buypriceoverweightxx			NUMERIC(15,2) NOT NULL,	
buypriceunderweightxx			NUMERIC(15,2) NOT NULL,	

selllimweight				INT NOT NULL,
sellpriceoverweightxy			NUMERIC(15,2) NOT NULL,	
sellpriceunderweightxy			NUMERIC(15,2) NOT NULL,	
sellpriceoverweightxx			NUMERIC(15,2) NOT NULL,	
sellpriceunderweightxx			NUMERIC(15,2) NOT NULL,

nosexvalue				INT NOT NULL CHECK(nosexvalue = -1 OR nosexvalue = 1),	

);

COMMIT;

CREATE GENERATOR GEN_PK_RULES;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_RULES FOR rules
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.ruleid IS NULL) THEN 
    NEW.ruleid = GEN_ID(GEN_PK_RULES, 1);
END^

SET TERM ; ^

COMMIT;