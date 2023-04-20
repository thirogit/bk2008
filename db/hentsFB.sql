CREATE TABLE hents
(
hentid					INT NOT NULL PRIMARY KEY,
alias					VARCHAR(24) NOT NULL UNIQUE,
name					VARCHAR(64) NOT NULL,
zip 					VARCHAR(10),
city					VARCHAR(48),
street					VARCHAR(48),
pobox					VARCHAR(10),
nip	    				CHAR(10),
country					INT NOT NULL,
arimrno					CHAR(14) UNIQUE NOT NULL,
wetno					CHAR(8) UNIQUE,
phone					VARCHAR(12),
plate					VARCHAR(16),
henttype				INT NOT NULL,
extras					VARCHAR(255),
creattime				TIMESTAMP NOT NULL,
creator					INT NOT NULL,
lmodiftime				TIMESTAMP NOT NULL,
lastmodifier			INT NOT NULL,
accountno				CHAR(26),
bankname				VARCHAR(50),
pesel					CHAR(11),
regon					VARCHAR(14),
idno					VARCHAR(12),
issuepost				VARCHAR(50),
issuedate				DATE,
syncit					INT,
cellphone              	VARCHAR(16),
email                  	VARCHAR(64),
latitude				NUMERIC(15,12),
longitude				NUMERIC(15,15),

CONSTRAINT hents_creator_fk 	 FOREIGN KEY (creator)  REFERENCES users(userid),
CONSTRAINT hents_lastmodifier_fk	 FOREIGN KEY (lastmodifier)     REFERENCES users(userid),
CONSTRAINT hents_countries_fk FOREIGN KEY (country) REFERENCES countries(number),
CONSTRAINT hents_henttypes_fk FOREIGN KEY (henttype) REFERENCES henttypes(henttypeid)
);

COMMIT;

CREATE GENERATOR GEN_PK_HENTS;

COMMIT;

SET TERM ^ ;

CREATE TRIGGER BI_HENTS FOR hents
ACTIVE BEFORE INSERT
AS
BEGIN
  IF(NEW.hentid IS NULL) THEN 
    NEW.hentid = GEN_ID(GEN_PK_HENTS, 1);
END^

SET TERM ; ^

COMMIT;