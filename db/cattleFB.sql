CREATE TABLE cattle
(
animalid  				INT  NOT NULL PRIMARY KEY,
ean					CHAR(14) NOT NULL,	
birthdate				DATE,
birthplace				VARCHAR(32),	
sex					INT CHECK(sex = -1 OR sex = 1 OR sex = 0),			
stock					INT,
motherean				CHAR(14),
weight					INT,
passno					CHAR(10),
passdate				DATE,
fstownr					INT,
docin					INT,
docout					INT,
extras					VARCHAR(255),
creattime				TIMESTAMP NOT NULL,
creator					INT NOT NULL,
lmodiftime				TIMESTAMP NOT NULL,
lastmodifier				INT NOT NULL,
myprice                 		NUMERIC(15,5),
termbuystock				INT,
termbuyweight				INT,
termsellstock				INT,
termsellweight				INT,
termbuyprice				NUMERIC(15,5),
termsellprice				NUMERIC(15,5),
outgrp					INT,
ingrp					INT,
passimglabel				CHAR(24) UNIQUE,
rrinv					INT,
latitude				NUMERIC(15,12),
longitude				NUMERIC(15,15),

CONSTRAINT termbuystock_fk FOREIGN KEY (termbuystock) REFERENCES stocks(stockid),
CONSTRAINT termsellstock_fk FOREIGN KEY (termsellstock) REFERENCES stocks(stockid),
CONSTRAINT firstowner_fk FOREIGN KEY (fstownr) REFERENCES hents(hentid),
CONSTRAINT docin_fk 	 FOREIGN KEY (docin)      REFERENCES indocs(docid),
CONSTRAINT docout_fk 	 FOREIGN KEY (docout)     REFERENCES outdocs(docid),
CONSTRAINT stocks_fk 	 FOREIGN KEY (stock)     REFERENCES stocks(stockid),
CONSTRAINT cattle_creator_fk 	 FOREIGN KEY (creator)  REFERENCES users(userid),
CONSTRAINT cattle_lastmodifier_fk	 FOREIGN KEY (lastmodifier)     REFERENCES users(userid),
CONSTRAINT cattle_rrinv_fk	 	 FOREIGN KEY (rrinv)     REFERENCES INVOICES(invoiceid)
);



CREATE GENERATOR GEN_PK_CATTLE;


SET TERM ^ ;

CREATE TRIGGER BI_CATTLE FOR cattle
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.animalid IS NULL) THEN 
    NEW.animalid = GEN_ID(GEN_PK_CATTLE, 1);
END^

SET TERM ; ^

COMMIT;