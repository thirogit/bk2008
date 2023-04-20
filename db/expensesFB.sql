CREATE TABLE expenses			
(
expensetitle			VARCHAR(100) NOT NULL,
expenseid				INT NOT NULL PRIMARY KEY,
grosvalue				NUMERIC(15,5) NOT NULL,
vatrate					NUMERIC(15,5) NOT NULL,
expensetype				CHAR NOT NULL CHECK(expensetype = 'F' OR expensetype = '$'),
expensecategory			INT, 
invoiceno				VARCHAR(30),
expensedate				DATE,
extras					VARCHAR(255),
creattime				TIMESTAMP NOT NULL,
creator					INT NOT NULL,
lmodiftime				TIMESTAMP NOT NULL,
lastmodifier			INT NOT NULL,
outdoc				INT,
indoc				INT,


CONSTRAINT expenses_outdoc_fk 	 FOREIGN KEY (outdoc)  REFERENCES outdocs(docid),
CONSTRAINT expenses_indoc_fk 	 FOREIGN KEY (indoc)  REFERENCES indocs(docid),
CONSTRAINT expenses_creator_fk 	 FOREIGN KEY (creator)  REFERENCES users(userid),
CONSTRAINT expenses_lastmodifier_fk	 FOREIGN KEY (lastmodifier)     REFERENCES users(userid),
CONSTRAINT expensecategory FOREIGN KEY (expensecategory) REFERENCES expensecategory(expensecategoryid)
);

COMMIT;

CREATE GENERATOR GEN_PK_EXPENSE;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_EXPENSES FOR EXPENSES
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.expenseid IS NULL) THEN 
    NEW.expenseid = GEN_ID(GEN_PK_EXPENSE, 1);
END^

SET TERM ; ^

COMMIT;
