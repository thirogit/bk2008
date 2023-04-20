CREATE TABLE cowpricing
(
cow 					INT NOT NULL,
invoice					INT NOT NULL,
price					NUMERIC(15,5) NOT NULL,
stock					INT NOT NULL,
weight					INT NOT NULL,
CONSTRAINT cowpricing_stock_fk 	FOREIGN KEY (stock)     REFERENCES stocks(stockid),
CONSTRAINT cowpricing_invoice_fk FOREIGN KEY (invoice) REFRENCES invoices(invoiceid),
CONSTRAINT cowpricing_cattle_fk FOREIGN KEY (cow) REFRENCES cattle(animalid),
CONSTRAINT cowpricing_pk  		PRIMARY KEY(cow,invoice)
);

COMMIT;