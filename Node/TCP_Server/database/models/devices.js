module.exports = (Sequelize, db) => {
    const Devices = db.define('Devices', {
        device_room: {
            type: Sequelize.STRING,
            allowNull: false
        },
        device_name: {
            type: Sequelize.STRING,
            allowNull: false
        },
        device_description: {
            type: Sequelize.STRING,
            allowNull: true
        }
    })

    Devices.associate = function(models) {
        Devices.hasMany(models.Actions, { as: 'actions' })
        Devices.hasMany(models.Configs, { as: 'configs' })
    }

    Devices.sync({ force: false }).then(() => { })

    return Devices;
}