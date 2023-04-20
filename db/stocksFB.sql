CREATE TABLE stocks			
(
stockname				VARCHAR(64),
stockcode				VARCHAR(8) NOT NULL UNIQUE,
stockid					INT NOT NULL PRIMARY KEY,
rule					INT,
myperkgprice				NUMERIC(15,2),
predefsex				INT CHECK(predefsex = -1 OR predefsex = 1 OR predefsex = 0),

CONSTRAINT stocks_rule_fk	 	 FOREIGN KEY (rule)     REFERENCES rules(ruleid)
);

COMMIT;


CREATE GENERATOR GEN_PK_STOCKS;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_STOCKS FOR STOCKS
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.stockid IS NULL) THEN 
    NEW.stockid = GEN_ID(GEN_PK_STOCKS, 1);
END^

SET TERM ; ^

COMMIT;


SET TERM ^ ;
CREATE TRIGGER STOCCODEUPPER FOR STOCKS 
ACTIVE BEFORE INSERT OR UPDATE POSITION 0
AS
BEGIN 
    NEW.STOCKCODE = UPPER(NEW.stockcode);
END^
SET TERM ; ^

COMMIT;



INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Byd³o czarno bia³e','ho',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Byd³o czerwono bia³e','rw',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Polska czerwona','rp',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Europejskie byd³o czerwone','re',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Simentaler','sm',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Jersey','je',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Charolais','ch',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Angus czarny','an',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Angus czerwony','ar',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Piemontese (Piedmount)','pi',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Limousin','lm',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Krzy¿ówka z rasami miêsnymi','mm',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Herefold','hh',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Salers','sl',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Welsh Black','wb',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Blonde d''Aquitaine','bd',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Belgian Blue','bb',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Brown Swiss','bs',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Montbeliarde','mo',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Normandy','no',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Ayrshier','ay',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Tyrol Grey','al',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Dexter','dr',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Krzy¿ówka bez ras miêsnych','ms',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Marchigiana','mr',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Shorthorn','sh',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Abondance','ab',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Aubrac','au',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Chianina','ca',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Galloway','ga',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Guernsey','gu',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Gelbvieh','gv',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Highland Cattle','hi',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Beefmaster','bm',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Braford','bo',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Brahman','br',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Brangus','bn',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Maine-Anjou','ma',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Murray-Grey','mg',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Pinzgau','pz',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Romagnola','rn',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Santa Gertrudis','sg',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('South Devon','sd',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Sahiwa','sw',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Tarentaise','ta',0);
INSERT INTO stocks(stockname,stockcode,predefsex) VALUES('Buffalo (Bubalis bubalis)','bf',0);

COMMIT;