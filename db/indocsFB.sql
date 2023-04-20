CREATE TABLE indocs
(
hent					INT NOT NULL,
docdate					DATE NOT NULL,
extras					VARCHAR(255),
plateno					VARCHAR(16),
loaddate				DATE NOT NULL,
creattime				TIMESTAMP NOT NULL,
creator					INT NOT NULL,
lmodiftime				TIMESTAMP NOT NULL,
lastmodifier				INT NOT NULL,
docid					INT  NOT NULL PRIMARY KEY,
reason					INT NOT NULL,
invoice					INT,

CONSTRAINT buyinvoice_fk    	 FOREIGN KEY (invoice) REFERENCES invoices(invoiceid),
CONSTRAINT deliver_fk 	 	 FOREIGN KEY (hent)     REFERENCES hents(hentid),
CONSTRAINT increator_fk 	 FOREIGN KEY (creator)  REFERENCES users(userid),
CONSTRAINT inlastmodifier_fk	 FOREIGN KEY (lastmodifier)     REFERENCES users(userid),
CONSTRAINT inreason_fk	 	 FOREIGN KEY (reason)     REFERENCES inoutreasons(reasonid)
);



CREATE GENERATOR GEN_ID_INDOCS;


SET TERM ^ ;

CREATE TRIGGER BI_INDOCS_ID FOR indocs
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.docid IS NULL) THEN 
    NEW.docid = GEN_ID(GEN_ID_INDOCS, 1);
END^

SET TERM ; ^


COMMIT;