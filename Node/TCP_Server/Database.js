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
        this.enableForeinKeys = 'PRAGMA foreign_keys = ON;';   
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
        
        this.db.exec(this.enableForeinKeys, err => {
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
        this.dbName_actions = 'Actions';
        this.dbName_config = 'Configs';
        this.dbPath = './db/devices.db';
        this.dbSchema = `
            CREATE TABLE IF NOT EXISTS ${this.dbName} (
            id integer NOT NULL PRIMARY KEY, 
            room text NOT NULL,
            name text UNIQUE,
            description text NOT NULL
            );    
            CREATE TABLE IF NOT EXISTS ${this.dbName_actions} (
            action_id integer NOT NULL PRIMARY KEY, 
            action_name text NOT NULL,             
            action_parameter text NOT NULL, 
            action_device_id text NOT NULL,
            FOREIGN KEY(action_device_id) REFERENCES Devices(id)           
            );
            CREATE TABLE IF NOT EXISTS ${this.dbName_config} (
                config_id integer NOT NULL PRIMARY KEY, 
                config_name text NOT NULL,   
                config_device_id text NOT NULL,
                FOREIGN KEY(config_device_id) REFERENCES Devices(id)           
            );
            `;  
       
        this.initDB();
    }

    get () {
        let sql = 
        `
            SELECT DISTINCT id, room, name, description
            FROM ${this.dbName}            
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
    
    add (room, name, description) {
        return new Promise((resolve) => {
            this.db.run(`INSERT INTO ${this.dbName} (room, name, description) VALUES(?,?,?)`, [room, name, description], function (err) {
                if (err) {
                    console.log(err.message)
                    throw err;
                }
                console.log(`Device added: ${this.lastID}`)
                resolve(JSON.stringify({msg: `Device added: ${this.lastID}`}))
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
                console.log(`Device updated: ${this.changes}`)
                resolve()
            })
        })
    }

    getActions (id) { //one device or all (?)
        let sql = 
        `
            SELECT id, action_id, action_name, action_parameter
            FROM ${this.dbName}
            LEFT JOIN Actions ON Actions.action_device_id = Devices.id;
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

    addAction (action_name, action_parameter, device_id) {
        return new Promise((resolve) => {
            this.db.run(`INSERT INTO ${this.dbName_actions} (action_name, action_parameter, action_device_id) VALUES(?,?,?)`, [action_name, action_parameter, device_id], function (err) {
                if (err) {
                    console.log(err.message)
                    throw err;
                }
                console.log(`Action added: ${this.lastID}`)
                resolve(JSON.stringify({msg: `Action added: ${this.lastID}`}))
            })
        })
    }

    updateAction (id, action_name, action_parameter, device_id) {
        let newData = [action_name, action_parameter, device_id, id];
        let sql = `
            UPDATE ${this.dbName_actions}
            SET action_name = ?, action_parameter = ?, action_device_id = ?
            WHERE id = ?
        `;

        return new Promise((resolve) => {
            this.db.run(sql, newData, function (err) {
                if (err) {
                    console.error(err.message);
                    throw err;
                }
                console.log(`Action updated: ${this.changes}`)
                resolve()
            })
        })
    }

    removeAction (id) {
        return new Promise(resolve => {
            this.db.run(`DELETE FROM ${this.dbName_actions} WHERE action_id=?`, id, function (err) {
                if (err) {
                    console.error(err.message)
                    throw err;
                }
                console.log(`Action deleted ${this.changes}`)
                resolve(JSON.stringify({msg: `Action deleted ${this.changes}`}))
            });
        });

    }

    getConfigs (id) { //one device or all (?)
        let sql = 
        `
            SELECT id, config_id, config_name
            FROM ${this.dbName}
            LEFT JOIN Configs ON Configs.config_device_id = Devices.id;
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

    addConfig (config_name, config_device_id) {
        return new Promise((resolve) => {
            this.db.run(`INSERT INTO ${this.dbName_config} (config_name, config_device_id) VALUES(?,?)`, [config_name, config_device_id], function (err) {
                if (err) {
                    console.log(err.message)
                    throw err;
                }
                console.log(`Action added: ${this.lastID}`)
                resolve(JSON.stringify({msg: `Action added: ${this.lastID}`}))
            })
        })
    }

    updateConfig (id, config_name, config_device_id) {
        let newData = [config_name, config_device_id, id];
        let sql = `
            UPDATE ${this.dbName_config}
            SET config_name = ?, config_device_id = ?
            WHERE config_id = ?
        `;

        return new Promise((resolve) => {
            this.db.run(sql, newData, function (err) {
                if (err) {
                    console.error(err.message);
                    throw err;
                }
                console.log(`Action updated: ${this.changes}`)
                resolve()
            })
        })
    }

    removeConfig (id) {
        return new Promise(resolve => {
            this.db.run(`DELETE FROM ${this.dbName_config} WHERE config_id=?`, id, function (err) {
                if (err) {
                    console.error(err.message)
                    throw err;
                }
                console.log(`Action deleted ${this.changes}`)
                resolve(JSON.stringify({msg: `Action deleted ${this.changes}`}))
            });
        });

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
                console.log(`Hint added: ${this.lastID}`)
                resolve(JSON.stringify({msg: `Hint added: ${this.lastID}`}))
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
                console.log(`Hint updated: ${this.changes}`)
                resolve()
            })
        })
    }
}




module.exports = {Database, Devices, Hints}
    