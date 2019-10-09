const sqlite3 = require('sqlite3').verbose()

class Database {

    constructor () {
        this.db;
        this.dbName = 'Database';
        this.dbPath = './db/name.db';
        this.dbSchema = `CREATE TABLE IF NOT EXISTS Database (
            id integer NOT NULL PRIMARY KEY,   
            hint text NOT NULL 
        )`;         
    }

    initDB () {
        this.db = new sqlite3.Database(this.dbPath, err => {
            if (err) {
                console.log(err)
                return;
            }
            console.log(`Connected to ${this.dbName}`)
        })

        this.db.exec(this.dbSchema, err => {
            if (err) {
                console.log(err)
            } else {
                //console.log('Created table')
            }
        })
    }

    get () {
        let sql = 
        `
            SELECT * FROM ${this.dbName}
            ORDER BY id
        `;

        // promisify to prevent callback hell
        return new Promise((resolve) => {
            this.db.all(sql, [], (err, rows) => {
                if (err) {
                    throw err;
                }                
                resolve(rows)
            })
        })
    }

    add (data) {
        return new Promise((resolve) => {
            this.db.run(`INSERT INTO ${this.dbName}(data) VALUES(?)`, [data], function (err) {
                if (err) {
                    console.log(err.message)
                    throw err;
                }

                // get the last insert id
                console.log(`A row has been inserted with row id ${this.lastID}`)
                resolve(JSON.stringify({msg: `A row has been inserted with row id ${this.lastID}`}))
            })
        })
    }

    update (id, data) {
        let newData = [data, id];
        let sql = `
            UPDATE ${this.dbName}
            SET data = ?
            WHERE id = ?
        `;

        return new Promise((resolve) => {
            this.db.run(sql, newData, function (err) {
                if (err) {
                    console.error(err.message);
                    throw err;
                }
                console.log(`Row updated: ${this.changes}`)
                resolve()
            });
        });

    }

    remove (id) {
        return new Promise(resolve => {
            this.db.run(`DELETE FROM ${this.dbName} WHERE id=?`, id, function (err) {
                if (err) {
                    console.error(err.message)
                    throw err;
                }
                console.log(`Row deleted ${this.changes}`)
                resolve(JSON.stringify({msg: `Row deleted ${this.changes}`}))
            });
        });

    }
}

class Devices extends Database{
    constructor () {
        super()
        this.dbName = 'Devices';
        this.dbPath = './db/devices.db';
        this.dbSchema = `CREATE TABLE IF NOT EXISTS ${this.dbName} (
            id integer NOT NULL PRIMARY KEY, 
            room text NOT NULL,             
            address integer NOT NULL, 
            name text NOT NULL,
            description text NOT NULL,
            config text NOT NULL,
            actions text NOT NULL
        )`;  
        this.initDB();
    }
    add (address, room, name, description, config, actions) {
        return new Promise((resolve) => {
            this.db.run(`INSERT INTO ${this.dbName} (address, room, name, description, config, actions) VALUES(?, ?,?,?,?,?)`, [address, room, name, description, config, actions], function (err) {
                if (err) {
                    console.log(err.message)
                    throw err;
                }
                // get the last insert id
                console.log(`A row has been inserted with row id ${this.lastID}`)
                resolve(JSON.stringify({msg: `A row has been inserted with row id ${this.lastID}`}))
            })
        })
    }

    update (id, address, room, name, description, config, actions) {
        let newData = [address, room, name, description, config, actions, id];
        let sql = `
            UPDATE ${this.dbName}
            SET address = ?, room = ?, name = ?, description = ?, config = ?, actions = ?
            WHERE id = ?
        `;

        return new Promise((resolve) => {
            this.db.run(sql, newData, function (err) {
                if (err) {
                    console.error(err.message);
                    throw err;
                }
                console.log(`Row updated: ${this.changes}`)
                resolve()
            })
        })
    }
}

class Hints extends Database{
    constructor () {
        super()
        this.dbName = 'Hints';
        this.dbPath = './db/hints.db';
        this.dbSchema = `CREATE TABLE IF NOT EXISTS ${this.dbName} (
            id integer NOT NULL PRIMARY KEY,              
            room text NOT NULL, 
            hint text NOT NULL
        )`;  
        this.initDB();
    }   

    add (room, hint) {
        return new Promise((resolve) => {
            this.db.run(`INSERT INTO ${this.dbName} (room, hint) VALUES(?,?)`, [room, hint], function (err) {
                if (err) {
                    console.log(err.message)
                    throw err;
                }

                // get the last insert id
                console.log(`A row has been inserted with row id ${this.lastID}`)
                resolve(JSON.stringify({msg: `A row has been inserted with row id ${this.lastID}`}))
            })
        })
    }

    update (id, room, hint) {
        let newData = [room, hint, id];
        let sql = `
            UPDATE ${this.dbName}
            SET room = ?, hint = ?
            WHERE id = ?
        `;

        return new Promise((resolve) => {
            this.db.run(sql, newData, function (err) {
                if (err) {
                    console.error(err.message);
                    throw err;
                }
                console.log(`Row updated: ${this.changes}`)
                resolve()
            })
        })
    }
}

module.exports = {Database, Devices, Hints}
    