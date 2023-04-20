CREATE TABLE outdocs
(
hent					INT NOT NULL,
docdate					DATE NOT NULL,
extras					VARCHAR(255),
plateno					VARCHAR(16),
loaddate				DATE NOT NULL,
creattime				TIMESTAMP NOT NULL,
creator					INT NOT NULL,
lmodiftime				TIMESTAMP NOT NULL,
lastmodifier			INT NOT NULL,
docid				   	INT NOT NULL PRIMARY KEY,
reason					INT NOT NULL,
invoice					INT,

CONSTRAINT sellinvoice_fk    	 FOREIGN KEY (invoice) REFERENCES invoices(invoiceid),
CONSTRAINT buyer_fk 		 FOREIGN KEY (hent)     REFERENCES hents(hentid),
CONSTRAINT outcreator_fk 	 FOREIGN KEY (creator)  REFERENCES users(userid),
CONSTRAINT outlastmodifier_fk	 FOREIGN KEY (lastmodifier)     REFERENCES users(userid),
CONSTRAINT outreason_fk	 	 FOREIGN KEY (reason)     REFERENCES inoutreasons(reasonid)
);



CREATE GENERATOR GEN_ID_OUTDOCS;



SET TERM ^ ;

CREATE TRIGGER BI_OUTODCS_ID FOR outdocs
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.docid IS NULL) THEN 
    NEW.docid = GEN_ID(GEN_ID_OUTDOCS, 1);
END^

SET TERM ; ^

COMMIT;