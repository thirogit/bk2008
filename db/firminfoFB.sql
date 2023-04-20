CREATE TABLE firminfo
(
firminfoid				INT NOT NULL PRIMARY KEY,
name					VARCHAR(64) NOT NULL,
zip 					VARCHAR(10),
city					VARCHAR(48),
street					VARCHAR(48),
pobox					VARCHAR(10),
nip	    				CHAR(10),
country					INT NOT NULL,
arimrno					CHAR(14),
wetno					CHAR(8),
phone					VARCHAR(12),
plate					VARCHAR(16),
accountno				CHAR(26),
bankname				VARCHAR(50),
regon					CHAR(9),

CONSTRAINT countries_fk FOREIGN KEY (country) REFERENCES countries(number)
);

COMMIT;

INSERT INTO FIRMINFO (FIRMINFOID, NAME, ZIP, CITY, STREET, POBOX, COUNTRY, ARIMRNO, WETNO)
 VALUES (
1, 
'Nazwa Firmy', 
'00-000', 
'Miasto', 
'Ulica', 
'0a', 
616, 
'PL000000000000', 
'00000000'
);

COMMIT;
