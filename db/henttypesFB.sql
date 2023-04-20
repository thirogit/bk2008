CREATE TABLE henttypes
(
henttypeid			INT NOT NULL PRIMARY KEY,
henttypename			VARCHAR(32)  NOT NULL
);


COMMIT;


INSERT INTO henttypes(henttypeid,henttypename) VALUES(1,'Firma');
INSERT INTO henttypes(henttypeid,henttypename) VALUES(2,'Indywidualny');

COMMIT;