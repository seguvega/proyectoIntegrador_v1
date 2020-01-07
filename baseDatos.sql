
CREATE TABLE actuador (
                id_a INT AUTO_INCREMENT NOT NULL,
                nombre VARCHAR(25) NOT NULL,
                estado BOOLEAN NOT NULL,
                PRIMARY KEY (id_a)
);


CREATE TABLE registro_actuador (
                id INT AUTO_INCREMENT NOT NULL,
                fecha DATETIME NOT NULL,
                estado BOOLEAN NOT NULL,
                id_a INT NOT NULL,
                PRIMARY KEY (id)
);


CREATE TABLE sensor (
                id_s INT AUTO_INCREMENT NOT NULL,
                nombre VARCHAR(25) NOT NULL,
                valor DOUBLE PRECISION NOT NULL,
                PRIMARY KEY (id_s)
);


CREATE TABLE registro (
                id INT NOT NULL,
                fecha DATETIME NOT NULL,
                dato DOUBLE PRECISION NOT NULL,
                id_s INT NOT NULL,
                PRIMARY KEY (id)
);


ALTER TABLE registro_actuador ADD CONSTRAINT actuador_registro_actuador_fk
FOREIGN KEY (id_a)
REFERENCES actuador (id_a)
ON DELETE NO ACTION
ON UPDATE NO ACTION;

ALTER TABLE registro ADD CONSTRAINT sensor_registro_fk
FOREIGN KEY (id_s)
REFERENCES sensor (id_s)
ON DELETE NO ACTION
ON UPDATE NO ACTION;
