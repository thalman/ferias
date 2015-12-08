create table 'users' (
    'id' INTEGER PRIMARY KEY ASC,
    'name' VARCHAR(25),
    'password' VARCHAR(100),
    'admin' INTEGER
);

create table 'emails' (
    'id' INTEGER PRIMARY KEY ASC,
    'email' varchar(50) unique,
    'user_id' INTEGER,
    FOREIGN KEY ('user_id') REFERENCES users('id')
);

create table 'replies' (
    'id' INTEGER PRIMARY KEY ASC,
    'user_id' INTEGER UNIQUE,
    'active' INTEGER,
    'from_date' INTEGER,
    'to_date' INTEGER,
    'subject' VARCHAR(255),
    'body' VARCHAR(1024),
    FOREIGN KEY ('user_id') REFERENCES users('id')
);
CREATE INDEX 'replies_user_id_idx' ON replies ('user_id');

create table 'sent_replies' (
    'id' INTEGER PRIMARY KEY ASC,
    'user_id' INTEGER UNIQUE,
    'from' VARCHAR(255),
    'date' INTEGER,
    FOREIGN KEY ('user_id') REFERENCES users('id')
);
CREATE INDEX 'sent_replies_email_idx' ON sent_replies ('from');

insert into users ( 'name', 'password', 'admin' ) values ( 'admin', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918', 1 );
