Sequalize = require('sequelize')
module.exports = new Sequalize('Devices', 'username', 'password', {
    host: 'localhost',
    dialect: 'sqlite',
    pool: {
        max: 5,
        min: 0,
        idle: 10000
    },
    storage: './db/devices.db'
});



