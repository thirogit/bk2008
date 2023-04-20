CREATE TABLE farms
(
    farmid                  INT NOT NULL PRIMARY KEY,
    name					VARCHAR(64) NOT NULL,
    zip 					VARCHAR(10),
    city					VARCHAR(48),
    street					VARCHAR(48),
    pobox					VARCHAR(10),
    farmno                  CHAR(9),
    wetno                   CHAR(8),
    country					INT NOT NULL,
    
    CONSTRAINT farms_countries_fk FOREIGN KEY (country) REFERENCES countries(number)
    

);

CREATE TABLE herds
(
    herdid                  INT PRIMARY KEY,
    farm                    INT NOT NULL
    name					VARCHAR(64) NOT NULL,
    zip 					VARCHAR(10),
    city					VARCHAR(48),
    street					VARCHAR(48),
    pobox					VARCHAR(10),
    herdno                  INT NOT NULL CHECK(herdno > 0 AND herdno < 1000),
        
    CONSTRAINT herds_farms_fk FOREIGN KEY(farm) REFERENCES farms(farmid)
);

CREATE TABLE seasons
(
    seasonid            INT PRIMARY KEY,
    seasonname          VARCHAR(24) NOT NULL,
    seasonstart         date NOT NULL,
    seasonend           DATE NOT NULL,
    herd                INT NOT NULL,
    
    CONSTRAINT seasons_herds_fk FOREIGN KEY(herd) REFERENCES herds(herdid)
                
);
