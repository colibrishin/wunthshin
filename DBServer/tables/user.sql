CREATE TABLE users(
    id BIGSERIAL UNIQUE NOT NULL PRIMARY KEY, 
    name VARCHAR(255) NOT NULL UNIQUE, 
    email VARCHAR(255) NOT NULL,
    password BYTEA NOT NULL
);
