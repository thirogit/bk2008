CREATE TABLE inoutreasons			
(
reasonid				INT NOT NULL PRIMARY KEY,
reasoncode				VARCHAR(5) NOT NULL,
reasonname				VARCHAR(48),
inoutreason				INT NOT NULL CHECK(inoutreason = -1 OR inoutreason = 1 OR inoutreason = 0)
);

COMMIT;

INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(1,'UR','URODZENIE',1);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(2,'K','KUPNO',1);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(3,'I','IMPORT',1);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(4,'E','EXPORT',-1);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(5,'SU','SPRZEDA¯ W CELU UBOJU',-1);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(6,'S','SPRZEDA¯',-1);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(7,'IN','INNE',0);
INSERT INTO inoutreasons(reasonid,reasoncode,reasonname,inoutreason) VALUES(8,'UB','UBÓJ',-1);

COMMIT;