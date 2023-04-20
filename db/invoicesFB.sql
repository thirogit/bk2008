CREATE TABLE invoices
(
invoiceno				INT,
invoiceid  				INT  NOT NULL PRIMARY KEY,
extras					VARCHAR(255),
invoicedate				DATE NOT NULL,
paid					DATE,
invtype					INT NOT NULL CHECK(invtype >= -1 AND invtype <= 2),
correct					INT,
invoicevalue		    		NUMERIC(15,5) NOT NULL,
VATrate					NUMERIC(15,5) NOT NULL,
payway					CHAR(1) NOT NULL CHECK(payway = '$' OR payway = '@'),
customnumber				VARCHAR(15),
creattime				TIMESTAMP NOT NULL,
creator					INT NOT NULL,
lmodiftime				TIMESTAMP NOT NULL,
lastmodifier				INT NOT NULL,
payduedays				INT,

CONSTRAINT inv_creator_fk 	 FOREIGN KEY (creator)  REFERENCES users(userid),
CONSTRAINT inv_lastmodifier_fk	 FOREIGN KEY (lastmodifier)     REFERENCES users(userid),
CONSTRAINT inv_correct_fk 	 FOREIGN KEY (correct)      REFERENCES invoices(invoiceid),
CONSTRAINT unique_invno		 UNIQUE(invoiceno,invtype)

);


CREATE GENERATOR GEN_ID_INVOICES;



SET TERM ^ ;

CREATE TRIGGER BI_INVOICES_ID FOR invoices
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.invoiceid IS NULL) THEN 
    NEW.invoiceid = GEN_ID(GEN_ID_INVOICES, 1);
END^

SET TERM ; ^

COMMIT;


